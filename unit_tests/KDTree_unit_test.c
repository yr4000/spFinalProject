/*
 * KDTree_unit_test.c
 *
 *  Created on: 17 баев 2016
 *      Author: Yair
 */

#include "../KDTree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

#include "../unit_tests/DSBuilders.h"
#include "../unit_tests/unit_test_util.h"

int COOR = 0;


void printPointsOfKDTreeToArray(KDTreeNode tree,int* counter, int* pointsIndexesLestToRight){
	if(isLeaf(tree)){
		pointsIndexesLestToRight[*counter] = tree->data->index;
		*counter = *counter+1;
		return;
	}
	printPointsOfKDTreeToArray(tree->left,counter,pointsIndexesLestToRight);
	printPointsOfKDTreeToArray(tree->right,counter,pointsIndexesLestToRight);
}

bool compareArrays(int* A, int* B, int size){
	int i;
	for(i=0;i<size;i++){
		if(A[i]!=B[i]) return false;
	}
	return true;
}

bool compareSPBPQToIntArray(SPBPQueue q, int* expected){
	int i;
	for(i=0;i<3;i++){
		SPListElement e = spBPQueuePeek(q);
		ASSERT_TRUE(e->index==expected[i]);
		spBPQueueDequeue(q);
		spListElementDestroy(e);
	}
	return true;
}

bool checkSearchByMethod(KDArray arr, spKDTreeSplitMethodEnum method,SPPoint p, int* expected){
	SPBPQueue q = spBPQueueCreate(3);
	KDTreeNode tree = createKDTree(arr,method,COOR);
	KNNSearch(q,tree,p);
	ASSERT_TRUE(compareSPBPQToIntArray(q,expected));
	spBPQueueDestroy(q);
	destroyKDTree(tree);
	return true;
}
//----------------------------------------------------------------

bool testCreateKDTree(){
	int expectedMax[] = {6,1,2,7,5,4,3};
	int expectedInc[] = {6,1,5,4,2,7,3};
	int expectedExample[] = {1,5,3,4,2};
	int counter = 0;
	KDArray arr = buildKDArray();
	int indexesArray[arr->arrSize];
	KDTreeNode tree = createKDTree(arr,MAX_SPREAD,COOR); //Manually checked, works.
	printPointsOfKDTreeToArray(tree,&counter,indexesArray);
	ASSERT_TRUE(compareArrays(expectedMax,indexesArray,arr->arrSize));
	destroyKDTree(tree);
	tree = createKDTree(arr,RANDOM,COOR);
	destroyKDTree(tree);
	tree = createKDTree(arr,INCREMENTAL,COOR);
	counter = 0;
	printPointsOfKDTreeToArray(tree,&counter,indexesArray);
	ASSERT_TRUE(compareArrays(expectedInc,indexesArray,arr->arrSize));
	destroyKDTree(tree);
	destroyKDArray(arr);

	//this is for their example
	counter = 0;
	arr = buildKDArray2();
	tree = createKDTree(arr,MAX_SPREAD,COOR);
	printPointsOfKDTreeToArray(tree,&counter,indexesArray);
	ASSERT_TRUE(compareArrays(expectedExample,indexesArray,arr->arrSize));
	destroyKDTree(tree);
	tree = createKDTree(arr,INCREMENTAL,COOR);
	counter = 0;
	printPointsOfKDTreeToArray(tree,&counter,indexesArray);
	ASSERT_TRUE(compareArrays(expectedExample,indexesArray,arr->arrSize));
	destroyKDTree(tree);
	destroyKDArray(arr);

	return true;

}

bool testKNNSearch(){
	int expectedP1[] = {2,1,4};
	int expectedP2[] = {5,4,1};
	KDArray arr = buildKDArray();
	double data1[] = {1,1};
	double data2[] = {-1,2};
	SPPoint p1 = spPointCreate(data1,2,1);
	SPPoint p2 = spPointCreate(data2,2,2);
	ASSERT_TRUE(checkSearchByMethod(arr,INCREMENTAL,p1,expectedP1));
	ASSERT_TRUE(checkSearchByMethod(arr,RANDOM,p1,expectedP1));
	ASSERT_TRUE(checkSearchByMethod(arr,MAX_SPREAD,p1,expectedP1));
	ASSERT_TRUE(checkSearchByMethod(arr,INCREMENTAL,p2,expectedP2));
	ASSERT_TRUE(checkSearchByMethod(arr,RANDOM,p2,expectedP2));
	ASSERT_TRUE(checkSearchByMethod(arr,MAX_SPREAD,p2,expectedP2));
	destroyKDArray(arr);
	spPointDestroy(p1);
	spPointDestroy(p2);
	return true;
}


//int main(){
//	RUN_TEST(testCreateKDTree);
//	RUN_TEST(testKNNSearch);
//	return 0;
//}
