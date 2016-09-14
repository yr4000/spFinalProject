/*
 * main.cpp
 *
 *  Created on: 22  2016
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


// this function extract the features of the images to the feats files.
bool extractFeatures(SPConfig config,SP_CONFIG_MSG msg,ImageProc proc){
	int i;
	char imagePath[LENGTH_OF_LINE + 1];
	SPPoint* arr;
	//for each image, extract the features to a feat file named <imageName>.feats
	for(i=0;i<spConfigGetNumOfImages(config,&msg);i++){
		int numOfFeatures = spConfigGetNumOfFeatures(config,&msg);
		if (numOfFeatures < 0){
			spLoggerPrintError("Extract features from images failed. The config given is NULL.",__FILE__,__func__,__LINE__);
			return false; //out
		}
		spConfigGetImagePath(imagePath,config,i);
		arr = proc.getImageFeatures(imagePath,i,&numOfFeatures);
		if(arr==NULL || initExtractionMode(arr,i,config,numOfFeatures)!=SP_EXTRACT_SUCCESS){
			spLoggerPrintError("Extraction didn't succeed, cannot get image features",__FILE__,__func__,__LINE__);
			return false; //out
		}
		destroySPPointArray(arr,numOfFeatures);
	}
	spLoggerPrintInfo("ExtractionMode: The extraction of all images features succeed");
	return true;
}

// this function responsible to show the results to the user
bool showResults(ImageProc proc, SPConfig config, int* appreanceOfImagesFeatures,char* queryImagePath,int numberOfImages){
	int i,j;
	SP_CONFIG_MSG msg;
	int SpNumOfSimilarImages = getSpNumOfSimilarImages(config, &msg);
	if (SpNumOfSimilarImages < 0){
		spLoggerPrintError("ShowResults failed. The config given is NULL.",__FILE__,__func__,__LINE__);
		return false; //out
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
		char* imagePath = (char*)calloc(LENGTH_OF_LINE,sizeof(char));
		if(imagePath == NULL){
			spLoggerPrintError("Show results images failed. Allocation failure",__FILE__,__func__,__LINE__);
			return false; //out
		}
		msg = spConfigGetImagePath(imagePath,config,index);
		if( msg != SP_CONFIG_SUCCESS){
			if( msg == SP_CONFIG_INVALID_ARGUMENT){
				spLoggerPrintError("Show results images failed. The image path is null.",__FILE__,__func__,__LINE__);
			}
			else{
				spLoggerPrintError("Show results images failed. The index given is bigger than the number of images. Index out of range." ,__FILE__,__func__,__LINE__);
			}
			return false; //out
		}
		// if spConfigMinimalGui true, show minimal gui, else print path
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
	return true;
}

int main(int argc, char* argv[]){
	setbuf(stdout, NULL);
	char configFileName[LENGTH_OF_LINE];
	SP_CONFIG_MSG msg;

	//here we receive the config file name
	if(argc >= 3 && !strcmp(argv[1], "-c"))
		strcpy(configFileName,argv[2]);
	else
		strcpy(configFileName, "spcbir.config");

	//the data in config will define the function future behaver
	SPConfig config = spConfigCreate(configFileName,&msg);
	if(msg!=SP_CONFIG_SUCCESS || config == NULL){
		spLoggerPrintError("Did not succeed creating the config",__FILE__,__func__,__LINE__);
		spConfigDestroy(config);
		printf("Exiting..\n");
		return 1;
	}

	int numberOfImages = spConfigGetNumOfImages(config,&msg);
	if(msg!=SP_CONFIG_SUCCESS){
		spLoggerPrintError("Did not succeed getting the number of images from the config",__FILE__,__func__,__LINE__);
		spConfigDestroy(config);
		printf("Exiting...\n");
		return 1;
	}

	ImageProc proc(config);

	//now we check spExtractionMode and continue in the path needed.
	//if spExtractionMode==true, run over each picture, extract it features
	//and store them in a file
	if(spConfigIsExtractionMode(config, &msg)){
		spLoggerPrintInfo("The extraction of all images features begins.");
		if (!extractFeatures(config,msg,proc)){
			spConfigDestroy(config);
			printf("Exiting...\n");
			return 1;
		}

	}
	//here we create the KDTree from all the features for future searches.
	KDTreeNode tree = createTreeFromAllFeatures(config,numberOfImages);
	if (tree == NULL){
		spConfigDestroy(config);
		printf("Exiting...\n");
		return 1;
	}
	spLoggerPrintInfo("Getting started receiving query images from the user.");
	while(true){
		//receive an image query path to search from the user. if the string "<>" has been
		//received, we break the loop and finishing the program.
		char queryImagePath[LENGTH_OF_LINE +1];
		printf("Please enter an image path:\n");
		scanf("%s",queryImagePath);
		if(strcmp(queryImagePath,"<>")==0){
			spLoggerPrintInfo("User chose to quit the program.");
			break;
		}
		//here we extract the features of the query.
		int queryImageFeaturesNum;
		SPPoint* queryImageFeatures = proc.getImageFeatures(queryImagePath,0,&queryImageFeaturesNum);
		if(!queryImageFeatures){
			printf("The path you entered is not valid. Please try another one.\n");
			spLoggerPrintWarning("Did not manage to extract the features of the query, check if exists in the directory or choose another one instesd.",__FILE__,__func__,__LINE__);
			continue;
		}
		// here we will find and count the number of times features of images
		// appeared as closest features to the a features of the query image.
		int* appreanceOfImagesFeatures = getAppreanceOfImagesFeatures(config,tree,queryImageFeatures,queryImageFeaturesNum,numberOfImages);
		destroySPPointArray(queryImageFeatures,queryImageFeaturesNum); //(destroys the features of query for they are not needed anymore)
		if(appreanceOfImagesFeatures == NULL){
			spConfigDestroy(config);
			printf("Exiting...\n");
			return 1;
		}
		//here we will show our results.
		spLoggerPrintInfo("Started showing the images results.");
		if (!showResults(proc,config,appreanceOfImagesFeatures,queryImagePath,numberOfImages)){
			spConfigDestroy(config);
			printf("Exiting...\n");
			return 1;
		}
	}
	printf("Exiting...\n");
	destroyKDTree(tree);
	spConfigDestroy(config);
	return 0;

}


