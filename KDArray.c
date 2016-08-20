/*
 * KDArray.c
 *
 *  Created on: 17 ���� 2016
 *      Author: Yair
 */



#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "KDArray.h"
#include "SPPoint.h"


void destroyKDArray(KDArray arr){
	if(arr == NULL) return;
	int i;
	for(i=0;i<arr->arrSize;i++){
		spPointDestroy(arr->PArr[i]);
	}
	free(arr->PArr);
	free(*(arr->sortedIndexesMatrix));
	free(arr->sortedIndexesMatrix);
//	free(arr); //TODO why is it wrong?

}

//TODO add memory allocation failure checks
KDArray init(SPPoint * PointsArray, int arraySize){
	if(PointsArray == NULL || arraySize <= 0) return NULL;
	int i,j;
	int dim = PointsArray[0]->dim;
	KDArray res = (KDArray) malloc(sizeof(*res));

	//copying the points array to KDArray and sorting them by index (n*logn)
	initialiseSPPointArrayForKDArray(res,PointsArray,arraySize);
	qsort(res->PArr,arraySize,sizeof(SPPoint),compareSPPointByIndex);
	res->arrSize = arraySize;

	// IMPORTANT - the indexes in the matrix are not the indexes of the points,
	// but their indexes in the points array of KDArray
	res->sortedIndexesMatrix = (int**)malloc(sizeof(int*)*dim);
	int* data = (int*)malloc(sizeof(int*)*dim*arraySize); //data of the index matrix
	double** SArr = createSorting2DArray(arraySize);

	//creating the matrix.
	for(i=0;i<dim;i++){
		// for each coor create an index array for the matrix and sort SArr acording to coor
		res->sortedIndexesMatrix[i] = data+i*arraySize;
		initialize2DArrayByCoor(SArr,i,res->PArr,arraySize);
		qsort(SArr,arraySize,sizeof(double*),compare2DArray); //O(d*nlogn)
		//copy each value in SArr ro his place in the matrix.
		for(j=0;j<arraySize;j++){
			res->sortedIndexesMatrix[i][j] = (int)SArr[j][0]; //O(d*n)
		}
	}

	return res;
}

//TODO add memory allocation failure checks
void initialiseSPPointArrayForKDArray(KDArray arr, SPPoint* PArr, int arraySize){
	if(arr==NULL || PArr==NULL || arraySize<=0) return;
	int i;
	arr->PArr = (SPPoint*) malloc(sizeof(SPPoint)*arraySize); // is it fine?
	//initialising res->PArr.
	for(i=0; i<arraySize; i++){
		arr->PArr[i] = spPointCopy(PArr[i]);
	}
}

//TODO add memory allocation failure checks
double** createSorting2DArray(int n){
	if(n<=0) return NULL;
	int i;
	double **res = (double**)malloc(sizeof(double)*n);
	double *data = (double*)malloc(sizeof(double)*n*2);
	for(i=0;i<n;i++){
		res[i] = data+i*2;
	}
	return res;
}


void initialize2DArrayByCoor(double **arr, int coor,SPPoint *PArr,int sizeOfPArr){
	if(arr == NULL || coor<0 || PArr == NULL || sizeOfPArr <= 0) return;
	int i;
	for(i=0;i<sizeOfPArr;i++){
		arr[i][0] = i;
		arr[i][1] = PArr[i]->data[coor]; // for each index arr[i][1] contains the value of point i at coordinate coor.
	}
}

int compare2DArray(const void* A,const void* B){
	double* arg1 = *(double**)A;
	double* arg2 = *(double**)B;
	if(arg1[1]<arg2[1]) return -1;
	else if(arg1[1]>arg2[1]) return 1;
	else{
		if(arg1[0]<arg2[0]) return -1;
		else if(arg1[0]>arg2[0]) return 1;
	}
	return 0;
}

int compareSPPointByIndex(const void* A,const void* B){
	SPPoint p1 = *(SPPoint*)A;
	SPPoint p2 = *(SPPoint*)B;
	return p1->index - p2->index;
}


KDArray* split(KDArray arr, int coor){
	int i,j,lc=0,rc = 0,LEFT = 1, RIGHT = 0;
	KDArray* res = (KDArray*)malloc(sizeof(KDArray*)*2);
	res[0] = (KDArray)malloc(sizeof(KDArray));
	res[1] = (KDArray)malloc(sizeof(KDArray));
	int sizeL = ceil(((double)arr->arrSize)/2); //sizes of the left and right KDArrays;
	int sizeR = floor(((double)arr->arrSize)/2);

	//initialising res[0] and res[1] and initialise the data arrays for their matrixes
	int** KDArrMatrixesData = initialise2KDArraysReturnData(res,sizeL,sizeR,arr);
	int* map = initialiseMap(arr,coor);

	// here we copy the relevant SPPoints to the new KDArrays, according to the map.
	splitSPPointArrayAcordingToMap(arr,map,res);

	for(i=0;i<arr->PArr[0]->dim;i++){ //for each coordinate of the points
		//first we assign the relevant pointers in the matrixes arrays to their data arrays.
		res[0]->sortedIndexesMatrix[i] = KDArrMatrixesData[0] +i*sizeL;
		res[1]->sortedIndexesMatrix[i] = KDArrMatrixesData[1] +i*sizeR;
		for(j=0;j<arr->arrSize;j++){	//for each point in arr:
			// if the the map at index arr->sortedIndexesMatrix[i][j] shows 1,
			// assign this index to the left KDArray sortedIndexesMatrix.
			// we know that the right sorted order will be remained and thats why it will work.
			if(map[arr->sortedIndexesMatrix[i][j]]==1){
				res[0]->sortedIndexesMatrix[i][lc] = arr->sortedIndexesMatrix[i][j];
				lc++;
			}
			// else, assign it to the right KDArray sortedIndexesMatrix.
			else{
				res[1]->sortedIndexesMatrix[i][rc] = arr->sortedIndexesMatrix[i][j];
				rc++;
			}
		}
		lc=rc=0;
	}

	// here we fix the indexes of the matrixes, for later splits
	fixKDArrayIndexesAfterSplit(res[0],arr->arrSize,LEFT,map);
	fixKDArrayIndexesAfterSplit(res[1],arr->arrSize,RIGHT,map);
	free(map);
	return res;
}

//TODO: add memory allocation failure checks
int** initialise2KDArraysReturnData(KDArray* arr, int sizeL, int sizeR,KDArray mother){
	int dim = mother->PArr[0]->dim; // this is the dimension of every point
	int **res = (int**)malloc(sizeof(int*)*2);
	//Here we allocating everything needed for KDArr-left
	arr[0]->PArr = (SPPoint*)malloc(sizeof(SPPoint)*sizeL);
	arr[0]->arrSize = sizeL;
	arr[0]->sortedIndexesMatrix = (int**)malloc(sizeof(int*)*dim);
	res[0] = (int*)malloc(sizeof(int)*sizeL*dim);

	//Here we allocating everything needed for KDArr-right
	arr[1]->PArr = (SPPoint*)malloc(sizeof(SPPoint)*sizeR);
	arr[1]->arrSize = sizeR;
	arr[1]->sortedIndexesMatrix = (int**)malloc(sizeof(int*)*dim);
	res[1] = (int*)malloc(sizeof(int)*sizeR*dim);

	return res;
}

//TODO: add memory allocation failure checks.
//TODO: this implementation won't work the second time. fixed?
// this map is initalised based only on sortedIndexesMatrix.
// sortedIndexesMatrix is based on that that all the point in the KDArray
// are sorted by index. if they are not, it wont work.
int* initialiseMap(KDArray arr, int coor){
	if(arr == NULL || coor<0 || coor>=arr->PArr[0]->dim) return NULL;
	int i;
	int sizeL = ceil(((double)arr->arrSize)/2);
	int* map = (int*)calloc(arr->arrSize,sizeof(int));
	for(i=0;i<sizeL;i++){
		map[arr->sortedIndexesMatrix[coor][i]] = 1; //for this to work i need to change the points indexes.
	}
	return map;
}

//TODO add if NULL then return false..
void splitSPPointArrayAcordingToMap(KDArray mother,int* map, KDArray* res){
	int i,lc=0,rc = 0;
	for(i=0;i<mother->arrSize;i++){
		if(map[i]==1){
			res[0]->PArr[lc] = spPointCopy(mother->PArr[i]);
			lc++;
		}
		else{
			res[1]->PArr[rc] = spPointCopy(mother->PArr[i]);
			rc++;
		}
	}
}

//TODO add memory allocation failure
void fixKDArrayIndexesAfterSplit(KDArray arr, int motherSize,int side, int* map){
	int* mapI = (int*)malloc(sizeof(int)*motherSize);
	int i,j,c=0;
	int dim = arr->PArr[0]->dim;
	for(i=0;i<motherSize;i++){mapI[i]=-1;}
	for(i=0;i<motherSize;i++){
		if(map[i] == side){
			mapI[i]=c;
			//arr->PArr[i]->index=c; //wrong and MEYUTAR
			c++;
		}
	}
	for(j=0;j<dim;j++){
		for(i=0;i<arr->arrSize;i++){
				arr->sortedIndexesMatrix[j][i] = mapI[arr->sortedIndexesMatrix[j][i]];
		}
	}
	free(mapI);
}

double getMedianOfCoor(KDArray arr, int coor){
	int med = ceil((double)arr->arrSize/2)-1;
	int pointIndex = arr->sortedIndexesMatrix[coor][med];
	return arr->PArr[pointIndex]->data[coor];
}

void destroy2DKDArray(KDArray* arr){
	destroyKDArray(arr[0]);
	destroyKDArray(arr[1]);
	free(arr);
}