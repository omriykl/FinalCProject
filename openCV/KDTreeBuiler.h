/*
 * KDTreeBuiler.h
 *
 *  Created on: Aug 22, 2016
 *      Author: guyyt
 */

#ifndef KDTREEBUILER_H_
#define KDTREEBUILER_H_



#endif /* KDTREEBUILER_H_ */

typedef struct KD_Tree_Node* KDTreeNode;

KDTreeNode CreateTreeNode(SPKDArray kda,SPConfig spConfig,int incNextDim);

SPBPQueue FindkNearestNeighbors(KDTreeNode curr,SPPoint P,SPConfig conf);
