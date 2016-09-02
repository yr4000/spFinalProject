/*
 * Extracted_unit_test.c
 *
 *  Created on: 23 баев 2016
 *      Author: Yair
 */

#include "../Extracted.h"
#include "unit_test_util.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../SPConfig.h"
#include "unit_test_util.h"
#include <string.h>
#define STRING_END '\0'

int NUM_OF_FEATURES = 7;

//TODO did not check any nulls
bool testGetFeatsFileName(){
	SP_CONFIG_MSG msg;
	SPConfig config = spConfigCreate("C:\\Users\\gal\\workspace\\spFinalProject\\spFinalProject\\myConfig.config",&msg);
	ASSERT_FALSE(getFeatsFileName(NULL,3));
	ASSERT_FALSE(getFeatsFileName(config,-1));
	char* filePath = getFeatsFileName(config,404);
	ASSERT_TRUE(strcmp(filePath,"C:\\Users\\gal\\workspace\\spFinalProject\\spFinalProject\\images\\img404.feats")==0);
	free(filePath);
	spConfigDestroy(config);
	return true;
}

//TODO won't work anymore for there is no config file, need to be created.
bool testInitExtractionMode(){
	SP_CONFIG_MSG msg = SP_CONFIG_SUCCESS;
	SPConfig config = spConfigCreate("C:\\Users\\gal\\workspace\\spFinalProject\\spFinalProject\\myConfig.config",&msg);
	int dim,size;
	double** data = createData(&dim,&size);
	SPPoint* arr = createSPPointArray(data,size,dim);
	ASSERT_TRUE(initExtractionMode(NULL,1,config,NUM_OF_FEATURES)==SP_EXTRACT_INVALID_ARGUMENT);
	ASSERT_TRUE(initExtractionMode(arr,-1,config,NUM_OF_FEATURES)==SP_EXTRACT_INVALID_ARGUMENT);
	ASSERT_TRUE(initExtractionMode(arr,1,NULL,NUM_OF_FEATURES==SP_EXTRACT_INVALID_ARGUMENT));
	ASSERT_TRUE(initExtractionMode(arr,404,config,NUM_OF_FEATURES)==SP_EXTRACT_SUCCESS);
	destroySPPointArray(arr,size);
	destroyData(data);
	spConfigDestroy(config);
	return true;

}

//TODO not correct anymore... maybe
bool testInitNonExtractionMode(){
	SP_CONFIG_MSG msg = SP_CONFIG_SUCCESS;
	SPConfig config = spConfigCreate("C:\\Users\\gal\\workspace\\spFinalProject\\spFinalProject\\myConfig.config",&msg);
	int dim,size,i,numOfFeatures=0;
	SPPoint* arr = (SPPoint*)malloc(sizeof(SPPoint)*NUM_OF_FEATURES); //TODO: why when i malloced it in the function it didn't work?
	double** data = createData(&dim,&size);
	ASSERT_TRUE(initNonExtractionMode(NULL,2,config,&numOfFeatures)==SP_EXTRACT_INVALID_ARGUMENT);
	ASSERT_TRUE(initNonExtractionMode(&arr,-1,config,&numOfFeatures)==SP_EXTRACT_INVALID_ARGUMENT);
	ASSERT_TRUE(initNonExtractionMode(&arr,2,NULL,&numOfFeatures)==SP_EXTRACT_INVALID_ARGUMENT);
	ASSERT_TRUE(initNonExtractionMode(&arr,405,config,&numOfFeatures)==SP_EXTRACT_FAILED_TO_OPEN_FILE);
	ASSERT_TRUE(initNonExtractionMode(&arr,404,config,&numOfFeatures)==SP_EXTRACT_SUCCESS);
	for(i=0;i<14;i++){
		ASSERT_TRUE(arr[i/2]->data[i%2]==data[NUM_OF_FEATURES-1-i/2][i%2]);
	}
	destroySPPointArray(arr,size);
	destroyData(data);
	spConfigDestroy(config);
	return true;
}

//int main(){
////	setbuf(stdout, NULL);
////	char str[15];
////	int aint = 404;
////	sprintf(str,"%d",aint);
////	printf("%s\n",str);
////	char buff[10];
////	itoa(aint,buff,10);
////	printf("%s\n",buff,10);
//
//	RUN_TEST(testGetFeatsFileName);
//	RUN_TEST(testInitExtractionMode);
//	RUN_TEST(testInitNonExtractionMode);
//	return 0;
//}


