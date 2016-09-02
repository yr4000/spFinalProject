#include "SPLogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//File open mode
#define SP_LOGGER_OPEN_MODE "w"

// Global variable holding the logger
SPLogger logger = NULL;

SP_LOGGER_MSG printTheParagraph(const char* file, const char* func,
		const int line);
SP_LOGGER_MSG printAllTypes(const char* type_str);

struct sp_logger_t {
	FILE* outputChannel; //The logger file
	bool isStdOut; //Indicates if the logger is stdout
	SP_LOGGER_LEVEL level; //Indicates the level
};

SP_LOGGER_MSG spLoggerCreate(const char* filename, SP_LOGGER_LEVEL level) {
	if (logger != NULL) { //Already defined
		return SP_LOGGER_DEFINED;
	}
	logger = (SPLogger) malloc(sizeof(*logger));
	if (logger == NULL) { //Allocation failure
		return SP_LOGGER_OUT_OF_MEMORY;
	}
	logger->level = level; //Set the level of the logger
	if (filename == NULL) { //In case the filename is not set use stdout
		logger->outputChannel = stdout;
		logger->isStdOut = true;
	} else { //Otherwise open the file in write mode
		logger->outputChannel = fopen(filename, SP_LOGGER_OPEN_MODE);
		if (logger->outputChannel == NULL) { //Open failed
			free(logger);
			logger = NULL;
			return SP_LOGGER_CANNOT_OPEN_FILE;
		}
		logger->isStdOut = false;
	}
	return SP_LOGGER_SUCCESS; //there isn't any problem
}

void spLoggerDestroy() {
	if (!logger) {
		return;
	}
	if (!logger->isStdOut) {//Close file only if not stdout
		fclose(logger->outputChannel);
	}
	free(logger);//free allocation
	logger = NULL;
}

SP_LOGGER_MSG spLoggerPrintError(const char* msg, const char* file,
		const char* function, const int line){

	if (logger == NULL){ // logger is not defined.
			return SP_LOGGER_UNDIFINED;
	}

	else if (file == NULL || msg == NULL || line <= -1 || function == NULL){
		// msg , file, func are null , line isn't positive.
		return SP_LOGGER_INVAlID_ARGUMENT;
	}

// if the writing fails:
	else if (printAllTypes("ERROR") == SP_LOGGER_WRITE_FAIL
		    || printTheParagraph(file, function, line) == SP_LOGGER_WRITE_FAIL
		    || spLoggerPrintMsg(msg) == SP_LOGGER_WRITE_FAIL){
			return SP_LOGGER_WRITE_FAIL;
		}

	else{
			return SP_LOGGER_SUCCESS;//there isn't any problem
		}
}



SP_LOGGER_MSG spLoggerPrintWarning(const char* msg, const char* file,
		const char* function, const int line) {

	if (logger == NULL){ // logger is not defined.
				return SP_LOGGER_UNDIFINED;
		}

	else if (logger->level == SP_LOGGER_ERROR_LEVEL){
		// warning isn't printed, just error is printed.
		return SP_LOGGER_SUCCESS;
	}

	else if (file == NULL || msg == NULL || line <= -1 || function == NULL){
			// msg , file and func are null , line isn't positive.
			return SP_LOGGER_INVAlID_ARGUMENT;
		}

	// if the writing fails:
	else if (printAllTypes("WARNING") == SP_LOGGER_WRITE_FAIL
			    || printTheParagraph(file, function, line) == SP_LOGGER_WRITE_FAIL
			    || spLoggerPrintMsg(msg) == SP_LOGGER_WRITE_FAIL){
				return SP_LOGGER_WRITE_FAIL;
			}
	else{
		return SP_LOGGER_SUCCESS; //there isn't any problem
		}
}

SP_LOGGER_MSG spLoggerPrintInfo(const char* msg) {
	if (logger == NULL){ // logger is not defined.
			return SP_LOGGER_UNDIFINED;
			}
	else if (logger->level != SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL
				&& logger->level != SP_LOGGER_INFO_WARNING_ERROR_LEVEL){
		// info isn't printed, just error or/and warning are printed.
			return SP_LOGGER_SUCCESS;
}

	//empty message is not valid.
	else if (msg == NULL){
		return SP_LOGGER_INVAlID_ARGUMENT;
	}

	// if the writing fails:
	else if (printAllTypes("INFO") == SP_LOGGER_WRITE_FAIL
			|| spLoggerPrintMsg(msg) == SP_LOGGER_WRITE_FAIL){
		return SP_LOGGER_WRITE_FAIL;
	}

	else{
		return SP_LOGGER_SUCCESS; //there isn't any problem
		}
}


SP_LOGGER_MSG spLoggerPrintDebug(const char* msg, const char* file,
		const char* function, const int line) {

	if (logger == NULL){ // logger is not defined.
				return SP_LOGGER_UNDIFINED;
				}

	else if (file == NULL || msg == NULL || line <= -1 || function == NULL){
				// msg , file and func are null , line isn't positive.
				return SP_LOGGER_INVAlID_ARGUMENT;
			}

	else if (logger->level != SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL)
		// debug isn't printed, just error or/and warning or/and info are printed.
		return SP_LOGGER_SUCCESS;


	// if the writing fails:
	else if (printAllTypes("DEBUG") == SP_LOGGER_WRITE_FAIL
				    || printTheParagraph(file, function, line) == SP_LOGGER_WRITE_FAIL
				    || spLoggerPrintMsg(msg) == SP_LOGGER_WRITE_FAIL){
					return SP_LOGGER_WRITE_FAIL;
				}
	else{
		return SP_LOGGER_SUCCESS; //there isn't any problem
		}
}

SP_LOGGER_MSG spLoggerPrintMsg(const char* msg) {

	if (logger == NULL){ // logger is not defined.
					return SP_LOGGER_UNDIFINED;
					}

	//empty message is not valid.
	else if (msg == NULL){
			return SP_LOGGER_INVAlID_ARGUMENT;
		}

	//temp is an integer that tells us if we can write the message to file.
	int temp= fprintf(logger->outputChannel, "- message: %s\n", msg);

	if (temp<0){ // if temp<0 can't write.
		return SP_LOGGER_WRITE_FAIL;
	}
	else{
			return SP_LOGGER_SUCCESS; //there isn't any problem
			}
}

SP_LOGGER_MSG printAllTypes(const char* type_str) {

	//temp is an integer that tells us if we can write the message to file.
	int temp= fprintf(logger->outputChannel, "---%s---\n", type_str);
	if (temp < 0){
		return SP_LOGGER_WRITE_FAIL;
	}
	else{
		return SP_LOGGER_SUCCESS; //there isn't any problem
	}
}


SP_LOGGER_MSG printTheParagraph(const char* file, const char* func,
		const int line) {

	//the temps are integers that tells us if we can write the message to file.
	int temp_file= fprintf(logger->outputChannel, "- file: %s\n", file);
	int temp_func= fprintf(logger->outputChannel, "- function: %s\n", func);
	int temp_line= fprintf(logger->outputChannel, "- line: %d\n", line);
	if (temp_file < 0 ||temp_line  < 0 || temp_func < 0){
		return SP_LOGGER_WRITE_FAIL;
	}

	else{
	return SP_LOGGER_SUCCESS; //there isn't any problem
	}
}
