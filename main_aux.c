/*
 * main_aux.c
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "SPLogger.h"
#include "SPBPriorityQueue.h"
#include "Extracted.h"
#include "KDArray.h"
#include "main_aux.h"


//creates SPConfig from the path given in the arguments or
//set it to the default value "spcbir.config"
SPConfig createConfigFromFilePath(int argc, char* argv[]){
	bool isDefaultFile;
	char configFileName[LENGTH_OF_LINE + 1];
	SP_CONFIG_MSG msg;

	//here we receive the config file name
	if(argc >= 3 && !strcmp(argv[1], "-c")){
		strcpy(configFileName,argv[2]);
		isDefaultFile = false;
	}
	else if (argc == 1){
		strcpy(configFileName, "spcbir.config");
		isDefaultFile = true;
	}
	else{
		printf("Invalid command line : use -c <config_filename>\n");
		printf("Exiting...\n");
		return NULL;
	}


	//the data in config will define the function future behaver
	SPConfig config = spConfigCreate(configFileName,&msg);
	if(msg!=SP_CONFIG_SUCCESS || config == NULL){
		if(msg == SP_CONFIG_CANNOT_OPEN_FILE && isDefaultFile == true){
			printf("The default configuration file spcbir.config couldn't be open\n");
		}
		if(msg == SP_CONFIG_CANNOT_OPEN_FILE && isDefaultFile == false){
			printf("The The configuration file %s couldn't be open\n", configFileName);
		}

		spLoggerPrintError("Did not succeed creating the config",__FILE__,__func__,__LINE__);
		spConfigDestroy(config);
		printf("Exiting...\n");
		return NULL;
	}
	return config;
}

//this function, using the feats files of the images, extracts all their features into a huge
//SPPoints array, which is arr.
bool createWholePointsArray(int numberOfImages,int*sumNumOfFeatures,SPPoint** arr, SPConfig config){
	int i,j,numberOfFeatures;
	//for each image we extract all its features from its file.feats, and store it in temp.
	//then we realloc arr and stores them there.
	for(i=0;i<numberOfImages;i++){
		SPPoint* temp;
		if(initNonExtractionMode(&temp,i,config,&numberOfFeatures)!= SP_EXTRACT_SUCCESS){
			spLoggerPrintError("Extraction failed",__FILE__,__func__,__LINE__);
			return false;
		};
		*arr = realloc(*arr,(*sumNumOfFeatures+numberOfFeatures)*sizeof(SPPoint));
		for(j=0;j<numberOfFeatures;j++){
			(*arr)[*sumNumOfFeatures+j] = spPointCopy(temp[j]);
		}
		*sumNumOfFeatures+=numberOfFeatures;
		destroySPPointArray(temp,numberOfFeatures);
	}
	spLoggerPrintInfo("NonExtractionMode: The extraction of all images features succeed");
	return true; //nothing went wrong.
}

// create KDTree from the features of all the images in the stock
KDTreeNode createTreeFromAllFeatures(SPConfig config,int numberOfImages){
	bool succeedcreateWholePointsArray;
	int sumNumOfFeatures=0,ZERO=0;
	SPPoint* arr = NULL;
	SP_CONFIG_MSG msg;
	spKDTreeSplitMethodEnum spKDTreeSplitMethod = getSpKDTreeSplitMethod(config, &msg);
	if(msg != SP_CONFIG_SUCCESS){
		spLoggerPrintError("The config is null.",__FILE__,__func__,__LINE__);
	}
	//we will create one huge tree since the search in it will be faster.
	succeedcreateWholePointsArray = createWholePointsArray(numberOfImages,&sumNumOfFeatures,&arr,config);
	if(succeedcreateWholePointsArray == false){
		spLoggerPrintError("SPPoint array creation failed.",__FILE__,__func__,__LINE__);
		return NULL;
	}
	//here we create and search the tree.
	KDArray kdarr = kdArrayInit(arr,sumNumOfFeatures);
	if (kdarr == NULL){
		spLoggerPrintError("KDArray creation failed.",__FILE__,__func__,__LINE__);
		return NULL;
	}
	KDTreeNode tree= createKDTree(kdarr,spKDTreeSplitMethod,ZERO);
	if (tree == NULL){
		spLoggerPrintError("kd-tree creation failed.",__FILE__,__func__,__LINE__);
		return NULL;
	}
	destroyKDArray(kdarr);
	destroySPPointArray(arr,sumNumOfFeatures);
	return tree;
}

//returns an array with the indexes of the images whose features
//where the closest the querys
int* getAppreanceOfImagesFeatures(SPConfig config,KDTreeNode tree,SPPoint* queryImageFeatures,int queryImageFeaturesNum,int numberOfImages){
	int i,k;
	int KNN;
	SP_CONFIG_MSG msg;
	SPBPQueue q;
	int* appreanceOfImagesFeatures = (int*)calloc(numberOfImages,sizeof(int));
	if(appreanceOfImagesFeatures==NULL){
		spLoggerPrintError("Memory allocation failure",__FILE__,__func__,__LINE__);
		return NULL;
		}

	//for each feature we search the big tree.
	for(i=0;i<queryImageFeaturesNum;i++){
		KNN = getSpKNN (config, &msg);
		if(KNN < 0){
			spLoggerPrintError("The config is null.",__FILE__,__func__,__LINE__);
			return NULL;
		}
		q = spBPQueueCreate(KNN);
		KNNSearch(q,tree,queryImageFeatures[i]);

		//after we finished searching for the closest features, we will
		//count all the indexes of images appeared in q.
		SPListElement e;
		for(k=0;k<KNN;k++){
			e = spBPQueuePeek(q);
			appreanceOfImagesFeatures[spListElementGetIndex(e)]++;
			spBPQueueDequeue(q);
			spListElementDestroy(e);
		}
		spBPQueueDestroy(q);
	}
	return appreanceOfImagesFeatures;
}


