/*
 * Extracted.c
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SPLogger.h"
#include "Extracted.h"

int MAX_FILE_NAME_LENGTH = 1024;

//this function gets an array of points, the images index, a config object and
//the number of features (the size of the array), and generates a feat file with the
//data inside the points array.
//pre: arr!=NULL, index>=0, config!=NULL,numOfFeatures>=0
//pro: sends success message and create feat file.
SP_EXTRACTED_MSG initExtractionMode(SPPoint* arr, int index, SPConfig config,int numOfFeatues){
	if(arr==NULL || index<0 || config==NULL || numOfFeatues<0){
		spLoggerPrintError("Extraction mode is invalid",__FILE__,__func__,__LINE__);
		return SP_EXTRACT_INVALID_ARGUMENT;
	}
	int i,j;
	char* fileName = getFeatsFileName(config,index);
	if(fileName==NULL){
		spLoggerPrintError("SP_EXTRACT_FAILED_GENERATE_FILEPATH",__FILE__,__func__,__LINE__);
						spLoggerDestroy();
		return SP_EXTRACT_FAILED_GENERATE_FILEPATH;
	}
	FILE* f = fopen(fileName,"w");
	if(f==NULL){
		spLoggerPrintError("SP_EXTRACT_FAILED_TO_OPEN_FILE",__FILE__,__func__,__LINE__);
						spLoggerDestroy();
		return SP_EXTRACT_FAILED_TO_OPEN_FILE;
	}
	// at the start of the file we will save the number of features in it
	fprintf(f,"%d ", numOfFeatues);
	//for each feature, we save its dimention and then its data in order.
	for(i=0;i<numOfFeatues;i++){
		int dim = spPointGetDimension(arr[i]);
		fprintf(f,"%d ",dim);
		for(j=0;j<dim;j++){
			fprintf(f,"%f ",spPointGetAxisCoor(arr[i],j));
		}
	}
	fclose(f);
	free(fileName);
	return SP_EXTRACT_SUCCESS;
}

//this function extract the data from a feat file of the relevant image to the index,
// into the unintialised SPPoint* arr.
SP_EXTRACTED_MSG initNonExtractionMode(SPPoint** arr,int index, SPConfig config,int* numberOfFeatures){
	if(arr==NULL || index<0 || config==NULL){
		spLoggerPrintError("SP_EXTRACT_INVALID_ARGUMENT",__FILE__,__func__,__LINE__);
						spLoggerDestroy();
		return SP_EXTRACT_INVALID_ARGUMENT;
	}
	int i,j,dim;
	double* data;
	char* filePath = getFeatsFileName(config,index);
	if(filePath==NULL){
		spLoggerPrintError("SP_EXTRACT_FAILED_GENERATE_FILEPATH",__FILE__,__func__,__LINE__);
						spLoggerDestroy();
		return SP_EXTRACT_FAILED_GENERATE_FILEPATH;
	}
	FILE* f = fopen(filePath,"r");
	if(f==NULL){
		spLoggerPrintError("SP_EXTRACT_FAILED_TO_OPEN_FILE",__FILE__,__func__,__LINE__);
								spLoggerDestroy();
		free(filePath);
		return SP_EXTRACT_FAILED_TO_OPEN_FILE;
	}

	//writes the number of features into numberOfFeaturs
	fscanf(f,"%d",numberOfFeatures);
	*arr = (SPPoint*)malloc(sizeof(SPPoint)*(*numberOfFeatures));
	for(i=0;i<*numberOfFeatures;i++){
		if(ftell(f)==EOF){
			spLoggerPrintError("SP_EXTRACT_FILE_CONTENT_DOESNT_MATCH_THE_FORMAT",__FILE__,__func__,__LINE__);
			free(filePath);
			free(data);
			return SP_EXTRACT_FILE_CONTENT_DOESNT_MATCH_THE_FORMAT;
		}
		fscanf(f,"%d",&dim); //initialise dim
		data = (double*)malloc(sizeof(double)*dim);
		for(j=0;j<dim;j++){ //initialise data
			if(ftell(f)==EOF){
				spLoggerPrintError("SP_EXTRACT_FILE_CONTENT_DOESNT_MATCH_THE_FORMAT",__FILE__,__func__,__LINE__);
				free(filePath);
				free(data);
				return SP_EXTRACT_FILE_CONTENT_DOESNT_MATCH_THE_FORMAT;
			}
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
	char spImagesDirectory[MAX_FILE_NAME_LENGTH + 1];
	char spImagesPrefix[MAX_FILE_NAME_LENGTH + 1];
	char* filePath = (char*)calloc(MAX_FILE_NAME_LENGTH,sizeof(char));
	if(filePath==NULL){
		spLoggerPrintError("The filePath is NULL, allocation failure",__FILE__,__func__,__LINE__);
		return NULL;
	}
	char strIndex[MAX_FILE_NAME_LENGTH + 1];
	if(getSpImagesDirectory(spImagesDirectory, config) != SP_CONFIG_SUCCESS){
		spLoggerPrintError("The config file is NULL, allocation failure",__FILE__,__func__,__LINE__);
				return NULL;
	}
	if(getSpImagesPrefix(spImagesPrefix,config) != SP_CONFIG_SUCCESS){
		spLoggerPrintError("The config file is NULL, allocation failure",__FILE__,__func__,__LINE__);
						return NULL;
	}
	strcat(filePath, spImagesDirectory);
	strcat(filePath, spImagesPrefix);
	sprintf(strIndex, "%d", index);
	strcat(filePath,strIndex);
	strcat(filePath,".feats\0");
	return filePath;

}
