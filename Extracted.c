/*
 * Extracted.c
 *
 *  Created on: 22 баев 2016
 *      Author: Yair
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Extracted.h"
#include "SPConfig.h"
#include "SPPoint.h"

int MAX_FILE_NAME_LENGTH = 1024;

//this function gets an array of points, the images index, a config object and
//the number of features (the size of the array), and generates a feat file with the
//data inside the points array.
//pre: arr!=NULL, index>=0, config!=NULL,numOfFeatures>=0
//pro: sends success message and create feat file.
SP_EXTRACTED_MSG initExtractionMode(SPPoint* arr, int index, SPConfig config,int numOfFeatues){
	if(arr==NULL || index<0 || config==NULL || numOfFeatues<0) return SP_EXTRACT_INVALID_ARGUMENT;
	int i,j;
	char* fileName = getFeatsFileName(config,index);
	if(fileName==NULL) return SP_EXTRACT_FAILED_GENERATE_FILEPATH;
	FILE* f = fopen(fileName,"w");
	if(f==NULL) return SP_EXTRACT_FAILED_TO_OPEN_FILE;
	// at the start of the file we will save the number of features in it
	fprintf(f,"%d ", numOfFeatues);
	//for each feature, we save its dimention and then its data in order.
	for(i=0;i<numOfFeatues;i++){
		fprintf(f,"%d ",arr[i]->dim);
		for(j=0;j<arr[i]->dim;j++){
			fprintf(f,"%f ",arr[i]->data[j]);//TODO getter
		}
	}
	fclose(f);
	return SP_EXTRACT_SUCCESS;
}

//TODO: check what happened if fscanf reach to EOF.
// generally i am not sure i used it right...
//this function extract the data from a feat file of the relevant image to the index,
// into the unintialised SPPoint* arr;
SP_EXTRACTED_MSG initNonExtractionMode(SPPoint** arr,int index, SPConfig config,int* numberOfFeatures){
	if(arr==NULL || index<0 || config==NULL) return SP_EXTRACT_INVALID_ARGUMENT;
	int i,j,dim;
	double d;
	double* data;
	char* filePath = getFeatsFileName(config,index);
	if(filePath==NULL) return SP_EXTRACT_FAILED_GENERATE_FILEPATH;
	FILE* f = fopen(filePath,"r");
	if(f==NULL) return SP_EXTRACT_FAILED_TO_OPEN_FILE;

	//writes the number of features into numberOfFeaturs
	fscanf(f,"%d",numberOfFeatures);
	*arr = (SPPoint*)malloc(sizeof(SPPoint)*(*numberOfFeatures));//TODO why didn't it worked the usual way? why pointer to pointer?
	for(i=0;i<*numberOfFeatures;i++){
		if(ftell(f)==EOF) return SP_EXTRACT_FILE_CONTENT_DOESNT_MATCH_THE_FORMAT;
		fscanf(f,"%d",&dim); //initialise dim
		data = (double*)malloc(sizeof(double)*dim);
		for(j=0;j<dim;j++){ //initialise data
			if(ftell(f)==EOF) return SP_EXTRACT_FILE_CONTENT_DOESNT_MATCH_THE_FORMAT;
			fscanf(f,"%lf",&data[j]);
		}
		(*arr)[i] = spPointCreate(data,dim,index);
		free(data);
	}
	free(filePath);
	fclose(f);
	return SP_EXTRACT_SUCCESS;
}

char* getFeatsFileName(SPConfig config,int index){
	if(config==NULL || index<0) return NULL;
	char* filePath = (char*)calloc(MAX_FILE_NAME_LENGTH,sizeof(char));
	if(filePath==NULL) return NULL;
	char strIndex[MAX_FILE_NAME_LENGTH];
	strcat(filePath,config->spImagesDirectory); //spImagesDirectory
	strcat(filePath,config->spImagesPrefix);//spImagesPrefix
	itoa(index,strIndex,10);
	strcat(filePath,strIndex);
//	strcat(filePath,"%d");
//	sprintf(filePath,filePath,index);//add index TODO problem here
	strcat(filePath,".feats\0");
	//printf("%s",filePath); //for test
	return filePath;

}
