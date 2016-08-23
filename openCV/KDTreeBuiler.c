/*
 * KDTreeBuiler.c
 *
 *  Created on: Aug 22, 2016
 *      Author: guyyt
 */


#include "KDTreeBuiler.h"
#include "SPPoint.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SPConfig.h"
#include "SPListElement.h"
#include "SPBPriorityQueue.h"


struct KD_Tree_Node
{
	 int Dim ;// = The splitting dimension
	 int Val;// = The median value of the splitting dimension
	 KDTreeNode Left;// Pointer to the left subtree
	 KDTreeNode Right;// Pointer to the right subtree
	 SPPoint Data;//Pointer to a point (only if the current node is a leaf) otherwise this field value is NULL
};


KDTreeNode CreateTreeNode(KDArray kda,SPConfig spConfig,int i){
	KDTreeNode head=malloc( sizeof(KDTreeNode));
	int diff;
	if(kda.size==1){
		head->Dim=0//invalid;
		head->Val=0//invalid;
		head->Left=NULL;
		head->Right=NULL;
		head->Data=kda[0];
	}
	else{
		head->Dim=i;
		head->Val=??;
		Data->NULL;
		if(spConfig.spKDTreeSplitMethod== MAX_SPREAD){
			head->Left=CreateTreeNode();
			head->Right=CreateTreeNode();

		}
		else if(spConfig.spKDTreeSplitMethod== RANDOM){
			head->Left=CreateTreeNode();
			head->Right=CreateTreeNode();

		}
		else if(spConfig.spKDTreeSplitMethod== INCREMENTAL ){
			head->Left=CreateTreeNode();
			head->Right=CreateTreeNode();

		}

	}
	return head;

}

void kNearestNeighbors(KDTreeNode curr,SPBPQueue bpq, SPPoint P){
	double dist;
	if(curr==NULL){
		return;
	}

	if(curr->Left==NULL && curr->Right==NULL){//is leaf

		dist=spPointL2SquaredDistance()
	}
}


