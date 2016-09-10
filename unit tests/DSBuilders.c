/*
 * DSBuilders.c
 *
 *  Created on: 10 בספט 2016
 *      Author: Yair
 */

#include "DSBuilders.h"
#include "../SPPoint.h"
#include "../KDArray.h"
#include "unit_test_util.h"
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

// this function will create data for all the tests
double** createData(int* dim, int* size){
	double funcdata[] = {0,0,1,0,2,5,0,3,-1,2,-1,-1,5,-2};
	int sizeFD = 14;
	int i;
	*size = 7;
	*dim = 2;
	double** data = (double**)malloc(sizeof(double*)*(*size));
//	double* datadata = (double*)malloc(sizeof(double)*sizeFD);
	for(i=0; i<(*size); i++){
		data[i] = (double*)malloc(sizeof(double)*(*dim));
	}

	for(i=0; i<sizeFD; i++){
		data[i/(*dim)][i%(*dim)] = funcdata[i];
	}
	return data;
}

double** createData2(int* dim, int* size){
	double funcdata[] = {1,2,123,70,2,7,9,11,3,4};
	int sizeFD = 10;
	int i;
	*size = 5;
	*dim = 2;
	double** data = (double**)malloc(sizeof(double*)*(*size));
//	double* datadata = (double*)malloc(sizeof(double)*sizeFD);
	for(i=0; i<(*size); i++){
		data[i] = (double*)malloc(sizeof(double)*(*dim));
	}

	for(i=0; i<sizeFD; i++){
		data[i/(*dim)][i%(*dim)] = funcdata[i];
	}
	return data;
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
	destroy2DDoubleArray(data,size);
	destroySPPointArray(PArr,size);
	return arr;
}

KDArray buildKDArray2(){
	int dim, size;
	double** data = createData2(&dim,&size);
	SPPoint* PArr = createSPPointArray(data,size,dim);
	KDArray arr = kdArrayInit(PArr,size);
	destroy2DDoubleArray(data,size);
	destroySPPointArray(PArr,size);
	return arr;
}

