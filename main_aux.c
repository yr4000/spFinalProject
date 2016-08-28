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
	return b-a;
}

int* getAppreanceOfImagesFeatures(SPConfig config,SP_CONFIG_MSG* msg,SPPoint* queryImageFeatures,int queryImageFeaturesNum,SPPoint* arr){
	int i,j,k,numOfFeatures,spNumOfSimilarImages,ZERO=0;
	spKDTreeSplitMethod method;
	int* appreanceOfImagesFeatures = (int*)malloc(sizeof(int)*spConfigGetNumOfImages(config,msg));
	if(appreanceOfImagesFeatures==NULL || *msg!=SP_CONFIG_SUCCESS){
		//TODO
	}

	for(i=0;i<queryImageFeaturesNum;i++){
		SPBPQueue q = spBPQueueCreate(spNumOfSimilarImages); //TODO need to change spNumOfSimilarImages

		//for each image we will create a KDTree and search for the nearest
		//features. after the search we will destroy it immediately.
		//TODO: we always work on the same q for the same image feature. is that a problem?
		// if possible - put it in a function that returns int* appreanceOfImagesFeatures - in main_aux.c
		for(j=0;j<spConfigGetNumOfImages(config,msg);j++){
			if(initNonExtractionMode(&arr,j,config,&numOfFeatures)!=SP_EXTRACT_SUCCESS){};
			KDArray kdarr = kdArrayInit(arr,numOfFeatures);
			KDTreeNode tree= createKDTree(kdarr,method,ZERO);//TODO chage method to the realevent config thing
			KNNSearch(q,tree,queryImageFeatures[i]);
			destroyKDTree(tree);
			destroyKDArray(kdarr);
			destroySPPointArray(arr,numOfFeatures);
		}

		//after we finished searching for the closest features, we will
		//count all the indexes of images appeared in q.
		SPListElement e;
		for(k=0;k<spNumOfSimilarImages;k++){ //TODO change spNumOfSimilarImages
			e = spBPQueuePeek(q);
			appreanceOfImagesFeatures[e->index]++;
			spBPQueueDequeue(q);
			spListElementDestroy(e);
		}
		spBPQueueDestroy(q);
	}
	return appreanceOfImagesFeatures;
}
