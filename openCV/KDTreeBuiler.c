/*
 * KDTreeBuiler.c
 *
 *  Created on: Aug 22, 2016
 *      Author: guyyt
 */

#include "KDTreeBuiler.h"
#include "SPKDArrays.h"
#include "SPPoint.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "SPConfig.h"
#include "SPListElement.h"
#include "SPLogger.h"
#include "SPBPriorityQueue.h"
#include <time.h>


struct KD_Tree_Node
{
	 int Dim ;// = The splitting dimension
	 double Val;// = The median value of the splitting dimension
	 KDTreeNode Left;// Pointer to the left subtree
	 KDTreeNode Right;// Pointer to the right subtree
	 SPPoint Data;//Pointer to a point (only if the current node is a leaf) otherwise this field value is NULL
};

int cmpFuncSPListElementByVals (const void * a, const void * b)
{
	 SPListElement * a1 = ( SPListElement *) a;
	 SPListElement * b1 = ( SPListElement *) b;

	 //sort from big to small then by lowest dim
	if (spListElementGetValue(*a1) > spListElementGetValue(*b1)){
		return -1;
	}
	else if(spListElementGetValue(*a1) == spListElementGetValue(*b1)){
		if(spListElementGetIndex(*a1) < spListElementGetIndex(*b1)){
			return -1;
		}
	}
	return 1;
}


KDTreeNode CreateTreeFromArray(SPKDArray kda,SPConfig spConfig){

	spLoggerPrintDebug("start CreateTreeFromArray",__FILE__, __func__, __LINE__);

	return CreateTreeNode(kda,spConfig,0,spConfigeGetSplitMethod(spConfig));
}


KDTreeNode CreateTreeNode(SPKDArray kda,SPConfig spConfig,int incNextDim,SP_SPLIT_METHOD method){

	if(kda==NULL || spConfig==NULL || incNextDim<0 || method<0){
		spLoggerPrintWarning("something is null",__FILE__, __func__, __LINE__);
		return NULL;

	}
	KDTreeNode head=(KDTreeNode)malloc( sizeof(*head));
	if(head==NULL){
		spLoggerPrintError("error allocation head",__FILE__, __func__, __LINE__);
	}
	SPKDArray* splitReturn=NULL;
	int i,j,dimToSplitBy;
	double min,max,temp;
	int numOfDims=SPKDArrayGetNumOfDims(kda);
	int numOfPoints=SPKDArrayGetNumOfPoints(kda);
	SPPoint* points=SPKDArrayGetpoints(kda);

	if(numOfDims<0 || numOfPoints<0 || points==NULL){
			spLoggerPrintWarning("something is null",__FILE__, __func__, __LINE__);
			return NULL;
		}

	SPListElement* allDimsDiff=(SPListElement*)malloc(sizeof(SPListElement)* numOfDims);
	if(allDimsDiff==NULL){
			spLoggerPrintError("error allocation allDimsDiff",__FILE__, __func__, __LINE__);
			return NULL;

	}

	if(numOfPoints==1){
		head->Dim=-1;//invalid;
		head->Val=-999.999;//invalid;
		head->Left=NULL;
		head->Right=NULL;
		head->Data=spPointCopy(points[0]);
	}
	else{

		if(method== MAX_SPREAD){

			if(points[0]==NULL){
					spLoggerPrintWarning("points[0] in null",__FILE__, __func__, __LINE__);
					return NULL;
				}

			for(i=0;i<numOfDims;i++){

				if(points[0]==NULL){
					spLoggerPrintWarning("points[0] in null",__FILE__, __func__, __LINE__);
					return NULL;
				}
				else{
					min=spPointGetData(points[0])[i];
					max=spPointGetData(points[0])[i];
				}

				for(j=0;j<numOfPoints;j++){

					if(points[j]==NULL){
						spLoggerPrintWarning("points[j] in null",__FILE__, __func__, __LINE__);
					}
					else if(spPointGetData(points[j])==NULL){
						spLoggerPrintWarning("points[j]->data in null",__FILE__, __func__, __LINE__);
					}
					else if(spPointGetDimension(points[j])!=numOfDims){
						spLoggerPrintWarning("points[j]->dims!=numOfDims",__FILE__, __func__, __LINE__);
					}
					else{
						temp=spPointGetData(points[j])[i];
						if(temp<min) min=temp;
						if(temp>max) max=temp;
					}

				}

				allDimsDiff[i]= spListElementCreate(i, max-min);

				if(allDimsDiff[i]==NULL){
					spLoggerPrintError("error allocation allDimsDiff[i]",__FILE__, __func__, __LINE__);
				}

			}

			qsort(allDimsDiff,numOfDims,sizeof(SPListElement),cmpFuncSPListElementByVals);

			dimToSplitBy = spListElementGetIndex(allDimsDiff[0]);



		}
		else if(method== RANDOM){
			srand(time(NULL));
			dimToSplitBy=rand()%numOfDims;

		}
		else if(method== INCREMENTAL ){

			dimToSplitBy=(incNextDim)%numOfDims;

		}

		if(dimToSplitBy<0 || dimToSplitBy>numOfDims){
			spLoggerPrintError("dimToSplitBy<0 || dimToSplitBy>numOfDims",__FILE__, __func__, __LINE__);
			return NULL;
		}

		head->Dim=dimToSplitBy;
		head->Data=NULL;

		splitReturn=split(kda, dimToSplitBy);

		if(splitReturn==NULL){
			spLoggerPrintError("splitReturn==NULL",__FILE__, __func__, __LINE__);
			return NULL;
		}
		else if(splitReturn[0]==NULL || splitReturn[1]==NULL){
			spLoggerPrintError("splitReturn[0 or 1]==NULL",__FILE__, __func__, __LINE__);
			return NULL;
		}

		//the value of the coor of the most right point in the left array
		head->Val=spPointGetAxisCoor(KDArrayGetTheMostRightPoint(splitReturn[0],dimToSplitBy),dimToSplitBy);

		head->Left=CreateTreeNode(splitReturn[0],spConfig,incNextDim+1,method);
		head->Right=CreateTreeNode(splitReturn[1],spConfig,incNextDim+1,method);

	}

	//FREE
	if(method== MAX_SPREAD){
		for(i=0;i<numOfDims;i++){

				spListElementDestroy(allDimsDiff[i]);
			}
	}


	free(allDimsDiff);
	SPKDArrayDestroy(kda);
//	free(splitReturn);

	return head;



}
SPPoint KDTreeNodeGetData(KDTreeNode node){
	return node->Data;
}

KDTreeNode KDTreeNodeGetLeft(KDTreeNode node){
	return node->Left;
}
KDTreeNode KDTreeNodeGetRight(KDTreeNode node){
	return node->Right;
}


void kNearestNeighbors(KDTreeNode curr,SPBPQueue bpq, SPPoint P){
	double dist;
	double temp;
	SPListElement elem;
	bool sideToSearch;//true = left, false= right
	if(curr==NULL){
		spLoggerPrintWarning("curr in null",__FILE__, __func__, __LINE__);
		return;
	}

	if(curr->Left==NULL && curr->Right==NULL){//is leaf

		dist=spPointL2SquaredDistance(curr->Data,P);
		elem=spListElementCreate(spPointGetIndex(curr->Data),dist);
		spBPQueueEnqueue(bpq,elem);
		return;
	}
	if(spPointGetData(P)[curr->Dim] <= curr->Val){
		sideToSearch = true;
		kNearestNeighbors(curr->Left , bpq,  P);
	}
	else{
		sideToSearch= false;
		kNearestNeighbors(curr->Right , bpq,  P);
	}

	//check if |curr.val - P[curr.dim]|^2 is less than the priority of the max-priority element of bpq

	//temp=(curr->Val - spPointGetData(P)[curr->Dim]);
	temp=curr->Val - spPointGetAxisCoor(P,curr->Dim);

	//checkCondition= ((temp*temp) < spBPQueueMaxValue(bpq));

	if(!spBPQueueIsFull(bpq) || ((temp*temp) < spBPQueueMaxValue(bpq))){
		if(sideToSearch==true){
			kNearestNeighbors(curr->Right , bpq,  P);
		}
		else{
			kNearestNeighbors(curr->Left , bpq,  P);
		}
	}
}

SPBPQueue FindkNearestNeighbors(KDTreeNode curr,SPPoint P,SPConfig conf){
	SPBPQueue bpq = spBPQueueCreate(spConfigGetspKNN(conf));
      kNearestNeighbors(curr, bpq,  P);
      return bpq;
}



void KDTreeDestroy(KDTreeNode node){
	spPointDestroy(node->Data);
	if(node->Left!=NULL){
		KDTreeDestroy(node->Left);
	}
	if(node->Right!=NULL){
			KDTreeDestroy(node->Right);
	}
	free(node);
}


