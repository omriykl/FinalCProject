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
#include <stdlib.h>
#include "SPConfig.h"
#include "SPListElement.h"
#include "SPBPriorityQueue.h"
#include <time.h>


struct KD_Tree_Node
{
	 int Dim ;// = The splitting dimension
	 int Val;// = The median value of the splitting dimension
	 KDTreeNode Left;// Pointer to the left subtree
	 KDTreeNode Right;// Pointer to the right subtree
	 SPPoint Data;//Pointer to a point (only if the current node is a leaf) otherwise this field value is NULL
};

int cmpFuncByVals (const void * a, const void * b)
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

KDTreeNode CreateTreeNode(SPKDArray kda,SPConfig spConfig,int incNextDim){
	KDTreeNode head=(KDTreeNode)malloc( sizeof(KDTreeNode));
	SPListElement* allDimsDiff=(SPListElement*)malloc(sizeof(SPListElement)*(kda->numOfDims));
	SPKDArray* splitReturn;

	int i,j,dimToSplitBy;
	double min,max,temp;
	time_t t;


	if(kda->numOfPoints==1){
		head->Dim=-1;//invalid;
		head->Val=-1;//invalid;
		head->Left=NULL;
		head->Right=NULL;
		head->Data=kda->points;
	}
	else{

		if(spConfig->spKDTreeSplitMethod== MAX_SPREAD){

			for(i=0;i<kda->numOfDims;i++){
				min=kda->points[0].data[i];
				max=kda->points[0].data[i];

				for(j=0;j<kda->numOfPoints;j++){
					temp==kda->points[j].data[i];
					if(temp<min) min=temp;
					if(temp>max) max=temp;
				}

				allDimsDiff[i]= spListElementCreate(i, max-min);

			}

			qsort(allDimsDiff,kda->numOfDims,sizeof(SPListElement),cmpFuncByVals);
			dimToSplitBy=allDimsDiff[0].index;
			head->Dim=dimToSplitBy;
			head->Data=NULL;

			splitReturn=Split(kda, dimToSplitBy);
			head->Val=??;
			head->Left=CreateTreeNode(splitReturn[0],spConfig,incNextDim+1);
			head->Right=CreateTreeNode(splitReturn[1],spConfig,incNextDim+1);

		}
		else if(spConfig->spKDTreeSplitMethod== RANDOM){
			srand((unsigned) time(&t));
			dimToSplitBy=rand()%kda->numOfDims;

			head->Dim=dimToSplitBy;
			head->Data=NULL;

			splitReturn=Split(kda, dimToSplitBy);

			head->Val=??;

			head->Left=CreateTreeNode(splitReturn[0],spConfig,incNextDim+1);
			head->Right=CreateTreeNode(splitReturn[1],spConfig,incNextDim+1);

		}
		else if(spConfig->spKDTreeSplitMethod== INCREMENTAL ){
			dimToSplitBy=(incNextDim)%kda->numOfDims;

			head->Dim=dimToSplitBy;
			head->Data=NULL;

			splitReturn=Split(kda, dimToSplitBy);

			head->Val=??;

			head->Left=CreateTreeNode(splitReturn[0],spConfig,incNextDim+1);
			head->Right=CreateTreeNode(splitReturn[1],spConfig,incNextDim+1);
		}

	}
	//FREE
	for(i=0;i<kda->numOfDims;i++){
		spListElementDestroy(allDimsDiff[i]);
	}
	free(allDimsDiff);
	SPKDArrayDestroy(splitReturn[0]);
	SPKDArrayDestroy(splitReturn[1]);
	free(splitReturn);
	free(t);
	return head;

}

SPBPQueue FindkNearestNeighbors(KDTreeNode curr,SPPoint P,SPConfig conf){
	SPBPQueue bpq=spBPQueueCreate(conf->spKNN);

	kNearestNeighbors(curr, bpq,  P);
}

void kNearestNeighbors(KDTreeNode curr,SPBPQueue bpq, SPPoint P){
	double dist;
	SPListElement elem;
	bool temp;
	if(curr==NULL){
		return;
	}

	if(curr->Left==NULL && curr->Right==NULL){//is leaf

		dist=spPointL2SquaredDistance(curr->Data,P);
		elem=spListElementCreate(curr->Data->index,dist);
		spBPQueueEnqueue(bpq,elem);
		return;
	}
	if(P->data[curr->Dim] <= curr->Val){
		kNearestNeighbors(curr->Left , bpq,  P);
	}
	else{
		kNearestNeighbors(curr->Right , bpq,  P);
	}
	//TODO: ??????
	temp= (pow(curr->Val - P->data[curr->Dim],2)<0);//???
	if(!spBPQueueIsFull(bpq) || temp){
		//???
	}
}


