/*
 * KDArray.c
 *
 *  Created on: 17 ���� 2016
 *      Author: Yair
 */



#include <stdlib.h>
#include <math.h>
#include "SPLogger.h"
#include "KDArray.h"



void destroyKDArray(KDArray arr){
	int dim = spPointGetDimension(arr->PArr[0]);
	if(arr == NULL) return;
	destroySPPointArray(arr->PArr,arr->arrSize);
	destroy2DIntArray(arr->sortedIndexesMatrix,dim);
	free(arr);

}


KDArray kdArrayInit(SPPoint * PointsArray, int arraySize){
	if(PointsArray == NULL || arraySize <= 0){
		spLoggerPrintError("kd-array creation failed due to invalid argument: Points array is null or that array size is negative",__FILE__,__func__,__LINE__);
		return NULL;
	}
	int i,j;
	int dim = spPointGetDimension(PointsArray[0]);
	KDArray res = (KDArray) malloc(sizeof(*res));
	if(res==NULL){
		spLoggerPrintError("kd-array creation failed due to Allocation failure",__FILE__,__func__,__LINE__);

		return NULL;
	}

	//copying the points array to KDArray and sorting them by index (n*logn)
	initialiseSPPointArrayForKDArray(res,PointsArray,arraySize);
	if(res->PArr==NULL) return NULL;
	qsort(res->PArr,arraySize,sizeof(SPPoint),compareSPPointByIndex);
	res->arrSize = arraySize;

	// IMPORTANT - the indexes in the matrix are not the indexes of the points,
	// but their indexes in the points array of KDArray
	res->sortedIndexesMatrix = (int**)malloc(sizeof(int*)*dim);
	if(res->sortedIndexesMatrix==NULL) return NULL;

	double** SArr = createSorting2DArray(arraySize);
	if(SArr==NULL){
		spLoggerPrintError("The creationSortion2DArray failed",__FILE__,__func__,__LINE__);

		return NULL;
	}

	//creating the matrix.
	for(i=0;i<dim;i++){
		// for each coor create an index array for the matrix and sort SArr acording to coor
		res->sortedIndexesMatrix[i] = (int*)malloc(sizeof(int)*arraySize);
		initialize2DArrayByCoor(SArr,i,res->PArr,arraySize);
		qsort(SArr,arraySize,sizeof(double*),compare2DArray); //O(d*nlogn)
		//copy each value in SArr ro his place in the matrix.
		for(j=0;j<arraySize;j++){
			res->sortedIndexesMatrix[i][j] = (int)SArr[j][0]; //O(d*n)
		}
	}
	destroy2DDoubleArray(SArr, arraySize);

	return res;
}

void initialiseSPPointArrayForKDArray(KDArray arr, SPPoint* PArr, int arraySize){
	if(arr==NULL || PArr==NULL || arraySize<=0) return;
	int i;
	arr->PArr = (SPPoint*) malloc(sizeof(SPPoint)*arraySize);
	if(arr->PArr==NULL){
		spLoggerPrintError("Allocation failure",__FILE__,__func__,__LINE__);

		return;
	}
	//initialising res->PArr.
	for(i=0; i<arraySize; i++){
		arr->PArr[i] = spPointCopy(PArr[i]);
	}
}

double** createSorting2DArray(int n){
	if(n<=0) return NULL;
	int i;
	double **res = (double**)malloc(sizeof(double)*n);
	if(res==NULL){
		spLoggerPrintError("Allocation failure",__FILE__,__func__,__LINE__);
		return NULL;
	}
	for(i=0;i<n;i++){
		res[i] = (double*)malloc(sizeof(double)*n*2);
		if(res[i]==NULL){
			spLoggerPrintError("Allocation failure",__FILE__,__func__,__LINE__);
			return NULL;
		};
	}
	return res;
}


void initialize2DArrayByCoor(double **arr, int coor,SPPoint *PArr,int sizeOfPArr){
	if(arr == NULL || coor<0 || PArr == NULL || sizeOfPArr <= 0){
		spLoggerPrintError("Invalid argument",__FILE__,__func__,__LINE__);

		return;
	}
	int i;
	for(i=0;i<sizeOfPArr;i++){
		arr[i][0] = i;
		arr[i][1] = spPointGetAxisCoor(PArr[i],coor); // for each index arr[i][1] contains the value of point i at coordinate coor.
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
	return spPointGetIndex(p1) - spPointGetIndex(p2);
}


KDArray* split(KDArray arr, int coor){
	int i,j,lc=0,rc = 0,LEFT = 1, RIGHT = 0;
	int dim =0;
	KDArray* res = (KDArray*)malloc(sizeof(KDArray)*2);
	if(res==NULL){
		spLoggerPrintError("Allocation failure",__FILE__,__func__,__LINE__);

		return NULL;
	}
	res[0] = (KDArray)malloc(sizeof(struct sp_kd_array));
	res[1] = (KDArray)malloc(sizeof(struct sp_kd_array));
	if(res[0]==NULL || res[1]==NULL){
		spLoggerPrintError("Allocation failure",__FILE__,__func__,__LINE__);

		return NULL;
	}
	int sizeL = ceil(((double)arr->arrSize)/2); //sizes of the left and right KDArrays;
	int sizeR = floor(((double)arr->arrSize)/2);

	//initialising res[0] and res[1] and initialise the data arrays for their matrixes
	if(!initialise2KDArrays(&res,sizeL,sizeR,arr)) return NULL;

	int* map = initialiseMap(arr,coor);
	if(map==NULL) return NULL;

	// here we copy the relevant SPPoints to the new KDArrays, according to the map.
	if(!splitSPPointArrayAcordingToMap(arr,map,res)) return NULL;

	dim = spPointGetDimension(arr->PArr[0]);
	for(i=0;i<dim;i++){ //for each coordinate of the points
		//first we assign the relevant pointers in the matrixes arrays to their data arrays.
		res[0]->sortedIndexesMatrix[i] = (int*)malloc(sizeof(int)*sizeL); // KDArrMatrixesData[0] +i*sizeL;
		res[1]->sortedIndexesMatrix[i] = (int*)malloc(sizeof(int)*sizeL);// KDArrMatrixesData[1] +i*sizeR;
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
	if(!fixKDArrayIndexesAfterSplit(res[0],arr->arrSize,LEFT,map)) return NULL;
	if(!fixKDArrayIndexesAfterSplit(res[1],arr->arrSize,RIGHT,map)) return NULL;
	free(map);
	return res;
}

bool initialise2KDArrays(KDArray** arr, int sizeL, int sizeR,KDArray mother){
	int dim = spPointGetDimension(mother->PArr[0]); // this is the dimension of every point

	//Here we allocating everything needed for KDArr-left
	(*arr)[0]->PArr = (SPPoint*)malloc(sizeof(SPPoint)*sizeL);
	(*arr)[0]->arrSize = sizeL;
	(*arr)[0]->sortedIndexesMatrix = (int**)malloc(sizeof(int*)*dim);

	//Here we allocating everything needed for KDArr-right
	(*arr)[1]->PArr = (SPPoint*)malloc(sizeof(SPPoint)*sizeR);
	(*arr)[1]->arrSize = sizeR;
	(*arr)[1]->sortedIndexesMatrix = (int**)malloc(sizeof(int*)*dim);

	if((*arr)[0]->PArr==NULL || (*arr)[0]->sortedIndexesMatrix==NULL
			|| (*arr)[1]->PArr==NULL || (*arr)[1]->sortedIndexesMatrix==NULL){
		spLoggerPrintError("Allocation failure",__FILE__,__func__,__LINE__);
		return false;
	}
	return true;
}

// this map is initalised based only on sortedIndexesMatrix.
// sortedIndexesMatrix is based on that that all the point in the KDArray
// are sorted by index. if they are not, it wont work.
int* initialiseMap(KDArray arr, int coor){
	if(arr == NULL || coor<0 || coor>=spPointGetDimension(arr->PArr[0])){
		spLoggerPrintError("Cannot initialize map",__FILE__,__func__,__LINE__);

		return NULL;
	}
	int i;
	int sizeL = ceil(((double)arr->arrSize)/2);
	int* map = (int*)calloc(arr->arrSize,sizeof(int));
	if(map==NULL){
		spLoggerPrintError("Allocation failure",__FILE__,__func__,__LINE__);

		return NULL;
	}
	for(i=0;i<sizeL;i++){
		map[arr->sortedIndexesMatrix[coor][i]] = 1;
	}
	return map;
}


bool splitSPPointArrayAcordingToMap(KDArray mother,int* map, KDArray* res){
	if(mother==NULL || map==NULL || res==NULL) return false;
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
	return true;
}

bool fixKDArrayIndexesAfterSplit(KDArray arr, int motherSize,int side, int* map){
	int* mapI = (int*)malloc(sizeof(int)*motherSize);
	if(mapI==NULL) return false;
	int i,j,c=0;
	int dim = spPointGetDimension(arr->PArr[0]);
	for(i=0;i<motherSize;i++){mapI[i]=-1;}
	for(i=0;i<motherSize;i++){
		if(map[i] == side){
			mapI[i]=c;
			c++;
		}
	}
	for(j=0;j<dim;j++){
		for(i=0;i<arr->arrSize;i++){
			arr->sortedIndexesMatrix[j][i] = mapI[arr->sortedIndexesMatrix[j][i]];
		}
	}
	free(mapI);
	return true;
}

double getMedianOfCoor(KDArray arr, int coor){
	int med = ceil((double)arr->arrSize/2)-1;
	int pointIndex = arr->sortedIndexesMatrix[coor][med];
	return spPointGetAxisCoor(arr->PArr[pointIndex],coor);
}

void destroy2DKDArray(KDArray* arr){
	destroyKDArray(arr[0]);
	destroyKDArray(arr[1]);
	free(arr);
}

void destroySPPointArray(SPPoint* arr, int size){
	if(arr == NULL || size<=0) return;
	int i;
	for(i=0;i<size;i++){
		spPointDestroy(arr[i]);
	}
	free(arr);
}

void destroy2DDoubleArray(double** arr, int size){
	if(arr==NULL || size<=0) return;
	int i;
	for(i=0;i<size;i++){
		free(arr[i]);
	}
	free(arr);
}

void destroy2DIntArray(int** arr, int size){
	if(arr==NULL || size<=0) return;
	int i;
	for(i=0;i<size;i++){
		free(arr[i]);
	}
	free(arr);
}

