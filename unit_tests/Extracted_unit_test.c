/*
 * Extracted_unit_test.c
 *
 *  Created on: 23 баев 2016
 *      Author: Yair
 */

#include "../Extracted.h"
#include "DSBuilders.h"
#include "unit_test_util.h"
#include <string.h>
#include <stdlib.h>

int NUM_OF_FEATURES = 7;
char PATH[] = "./myConfig.config";

bool testGetFeatsFileName(){
	SP_CONFIG_MSG msg;
	SPConfig config = spConfigCreate(PATH,&msg);
	ASSERT_FALSE(getFeatsFileName(NULL,3));
	ASSERT_FALSE(getFeatsFileName(config,-1));
	char* filePath = getFeatsFileName(config,404);
	ASSERT_TRUE(strcmp(filePath,"./images/img404.feats")==0);
	free(filePath);
	spConfigDestroy(config);
	return true;
}

bool testInitExtractionMode(){
	SP_CONFIG_MSG msg = SP_CONFIG_SUCCESS;
	SPConfig config = spConfigCreate(PATH,&msg);
	int dim,size;
	double** data = createData(&dim,&size);
	SPPoint* arr = createSPPointArray(data,size,dim);
	ASSERT_TRUE(initExtractionMode(NULL,1,config,NUM_OF_FEATURES)==SP_EXTRACT_INVALID_ARGUMENT);
	ASSERT_TRUE(initExtractionMode(arr,-1,config,NUM_OF_FEATURES)==SP_EXTRACT_INVALID_ARGUMENT);
	ASSERT_TRUE(initExtractionMode(arr,1,NULL,NUM_OF_FEATURES==SP_EXTRACT_INVALID_ARGUMENT));
	ASSERT_TRUE(initExtractionMode(arr,404,config,NUM_OF_FEATURES)==SP_EXTRACT_SUCCESS);
	destroySPPointArray(arr,size);
	destroy2DDoubleArray(data,size);
	spConfigDestroy(config);
	return true;

}

bool testInitNonExtractionMode(){
	SP_CONFIG_MSG msg = SP_CONFIG_SUCCESS;
	SPConfig config = spConfigCreate(PATH,&msg);
	int dim,size,i,numOfFeatures=0;
	SPPoint* arr = NULL;
	double** data = createData(&dim,&size);
	ASSERT_TRUE(initNonExtractionMode(NULL,2,config,&numOfFeatures)==SP_EXTRACT_INVALID_ARGUMENT);
	ASSERT_TRUE(initNonExtractionMode(&arr,-1,config,&numOfFeatures)==SP_EXTRACT_INVALID_ARGUMENT);
	ASSERT_TRUE(initNonExtractionMode(&arr,2,NULL,&numOfFeatures)==SP_EXTRACT_INVALID_ARGUMENT);
	ASSERT_TRUE(initNonExtractionMode(&arr,405,config,&numOfFeatures)==SP_EXTRACT_FAILED_TO_OPEN_FILE);
	ASSERT_TRUE(initNonExtractionMode(&arr,404,config,&numOfFeatures)==SP_EXTRACT_SUCCESS);
	for(i=0;i<14;i++){
		ASSERT_TRUE(arr[i/2]->data[i%2]==data[NUM_OF_FEATURES-1-i/2][i%2]);
	}
	destroySPPointArray(arr,numOfFeatures);
	destroy2DDoubleArray(data,size);
	spConfigDestroy(config);
	return true;
}

//int main(){
//	RUN_TEST(testGetFeatsFileName);
//	RUN_TEST(testInitExtractionMode);
//	RUN_TEST(testInitNonExtractionMode);
//	return 0;
//}


