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
#include "SPBPriorityQueue.h"
#include <time.h>


struct KD_Tree_Node
{
	 int Dim ;// = The splitting dimension
	 double Val;// = The median value of the splitting dimension
	 KDTreeNode* Left;// Pointer to the left subtree
	 KDTreeNode* Right;// Pointer to the right subtree
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
	return CreateTreeNode(kda,spConfig,0);
}


KDTreeNode CreateTreeNode(SPKDArray kda,SPConfig spConfig,int incNextDim){
	KDTreeNode head=(KDTreeNode)malloc( sizeof(KDTreeNode));
	SPKDArray* splitReturn=NULL;
	int i,j,dimToSplitBy;
	double min,max,temp;
	time_t t;
	int numOfDims=SPKDArrayGetNumOfDims(kda);
	int numOfPoints=SPKDArrayGetNumOfPoints(kda);
	SPPoint* points=SPKDArrayGetpoints(kda);
	SPListElement* allDimsDiff=(SPListElement*)malloc(sizeof(SPListElement)* numOfDims);


	if(numOfPoints==1){
		head->Dim=-1;//invalid;
		head->Val=-1;//invalid;
		head->Left=NULL;
		head->Right=NULL;
		head->Data=points[0];
	}
	else{

		if(spConfigeGetSplitMethod(spConfig)== MAX_SPREAD){

			for(i=0;i<numOfDims;i++){

				min=spPointGetData(points[0])[i];
				max=spPointGetData(points[0])[i];

				for(j=0;j<numOfPoints;j++){

					temp=spPointGetData(points[j])[i];
					if(temp<min) min=temp;
					if(temp>max) max=temp;
				}

				allDimsDiff[i]= spListElementCreate(i, max-min);

			}

			qsort(allDimsDiff,numOfDims,sizeof(SPListElement),cmpFuncSPListElementByVals);

			dimToSplitBy = spListElementGetIndex(allDimsDiff[0]);

		}
		else if(spConfigeGetSplitMethod(spConfig)== RANDOM){
			srand((unsigned) time(&t));
			dimToSplitBy=rand()%numOfDims;

		}
		else if(spConfigeGetSplitMethod(spConfig)== INCREMENTAL ){

			dimToSplitBy=(incNextDim)%numOfDims;
		}

		head->Dim=dimToSplitBy;
		head->Data=NULL;

		splitReturn=split(kda, dimToSplitBy);

		//the value of the coor of the most right point in the left array
		head->Val=spPointGetAxisCoor(KDArrayGetTheMostRightPoint(splitReturn[0]),dimToSplitBy);

		*head->Left=CreateTreeNode(splitReturn[0],spConfig,incNextDim+1);
		*head->Right=CreateTreeNode(splitReturn[1],spConfig,incNextDim+1);


	}

	//FREE
	for(i=0;i<numOfDims;i++){

		spListElementDestroy(allDimsDiff[i]);
	}
	free(allDimsDiff);
	SPKDArrayDestroy(splitReturn[0]);
	SPKDArrayDestroy(splitReturn[1]);
	free(splitReturn);
	//free(t);
	return head;

}
SPPoint KDTreeNodeGetData(KDTreeNode node){
	return node->Data;
}

KDTreeNode* KDTreeNodeGetLeft(KDTreeNode node){
	return node->Left;
}
KDTreeNode* KDTreeNodeGetRight(KDTreeNode node){
	return node->Right;
}


void kNearestNeighbors(KDTreeNode curr,SPBPQueue bpq, SPPoint P){
	double dist;
	SPListElement elem;
	bool sideToSearch;//true = left, false= right
	bool checkCondition;
	if(curr==NULL){
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
		kNearestNeighbors(*curr->Left , bpq,  P);
	}
	else{
		sideToSearch= false;
		kNearestNeighbors(*curr->Right , bpq,  P);
	}

	//check if |curr.val - P[curr.dim]|^2 is less than the priority of the max-priority element of bpq
	checkCondition= (pow(curr->Val - spPointGetData(P)[curr->Dim],2) < spBPQueueMaxValue(bpq));

	if(!spBPQueueIsFull(bpq) || checkCondition){
		if(sideToSearch==true){
			kNearestNeighbors(*curr->Right , bpq,  P);
		}
		else{
			kNearestNeighbors(*curr->Left , bpq,  P);
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
		KDTreeDestroy(*node->Left);
	}
	if(node->Right!=NULL){
			KDTreeDestroy(*node->Right);
		}
	free(node);
}


