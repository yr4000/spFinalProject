/*
 * main_aux.c
 *
 *  Created on: 25 ���� 2016
 *      Author: Yair
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "SPPoint.h"
#include "SPBPriorityQueue.h"
#include "SPConfig.h"
#include "Extracted.h"
#include "KDArray.h"
#include "KDTree.h"

//this function, using the feats files of the images, extracts all their features into a huge
//SPPoints array, which is arr.
bool createWholePointsArray(int numberOfImages,int*sumNumOfFeatures,SPPoint** arr, SPConfig config){
	int i,j,numberOfFeatures;
	//for each image we extract all its features from its file.feats, and store it in temp.
	//then we realloc arr and stores them there.
	for(i=0;i<numberOfImages;i++){
		SPPoint* temp;
		if(initNonExtractionMode(&temp,i,config,&numberOfFeatures)!=SP_EXTRACT_SUCCESS){
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
	return true; //nothing went wrong.
}

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
		return NULL;
	}
	//here we create and search the tree.
	KDArray kdarr = kdArrayInit(arr,sumNumOfFeatures);
	if (kdarr == NULL){
		spLoggerPrintError("kd-array creation failed.",__FILE__,__func__,__LINE__);
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

int* getAppreanceOfImagesFeatures(SPConfig config,KDTreeNode tree,SPPoint* queryImageFeatures,int queryImageFeaturesNum,int numberOfImages){
	int i,k;
	int KNN;
	SP_CONFIG_MSG msg;
	SPBPQueue q;
	int* appreanceOfImagesFeatures = (int*)calloc(numberOfImages,sizeof(int));
	if(appreanceOfImagesFeatures==NULL){
		//TODO
	}
	//TODO create a new function for this code
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


