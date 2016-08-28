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
#include "unit_tests/unit_test_util.h"
#include <string.h>
#define STRING_END '\0'

#define TEST_MISSING_FILE "C:\\Users\\Yair\\workspaceC\\spFinalProject\\config files\\NoSuchFile"
#define TEST_DEFAULT_VAL "C:\\Users\\Yair\\workspaceC\\spFinalProject\\config files\\new1.txt"
#define TEST_MISSING_NUM_IMG "C:\\Users\\Yair\\workspaceC\\spFinalProject\\config files\\new2.txt"
#define TEST_MISSING_DIR "C:\\Users\\Yair\\workspaceC\\spFinalProject\\config files\\new3.txt"
#define TEST_MISSING_PREFIX "C:\\Users\\Yair\\workspaceC\\spFinalProject\\config files\\new4.txt"
#define TEST_MISSING_SUFFIX "C:\\Users\\Yair\\workspaceC\\spFinalProject\\config files\\new5.txt"
#define TEST_BLANK_LINE "C:\\Users\\Yair\\workspaceC\\spFinalProject\\config files\\new6.txt"
#define TEST_INVALID_PARAMETER_NAME "C:\\Users\\Yair\\workspaceC\\spFinalProject\\config files\\new7.txt"
#define TEST_SPACE_STRING_VALUE "C:\\Users\\Yair\\workspaceC\\spFinalProject\\config files\\new8.txt"
#define TEST_SPACE_INT_VALUE "C:\\Users\\Yair\\workspaceC\\spFinalProject\\config files\\new9.txt"
#define TEST_REGULAR "C:\\Users\\Yair\\workspaceC\\spFinalProject\\config files\\new10.txt"
#define TEST_ANOTHER_REGULAR "C:\\Users\\Yair\\workspaceC\\spFinalProject\\config files\\new11.txt"
#define TEST_VALUE_LESS_ZERO "C:\\Users\\Yair\\workspaceC\\spFinalProject\\config files\\new12.txt"
#define TEST_VALUE_MORE_28 "C:\\Users\\Yair\\workspaceC\\spFinalProject\\config files\\new13.txt"
#define TEST_NOT_TRUE_FALSE "C:\\Users\\Yair\\workspaceC\\spFinalProject\\config files\\new14.txt"
#define TEST_VALUE_MORE_FOUR "C:\\Users\\Yair\\workspaceC\\spFinalProject\\config files\\new15.txt"
#define TEST_SIGN "C:\\Users\\Yair\\workspaceC\\spFinalProject\\config files\\new16.txt"


bool testMissingFile(){

	char imagePath[1024];
	char pcaPath[1024];
	char path[1024] = TEST_MISSING_FILE;
	SP_CONFIG_MSG* msg = (SP_CONFIG_MSG*) malloc(sizeof(char)*1024);
	SPConfig co =spConfigCreate(path, msg);

	ASSERT_TRUE(*msg == SP_CONFIG_CANNOT_OPEN_FILE);

	ASSERT_TRUE(spConfigIsExtractionMode(co, msg) == false);
	ASSERT_TRUE(*msg == SP_CONFIG_INVALID_ARGUMENT);

	ASSERT_TRUE(spConfigMinimalGui(co, msg) == false);
	ASSERT_TRUE(*msg == SP_CONFIG_INVALID_ARGUMENT);

	ASSERT_TRUE(spConfigGetNumOfImages(co, msg) == -1);
	ASSERT_TRUE(*msg == SP_CONFIG_INVALID_ARGUMENT);

	ASSERT_TRUE(spConfigGetNumOfFeatures(co, msg) == -1);
	ASSERT_TRUE(*msg == SP_CONFIG_INVALID_ARGUMENT);

	ASSERT_TRUE(spConfigGetPCADim(co, msg) == -1);
	ASSERT_TRUE(*msg == SP_CONFIG_INVALID_ARGUMENT);

	spConfigGetImagePath(imagePath,co,15);
	ASSERT_TRUE(*msg == SP_CONFIG_INVALID_ARGUMENT);

	spConfigGetPCAPath(pcaPath, co);
	ASSERT_TRUE(*msg == SP_CONFIG_INVALID_ARGUMENT);


	spConfigDestroy (co);
	free(msg);
	return true;

}


bool testDefaultValues()
{
	char path[1024] = TEST_DEFAULT_VAL;
	SP_CONFIG_MSG *msg = (SP_CONFIG_MSG *) malloc(sizeof(char) * 1024);
	SPConfig co = spConfigCreate(path, msg);
	ASSERT_TRUE(!strcmp(co->spImagesDirectory,"./images/"));
	ASSERT_TRUE(!strcmp(co->spImagesPrefix,"img"));
	ASSERT_TRUE(!strcmp(co->spImagesSuffix,".png"));
	ASSERT_TRUE(co->spNumOfImages == 17);
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

	ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);

	spConfigDestroy(co);
	free(msg);
	return true;
}

bool testMissingNumImages(){
	char path[1024]= TEST_MISSING_NUM_IMG;
	SP_CONFIG_MSG* msg = (SP_CONFIG_MSG*) malloc(sizeof(char)*1024);
	SPConfig co =spConfigCreate(path, msg);

	ASSERT_TRUE(*msg == SP_CONFIG_MISSING_NUM_IMAGES);

	spConfigDestroy (co);
	free(msg);
	return true;
}

bool testMissingDirectory(){
	char path[1024] = TEST_MISSING_DIR;
	SP_CONFIG_MSG* msg = (SP_CONFIG_MSG*) malloc(sizeof(char)*1024);
	SPConfig co =spConfigCreate(path, msg);

	ASSERT_TRUE(*msg == SP_CONFIG_MISSING_DIR);
	spConfigDestroy (co);
	free(msg);
	return true;

}

bool testMissingPrefix(){
	char path[1024]= TEST_MISSING_PREFIX;
	SP_CONFIG_MSG* msg = (SP_CONFIG_MSG*) malloc(sizeof(char)*1024);
	SPConfig co =spConfigCreate(path, msg);

	ASSERT_TRUE(*msg == SP_CONFIG_MISSING_PREFIX);
	spConfigDestroy (co);
	free(msg);
	return true;

}

bool testMissingSuffix(){
	char path[1024] = TEST_MISSING_SUFFIX;
	SP_CONFIG_MSG* msg = (SP_CONFIG_MSG*) malloc(sizeof(char)*1024);
	SPConfig co =spConfigCreate(path, msg);

	ASSERT_TRUE(*msg == SP_CONFIG_MISSING_SUFFIX);
	spConfigDestroy (co);
	free(msg);
	return true;

}

bool testBlankLine(){
	char path[1024] = TEST_BLANK_LINE;
	SP_CONFIG_MSG* msg = (SP_CONFIG_MSG*) malloc(sizeof(char)*1024);
	SPConfig co =spConfigCreate(path, msg);

	ASSERT_TRUE(*msg == SP_CONFIG_INVALID_STRING);
	spConfigDestroy (co);
	free(msg);
	return true;

}

bool testLineWithInvalidParameterName(){
	char path[1024] = TEST_INVALID_PARAMETER_NAME;
	SP_CONFIG_MSG* msg = (SP_CONFIG_MSG*) malloc(sizeof(char)*1024);
	SPConfig co =spConfigCreate(path, msg);

	ASSERT_TRUE(*msg == SP_CONFIG_INVALID_STRING);
	spConfigDestroy (co);
	free(msg);
	return true;

}

bool testSpaceInsideStringParameterValue(){
	char path[1024]= TEST_SPACE_STRING_VALUE;
	SP_CONFIG_MSG* msg = (SP_CONFIG_MSG*) malloc(sizeof(char)*1024);
	SPConfig co =spConfigCreate(path, msg);

	ASSERT_TRUE(*msg == SP_CONFIG_INVALID_STRING);
	spConfigDestroy (co);
	free(msg);
	return true;

}

bool testSpaceInsideIntParameterValue(){
	char path[1024] = TEST_SPACE_INT_VALUE;
	SP_CONFIG_MSG* msg = (SP_CONFIG_MSG*) malloc(sizeof(char)*1024);
	SPConfig co =spConfigCreate(path, msg);

	ASSERT_TRUE(*msg == SP_CONFIG_INVALID_INTEGER);
	spConfigDestroy (co);
	free(msg);
	return true;

}

//TODO: when i add an empty line it does not work. need to fix
bool testRegularConfigFIle()
{
	char imagePath[1024];
	char pcaPath[1024];
	char path[1024]= TEST_REGULAR;
	char resultGetImagePath[1024] ="./images/img16.png";
	char resultGetPCAPath[1024] = "./images/pca.yml";

	SP_CONFIG_MSG *msg = (SP_CONFIG_MSG *) malloc(sizeof(char) * 1024);
	SPConfig co = spConfigCreate(path, msg);
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

	ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);

	ASSERT_TRUE(spConfigIsExtractionMode(co, msg) == true);
	ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);

	ASSERT_TRUE(spConfigMinimalGui(co, msg) == false);
	ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);

	ASSERT_TRUE(spConfigGetNumOfImages(co, msg) == 17);
	ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);

	ASSERT_TRUE(spConfigGetNumOfFeatures(co, msg) == 100);
	ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);

	ASSERT_TRUE(spConfigGetPCADim(co, msg) == 20);
	ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);

	spConfigGetImagePath(imagePath,co,16);
	ASSERT_TRUE(!strcmp(imagePath, resultGetImagePath));
	ASSERT_TRUE(spConfigGetImagePath(imagePath,co,16) == SP_CONFIG_SUCCESS);


	ASSERT_TRUE(spConfigGetImagePath(imagePath,co,18) == SP_CONFIG_INDEX_OUT_OF_RANGE);

	spConfigGetPCAPath(pcaPath, co);
	ASSERT_TRUE(!strcmp(pcaPath, resultGetPCAPath));
	ASSERT_TRUE(spConfigGetPCAPath(pcaPath, co) == SP_CONFIG_SUCCESS);

	spConfigDestroy(co);
	free(msg);
	return true;
}

bool testAnotherRegularConfigFIle()
{
	char imagePath[1024];
	char pcaPath[1024];
	char path[1024] = TEST_ANOTHER_REGULAR;
	char resultGetImagePath[1024] ="./imagess/imgg11.pngg";
	char resultGetPCAPath[1024] = "./imagess/pcaa.yml";


	SP_CONFIG_MSG *msg = (SP_CONFIG_MSG *) malloc(sizeof(char) * 1024);
	SPConfig co = spConfigCreate(path, msg);

	ASSERT_TRUE(!strcmp(co->spImagesDirectory,"./imagess/"));
	ASSERT_TRUE(!strcmp(co->spImagesPrefix,"imgg"));
	ASSERT_TRUE(!strcmp(co->spImagesSuffix, ".pngg"));
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

	ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);

	ASSERT_TRUE(spConfigIsExtractionMode(co, msg) == false);
	ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);

	ASSERT_TRUE(spConfigMinimalGui(co, msg) == true);
	ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);

	ASSERT_TRUE(spConfigGetNumOfImages(co, msg) == 12);
	ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);

	ASSERT_TRUE(spConfigGetNumOfFeatures(co, msg) == 82);
	ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);

	ASSERT_TRUE(spConfigGetPCADim(co, msg) == 22);
	ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);

	spConfigGetImagePath(imagePath,co,11);
	ASSERT_TRUE(!strcmp(imagePath, resultGetImagePath));
	ASSERT_TRUE(spConfigGetImagePath(imagePath,co,11) == SP_CONFIG_SUCCESS);


	ASSERT_TRUE(spConfigGetImagePath(imagePath,co,17) == SP_CONFIG_INDEX_OUT_OF_RANGE);

	spConfigGetPCAPath(pcaPath, co);
	ASSERT_TRUE(!strcmp(pcaPath, resultGetPCAPath));
	ASSERT_TRUE(spConfigGetPCAPath(pcaPath, co) == SP_CONFIG_SUCCESS);

	spConfigDestroy(co);
	free(msg);
	return true;
}

bool testInvalidParameterValueLessThan0(){
	char path[1024] = TEST_VALUE_LESS_ZERO;
	SP_CONFIG_MSG* msg = (SP_CONFIG_MSG*) malloc(sizeof(char)*1024);
	SPConfig co =spConfigCreate(path, msg);

	ASSERT_TRUE(*msg == SP_CONFIG_INVALID_INTEGER);
	spConfigDestroy (co);
	free(msg);
	return true;
}

bool testInvalidParameterValueMoreThan28(){
	char path[1024]= TEST_VALUE_MORE_28;
	SP_CONFIG_MSG* msg = (SP_CONFIG_MSG*) malloc(sizeof(char)*1024);
	SPConfig co =spConfigCreate(path, msg);

	ASSERT_TRUE(*msg == SP_CONFIG_INVALID_INTEGER);
	spConfigDestroy (co);
	free(msg);
	return true;
}

bool testInvalidParameterValueNotTrueFalse(){
	char path[1024]= TEST_NOT_TRUE_FALSE;
	SP_CONFIG_MSG* msg = (SP_CONFIG_MSG*) malloc(sizeof(char)*1024);
	SPConfig co =spConfigCreate(path, msg);

	ASSERT_TRUE(*msg == SP_CONFIG_INVALID_ARGUMENT);
	spConfigDestroy (co);
	free(msg);
	return true;
}

bool testInvalidParameterValueMoreThan4()
{
	char path[1024] = TEST_VALUE_MORE_FOUR;
	SP_CONFIG_MSG *msg = (SP_CONFIG_MSG *) malloc(sizeof(char) * 1024);
	SPConfig co = spConfigCreate(path, msg);

	ASSERT_TRUE(*msg == SP_CONFIG_INVALID_INTEGER);
	spConfigDestroy(co);
	free(msg);
	return true;
}

bool testSignInsideIntParameterValue(){
	char path[1024] = TEST_SIGN;
	SP_CONFIG_MSG* msg = (SP_CONFIG_MSG*) malloc(sizeof(char)*1024);
	SPConfig co =spConfigCreate(path, msg);

	ASSERT_TRUE(*msg == SP_CONFIG_INVALID_INTEGER);
	spConfigDestroy (co);
	free(msg);
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

