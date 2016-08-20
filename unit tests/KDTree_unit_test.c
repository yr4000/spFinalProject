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




//void printPointsOfKDTree(KDTreeNode tree,int* counter, int* pointsIndexesLestToRight){
//	if(isLeaf(tree)){
//		pointsIndexesLestToRight[counter] = tree->data->index;
//		counter++;
//	}
//}

//----------------------------------------------------------------

bool testCreateKDTree(){
	KDArray arr = buildKDArray();
	KDTreeNode tree = createKDTree(arr,MAX_SPREAD,COOR); //Manually checked, works.
	destroyKDTree(tree);
	tree = createKDTree(arr,RANDOM,COOR);
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
