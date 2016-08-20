/*
 * KDTree.h
 *
 *  Created on: 9 баев 2016
 *      Author: Yair
 */

#ifndef KDTREE_H_
#define KDTREE_H_

#include "KDArray.h"
#include "SPPoint.h"
#include "SPBPriorityQueue.h"


typedef struct sp_kd_treeNode *KDTreeNode;

struct sp_kd_treeNode{
	int dim;
	int val;
	KDTreeNode left;
	KDTreeNode right;
	SPPoint data;
};

typedef enum sp_tempi{
	MAX_SPREAD,
	RANDOM,
	INCREMENTAL,
}spKDTreeSplitMethod;

typedef enum sp_right_or_left{
	rightTree,
	leftTree,
}RorL;

void destroyKDTree(KDTreeNode tree);

KDTreeNode createKDTree(KDArray arr,spKDTreeSplitMethod method, int coor);

KDTreeNode createLeaf(KDTreeNode res,SPPoint p);

double findMax(SPPoint* arr, int arrSize, int coor);

double findMin(SPPoint* arr, int arrSize, int coor);

double* createSpreadArr(KDArray arr);

void callCreateKDTreeRecursively(KDTreeNode res,KDArray arr, int coor, spKDTreeSplitMethod method);

void KNNSearch(SPBPQueue q, KDTreeNode tree, SPPoint p);

bool isLeaf(KDTreeNode tree);

#endif /* KDTREE_H_ */
