#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "unit_test_util.h" //SUPPORTING MACROS ASSERT_TRUE/ASSERT_FALSE etc..
#include "../SPLogger.h"


// This is a helper function which checks if two files are identical
static bool identicalFiles(const char* fname1, const char* fname2) {
	FILE *fp1, *fp2;
	fp1 = fopen(fname1, "r");
	fp2 = fopen(fname2, "r");
	char ch1 = EOF, ch2 = EOF;

	if (fp1 == NULL) {
		return false;
	} else if (fp2 == NULL) {
		fclose(fp1);
		return false;
	} else {
		ch1 = getc(fp1);
		ch2 = getc(fp2);

		while ((ch1 != EOF) && (ch2 != EOF) && (ch1 == ch2)) {
			ch1 = getc(fp1);
			ch2 = getc(fp2);
		}
		fclose(fp1);
		fclose(fp2);
	}
	if (ch1 == ch2) {
		return true;
	} else {
		return false;
	}
}

//Logger is not defined
static bool basicLoggerTest() {
	ASSERT_TRUE(spLoggerPrintError("A","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
	spLoggerDestroy();
	return true;
}

//Only Errors should be printed
static bool basicLoggerErrorTest() {
	const char* expectedFile = "basicLoggerErrorTestExp.log";
	const char* testFile = "basicLoggerErrorTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;
}

//All messages should be printed in debug level
static bool basicLoggerDebugTest() {
	const char* expectedFile = "basicLoggerDebugTestExp.log";
	const char* testFile = "basicLoggerDebugTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;
}

//----------------------------------------------------------------TODO

//Only Errors, Warnings and Info should be printed
static bool basicLoggerInfoTest() {
	const char* expectedFile = "basicLoggerInfoTestExp.log";
	const char* testFile = "basicLoggerInfoTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;
}

//Only Errors and warnings should be printed in debug level
static bool basicLoggerWarningTest() {
	const char* expectedFile = "basicLoggerWarningTestExp.log";
	const char* testFile = "basicLoggerWarningTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;
}

static bool loggerUndifinedTest() {
	//checking the situation of NULL as a logger (we should get the message "SP_LOGGER_UNDIFINED").
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_UNDIFINED);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
	return true;
}

// error- checking all the posibilities of null arguments
static bool errorTest(){
	const char* testFile = "basicLoggerErrorTest.log";
	spLoggerCreate(testFile,SP_LOGGER_ERROR_LEVEL);
	ASSERT_TRUE(spLoggerPrintError(NULL,"sp_logger_unit_test.c",  __func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c", NULL,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
	ASSERT_TRUE(spLoggerPrintError("MSGA",NULL, __func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c", __func__,-5) == SP_LOGGER_INVAlID_ARGUMENT);
	spLoggerDestroy();
	return true;
}

// warning- checking all the posibilities of null arguments
static bool warningTest(){
	const char* testFile = "basicLoggerWarningTest.log";
	spLoggerCreate(testFile,SP_LOGGER_WARNING_ERROR_LEVEL);
	ASSERT_TRUE(spLoggerPrintWarning(NULL,"sp_logger_unit_test.c",  __func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c", NULL,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB",NULL, __func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c", __func__,-5) == SP_LOGGER_INVAlID_ARGUMENT);
	spLoggerDestroy();
	return true;
}

// info- checking all the posibilities of null arguments
static bool infoTest(){
	const char* testFile = "basicLoggerInfoTest.log";
	spLoggerCreate(testFile,SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL);
	ASSERT_TRUE(spLoggerPrintInfo(NULL) == SP_LOGGER_INVAlID_ARGUMENT);
	spLoggerDestroy();
	return true;
}

// debug- checking all the posibilities of null arguments
static bool debugTest(){
	const char* testFile = "basicLoggerDebugTest.log";
	spLoggerCreate(testFile,SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL);
	ASSERT_TRUE(spLoggerPrintDebug(NULL,"sp_logger_unit_test.c",  __func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c", NULL,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD",NULL, __func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c", __func__,-5) == SP_LOGGER_INVAlID_ARGUMENT);
	spLoggerDestroy();
	return true;
}

// checking all the function above
// (in order to run all the tests, we should make "EXP.log" files those 4 files are incloded in the zip file.
// for each logger mode:error, warning, info, debug. so each output can be compared to the correct log file.
//int main() {
//	RUN_TEST(errorTest);
//	RUN_TEST(warningTest);
//	RUN_TEST(infoTest);
//	RUN_TEST(debugTest);
//	RUN_TEST(basicLoggerTest);
//	RUN_TEST(basicLoggerErrorTest);
//	RUN_TEST(basicLoggerDebugTest);
//	RUN_TEST(basicLoggerInfoTest);
//	RUN_TEST(basicLoggerWarningTest);
//	RUN_TEST(loggerUndifinedTest);
//	return 0;
//}
