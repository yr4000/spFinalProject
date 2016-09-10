/*
 * DSBuilders.h
 *
 *  Created on: 10 בספט 2016
 *      Author: Yair
 */

#ifndef DSBUILDERS_H_
#define DSBUILDERS_H_

#include "../SPPoint.h"
#include "../KDArray.h"


SPPoint* createSPPointArray(double** data,int size,int dim);

double** createData(int* dim, int* size);

double** createData2(int* dim, int* size);

bool spPointCompare(SPPoint A, SPPoint B);

KDArray buildKDArray();

KDArray buildKDArray2();





#endif /* DSBUILDERS_H_ */
