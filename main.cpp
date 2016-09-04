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
#include "SPLogger.h"
#include "SPConfig.h"
#include "SPPoint.h"
#include "Extracted.h"
#include "KDTree.h"
#include "main_aux.h"
}

#define LENGTH_OF_LINE 1024

using namespace sp;
//TODO this is a push test


int extractFeatures(SPConfig config,SP_CONFIG_MSG msg,ImageProc proc){
	int i;
	char imagePath[LENGTH_OF_LINE + 1];
	SPPoint* arr;
	for(i=0;i<spConfigGetNumOfImages(config,&msg);i++){
		int numOfFeatures = spConfigGetNumOfFeatures(config,&msg);
		if (numOfFeatures < 0){
			spLoggerPrintError("Extract features from images failed. The config given is NULL.",__FILE__,__func__,__LINE__);
			return 1; //out
		}
		spConfigGetImagePath(imagePath,config,i);
		arr = proc.getImageFeatures(imagePath,i,&numOfFeatures);
		if(arr==NULL || initExtractionMode(arr,i,config,numOfFeatures)!=SP_EXTRACT_SUCCESS){
			spLoggerPrintError("Extraction didn't succeed, cannot get image features",__FILE__,__func__,__LINE__);
			return 1; //out
		}
		destroySPPointArray(arr,numOfFeatures);
	}
	return 0;
}

int showResults(ImageProc proc, SPConfig config, int* appreanceOfImagesFeatures,char* queryImagePath,int numberOfImages){
	int i,j;
	SP_CONFIG_MSG msg;
	int SpNumOfSimilarImages = getSpNumOfSimilarImages(config, &msg);
	if (SpNumOfSimilarImages < 0){
		spLoggerPrintError("ShowResults failed. The config given is NULL.",__FILE__,__func__,__LINE__);
		return 1; //out
	}
	if(!spConfigMinimalGui(config, &msg)){printf("Best candidates for - %s - are:\n",queryImagePath);}
	for(i=0;i<SpNumOfSimilarImages;i++){
		int max = 0,index = 0;
		//finding the index with maximum appearances
		for(j=0;j<numberOfImages;j++){
			if(max<appreanceOfImagesFeatures[j]){
				max = appreanceOfImagesFeatures[j];
				index = j;
			}
		}
		char* imagePath = (char*)calloc(LENGTH_OF_LINE,sizeof(char)); //TODO temporary?
		if(imagePath == NULL){
			spLoggerPrintError("Show results images failed. Allocation failure",__FILE__,__func__,__LINE__);
			return 1; //out
		}
		msg = spConfigGetImagePath(imagePath,config,index);
		if( msg != SP_CONFIG_SUCCESS){
			if( msg == SP_CONFIG_INVALID_ARGUMENT){
				spLoggerPrintError("Show results images failed. The image path is null.",__FILE__,__func__,__LINE__);
			}
			else{
				spLoggerPrintError("Show results images failed. The index given is bigger than the number of images. Index out of range." ,__FILE__,__func__,__LINE__);
			}
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
	char configFileName[LENGTH_OF_LINE];
	SP_CONFIG_MSG msg;

	if(argc >= 3 && !strcmp(argv[1], "-c"))
		strcpy(configFileName,argv[2]);
	else
		strcpy(configFileName, "spcbir.config");

	SPConfig config = spConfigCreate(configFileName,&msg); //the data in this object will define the function future behaver
	if(msg!=SP_CONFIG_SUCCESS || config == NULL){
		spLoggerPrintError("Did not succeed creating the config",__FILE__,__func__,__LINE__);
		spConfigDestroy(config);
		return 1;
	}

	int numberOfImages = spConfigGetNumOfImages(config,&msg);
	if(msg!=SP_CONFIG_SUCCESS){
		spLoggerPrintError("Did not succeed getting the number of images from the config",__FILE__,__func__,__LINE__);
		spConfigDestroy(config);
		return 1;
	}
	//TODO: check later how to initialise this object most correctly
	//do i need to check failure here?
	ImageProc proc(config);
	//now need to check spExtractionMode and continue in the path needed.
	//if spExtractionMode==true, run over each picture, extract it features
	//and store them in a file
	if(spConfigIsExtractionMode(config, &msg)){
		spLoggerPrintInfo("The extraction of all images features begins.");
		if (extractFeatures(config,msg,proc) == 1){
			spConfigDestroy(config);
			return 1;
		}

	}
	KDTreeNode tree = createTreeFromAllFeatures(config,numberOfImages);
	if (tree == NULL){
		spConfigDestroy(config);
		return 1;
	}
	spLoggerPrintInfo("Getting started receiving query images from the user.");
	while(true){
		//receive an image to search from the use. if the string "<>" has been
		//received, we break the loop and finishing the program.
		char queryImagePath[LENGTH_OF_LINE +1];//TODO check if this makes no problems.
		printf("Please enter an image path:\n");
		scanf("%s",queryImagePath);
		if(strcmp(queryImagePath,"<>")==0){
			spLoggerPrintInfo("User chose to quit the program.");
			break;
		}
		//here we extract the features of this image.
		int queryImageFeaturesNum;
		SPPoint* queryImageFeatures = proc.getImageFeatures(queryImagePath,0,&queryImageFeaturesNum);
		if(!queryImageFeatures){
			spLoggerPrintWarning("Did not manage to extract the features of the query, check if exists in the directory or choose another one instesd.",__FILE__,__func__,__LINE__);
			continue;
		}
		// here we will find and count the number of times features of images
		// appeared ad closest features to the a feature of the query image
		int* appreanceOfImagesFeatures = getAppreanceOfImagesFeatures(config,tree,queryImageFeatures,queryImageFeaturesNum,numberOfImages);
		if(appreanceOfImagesFeatures == NULL){
			spConfigDestroy(config);
			return 1;
		}
		//here we will show our results.
		spLoggerPrintInfo("Started showing the images results.");
		if (showResults(proc,config,appreanceOfImagesFeatures,queryImagePath,numberOfImages) == 1){
			spConfigDestroy(config);
			return 1;
		}
	}
	destroyKDTree(tree); //TODO a problem here!
	spConfigDestroy(config);
	return 0;

}


