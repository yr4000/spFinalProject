/*
 * KDTree.h
 *
 *  Created on: 9 баев 2016
 *      Author: Yair
 */

#ifndef KDTREE_H_
#define KDTREE_H_

#include "KDArray.h"
#include "SPConfig.h"
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

//typedef enum sp_tempi{
//	MAX_SPREAD,
//	RANDOM,
//	INCREMENTAL,
//}spKDTreeSplitMethod;

typedef enum sp_right_or_left{
	rightTree,
	leftTree,
}RorL;

//destroys recursively the tree.
//pre - i think it take care of everything, not needed
//post = all mem aloc are freed.
void destroyKDTree(KDTreeNode tree);

// pre: arr!=NULL,method!=NULL,coor>=0
//pro: a KDTree made out of arr items (EIVARIM)
KDTreeNode createKDTree(KDArray arr,spKDTreeSplitMethod method, int coor);

//pre: p!=NULL,res!=NULL
KDTreeNode createLeaf(KDTreeNode res,SPPoint p);

//finds the greatest value in arr
double findMax(SPPoint* arr, int arrSize, int coor);

//finds the smallest value in arr
double findMin(SPPoint* arr, int arrSize, int coor);

//FFC = Func For Create, creates an array to find according to which coordinate
// to split if method is MAX_SPREAD
double* createSpreadArr(KDArray arr);

//FFS only, for easier debugging.
void callCreateKDTreeRecursively(KDTreeNode res,KDArray arr, int coor, spKDTreeSplitMethod method);

//searches the X closest points to p in tree, and stores them in q.
void KNNSearch(SPBPQueue q, KDTreeNode tree, SPPoint p);

//checks if tree is a leaf.
bool isLeaf(KDTreeNode tree);

#endif /* KDTREE_H_ */
