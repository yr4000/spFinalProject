/*
 * main_aux.c
 *
 *  Created on: 25 баев 2016
 *      Author: Yair
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "SPPoint.h"
#include "SPBPriorityQueue.h"
#include "SPConfig.h"
#include "Extracted.h"
#include "KDArray.h"
#include "KDTree.h"

int compareIntBigToSmall(const void *a,const void* b){
	int A = *(int*)a;
	int B = *(int*)b;
	return B-A;
}

void createWholePointsArray(int numberOfImages,int*sumNumOfFeatures,SPPoint** arr, SPConfig config){
	int i,j,numberOfFeatures;
	for(i=0;i<numberOfImages;i++){
		SPPoint* temp;
		if(initNonExtractionMode(&temp,i,config,&numberOfFeatures)!=SP_EXTRACT_SUCCESS){};
		*arr = realloc(*arr,(*sumNumOfFeatures+numberOfFeatures)*sizeof(SPPoint));
		for(j=0;j<numberOfFeatures;j++){
			(*arr)[*sumNumOfFeatures+j] = spPointCopy(temp[j]);
		}
		*sumNumOfFeatures+=numberOfFeatures;
		destroySPPointArray(temp,numberOfFeatures);
	}
}
//TODO take this code out of the loop
int* getAppreanceOfImagesFeatures(SPConfig config,SP_CONFIG_MSG* msg,SPPoint* queryImageFeatures,int queryImageFeaturesNum,SPPoint* arr){
	int i,j,k,sumNumOfFeatures=0,ZERO=0;
	int numberOfImages = spConfigGetNumOfImages(config,msg);
	if(msg!=SP_CONFIG_SUCCESS){
		//TODO
	}

	int* appreanceOfImagesFeatures = (int*)calloc(numberOfImages,sizeof(int));
	if(appreanceOfImagesFeatures==NULL || *msg!=SP_CONFIG_SUCCESS){
		//TODO
	}
	arr = (SPPoint*)malloc(sizeof(SPPoint)); //TODO why is it necessary?
//	arr = (SPPoint*)malloc(basicPArrSize*spConfigGetNumOfFeatures(config,msg)); //TODO maybe remove
	//we will create one huge tree since the search in it will be faster.
	//TODO: we always work on the same q for the same image feature. is that a problem?
	createWholePointsArray(numberOfImages,&sumNumOfFeatures,&arr,config);
//	for(i=0;i<numberOfImages;i++){
//		SPPoint* temp;
//		if(initNonExtractionMode(&temp,i,config,&numberOfFeatures)!=SP_EXTRACT_SUCCESS){};
//		for(j=0;j<numberOfFeatures;j++){
//			arr[sumNumOfFeatures+j] = spPointCopy(temp[j]);
//		}
//		sumNumOfFeatures+=numberOfFeatures;
//		destroySPPointArray(temp,numberOfFeatures);
//	}

	//making the array smaller if need to
//	arr = realloc(arr,basicPArrSize*sumNumOfFeatures);//TODO maybe remove
	//here we create and search the tree.
	KDArray kdarr = kdArrayInit(arr,sumNumOfFeatures);
	KDTreeNode tree= createKDTree(kdarr,config->spKDTreeSplitMethod,ZERO);//TODO create getMethod

	//TODO create a new function for this code
	//for each feature we search the big tree.
	for(i=0;i<queryImageFeaturesNum;i++){
		SPBPQueue q = spBPQueueCreate(config->spNumOfSimilarImages);//TODO create getSPN
		KNNSearch(q,tree,queryImageFeatures[i]);

		//after we finished searching for the closest features, we will
		//count all the indexes of images appeared in q.
		SPListElement e;
		for(k=0;k<config->spNumOfSimilarImages;k++){
			e = spBPQueuePeek(q);
			appreanceOfImagesFeatures[e->index]++;
			spBPQueueDequeue(q);
			spListElementDestroy(e);
		}
		spBPQueueDestroy(q);
	}
	destroyKDTree(tree);
	destroyKDArray(kdarr);
	destroySPPointArray(arr,sumNumOfFeatures);
	return appreanceOfImagesFeatures;
}
