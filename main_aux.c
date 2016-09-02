/*
 * main_aux.c
 *
 *  Created on: 25 ���� 2016
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

//this function, using the feats files of the images, extracts all their features into a huge
//SPPoints array, which is arr.
void createWholePointsArray(int numberOfImages,int*sumNumOfFeatures,SPPoint** arr, SPConfig config){
	int i,j,numberOfFeatures;
	//for each image we extract all its features from its file.feats, and store it in temp.
	//then we realloc arr and stores them there.
	for(i=0;i<numberOfImages;i++){
		SPPoint* temp;
		if(initNonExtractionMode(&temp,i,config,&numberOfFeatures)!=SP_EXTRACT_SUCCESS){
			//TODO
		};
		*arr = realloc(*arr,(*sumNumOfFeatures+numberOfFeatures)*sizeof(SPPoint));
		for(j=0;j<numberOfFeatures;j++){
			(*arr)[*sumNumOfFeatures+j] = spPointCopy(temp[j]);
		}
		*sumNumOfFeatures+=numberOfFeatures;
		destroySPPointArray(temp,numberOfFeatures);
	}
}

KDTreeNode createTreeFromAllFeatures(SPConfig config,int numberOfImages){
	int sumNumOfFeatures=0,ZERO=0;
	SPPoint* arr = NULL;
	//we will create one huge tree since the search in it will be faster.
	createWholePointsArray(numberOfImages,&sumNumOfFeatures,&arr,config);

	//here we create and search the tree.
	//TODO checks if kdarr or tree creations failed
	KDArray kdarr = kdArrayInit(arr,sumNumOfFeatures);
	KDTreeNode tree= createKDTree(kdarr,config->spKDTreeSplitMethod,ZERO);//TODO create getMethod
	destroyKDArray(kdarr);
	destroySPPointArray(arr,sumNumOfFeatures);
	return tree;
}
//TODO take this code out of the loop
int* getAppreanceOfImagesFeatures(SPConfig config,KDTreeNode tree,SPPoint* queryImageFeatures,int queryImageFeaturesNum,int numberOfImages){
	int i,k;

	int* appreanceOfImagesFeatures = (int*)calloc(numberOfImages,sizeof(int));
	if(appreanceOfImagesFeatures==NULL){
		//TODO
	}
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
	return appreanceOfImagesFeatures;
}
