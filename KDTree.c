/*
 * KDTree.c
 *
 *  Created on: 17 баев 2016
 *      Author: Yair
 */

#include "KDTree.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//Read me: to implement the initialiser of KDTree i need to implement also searchMin
// and max point[coor] value.
// the initialisation is recursive.


void destroyKDTree(KDTreeNode tree){
	if(isLeaf(tree)){
		spPointDestroy(tree->data);
		return;
	}
	destroyKDTree(tree->left);
//	free(tree->left);		//TODO why is it no good??
	destroyKDTree(tree->right);
//	free(tree->right);
//	free(tree);
}

KDTreeNode createKDTree(KDArray arr, spKDTreeSplitMethod method,int coor){
		int dim = arr->PArr[0]->dim;
		int i;
		KDTreeNode res = (KDTreeNode)malloc(sizeof(KDTreeNode));
		if(res == NULL) return NULL;

		if(arr->arrSize==1){
			//TODO later in the func to check if leaf returned NULL
			return createLeaf(res, arr->PArr[0]);

		}

		if(method == MAX_SPREAD){
			double* coordinateSpread = createSpreadArr(arr);
			double max = 0;
			for(i=0;i<dim;i++){
				if(coordinateSpread[i]>max){
					max = coordinateSpread[i];
					coor = i;
				}
			}
			callCreateKDTreeRecursively(res,arr,coor,method);
			return res;
		}

		else if(method == RANDOM){
			coor = rand()%dim;
			callCreateKDTreeRecursively(res,arr,coor,method);
			return res;
		}
		else if(method == INCREMENTAL){
			res->dim = coor;
			res->val = getMedianOfCoor(arr,coor);
			res->data = NULL;
			KDArray* subArray = split(arr,coor);
			if(subArray == NULL) return NULL;
			res->left = createKDTree(subArray[0],method,(coor+1)%dim);
			res->right = createKDTree(subArray[1],method,(coor+1)%dim);
			destroy2DKDArray(subArray);
			return res;
		}

		return NULL; //should never reach here
}

KDTreeNode createLeaf(KDTreeNode res, SPPoint p){
	if(p==NULL || res ==NULL) return NULL;
	res->dim = -1;
	res->val = -1;
	res->left = NULL;
	res->right = NULL;
	res->data = spPointCopy(p);
	return res;
}

double findMax(SPPoint* arr, int arrSize,int coor){
	if(arr == NULL || arrSize <= 0 || coor<0) return -1;
	int i;
	double max=0;
	for(i=0;i<arrSize;i++){
		if(arr[i]->data[coor] > max){
			max = arr[i]->data[coor];
		}
	}
	return max;
}

double findMin(SPPoint* arr, int arrSize,int coor){
	if(arr == NULL || arrSize <= 0 || coor<0) return -1;
	int i;
	double min = arr[0]->data[coor];
	for(i=0;i<arrSize;i++){
		if(arr[i]->data[coor] < min){
			min = arr[i]->data[coor];
		}
	}
	return min;
}

double* createSpreadArr(KDArray arr){
	int dim = arr->PArr[0]->dim;
	int i;
	double* coordinateSpread = (double*)malloc(sizeof(double)*dim);
	if(coordinateSpread==NULL) return NULL;
	for(i=0;i<dim;i++){
		coordinateSpread[i] = findMax(arr->PArr,arr->arrSize,i)
								- findMin(arr->PArr,arr->arrSize,i);
	}
	return coordinateSpread;
}

void callCreateKDTreeRecursively(KDTreeNode res, KDArray arr, int coor, spKDTreeSplitMethod method){
	if(res == NULL || arr == NULL || coor<0 || method > 2 || method < 0) return;
	res->dim = coor;
	res->val = getMedianOfCoor(arr,coor);
	res->data = NULL;
	KDArray* subArray = split(arr,coor);
	if(subArray == NULL) return;
	res->left = createKDTree(subArray[0],method,coor);
	res->right = createKDTree(subArray[1],method,coor);
	destroy2DKDArray(subArray);
	//TODO is it right to destroy them after the use? it should be,
	//for i copy everything in this function... except create leaf
}

//TODO the indexes are not saved on the split, it will cause problems here and need to be fixed
void KNNSearch(SPBPQueue q, KDTreeNode tree, SPPoint p){
	if(q==NULL || tree==NULL || p==NULL) return;
	RorL pathTaken;

	if(isLeaf(tree)){
		spBPQueueEnqueue(q,spListElementCreate(tree->data->index,spPointL2SquaredDistance(p,tree->data)));
		return;
	}

	if(p->data[tree->dim] <= tree->val){
		KNNSearch(q,tree->left,p);
		pathTaken = leftTree;
	}
	else{
		KNNSearch(q,tree->right,p);
		pathTaken = rightTree;
	}

	if(!spBPQueueIsFull(q) ||
			(pow(tree->val-p->data[tree->dim],2))<spBPQueueMaxValue(q)){
		if(pathTaken == leftTree) KNNSearch(q,tree->right,p);
		if(pathTaken == rightTree) KNNSearch(q,tree->left,p);
	}
}

bool isLeaf(KDTreeNode tree){
	if(tree->left==NULL && tree->right==NULL
			&& tree->dim==-1 && tree->val==-1
			&& tree->data != NULL) return true;
	return false;
}
