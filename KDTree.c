/*
 * KDTree.c
 *
 *  Created on: 17 ���� 2016
 *      Author: Yair
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "SPLogger.h"
#include "KDTree.h"

void destroyKDTree(KDTreeNode tree){
	if(tree==NULL) return;
	if(isLeaf(tree)){
		spPointDestroy(tree->data);
		free(tree);
		return;
	}
	destroyKDTree(tree->left);
	destroyKDTree(tree->right);
	free(tree);
}

KDTreeNode createKDTree(KDArray arr, spKDTreeSplitMethodEnum method,int coor){
		int dim = arr->PArr[0]->dim;
		int i;
		KDTreeNode res = (KDTreeNode)malloc(sizeof(struct sp_kd_treeNode));
		if(res == NULL){
			spLoggerPrintError("Kd-tree creation failed due to allocation failure",__FILE__,__func__,__LINE__);
							spLoggerDestroy();
			return NULL;
		}

		if(arr->arrSize==1){
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
			free(coordinateSpread);
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

		spLoggerPrintError("spKDTreeSplitMethod method is not MAX_SPREAD, INCREMENTAL, RANDOM ",__FILE__,__func__,__LINE__);
						spLoggerDestroy();
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

void callCreateKDTreeRecursively(KDTreeNode res, KDArray arr, int coor, spKDTreeSplitMethodEnum method){
	if(res == NULL || arr == NULL || coor<0 || method > 2 || method < 0){
		return;
	}
	res->dim = coor;
	res->val = getMedianOfCoor(arr,coor);
	res->data = NULL;
	KDArray* subArray = split(arr,coor);
	if(subArray == NULL) return;
	res->left = createKDTree(subArray[0],method,coor);
	res->right = createKDTree(subArray[1],method,coor);
	destroy2DKDArray(subArray);

}

void KNNSearch(SPBPQueue q, KDTreeNode tree, SPPoint p){
	if(q==NULL || tree==NULL || p==NULL){
		return;
	}
	RorL pathTaken;

	if(isLeaf(tree)){
		SPListElement e = spListElementCreate(spPointGetIndex(tree->data),spPointL2SquaredDistance(p,tree->data));
		spBPQueueEnqueue(q,e);
		spListElementDestroy(e);
		return;
	}

	if(spPointGetAxisCoor(p,tree->dim)<= tree->val){
		KNNSearch(q,tree->left,p);
		pathTaken = leftTree;
	}
	else{
		KNNSearch(q,tree->right,p);
		pathTaken = rightTree;
	}

	if(!spBPQueueIsFull(q) ||
			(pow((tree->val)-spPointGetAxisCoor(p,tree->dim),2))<spBPQueueMaxValue(q)){
		if(pathTaken == leftTree) KNNSearch(q,tree->right,p);
		else KNNSearch(q,tree->left,p);
	}
}

bool isLeaf(KDTreeNode tree){
	if(tree->left==NULL && tree->right==NULL
			&& tree->dim==-1 && tree->val==-1
			&& tree->data != NULL) return true;
	return false;
}
