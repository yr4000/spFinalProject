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

//TODO: this might be unnecessary. i only need the fucking functions.
Extracted createExtracted(int numOfPoints){
	Extracted res = (Extracted)malloc(sizeof(Extracted));
	res->features = (SPPoint*)malloc(sizeof(SPPoint)*numOfPoints);
	if(res==NULL || res->features==NULL) return NULL;
	return res;
}

//TODO: remeber to remove numOfFeatures when config is ready
SP_EXTRACTED_MSG initExtractionMode(SPPoint* arr, int index, SPConfig config,int numOfFeatues){
	if(arr==NULL || index<0 || config==NULL) return SP_EXTRACT_INVALID_ARGUMENT;
	int i,j;
	SP_EXTRACTED_MSG msg;
	char* fileName = getFeatsFileName(config,index);
	if(fileName==NULL) return SP_EXTRACT_FAILED_GENERATE_FILEPATH;
	FILE* f = fopen(fileName,"w");
	if(f==NULL) return SP_EXTRACT_FAILED_TO_OPEN_FILE;
	fprintf(f,"%d ", numOfFeatues); //TODO: change when i finally have config...
	for(i=0;i<numOfFeatues;i++){ //for test lets insert a constant.
		fprintf(f,"%d ",arr[i]->dim);
		for(j=0;j<arr[i]->dim;j++){
			fprintf(f,"%f ",arr[i]->data[j]);
		}
	}
	fclose(f);
	return SP_EXTRACT_SUCCESS;
}

//TODO: check what happened if fscanf reach to EOF.
// generally i am not sure i used it right...
SP_EXTRACTED_MSG initNonExtractionMode(SPPoint* arr,int index, SPConfig config){
	if(arr==NULL || index<0 || config==NULL) return SP_EXTRACT_INVALID_ARGUMENT;
	int i,j,dim,numberOfFeatures;
	double d;
	double* data;
	char* filePath = getFeatsFileName(config,index);
	if(filePath==NULL) return SP_EXTRACT_FAILED_GENERATE_FILEPATH;
	FILE* f = fopen(filePath,"r");
	if(f==NULL) return SP_EXTRACT_FAILED_TO_OPEN_FILE;

	fscanf(f,"%d",&numberOfFeatures);//initialise number of features. TODO consider not to do it.
//	arr = (SPPoint*)malloc(sizeof(SPPoint)*numberOfFeatures);
	for(i=0;i<numberOfFeatures;i++){
		if(ftell(f)==EOF) return SP_EXTRACT_FILE_CONTENT_DOESNT_MATCH_THE_FORMAT;
		fscanf(f,"%d",&dim); //initialise dim
		data = (double*)malloc(sizeof(double)*dim);
		for(j=0;j<dim;j++){ //initialise data
			if(ftell(f)==EOF) return SP_EXTRACT_FILE_CONTENT_DOESNT_MATCH_THE_FORMAT;
			fscanf(f,"%lf",&data[j]);
		}
		arr[i] = spPointCreate(data,dim,index);
		free(data);
	}
	fclose(f);
	return SP_EXTRACT_SUCCESS;
}

//TODO: change all this to the relevent fields of config. they are mentiones
// next to each line
char* getFeatsFileName(SPConfig config,int index){
	if(config==NULL || index<0) return NULL;
	char* filePath = (char*)calloc(MAX_FILE_NAME_LENGTH,sizeof(char)); //thats intersting. why calloc solved it?
	if(filePath==NULL) return NULL;
	strcat(filePath,"C:\\Users\\Yair\\workspaceC\\spFinalProject\\images\\"); //spImagesDirectory
	strcat(filePath,"img");//spImagesPrefix
	strcat(filePath,"%d");
	sprintf(filePath,filePath,index);//add index
	strcat(filePath,".feats");
	//printf("%s",filePath); //for test
	return filePath;

}
