/*
 * KD-Array.h
 *
 *  Created on: 31 αιεμ 2016
 *      Author: Yair
 */

#ifndef KD_ARRAY_H_
#define KD_ARRAY_H_

#include "SPPoint.h"


typedef struct sp_kd_array{
	SPPoint *PArr;
	int arrSize;
	int **sortedIndexesMatrix;
} *KDArray;

// destroys arr.
void destroyKDArray(KDArray arr);

//creates a new KDArray
KDArray init(SPPoint * PointsArray, int arraySize);

//initialise the SPPoints array of the KDArray.
void initialiseSPPointArrayForKDArray(KDArray arr, SPPoint* PArr, int arraySize);

// creates 2D array in size 2Xn
double** createSorting2DArray(int n);

// Receives 2D array where arr[i][0] is a SPPoint index.
// Initialise in arr[i][1] the value of the point[i][0]S[coor].
void initialize2DArrayByCoor(double **arr, int coor,SPPoint *PArr, int sizeOfPArr);

// comparation function for qsort
int compare2DArray(const void* A,const void* B);

// to sory points by index
int compareSPPointByIndex(const void* A,const void* B);

// this function gets a KDArray and returns a *KDArray with KDArray left
// and KDArray right
KDArray* split(KDArray arr, int coor);

//this function initialise the left and right sub-KDArrays of arr,
//and also returns two data arrays for their matrixes, uninitialised.
//this is a sub function for split.
int** initialise2KDArraysReturnData(KDArray* arr, int sizeL, int sizeR,KDArray mother);

// initialise the map needed for the function split
int* initialiseMap(KDArray arr, int coor);

//copy the relevant SPPoints to the new KDArrays, according to the map
void splitSPPointArrayAcordingToMap(KDArray mother,int* map, KDArray* res);

//fixes the indexes of the points
void fixKDArrayIndexesAfterSplit(KDArray arr, int motherSize,int side, int* map);

// gets median value of specific coor
double getMedianOfCoor(KDArray arr, int coor);

// destroy 2D KDArray
void destroy2DKDArray(KDArray* arr);

#endif /* KD_ARRAY_H_ */
