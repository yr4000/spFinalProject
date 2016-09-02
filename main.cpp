/*
 * main.cpp
 *
 *  Created on: 22 ���� 2016
 *      Author: Yair
 */


#include <cstdlib>
#include <cassert>
#include <cstring>
#include "SPImageProc.h"
extern "C"{
#include "SPPoint.h"
#include "SPBPriorityQueue.h"
#include "SPConfig.h"
#include "Extracted.h"
#include "KDArray.h"
#include "KDTree.h"
#include "main_aux.h"
#define LENGTH_OF_LINE 1024
}

using namespace sp;



int extractFeatures(SPConfig config,SP_CONFIG_MSG msg,ImageProc proc){
	int i;
	char imagePath[LENGTH_OF_LINE + 1];
	SPPoint* arr;
	for(i=0;i<spConfigGetNumOfImages(config,&msg);i++){
		//		imagePath = (char*)calloc(LENGTH_OF_LINE,sizeof(char)); //TODO: temporary
		int numOfFeatures = spConfigGetNumOfFeatures(config,&msg);
		spConfigGetImagePath(imagePath,config,i);
		arr = proc.getImageFeatures(imagePath,i,&numOfFeatures);
		if(arr==NULL || initExtractionMode(arr,i,config,numOfFeatures)!=SP_EXTRACT_SUCCESS){
			spLoggerPrintError("SP_EXTRACT didn't succeed, cannot get image features",__FILE__,__func__,__LINE__);
			spLoggerDestroy();
			spConfigDestroy(config);
			return 1; //out

		}
		destroySPPointArray(arr,numOfFeatures);
		//	free(imagePath);
	}
	return 0;
}

int showResults(ImageProc proc, SPConfig config, int* appreanceOfImagesFeatures,char* queryImagePath,int numberOfImages){
	int i,j;
	SP_CONFIG_MSG msg;
	int SpNumOfSimilarImages = getSpNumOfSimilarImages(config, &msg);
	if (SpNumOfSimilarImages < 0){
		spLoggerPrintError("The config given is NULL.",__FILE__,__func__,__LINE__);
		spLoggerDestroy();
		spConfigDestroy(config);
		return 1; //out
	}
	if(!spConfigMinimalGui(config, &msg)){printf("Best candidates for - %s - are:\n",queryImagePath);}
	for(i=0;i<SpNumOfSimilarImages;i++){
		int max = 0,index = 0;
		spLoggerPrintInfo("finding the image with maximum appearances.");
		//finding the index with maximum appearances
		for(j=0;j<numberOfImages;j++){
			if(max<appreanceOfImagesFeatures[j]){
				max = appreanceOfImagesFeatures[j];
				index = j;
			}
		}
		char* imagePath = (char*)calloc(LENGTH_OF_LINE,sizeof(char)); //TODO temporary?
		if(imagePath == NULL){
			spLoggerPrintError("Allocation failure",__FILE__,__func__,__LINE__);
			spLoggerDestroy();
			spConfigDestroy(config);
			return 1; //out
		}
		msg = spConfigGetImagePath(imagePath,config,index);
		if( msg != SP_CONFIG_SUCCESS){
			if( msg == SP_CONFIG_INVALID_ARGUMENT){
				spLoggerPrintError("The image path is null.",__FILE__,__func__,__LINE__);
			}
			else{
				spLoggerPrintError("The index given is bigger than the number of images."
						" Index out of range." ,__FILE__,__func__,__LINE__);
			}
			spLoggerDestroy();
			spConfigDestroy(config);
			return 1; //out
		}
		if(spConfigMinimalGui(config,&msg)){
			proc.showImage(imagePath);
		}
		else{
			printf("%s\n",imagePath);
		}
		appreanceOfImagesFeatures[index] = -1;
		max=0;
		free(imagePath);
	}
	free(appreanceOfImagesFeatures);
	return 0;
}

int main(int argc, char* argv[]){
	setbuf(stdout, NULL);
	int i,j,k;
	char* configFileName;
	SP_CONFIG_MSG msg;

	if(strcmp(argv[0],"-c")){//TODO what is going on?
		configFileName = argv[2];
	}
	else configFileName = "spcbir.config";

	SPConfig config = spConfigCreate(configFileName,&msg); //the data in this object will define the function future behaver
	if(msg!=SP_CONFIG_SUCCESS){
		spLoggerPrintError("Did not succeed creating the config",__FILE__,__func__,__LINE__);
		spLoggerDestroy();
		spConfigDestroy(config);
		return 1;

	}
	int numberOfImages = spConfigGetNumOfImages(config,&msg);
	if(msg!=SP_CONFIG_SUCCESS){
		spLoggerPrintError("Did not succeed getting from the config the number of images",__FILE__,__func__,__LINE__);
		spLoggerDestroy();
		spConfigDestroy(config);
		return 1;
	}
	//TODO: check later how to initialise this object most correctly
	//do i need to check failure here?
	ImageProc proc(config);
	//now i need to check spExtractionMode and continue in the path needed.
	//if spExtractionMode==true, run over each picture, extract it features
	//and store them in a file
	if(spConfigIsExtractionMode(config, &msg)){
		spLoggerPrintInfo("The extraction of all images features begins.");
		if (extractFeatures(config,msg,proc) == 1){
			spLoggerPrintError("extractFeatures features from images failed",__FILE__,__func__,__LINE__);
			spLoggerDestroy();
			spConfigDestroy(config);
			return 1;
		}
		//			for(i=0;i<spConfigGetNumOfImages(config,&msg);i++){
		//				imagePath = (char*)calloc(LENGTH_OF_LINE,sizeof(char)); //TODO: temporary
		//				int numOfFeatures = spConfigGetNumOfFeatures(config,&msg);
		//				spConfigGetImagePath(imagePath,config,i);
		//				arr = proc.getImageFeatures(imagePath,i,&numOfFeatures);
		//				if(arr==NULL || initExtractionMode(arr,i,config,numOfFeatures)!=SP_EXTRACT_SUCCESS){
		//				//out
		//			}
		//			destroySPPointArray(arr,numOfFeatures);
		//			free(imagePath);
		//		}
	}
	KDTreeNode tree = createTreeFromAllFeatures(config,numberOfImages);
	if (tree == NULL){
		spLoggerPrintError("kd-tree creation failed.",__FILE__,__func__,__LINE__);
		spLoggerDestroy();
		spConfigDestroy(config);
		return 1;
	}

	while(true){
		spLoggerPrintInfo("Getting started receiving query images from the user.");
		//receive an image to search from the use. if the string "<>" has been
		//received, we break the loop and finishing the program.
		char queryImagePath[LENGTH_OF_LINE +1];//TODO check if this makes no problems.
		printf("Please enter an image path:\n");
		scanf("%s",queryImagePath);
		if(strcmp(queryImagePath,"<>")==0) break;
		//here we extract the features of this image.
		int queryImageFeaturesNum;
		SPPoint* queryImageFeatures = proc.getImageFeatures(queryImagePath,0,&queryImageFeaturesNum);
		if(!queryImageFeatures){
			spLoggerPrintWarning("Did not manage to extract the features of the query.",__FILE__,__func__,__LINE__);
			continue;
		}
		// here we will find and count the number of times features of images
		// appeared ad closest features to the a feature of the query image
		int* appreanceOfImagesFeatures = getAppreanceOfImagesFeatures(config,tree,queryImageFeatures,queryImageFeaturesNum,numberOfImages);
		if(appreanceOfImagesFeatures == NULL){
			spLoggerDestroy();
			spConfigDestroy(config);
			return 1;
		}
		//here we will show our results.
		spLoggerPrintInfo("Started showing the results.");
		showResults(proc,config,appreanceOfImagesFeatures,queryImagePath,numberOfImages);
	}
	destroyKDTree(tree); //TODO a problem here!
	spLoggerDestroy();
	spConfigDestroy(config);
	return 0;

}


