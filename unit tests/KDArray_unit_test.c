/*
 * KDArray_unit_test.c
 *
 *  Created on: 17 баев 2016
 *      Author: Yair
 */


#include "../KDArray.h"
#include "unit_tests/unit_test_util.h"
#include "SPPoint.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>


SPPoint* createSPPointArray(double** data,int size,int dim){
	SPPoint* res = (SPPoint*)malloc(sizeof(SPPoint)*size);
	int i;
	for(i=0;i<size;i++){
		res[i] = spPointCreate(data[size-i-1],dim,size-i);
	}
	return res;
}

//void destroySPPointArray(SPPoint* arr, int size){
//	if(arr == NULL || size<=0) return;
//	int i;
//	for(i=0;i<size;i++){
//		spPointDestroy(arr[i]);
//	}
//	free(arr);
//}

// this function will create data for all the tests
double** createData(int* dim, int* size){
	double funcdata[] = {0,0,1,0,2,5,0,3,-1,2,-1,-1,5,-2};
	int sizeFD = 14;
	int i;
	*size = 7;
	*dim = 2;
	double** data = (double**)malloc(sizeof(double*)*(*size));
	double* datadata = (double*)malloc(sizeof(double)*sizeFD);
	for(i=0; i<sizeFD; i++){
		datadata[i] = funcdata[i];
	}
	for(i=0; i<(*size); i++){
		data[i] = datadata + i*(*dim);
	}
	return data;
}

void destroyData(double** data){
	if(data!=NULL){
	free(*data);
	free(data);
	}
}

bool spPointCompare(SPPoint A, SPPoint B){
	if(A == NULL && B == NULL) return true;
	int i;
	if(A->dim != B->dim) return false;
	if(A->index != B->index) return false;
	for(i=0;i<A->dim;i++){
		if(A->data[i]!=B->data[i]) return false;
	}
	return true;
}

KDArray buildKDArray(){
	int dim, size;
	double** data = createData(&dim,&size);
	SPPoint* PArr = createSPPointArray(data,size,dim);
	KDArray arr = kdArrayInit(PArr,size);
	destroyData(data);
	destroySPPointArray(PArr,size);
	return arr;
}

//------------------------------------------------------------------------
//bool testDestroyKDArray(){
//	int dim, size;
//	double** data = createData(&dim,&size);
//	SPPoint* PArr = createSPPointArray(data,size,dim);
//	KDArray arr = kdArrayInit(PArr,size);
//	destroyKDArray(arr);
//	free(arr);
//	destroySPPointArray(PArr,size);
//	destroyData(data);
//	return true;
//}

bool testInit(){
int dim, size,i,j;
int testArr[] = {4,5,0,3,1,2,6,6,5,0,1,4,3,2};
ASSERT_TRUE(kdArrayInit(NULL,1)==NULL);
double** data = createData(&dim,&size);
SPPoint* PArr = createSPPointArray(data,size,dim);
ASSERT_TRUE(kdArrayInit(PArr,0)==NULL);
KDArray arr = kdArrayInit(PArr,size);
for(i=0;i<size;i++){
	ASSERT_TRUE(spPointCompare(PArr[size-1-i],arr->PArr[i]));
}
ASSERT_TRUE(arr->arrSize==size);
for(i=0;i<dim;i++){
	for(j=0;j<size;j++){
		ASSERT_TRUE(arr->sortedIndexesMatrix[i][j] == testArr[i*size + j]);
	}
}
destroyData(data);
destroySPPointArray(PArr,size);
destroyKDArray(arr);
free(arr);
return true;
}

//TODO: doesn't check NULL, better think of all the possible cases...
bool testInitialiseSPPointArrayForKDArray(){
	int i,j,dim,size;
	double** data = createData(&dim,&size);
//	printf("%f\n",data[2][1]);

	SPPoint* ps = createSPPointArray((double**)data,size,dim);
	KDArray arr = (KDArray) malloc(sizeof(KDArray));
	initialiseSPPointArrayForKDArray(arr,ps,size);
	for(i=0; i<size; i++){
		for(j=0;j<dim;j++){
			ASSERT_TRUE(arr->PArr[size-i-1]->data[j]==data[i][j]);
		}
	}
	destroyData(data);
	destroySPPointArray(ps,size);
	destroySPPointArray(arr->PArr,size);
	free(arr);
	return true;
}

bool testCreateSorting2DArray(){
	int i;
	ASSERT_TRUE(createSorting2DArray(0)==NULL);
	double** arr = createSorting2DArray(5);
	for(i=0;i<10;i++){
		arr[i/2][i%2] = i;
	}
	ASSERT_TRUE(arr[1][1]=3);
	ASSERT_TRUE(arr[4][0]=9);
	destroyData(arr);
	return true;
}

bool testInitialize2DArrayByCoor(){
	int dim,size,i,coor;
	double** data = createData(&dim,&size);
	SPPoint* PArr = createSPPointArray(data,size,dim);
	double** SArr = createSorting2DArray(size);
	for(coor=0;coor<dim;coor++){
		initialize2DArrayByCoor(SArr,coor,PArr,size);
		for(i=0;i<size;i++){
			ASSERT_TRUE(SArr[i][0]==i);
			ASSERT_FALSE(SArr[i][1]!=PArr[i]->data[coor]);
		}
	}
	destroySPPointArray(PArr,size);
	destroyData(data);
	destroyData(SArr);
	return true;
}

bool testCompare2DArray(){
	int dim,size;
	double** arr = createData(&dim,&size);
	ASSERT_TRUE(compare2DArray(&arr[2],&arr[3])== 1);
	ASSERT_TRUE(compare2DArray(&arr[3],&arr[2])== -1);
	ASSERT_TRUE(compare2DArray(&arr[2],&arr[2])== 0);
	destroyData(arr);
	return true;

}

bool testSplit(){
	int i,j,k,coor,counter = 0;
	KDArray mother = buildKDArray();
//	int expected[][8] = {{4,5,0,3,5,0,4,3},{1,2,6,6,1,2},{5,0,1,6,6,5,0,1},{4,3,2,4,3,2}};
	int expected[][8] = {{2,3,0,1,3,0,2,1},{0,1,2,2,0,1},{2,0,1,3,3,2,0,1},{2,1,0,2,1,0}};

	for(coor=0;coor<mother->PArr[0]->dim;coor++){
		KDArray* res = split(mother,coor);
		for(k=0;k<2;k++){
			for(j=0;j<mother->PArr[0]->dim;j++){
				for(i=0;i<res[k]->arrSize;i++){
					ASSERT_TRUE(res[k]->sortedIndexesMatrix[j][i]
					             == expected[counter][j*res[k]->arrSize+i]);
				}
			}
			counter++;
		}
		destroyKDArray(res[0]);//TODO problem here - cant free res1 and res0 same time
		destroyKDArray(res[1]);
		free(res[0]); //cant do that without the program to collapse. claims that i got out of the borders of the heap?!
//		free(res[1]);// this also.
		free(res);
	}
	destroyKDArray(mother);
	free(mother);
	return true;
}

bool testSplitSPPointArrayAcordingToMap(){
	int coor,i;
	KDArray mother = buildKDArray();
	KDArray* arr = (KDArray*)malloc(sizeof(KDArray)*2);
//	printf("%d\n",sizeof(KDArray));
	arr[0] = (KDArray)malloc(sizeof(KDArray));
	arr[1] = (KDArray)malloc(sizeof(KDArray));
//	free(arr[0]); // here free work fine.
//	free(arr[1]);
//	free(arr);
	int sizeL = ceil(((double)mother->arrSize)/2);
	int sizeR = floor(((double)mother->arrSize)/2);
	int** KDArrMatrixesData = initialise2KDArraysReturnData(&arr,sizeL,sizeR,mother);
//	destroyKDArray(arr[1]);
//	free(arr[1]);

	for(coor = 0;coor<mother->PArr[0]->dim;coor++){
		int* map = initialiseMap(mother,coor);
		splitSPPointArrayAcordingToMap(mother,map,arr);
		for(i=0;i<sizeL;i++){
			ASSERT_TRUE(map[arr[0]->PArr[i]->index-1]==1);
		}
		for(i=0;i<sizeR;i++){
			ASSERT_TRUE(map[arr[1]->PArr[i]->index-1]==0);
		}
		free(map);
	}
	destroyKDArray(mother);
	free(mother);
	destroyKDArray(arr[0]); //TODO same problem here
//	destroyKDArray(arr[1]);
//		free(arr[0]);
//		free(arr[1]);
//	free(arr); //TODO there is some kind of a problem here - i can't do free(arr[0])
	//send Moab email...
	//https://www.google.co.il/webhp?sourceid=chrome-instant&ion=1&espv=2&ie=UTF-8#q=No+source+available+for+%22__mingw_CRTStartup()+at
	free(*KDArrMatrixesData);
//	free(KDArrMatrixesData); //TODO problem here
	return true;
}
// TODO right now i think i might delete it.
//bool testInitialise2KDArraysReturnData(){
//
//}

bool testInitialiseMap(){
	int i,j;
	KDArray arr = buildKDArray();
	int* map;
	ASSERT_TRUE(initialiseMap(NULL,1) == NULL);
	ASSERT_TRUE(initialiseMap(arr,-1)==NULL);
	ASSERT_TRUE(initialiseMap(arr,3)==NULL);
	for(i=0;i<arr->PArr[0]->dim;i++){
		map = initialiseMap(arr,i);
		for(j=0;j<arr->arrSize;j++){
			if(j<ceil((double)arr->arrSize/2)){
				ASSERT_TRUE(map[arr->sortedIndexesMatrix[i][j]]==1);
			}
			else{
				ASSERT_TRUE(map[arr->sortedIndexesMatrix[i][j]]==0);
			}
		}
		free(map);
	}
	destroyKDArray(arr);
	free(arr);
	return true;
}

bool testDestroyKDArray(){
	KDArray mother = buildKDArray();
	KDArray* children = split(mother,0);
	destroyKDArray(children[0]);
	destroyKDArray(children[1]);
	free(children[0]);
	free(children[1]);
	free(children);
	destroyKDArray(mother);
	free(mother);
	return true;
}

//int main(){
//	RUN_TEST(testInitialiseSPPointArrayForKDArray);
//	RUN_TEST(testCreateSorting2DArray);
//	RUN_TEST(testInitialize2DArrayByCoor);
//	RUN_TEST(testCompare2DArray);
//	RUN_TEST(testInit);
////	RUN_TEST(testDestroyKDArray);
//	RUN_TEST(testInitialiseMap);
//	RUN_TEST(testSplitSPPointArrayAcordingToMap);
//	RUN_TEST(testSplit);
////	RUN_TEST(testDestroyKDArray);
//	return 0;
//}


