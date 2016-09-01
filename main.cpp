/*
 * main.cpp
 *
 *  Created on: 22 баев 2016
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
}

using namespace sp;

void extractFeatures(SPConfig config,SP_CONFIG_MSG msg,ImageProc proc){
	int i;
	char imagePath[LENGTH_OF_LINE];
	SPPoint* arr;
	for(i=0;i<spConfigGetNumOfImages(config,&msg);i++){
//		imagePath = (char*)calloc(LENGTH_OF_LINE,sizeof(char)); //TODO: temporary
		int numOfFeatures = spConfigGetNumOfFeatures(config,&msg);
		spConfigGetImagePath(imagePath,config,i);
		arr = proc.getImageFeatures(imagePath,i,&numOfFeatures);
		if(arr==NULL || initExtractionMode(arr,i,config,numOfFeatures)!=SP_EXTRACT_SUCCESS){
		//TODO logger?
		//out
		}
	destroySPPointArray(arr,numOfFeatures);
//	free(imagePath);
	}
}

void showResults(ImageProc proc, SPConfig config, int* appreanceOfImagesFeatures,char* queryImagePath,int numberOfImages){
	int i,j;
	SP_CONFIG_MSG msg;
	if(!config->spMinimalGUI){printf("Best candidates for - %s - are:\n",queryImagePath);}
	for(i=0;i<config->spNumOfSimilarImages;i++){ //TODO getter
		int max = 0,index = 0;
		for(j=0;j<numberOfImages;j++){
			if(max<appreanceOfImagesFeatures[j]){
				max = appreanceOfImagesFeatures[j];
				index = j;
			}
		}
		char* imagePath = (char*)calloc(LENGTH_OF_LINE,sizeof(char)); //TODO temporary?
		spConfigGetImagePath(imagePath,config,index);
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
}

int main(int argc, char* argv[]){
	setbuf(stdout, NULL);
	int i,j,k;
	char* configFileName;
	SP_CONFIG_MSG msg;

	//TODO: need to add some error messages to cases here
	if(strcmp(argv[0],"-c")){//TODO what is going on?
		configFileName = argv[2];
	}
	else configFileName = "spcbir.config";

	SPConfig config = spConfigCreate(configFileName,&msg); //the data in this object will define the function future behaver
	if(msg!=SP_CONFIG_SUCCESS){
		//TODO logger and exit(return)
	}
	int numberOfImages = spConfigGetNumOfImages(config,&msg);
	if(msg!=SP_CONFIG_SUCCESS){
		//TODO
	}
	//TODO: check later how to initialise this object most correctly
	//do i need to check failure here?
	ImageProc proc(config);
	//now i need to check spExtractionMode and continue in the path needed.
		//if spExtractionMode==true, run over each picture, extract it features
		//and store them in a file
		if(config->spExtractionMode){
			extractFeatures(config,msg,proc);
//			for(i=0;i<spConfigGetNumOfImages(config,&msg);i++){
//				imagePath = (char*)calloc(LENGTH_OF_LINE,sizeof(char)); //TODO: temporary
//				int numOfFeatures = spConfigGetNumOfFeatures(config,&msg);
//				spConfigGetImagePath(imagePath,config,i);
//				arr = proc.getImageFeatures(imagePath,i,&numOfFeatures);
//				if(arr==NULL || initExtractionMode(arr,i,config,numOfFeatures)!=SP_EXTRACT_SUCCESS){
//				//TODO logger?
//				//out
//			}
//			destroySPPointArray(arr,numOfFeatures);
//			free(imagePath);
//		}
	}
		KDTreeNode tree = createTreeFromAllFeatures(config,numberOfImages);

		while(true){
			//recieve an image to search from the use. if the string "<>" has been
			//recieved, we break the loop and finishing the program.
			char queryImagePath[1024];//TODO check if this makes no problems.
			printf("Please enter an image path:\n");
			scanf("%s",queryImagePath);
			if(strcmp(queryImagePath,"<>")==0) break;
			//here we extract the features of this image.
			int queryImageFeaturesNum;
			SPPoint* queryImageFeatures = proc.getImageFeatures(queryImagePath,0,&queryImageFeaturesNum);
			if(!queryImageFeatures){
				printf("some bug"); //TODO change it, notice logger takes care of it.
				continue;
			}
			// here we will find and count the number of times features of images
			// appeared ad closest features to the a feature of the query image
			int* appreanceOfImagesFeatures = getAppreanceOfImagesFeatures(config,tree,queryImageFeatures,queryImageFeaturesNum,numberOfImages);

			//here we will show our results.
			showResults(proc,config,appreanceOfImagesFeatures,queryImagePath,numberOfImages);
		}
		destroyKDTree(tree); //TODO a problem here!
}
