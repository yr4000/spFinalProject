///*
// * main.cpp
// *
// *  Created on: 22 баев 2016
// *      Author: Yair
// */
//
//
//#include <cstdlib>
//#include <cassert>
//#include <cstring>
//#include "SPImageProc.h"
//extern "C"{
//#include "SPPoint.h"
//#include "SPBPriorityQueue.h"
//#include "SPConfig.h"
//#include "Extracted.h"
//#include "KDArray.h"
//#include "KDTree.h"
//#include "main_aux.h"
//}
//
//using namespace sp;
//
//int main(){
//	setbuf(stdout, NULL);
//	int i,j,k,spNumOfSimilarImages,ZERO=0;
//	spKDTreeSplitMethod method; //TODO temporary
//	char* configFileName, *imagePath; //TODO: temporary
//	SP_CONFIG_MSG* msg; //TODO probably temporary, probably for the logger
//	SPConfig config = spConfigCreate(configFileName,msg); //the data in this object will define the function future behaver
//	ImageProc proc(config);//TODO: check later how to initialise this object most correctly
//	//now i need to check spExtractionMode and continue in the path needed.
//	int numOfFeatures = spConfigGetNumOfFeatures(config,msg);
//	SPPoint* arr;
//	if(*msg!=SP_CONFIG_SUCCESS){} //TODO consider to checks like this...
//		//if spExtractionMode==true, run over each picture, extract it features
//		//and store them in a file
//		if(config->spExtractionMode){
//			for(i=0;i<spConfigGetNumOfImages(config,msg);i++){
//				spConfigGetImagePath(imagePath,config,i);
//				arr = proc.getImageFeatures(imagePath,i,&numOfFeatures);
//				if(initExtractionMode(arr,i,config,numOfFeatures)!=SP_EXTRACT_SUCCESS){
//				//TODO logger?
//				//out
//			}
//			destroySPPointArray(arr,numOfFeatures);
//			free(imagePath);
//		}
//	}
//
//		while(true){
//			//recieve an image to search from the use. if the string "<>" has been
//			//recieved, we break the loop and finishing the program.
//			char queryImagePath[1024];
//			printf("Please enter an image path:\n");
//			scanf("%s",queryImagePath);
//			if(strcmp(queryImagePath,"<>")==0) break;
//			//here we extract the features of this image.
//			int queryImageFeaturesNum;
//			SPPoint* queryImageFeatures = proc.getImageFeatures(queryImagePath,0,&queryImageFeaturesNum);
//			if(!queryImageFeatures){
//				printf("some bug"); //TODO change it, notice logger takes care of it.
//				break;
//			}
//			// here we will find and count the number of times features of images
//			// appeared ad closest features to the a feature of the query image
//			int* appreanceOfImagesFeatures = getAppreanceOfImagesFeatures(config,msg,queryImageFeatures,queryImageFeaturesNum,arr);
//
//			//here we sort appreanceOfImagesFeatures from big to small
//			qsort(appreanceOfImagesFeatures,spConfigGetNumOfImages(config,msg),sizeof(int),
//					compareIntBigToSmall);
//
//			//here we will show our results.
//			for(i=0;i<spNumOfSimilarImages;i++){
//				spConfigGetImagePath(imagePath,config,i);
//				if(spConfigMinimalGui(config,msg)){
//					proc.showImage(imagePath);
//				}
//				else{
//					printf("Best candidates for - %s - are:\n",queryImagePath);
//					printf("%s\n",imagePath);
//				}
//			}
//		}
//}
