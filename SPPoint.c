/*
 * SPPoint.c
 *
 *  Created on: 16 במאי 2016
 *      Author: Yair
 */

#include <stdlib.h>
#include <assert.h>
#include "SPPoint.h"

//struct sp_point_t{
//	double* data;
//	int dim;
//	int index;
//};


//also, might need assertion.
SPPoint spPointCreate(double* data, int dim, int index){
	if(data==NULL||dim<=0||index<0) return NULL;
	SPPoint new = (SPPoint)malloc(sizeof(*new));
	if(new==NULL)
		return NULL;
	double *copyData = (double*)malloc(sizeof(double)*dim); //Apparently this s more secure
	if(copyData==NULL)
		return NULL;
	int i;
	for(i=0;i<dim;i++){
		copyData[i]=data[i];
	}
	new->data = copyData; //the operator -> is like (*new).copyData=...
	new->dim = dim;
	new->index = index;
	return new;
}

SPPoint spPointCopy(SPPoint source){
	assert(source!=NULL);
	SPPoint copy =(SPPoint)malloc(sizeof(*copy)) ;
	if(copy==NULL)
		return NULL;
	copy->dim = source->dim;
	copy->index = source->index;
	copy->data=(double*)malloc(source->dim*sizeof(double));
	int i;
	for(i=0;i<source->dim;i++){
		copy->data[i]=source->data[i];
	}
	return copy;
}


// i havn't done malloc for them...
void spPointDestroy(SPPoint point){
	if(point!=NULL){
		free(point->data);
		free(point);
	}
}

int spPointGetDimension(SPPoint point){
	assert(point!=NULL);
	return point->dim;
}

int spPointGetIndex(SPPoint point){
	assert(point!=NULL);
	return point->index;
}

double spPointGetAxisCoor(SPPoint point, int axis){
	assert(point!=NULL && axis<point->dim);
	return point->data[axis];
}

double spPointL2SquaredDistance(SPPoint p, SPPoint q){
	assert(p!=NULL && q!=NULL && p->dim==q->dim);
	int i;
	double sum=0;
	for(i=0;i<p->dim;i++){
		sum += ((p->data[i]-q->data[i])*(p->data[i]-q->data[i]));
	}
	return sum;
}



