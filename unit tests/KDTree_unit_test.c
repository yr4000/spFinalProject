/*
 * KDTree_unit_test.c
 *
 *  Created on: 17 баев 2016
 *      Author: Yair
 */

#include "../KDTree.h"
#include "unit_tests/unit_test_util.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

int COOR = 0;


void printPointsOfKDTree(KDTreeNode tree,int* counter, int* pointsIndexesLestToRight){
	if(isLeaf(tree)){
		pointsIndexesLestToRight[*counter] = tree->data->index;
		*counter = *counter+1;
		return;
	}
	printPointsOfKDTree(tree->left,counter,pointsIndexesLestToRight);
	printPointsOfKDTree(tree->right,counter,pointsIndexesLestToRight);
}

bool compareArrays(int* A, int* B, int size){
	int i;
	for(i=0;i<size;i++){
		if(A[i]!=B[i]) return false;
	}
	return true;
}
//----------------------------------------------------------------

bool testCreateKDTree(){
	int expectedMax[] = {6,1,2,7,5,4,3};
	int expectedInc[] = {6,1,5,4,2,7,3};
	int counter = 0;
	KDArray arr = buildKDArray();
	int* indexesArray[arr->arrSize];
	KDTreeNode tree = createKDTree(arr,MAX_SPREAD,COOR); //Manually checked, works.
	printPointsOfKDTree(tree,&counter,indexesArray);
	ASSERT_TRUE(compareArrays(expectedMax,indexesArray,arr->arrSize));
	destroyKDTree(tree);
	tree = createKDTree(arr,RANDOM,COOR);
	destroyKDTree(tree);
	tree = createKDTree(arr,INCREMENTAL,COOR);
	counter = 0;
	printPointsOfKDTree(tree,&counter,indexesArray);
	ASSERT_TRUE(compareArrays(expectedInc,indexesArray,arr->arrSize));
	destroyKDTree(tree);
	return true;

}

bool testCreateLeaf(){

}

bool testFindMax(){

}

bool testFindMin(){

}

bool testCreateSpreadArr(){

}

bool testCallCreateKDTreeRecursively(){

}

bool testKNNSearch(){

}

bool testIsLeaf(){

}

int main(){
	RUN_TEST(testCreateKDTree);
	return 0;
}
