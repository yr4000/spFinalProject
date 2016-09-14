/*
 * KDTree.h
 *
 */

#ifndef KDTREE_H_
#define KDTREE_H_

#include "SPPoint.h"
#include "KDArray.h"
#include "SPConfig.h"
#include "SPBPriorityQueue.h"

struct sp_kd_treeNode{
	int dim;
	int val;
	struct sp_kd_treeNode *left;
	struct sp_kd_treeNode *right;
	SPPoint data;
};

typedef struct sp_kd_treeNode *KDTreeNode;

//typedef enum sp_tempi{
//	MAX_SPREAD,
//	RANDOM,
//	INCREMENTAL,
//}spKDTreeSplitMethod;

typedef enum sp_right_or_left{
	rightTree,
	leftTree,
}RorL;

/**
 * Frees all memory resources associate with KDTree.
 * destroys recursively the tree, all memory allocations are freed.
 * If KDTree == NULL nothing is done.
 */
void destroyKDTree(KDTreeNode tree);

/**
 * * A function which initializes the kd-tree with the given kdarray pointsarray.
 * @param KDArray arr - the kd-array on which the kd-tree is based on.
 * @param spKDTreeSplitMethod method - the split method (random or incremental or manual).
 * pre: arr!=NULL,method!=NULL,coor>=0
 * @return a KDTree which have been made out of the kd-array items.
 *
 */
KDTreeNode createKDTree(KDArray arr,spKDTreeSplitMethodEnum method, int coor);

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
void callCreateKDTreeRecursively(KDTreeNode res,KDArray arr, int coor, spKDTreeSplitMethodEnum method);

/**
 * * A function which searches the X closest points to the sp-point which is a feature from the
 * query image in the kd-tree tree, and stores them in sp-bp-queue q which his max capacity is
 * sp-knn.
 * @param SPBPQueue q - the data structure (a queue with capacity) in which the sp-knn most closets
 * images with respect to a single feature of a query image.
 * @param KDTreeNode tree - the kdtree in which we would like to search for the closest images.
 * @param SPPoint p- a point which represents the feature of a query image which we would like to
 * find the closests images featurs to it.
 * @return if SPBPQueue q is NULL or KDTreeNode tree is NULL or SPPoint p is NULL) return null.
 *
 */
//.
void KNNSearch(SPBPQueue q, KDTreeNode tree, SPPoint p);

//checks if tree is a leaf.
bool isLeaf(KDTreeNode tree);

#endif /* KDTREE_H_ */
