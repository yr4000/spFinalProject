/*
 * KD-Array.h
 *
 *  Created on: 31 ���� 2016
 *      Author: Yair
 */

#ifndef KD_ARRAY_H_
#define KD_ARRAY_H_

#include "SPPoint.h"
#include <math.h>
#include <stdbool.h>

// KD array is a data structure which supports the KDTree data structure which is used
// for finding the closest images to a quary image. includes three fields: SPPoint *PArr,
// int arrSize, and int **sortedIndexesMatrix.
typedef struct sp_kd_array {
	SPPoint *PArr;
	int arrSize;
	int **sortedIndexesMatrix;
}* KDArray;

/**
 * Frees all memory resources associate with KDArray. 
 * If KDArray == NULL nothing is done.
 */
void destroyKDArray(KDArray arr);

/**
 * Creates a new KDArray struct. 
 * Initializes the kd-array with the data given by arr. The complexity of this operation is o(d*nlog(n)).
 * @param SPPoint * PointsArray - the array of SPPoints.
 * @param int arraySize - the size of the array (PointsArray).
 * @return NULL in case an error occurs: PointsArray is null, arraySize is negative
 * integer, allocation failiure. Otherwise returns the KDArray which had been created.
 * 
 */
KDArray kdArrayInit(SPPoint * PointsArray, int arraySize);

/**initialise the SPPoints array for the KDArray by copying the points array to KDArray
 * and sorting them by index. Time complexity is n*logn as expected.
 *
 * @param KDArray arr - the KDArray which had been generated by kdArrayInit.
 * @param SPPoint* PArr - the points array the init function recieved.
 * @param int arraySize - the size of the array (PointsArray).
 * @return NULL in the cases: arr==NULL, or PArr==NULL, arraySize<=0 return.
 * otherwise doesn't returns anything (void function).
 *
 */
void initialiseSPPointArrayForKDArray(KDArray arr, SPPoint* PArr, int arraySize);

/** A function which creates 2D array in size 2Xn to sort the relevant indexes.
 *
 * @param int n - array size which was given in the init function.
 * @return NULL in the cases: SArr==NULL, or data ==NULL.
 * otherwise returns double** res which called SArr.
 *
 */
double** createSorting2DArray(int n);

/** A function which receives 2D array where for each index i,
 * arr[i][1] contains the value of point i at coordinate coor arr[i][0].
 *
 * @param double **arr - The array SArr which the init function uses.
 * @param int coor - the integer i which is looping in a for loop in the init function.
 * @param SPPoint *PArr - res->PArr of the init function.
 * @param int sizeOfPArr - the array size.
 * @return NULL in the cases: arr == NULL, or coor<0, or PArr == NULL, or sizeOfPArr <= 0
 * otherwise doesn't return anything (void function).
 *
 */

void initialize2DArrayByCoor(double **arr, int coor, SPPoint *PArr,
		int sizeOfPArr);

// This function is a comparation function for qsort.
/** A function which receives 2D arrays and compares them by value,
 * arr[i][1] contains the value of point i at coordinate coor arr[i][0].
 *
 * @param const void* A - The first 2D array for compare.
 * @param const void* B - the second 2D array for compare.
 * @return -1 if A value is less than B, 1 otherwise and if their values are equivalent,
 * compare between their arr[0].
 *
 */
int compare2DArray(const void* A, const void* B);

/** to sort points by index
* This function is for sorting points by index
* the function two points and compares between them based on their indexes only.
 *
 * @param const void* A - The first point.
 * @param const void* B - the second point.
 * @return -1 if A index is less than B, 1 otherwise and returns 0 if theirs indexes are equivalent.
 *
 */
int compareSPPointByIndex(const void* A, const void* B);

/** this function gets a KDArray and returns a *KDArray with KDArray left
* and KDArray right, such that the first n/2 points with respect to the
* coordinate coor are now in KDArray left, and the rest n/2 of the points are now in
* KDArray right.
*/
KDArray* split(KDArray arr, int coor);

//this function initialise the left and right sub-KDArrays of arr,
//and also returns two data arrays for their matrixes, uninitialised.
//this is a sub function for split.
int** initialise2KDArraysReturnData(KDArray** arr, int sizeL, int sizeR,
		KDArray mother);

// initialise the map needed for the function split
int* initialiseMap(KDArray arr, int coor);

//copy the relevant SPPoints to the new KDArrays, according to the map
bool splitSPPointArrayAcordingToMap(KDArray mother, int* map, KDArray* res);

//fixes the indexes of the points, means makes the indexes in the
//new matrix be similiar to the points index of their place in the sub
//KDArray.
bool fixKDArrayIndexesAfterSplit(KDArray arr, int motherSize, int side,
		int* map);

// gets median value of specific coor
double getMedianOfCoor(KDArray arr, int coor);

// destroy 2D KDArray
void destroy2DKDArray(KDArray* arr);

// destroys SSPoints array
void destroySPPointArray(SPPoint* arr, int size);

#endif /* KD_ARRAY_H_ */
