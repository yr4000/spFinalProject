/*
 * Extracted_unit_test.c
 *
 *  Created on: 23 баев 2016
 *      Author: Yair
 */

#include "../Extracted.h"
#include "unit_tests/unit_test_util.h"
#include <string.h>

int NUM_OF_FEATURES = 7;

//TODO did not check any nulls + need to recheck when have a normal config file
bool testGetFeatsFileName(){
	SPConfig config;
	ASSERT_FALSE(getFeatsFileName(NULL,3));
	ASSERT_FALSE(getFeatsFileName(config,-1));
	char* filePath = getFeatsFileName(config,2);
	ASSERT_TRUE(strcmp(filePath,"C:\\Users\\Yair\\workspaceC\\spFinalProject\\images\\img2.feats")==0);
	free(filePath);
	return true;
}

bool testInitExtractionMode(){
	SPConfig config;
	int dim,size;
	double** data = createData(&dim,&size);
	SPPoint* arr = createSPPointArray(data,size,dim);
	ASSERT_TRUE(initExtractionMode(NULL,1,config,NUM_OF_FEATURES)==SP_EXTRACT_INVALID_ARGUMENT);
	ASSERT_TRUE(initExtractionMode(arr,-1,config,NUM_OF_FEATURES)==SP_EXTRACT_INVALID_ARGUMENT);
	ASSERT_TRUE(initExtractionMode(arr,1,NULL,NUM_OF_FEATURES==SP_EXTRACT_INVALID_ARGUMENT));
	ASSERT_TRUE(initExtractionMode(arr,1,config,NUM_OF_FEATURES)==SP_EXTRACT_SUCCESS);
	destroySPPointArray(arr,size);
	destroyData(data);
	return true;

}

bool testInitNonExtractionMode(){
	SPConfig config;
	int dim,size,i,numOfFeatures;
	SPPoint* arr;
//	= (SPPoint*)malloc(sizeof(SPPoint)*NUM_OF_FEATURES); //TODO: why when i malloced it in the function it didn't work?
	double** data = createData(&dim,&size);
	ASSERT_TRUE(initNonExtractionMode(NULL,2,config,&numOfFeatures)==SP_EXTRACT_INVALID_ARGUMENT);
	ASSERT_TRUE(initNonExtractionMode(&arr,-1,config,&numOfFeatures)==SP_EXTRACT_INVALID_ARGUMENT);
	ASSERT_TRUE(initNonExtractionMode(&arr,2,NULL,&numOfFeatures)==SP_EXTRACT_INVALID_ARGUMENT);
	ASSERT_TRUE(initNonExtractionMode(&arr,2,config,&numOfFeatures)==SP_EXTRACT_FAILED_TO_OPEN_FILE);
	ASSERT_TRUE(initNonExtractionMode(&arr,1,config,&numOfFeatures)==SP_EXTRACT_SUCCESS);
	for(i=0;i<14;i++){
		ASSERT_TRUE(arr[i/2]->data[i%2]==data[NUM_OF_FEATURES-1-i/2][i%2]);
	}
	destroySPPointArray(arr,size);
	destroyData(data);
	return true;
}

//int main(){
//	RUN_TEST(testGetFeatsFileName);
//	RUN_TEST(testInitExtractionMode);
//	RUN_TEST(testInitNonExtractionMode);
//	return 0;
//}


