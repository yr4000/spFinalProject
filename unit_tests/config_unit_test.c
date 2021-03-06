/*
 * Extracted_unit_test.c
 *
 *  Created on: 23 ���� 2016
 *      Author: Yair
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../SPConfig.h"
#include "../unit_tests/unit_test_util.h"
#define STRING_END '\0'

#define TEST_MISSING_FILE "./config_files/NoSuchFile"
#define TEST_DEFAULT_VAL "./config_files/new1.txt"
#define TEST_MISSING_NUM_IMG "./config_files/new2.txt"
#define TEST_MISSING_DIR "./config_files/new3.txt"
#define TEST_MISSING_PREFIX "./config_files/new4.txt"
#define TEST_MISSING_SUFFIX "./config_files/new5.txt"
#define TEST_BLANK_LINE "./config_files/new6.txt"
#define TEST_INVALID_PARAMETER_NAME "./config_files/new7.txt"
#define TEST_SPACE_STRING_VALUE "./config_files/new8.txt"
#define TEST_SPACE_INT_VALUE "./config_files/new9.txt"
#define TEST_REGULAR "./config_files/new10.txt"
#define TEST_ANOTHER_REGULAR "./config_files/new11.txt"
#define TEST_VALUE_LESS_ZERO "./config_files/new12.txt"
#define TEST_VALUE_MORE_28 "./config_files/new13.txt"
#define TEST_NOT_TRUE_FALSE "./config_files/new14.txt"
#define TEST_VALUE_MORE_FOUR "./config_files/new15.txt"
#define TEST_SIGN "./config_files/new16.txt"

bool testMissingFile(){

	char imagePath[1024];
	char pcaPath[1024];
	char path[1024] = TEST_MISSING_FILE;
	SP_CONFIG_MSG msg;
	SPConfig co =spConfigCreate(path, &msg);

	ASSERT_TRUE(msg == SP_CONFIG_CANNOT_OPEN_FILE);

	ASSERT_TRUE(spConfigIsExtractionMode(co, &msg) == false);
	ASSERT_TRUE(msg == SP_CONFIG_INVALID_ARGUMENT);

	ASSERT_TRUE(spConfigMinimalGui(co, &msg) == false);
	ASSERT_TRUE(msg == SP_CONFIG_INVALID_ARGUMENT);

	ASSERT_TRUE(spConfigGetNumOfImages(co, &msg) == -1);
	ASSERT_TRUE(msg == SP_CONFIG_INVALID_ARGUMENT);

	ASSERT_TRUE(spConfigGetNumOfFeatures(co, &msg) == -1);
	ASSERT_TRUE(msg == SP_CONFIG_INVALID_ARGUMENT);

	ASSERT_TRUE(spConfigGetPCADim(co, &msg) == -1);
	ASSERT_TRUE(msg == SP_CONFIG_INVALID_ARGUMENT);

	spConfigGetImagePath(imagePath,co,15);
	ASSERT_TRUE(msg == SP_CONFIG_INVALID_ARGUMENT);

	spConfigGetPCAPath(pcaPath, co);
	ASSERT_TRUE(msg == SP_CONFIG_INVALID_ARGUMENT);


	spConfigDestroy (co);
	return true;

}


bool testDefaultValues()
{
	char path[1024] = TEST_DEFAULT_VAL;
	SP_CONFIG_MSG msg;
	SPConfig co = spConfigCreate(path, &msg);
	ASSERT_TRUE(!strcmp(co->spImagesDirectory,"./images/"));
	ASSERT_TRUE(!strcmp(co->spImagesPrefix,"img"));
	ASSERT_TRUE(!strcmp(co->spImagesSuffix,".png"));
	ASSERT_TRUE(co->spNumOfImages == 1);
	ASSERT_TRUE(co->spPCADimension == 20);
	ASSERT_TRUE(!strcmp(co->spPCAFilename, "pca.yml"));
	ASSERT_TRUE(co->spNumOfFeatures == 100);
	ASSERT_TRUE(co->spExtractionMode == true);
	ASSERT_TRUE(co->spMinimalGUI == false);
	ASSERT_TRUE(co->spNumOfSimilarImages == 1);
	ASSERT_TRUE(co->spKDTreeSplitMethod == MAX_SPREAD);
	ASSERT_TRUE(co->spKNN == 1);
	ASSERT_TRUE(co->spLoggerLevel == 3);
	ASSERT_TRUE(!strcmp(co->spLoggerFilename,"stdout"));

	ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);

	spConfigDestroy(co);
	return true;
}

bool testMissingNumImages(){
	char path[1024]= TEST_MISSING_NUM_IMG;
	SP_CONFIG_MSG msg;
	SPConfig co =spConfigCreate(path, &msg);

	ASSERT_TRUE(msg == SP_CONFIG_MISSING_NUM_IMAGES);

	spConfigDestroy (co);
	return true;
}

bool testMissingDirectory(){
	char path[1024] = TEST_MISSING_DIR;
	SP_CONFIG_MSG msg;
	SPConfig co =spConfigCreate(path, &msg);

	ASSERT_TRUE(msg == SP_CONFIG_MISSING_DIR);
	spConfigDestroy (co);
	return true;

}

bool testMissingPrefix(){
	char path[1024]= TEST_MISSING_PREFIX;
	SP_CONFIG_MSG msg;
	SPConfig co =spConfigCreate(path, &msg);

	ASSERT_TRUE(msg == SP_CONFIG_MISSING_PREFIX);
	spConfigDestroy (co);
	return true;

}

bool testMissingSuffix(){
	char path[1024] = TEST_MISSING_SUFFIX;
	SP_CONFIG_MSG msg;
	SPConfig co =spConfigCreate(path, &msg);

	ASSERT_TRUE(msg == SP_CONFIG_MISSING_SUFFIX);
	spConfigDestroy (co);
	return true;

}

bool testBlankLine(){
	char path[1024] = TEST_BLANK_LINE;
	char imagePath[1024];
	char pcaPath[1024];
	char resultGetImagePath[1024] ="./images/img15.png";
		char resultGetPCAPath[1024] = "./images/pca.yml";

		SP_CONFIG_MSG msg;
		SPConfig co = spConfigCreate(path, &msg);
		ASSERT_TRUE(!strcmp(co->spImagesDirectory,"./images/"));
		ASSERT_TRUE(!strcmp(co->spImagesPrefix,"img"));
		ASSERT_TRUE(!strcmp(co->spImagesSuffix,".png"));
		ASSERT_TRUE(co->spNumOfImages == 17);
		ASSERT_TRUE(co->spPCADimension == 20);
		ASSERT_TRUE(!strcmp(co->spPCAFilename,"pca.yml"));
		ASSERT_TRUE(co->spNumOfFeatures == 100);
		ASSERT_TRUE(co->spExtractionMode == true);
		ASSERT_TRUE(co->spMinimalGUI == false);
		ASSERT_TRUE(co->spNumOfSimilarImages == 5);
		ASSERT_TRUE(co->spKDTreeSplitMethod == MAX_SPREAD);
		ASSERT_TRUE(co->spKNN == 1);
		ASSERT_TRUE(co->spLoggerLevel == 3);
		ASSERT_TRUE(!strcmp(co->spLoggerFilename,"stdout"));

		ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);

		ASSERT_TRUE(spConfigIsExtractionMode(co, &msg) == true);
		ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);

		ASSERT_TRUE(spConfigMinimalGui(co, &msg) == false);
		ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);

		ASSERT_TRUE(spConfigGetNumOfImages(co, &msg) == 17);
		ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);

		ASSERT_TRUE(spConfigGetNumOfFeatures(co, &msg) == 100);
		ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);

		ASSERT_TRUE(spConfigGetPCADim(co, &msg) == 20);
		ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);

		spConfigGetImagePath(imagePath,co,15);
		ASSERT_TRUE(!strcmp(imagePath, resultGetImagePath));
		ASSERT_TRUE(spConfigGetImagePath(imagePath,co,15) == SP_CONFIG_SUCCESS);


		ASSERT_TRUE(spConfigGetImagePath(imagePath,co,18) == SP_CONFIG_INDEX_OUT_OF_RANGE);

		spConfigGetPCAPath(pcaPath, co);
		ASSERT_TRUE(!strcmp(pcaPath, resultGetPCAPath));
		ASSERT_TRUE(spConfigGetPCAPath(pcaPath, co) == SP_CONFIG_SUCCESS);

	spConfigDestroy (co);
	return true;

}

bool testLineWithInvalidParameterName(){
	char path[1024] = TEST_INVALID_PARAMETER_NAME;
	SP_CONFIG_MSG msg;
	SPConfig co =spConfigCreate(path, &msg);

	ASSERT_TRUE(msg == SP_CONFIG_INVALID_STRING);
	spConfigDestroy (co);
	return true;

}

bool testSpaceInsideStringParameterValue(){
	char path[1024]= TEST_SPACE_STRING_VALUE;
	SP_CONFIG_MSG msg;
	SPConfig co =spConfigCreate(path, &msg);

	ASSERT_TRUE(msg == SP_CONFIG_INVALID_STRING);
	spConfigDestroy (co);
	return true;

}

bool testSpaceInsideIntParameterValue(){
	char path[1024] = TEST_SPACE_INT_VALUE;
	SP_CONFIG_MSG msg;
	SPConfig co =spConfigCreate(path, &msg);

	ASSERT_TRUE(msg == SP_CONFIG_INVALID_INTEGER);
	spConfigDestroy (co);
	return true;

}

bool testRegularConfigFIle()
{
	char imagePath[1024];
	char pcaPath[1024];
	char path[1024]= TEST_REGULAR;
	char resultGetImagePath[1024] ="./images/img16.png";
	char resultGetPCAPath[1024] = "./images/pca.yml";

	SP_CONFIG_MSG msg;
	SPConfig co = spConfigCreate(path, &msg);
	ASSERT_TRUE(!strcmp(co->spImagesDirectory,"./images/"));
	ASSERT_TRUE(!strcmp(co->spImagesPrefix,"img"));
	ASSERT_TRUE(!strcmp(co->spImagesSuffix,".png"));
	ASSERT_TRUE(co->spNumOfImages == 17);
	ASSERT_TRUE(co->spPCADimension == 20);
	ASSERT_TRUE(!strcmp(co->spPCAFilename,"pca.yml"));
	ASSERT_TRUE(co->spNumOfFeatures == 100);
	ASSERT_TRUE(co->spExtractionMode == true);
	ASSERT_TRUE(co->spMinimalGUI == false);
	ASSERT_TRUE(co->spNumOfSimilarImages == 5);
	ASSERT_TRUE(co->spKDTreeSplitMethod == MAX_SPREAD);
	ASSERT_TRUE(co->spKNN == 1);
	ASSERT_TRUE(co->spLoggerLevel == 3);
	ASSERT_TRUE(!strcmp(co->spLoggerFilename,"stdout"));

	ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);

	ASSERT_TRUE(spConfigIsExtractionMode(co, &msg) == true);
	ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);

	ASSERT_TRUE(spConfigMinimalGui(co, &msg) == false);
	ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);

	ASSERT_TRUE(spConfigGetNumOfImages(co, &msg) == 17);
	ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);

	ASSERT_TRUE(spConfigGetNumOfFeatures(co, &msg) == 100);
	ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);

	ASSERT_TRUE(spConfigGetPCADim(co, &msg) == 20);
	ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);


	ASSERT_TRUE(spConfigGetImagePath(imagePath,co,16) == SP_CONFIG_SUCCESS);
	ASSERT_TRUE(!strcmp(imagePath, resultGetImagePath));


	ASSERT_TRUE(spConfigGetImagePath(imagePath,co,18) == SP_CONFIG_INDEX_OUT_OF_RANGE);

	spConfigGetPCAPath(pcaPath, co);
	ASSERT_TRUE(!strcmp(pcaPath, resultGetPCAPath));
	ASSERT_TRUE(spConfigGetPCAPath(pcaPath, co) == SP_CONFIG_SUCCESS);

	spConfigDestroy(co);
	return true;
}

bool testAnotherRegularConfigFIle()
{
	char imagePath[1024];
	char pcaPath[1024];
	char path[1024] = TEST_ANOTHER_REGULAR;
	char resultGetImagePath[1024] ="./imagess/imgg11.jpg";
	char resultGetPCAPath[1024] = "./imagess/pcaa.yml";


	SP_CONFIG_MSG msg;
	SPConfig co = spConfigCreate(path, &msg);

	ASSERT_TRUE(!strcmp(co->spImagesDirectory,"./imagess/"));
	ASSERT_TRUE(!strcmp(co->spImagesPrefix,"imgg"));
	ASSERT_TRUE(!strcmp(co->spImagesSuffix, ".jpg"));
	ASSERT_TRUE(co->spNumOfImages == 12);
	ASSERT_TRUE(co->spPCADimension == 22);
	ASSERT_TRUE(!strcmp(co->spPCAFilename,"pcaa.yml"));
	ASSERT_TRUE(co->spNumOfFeatures == 82);
	ASSERT_TRUE(co->spExtractionMode == false);
	ASSERT_TRUE(co->spMinimalGUI == true);
	ASSERT_TRUE(co->spNumOfSimilarImages == 12);
	ASSERT_TRUE(co->spKDTreeSplitMethod == INCREMENTAL);
	ASSERT_TRUE(co->spKNN == 2);
	ASSERT_TRUE(co->spLoggerLevel == 2);
	ASSERT_TRUE(!strcmp(co->spLoggerFilename,"stdoutt"));

	ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);

	ASSERT_TRUE(spConfigIsExtractionMode(co, &msg) == false);
	ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);

	ASSERT_TRUE(spConfigMinimalGui(co, &msg) == true);
	ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);

	ASSERT_TRUE(spConfigGetNumOfImages(co, &msg) == 12);
	ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);

	ASSERT_TRUE(spConfigGetNumOfFeatures(co, &msg) == 82);
	ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);

	ASSERT_TRUE(spConfigGetPCADim(co, &msg) == 22);
	ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);

	spConfigGetImagePath(imagePath,co,11);
	ASSERT_TRUE(!strcmp(imagePath, resultGetImagePath));
	ASSERT_TRUE(spConfigGetImagePath(imagePath,co,11) == SP_CONFIG_SUCCESS);


	ASSERT_TRUE(spConfigGetImagePath(imagePath,co,17) == SP_CONFIG_INDEX_OUT_OF_RANGE);

	spConfigGetPCAPath(pcaPath, co);
	ASSERT_TRUE(!strcmp(pcaPath, resultGetPCAPath));
	ASSERT_TRUE(spConfigGetPCAPath(pcaPath, co) == SP_CONFIG_SUCCESS);

	spConfigDestroy(co);
	return true;
}

bool testInvalidParameterValueLessThan0(){
	char path[1024] = TEST_VALUE_LESS_ZERO;
	SP_CONFIG_MSG msg;
	SPConfig co =spConfigCreate(path, &msg);

	ASSERT_TRUE(msg == SP_CONFIG_INVALID_INTEGER);
	spConfigDestroy (co);
	return true;
}

bool testInvalidParameterValueMoreThan28(){
	char path[1024]= TEST_VALUE_MORE_28;
	SP_CONFIG_MSG msg;
	SPConfig co =spConfigCreate(path, &msg);

	ASSERT_TRUE(msg == SP_CONFIG_INVALID_INTEGER);
	spConfigDestroy (co);
	return true;
}

bool testInvalidParameterValueNotTrueFalse(){
	char path[1024]= TEST_NOT_TRUE_FALSE;
	SP_CONFIG_MSG msg;
	SPConfig co =spConfigCreate(path, &msg);

	ASSERT_TRUE(msg == SP_CONFIG_INVALID_ARGUMENT);
	spConfigDestroy (co);
	return true;
}

bool testInvalidParameterValueMoreThan4()
{
	char path[1024] = TEST_VALUE_MORE_FOUR;
	SP_CONFIG_MSG msg;
	SPConfig co = spConfigCreate(path, &msg);

	ASSERT_TRUE(msg == SP_CONFIG_INVALID_INTEGER);
	spConfigDestroy(co);
	return true;
}

bool testSignInsideIntParameterValue(){
	char path[1024] = TEST_SIGN;
	SP_CONFIG_MSG msg;
	SPConfig co =spConfigCreate(path, &msg);

	ASSERT_TRUE(msg == SP_CONFIG_INVALID_INTEGER);
	spConfigDestroy (co);
	return true;

}



int main()
{
	RUN_TEST(testMissingFile);
	RUN_TEST(testDefaultValues);
	RUN_TEST(testMissingNumImages);
	RUN_TEST(testMissingDirectory);
	RUN_TEST(testMissingPrefix);
	RUN_TEST(testMissingSuffix);
	RUN_TEST(testBlankLine);
	RUN_TEST(testLineWithInvalidParameterName);
	RUN_TEST(testSpaceInsideStringParameterValue);
	RUN_TEST(testSpaceInsideIntParameterValue);
	RUN_TEST(testRegularConfigFIle);
	RUN_TEST(testAnotherRegularConfigFIle);
	RUN_TEST(testInvalidParameterValueLessThan0);
	RUN_TEST(testInvalidParameterValueMoreThan28);
	RUN_TEST(testInvalidParameterValueNotTrueFalse);
	RUN_TEST(testInvalidParameterValueMoreThan4);
	RUN_TEST(testSignInsideIntParameterValue);
	return 0;
}

