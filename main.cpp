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
	char* imagePath;
	SPPoint* arr;
	for(i=0;i<spConfigGetNumOfImages(config,&msg);i++){
		imagePath = (char*)calloc(LENGTH_OF_LINE,sizeof(char)); //TODO: temporary
		int numOfFeatures = spConfigGetNumOfFeatures(config,&msg);
		spConfigGetImagePath(imagePath,config,i);
		arr = proc.getImageFeatures(imagePath,i,&numOfFeatures);
		if(arr==NULL || initExtractionMode(arr,i,config,numOfFeatures)!=SP_EXTRACT_SUCCESS){
		//TODO logger?
		//out
	}
	destroySPPointArray(arr,numOfFeatures);
	free(imagePath);
}
}

int main(int argc, char* argv[]){
	setbuf(stdout, NULL);
	int i,j,k,ZERO=0;
	spKDTreeSplitMethod method; //TODO temporary
	char* configFileName,*imagePath;
	SPPoint* arr;
	SP_CONFIG_MSG msg; //TODO probably temporary, probably for the logger

	//TODO: need to add some error messages to cases here
	if(strcmp(argv[0],"-c")){//TODO what is going on?
		configFileName = argv[2];
	}
	else configFileName = "spcbir.config";

	SPConfig config = spConfigCreate(configFileName,&msg); //the data in this object will define the function future behaver
	if(msg!=SP_CONFIG_SUCCESS){} //TODO consider to checks like this...
	ImageProc proc(config);//TODO: check later how to initialise this object most correctly

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
			int* appreanceOfImagesFeatures = getAppreanceOfImagesFeatures(config,&msg,queryImageFeatures,queryImageFeaturesNum,arr);

			//here we will show our results.
			if(!config->spMinimalGUI){printf("Best candidates for - %s - are:\n",queryImagePath);}
			for(i=0;i<config->spNumOfSimilarImages;i++){
				int max = 0,index = 0;
				for(j=0;j<config->spNumOfImages;j++){
					if(max<appreanceOfImagesFeatures[j]){
						max = appreanceOfImagesFeatures[j];
						index = j;
					}
				}
				imagePath = (char*)calloc(LENGTH_OF_LINE,sizeof(char)); //TODO temporary?
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
		}
}
