#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPLogger.h"
#include "SPConfig.h"
#include <ctype.h>
#include "limits.h"
#define EQUALS_SIGN '='
#define SPACE " "
#define STRING_END '\0'
#define LENGTH_OF_LINE 1024
#define OPEN_MODE "r"
#define TRUE "true"
#define FALSE "false"

int firstTypeOfError = 1; // in case of invalid line.
int secondTypeOfError = 2; // in case of invalid parameter value.
int thirdTypeOfError = 3; // in case of at least one parameter with no default value is not set.

// Global variable holding the config.
SPConfig config = NULL;



//sub function of create - extracts the values from the current line and insert them
// to value and name.
void extractValuesFromLine(char* source,char* destination, char* value, char* name){

	while (*source != 0)
	{
		// copy the char
		*destination = *source++;

		if (*source == EQUALS_SIGN)
		{
			destination++;
			*destination = 0;
			destination = value;
			*source++;
		}
		else {
			destination++;
		};
	}
	*destination = 0;

	//cutting the spaces in both sides of name and value.
	strcpy(name, trim(name));
	strcpy(value, trim(value));
}

SP_CONFIG_MSG isInvalidStringValue(SP_CONFIG_MSG* msg, char* filename,int numOfLine,char* noParameter,
		char* line, char* destination, char* value, char* name ){
	if (strpbrk(value, SPACE) != NULL)
	{
		*msg = SP_CONFIG_INVALID_STRING;
		massageCreater(filename, secondTypeOfError, numOfLine, noParameter, line,
				destination, name, value);
	}
	return *msg;
}

SP_CONFIG_MSG isInvalidIntValue(SP_CONFIG_MSG* msg, int intValue, int min, int max,
		char* filename,int numOfLine,char* noParameter,char* line, char* destination, char* name, char* value){

	if (intValue <= min || intValue>=max)
	{
		*msg = SP_CONFIG_INVALID_INTEGER;
		massageCreater(filename, secondTypeOfError, numOfLine, noParameter, line,
				destination, name, value);
	}
	return *msg;
}

SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg)
{

	//SPLogger logger = NULL;
	char noParameter[LENGTH_OF_LINE] = "x"; //used when there is thirdTypeOfError.

	*msg = SP_CONFIG_SUCCESS; //initial value of massage.

	if (filename == NULL)
	{
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}
	FILE *file;
	file = fopen(filename, OPEN_MODE);

	if (file == NULL)
	{ //cannot open file.
		*msg = SP_CONFIG_CANNOT_OPEN_FILE;
		fclose(file);
		return NULL;
	}
	config = (SPConfig) malloc(sizeof(*config));

	if (config == NULL)
	{ //Allocation failure
		*msg = SP_CONFIG_ALLOC_FAIL;
		return NULL;
	}


	char line[LENGTH_OF_LINE + 1], *destination, *source;
	char name[LENGTH_OF_LINE + 1] = { STRING_END } , value[LENGTH_OF_LINE + 1] = { STRING_END };
	int numOfLine = 0;
	int intValue;

	// initializing the config struct fields to the default ones.
	config->spPCADimension = 20;
	strcpy(config->spPCAFilename, "pca.yml");
	config->spNumOfFeatures = 100;
	config->spExtractionMode = true;
	config->spMinimalGUI = false;
	config->spNumOfSimilarImages = 1;
	config->spKNN = 1;
	config->spKDTreeSplitMethod = MAX_SPREAD;
	config->spLoggerLevel = 3;
	strcpy(config->spLoggerFilename, "stdout");
	memset(config->spImagesDirectory, STRING_END, LENGTH_OF_LINE +1);
	memset(config->spImagesPrefix, STRING_END, LENGTH_OF_LINE + 1);
	memset(config->spImagesSuffix, STRING_END, LENGTH_OF_LINE + 1);

	// flag which checks if spNumOfImages was initialized in the config file.
	bool exist_spNumOfImages = false;

	// looping the lines of the config file.
	while (fgets(line, LENGTH_OF_LINE + 1, file) != STRING_END)
	{
		strcpy(line, trim(line));
		numOfLine++;
		//ignore blank line
		if (line[0] == STRING_END){
			continue;
		}

		// not a comment
		if (line[0] != '#')
		{
			// source points to the line
			source = line;
			// destination  points to name
			destination = name;

			extractValuesFromLine(source,destination,value,name);

			//zero the line for continuing the loop correctly
			memset(line, STRING_END, LENGTH_OF_LINE +1);

			//checking each name if fits one of the parameters

			if (!strcmp(name, "spImagesDirectory"))
			{
				if(isInvalidStringValue(msg,filename,numOfLine,noParameter,line,
						destination,value,name)==SP_CONFIG_INVALID_STRING) return NULL;

				strcpy(config->spImagesDirectory, value);

			}

			else if (!strcmp(name, "spImagesPrefix"))
			{
				if(isInvalidStringValue(msg,filename,numOfLine,noParameter,line,
						destination,value,name)==SP_CONFIG_INVALID_STRING) return NULL;

				strcpy(config->spImagesPrefix, value);


			}

			else if (!strcmp(name, "spImagesSuffix"))
			{

				if (strcmp(value, ".jpg") && strcmp(value, ".png") && strcmp(value, ".bmp") && strcmp(value, ".gif"))
				{
					*msg = SP_CONFIG_INVALID_STRING;
					massageCreater(filename, secondTypeOfError, numOfLine, noParameter, line,
							destination, name, value);
				}

				strcpy(config->spImagesSuffix, value);


			}

			else if (!strcmp(name, "spNumOfImages"))
			{
				exist_spNumOfImages = true;
				intValue = spAtoi(value);
				if(isInvalidIntValue(msg,intValue,0,INT_MAX,filename,numOfLine
						,noParameter,line,destination,name, value)==SP_CONFIG_INVALID_INTEGER){
					return NULL;
				}


				config->spNumOfImages = intValue;

			}

			else if (!strcmp(name, "spPCADimension"))
			{
				intValue = spAtoi(value);
				if(isInvalidIntValue(msg,intValue,9,29,filename,numOfLine
						,noParameter,line,destination,name, value)==SP_CONFIG_INVALID_INTEGER){
					return NULL;
				}
				config->spPCADimension = intValue;
			}

			else if (!strcmp(name, "spPCAFilename"))
			{
				if(isInvalidStringValue(msg,filename,numOfLine,noParameter,line,
						destination,value,name)==SP_CONFIG_INVALID_STRING) return NULL;

				strcpy(config->spPCAFilename, value);

			}

			else if (!strcmp(name, "spNumOfFeatures"))
			{
				intValue = spAtoi(value);
				if(isInvalidIntValue(msg,intValue,0,INT_MAX,filename,numOfLine
						,noParameter,line,destination,name, value)==SP_CONFIG_INVALID_INTEGER){
					return NULL;
				}
				config->spNumOfFeatures = intValue;

			}

			else if (!strcmp(name, "spExtractionMode"))
			{
				if (!strcmp(value, TRUE))
				{
					config->spExtractionMode = true;

				}
				else if (!strcmp(value, FALSE))
				{
					config->spExtractionMode = false;
				}

				else
				{
					*msg = SP_CONFIG_INVALID_ARGUMENT;
					massageCreater(filename, secondTypeOfError, numOfLine, noParameter, line,
							destination, name, value);
					return NULL;
				}
			}

			else if (!strcmp(name, "spNumOfSimilarImages"))
			{
				intValue = spAtoi(value);
				if(isInvalidIntValue(msg,intValue,0,INT_MAX,filename,numOfLine
						,noParameter,line,destination,name, value)==SP_CONFIG_INVALID_INTEGER){
					return NULL;
				}

				config->spNumOfSimilarImages = intValue;

			}

			else if (!strcmp(name, "spKDTreeSplitMethod"))
			{

				if (!strcmp(value, "RANDOM"))
				{
					config->spKDTreeSplitMethod = RANDOM;
				}
				else if (!strcmp(value, "MAX_SPREAD"))
				{
					config->spKDTreeSplitMethod = MAX_SPREAD;
				}
				else if (!strcmp(value, "INCREMENTAL"))
				{
					config->spKDTreeSplitMethod = INCREMENTAL;
				}
				else
				{
					*msg = SP_CONFIG_INVALID_ARGUMENT;
					massageCreater(filename, secondTypeOfError, numOfLine, noParameter, line,
							destination, name, value);
					return NULL;
				}

			}

			else if (!strcmp(name, "spKNN"))
			{
				intValue = spAtoi(value);
				if(isInvalidIntValue(msg,intValue,0,INT_MAX,filename,numOfLine
						,noParameter,line,destination,name, value)==SP_CONFIG_INVALID_INTEGER){
					return NULL;
				}
				config->spKNN = intValue;
			}

			else if (!strcmp(name, "spMinimalGUI"))
			{
				if (!strcmp(value, TRUE))
				{
					config->spMinimalGUI = true;

				}
				else if (!strcmp(value, FALSE))
				{
					config->spMinimalGUI = false;
				}

				else
				{
					*msg = SP_CONFIG_INVALID_ARGUMENT;
					massageCreater(filename, secondTypeOfError, numOfLine, noParameter, line,
							destination, name, value);
					return NULL;
				}
			}

			else if (!strcmp(name, "spLoggerLevel"))
			{
				intValue = spAtoi(value);
				if(isInvalidIntValue(msg,intValue,0,5,filename,numOfLine
						,noParameter,line,destination,name, value)==SP_CONFIG_INVALID_INTEGER){
					return NULL;
				}
				config->spLoggerLevel = intValue;

			}

			else if (!strcmp(name, "spLoggerFilename"))
			{
				if(isInvalidStringValue(msg,filename,numOfLine,noParameter,line,
						destination,value,name)==SP_CONFIG_INVALID_STRING) return NULL;

				strcpy(config->spLoggerFilename, value);


			}


			else
			{
				massageCreater(filename, firstTypeOfError, numOfLine, noParameter, line,
						destination, name, value);
				*msg = SP_CONFIG_INVALID_STRING;
				return NULL;
			}
		}
	}





	//checking if one of those parameters without default values had been initialized.

	if (strlen(config->spImagesDirectory) == 0 )
	{
		*msg = SP_CONFIG_MISSING_DIR;
		char spImagesDirectory[LENGTH_OF_LINE+1] = "spImagesDirectory";
		massageCreater(filename, thirdTypeOfError, numOfLine, spImagesDirectory, line,
				destination, name, value);
		return NULL;
	}

	if (strlen(config->spImagesPrefix) == 0)
	{
		*msg = SP_CONFIG_MISSING_PREFIX;
		char spImagesPrefix[LENGTH_OF_LINE+1] = "spImagesPrefix";
		massageCreater(filename, thirdTypeOfError, numOfLine, spImagesPrefix, line,
				destination, name, value);
		return NULL;
	}

	if (strlen(config->spImagesSuffix) == 0)
	{
		*msg = SP_CONFIG_MISSING_SUFFIX;
		char spImagesSuffix[LENGTH_OF_LINE+1] = "spImagesSuffix";
		massageCreater(filename, thirdTypeOfError, numOfLine, spImagesSuffix, line,
				destination, name, value);
		return NULL;
	}

	if (exist_spNumOfImages == false)
	{
		*msg = SP_CONFIG_MISSING_NUM_IMAGES;
		char spNumOfImages[LENGTH_OF_LINE+1] = "spNumOfImages";
		massageCreater(filename, thirdTypeOfError, numOfLine, spNumOfImages, line,
				destination, name, value);
		return NULL;
	}

	spLoggerDestroy(); //TODO !!!
	if(spLoggerCreate(config->spLoggerFilename, config->spLoggerLevel) != SP_LOGGER_SUCCESS)
	{
		printf("ERROR! There was a problem with initializing the logger.");
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}


	return config;

}



char* trim(char *str)
{
	char *end;

	// Trim leading space
	while(isspace(*str)) str++;

	if(*str == 0){ // All spaces?
		return str;
	}


	// Trim trailing space
	end = str + strlen(str) - 1;
	while(end > str && isspace(*end)) end--;

	// Write new null terminator
	*(end+1) = 0;

	return str;

}

// An utility function to check whether x is numeric.
bool isNumericChar(char x)
{
	return (x >= '0' && x <= '9')? true: false;
}

// spAtoi function checks If the given string contains
// any invalid character, then this function returns 0, otherwise returns its numeric value.
int spAtoi(char *str)
{
	if (*str == NULL)
		return 0;

	int res = 0;  // Initialize result
	int i = 0;  // Initialize index of first digit

	// Iterate through all digits of input string and update result
	for (; str[i] != '\0'; ++i)
	{
		if (isNumericChar(str[i]) == false)
			return 0;

		res = res*10 + str[i] - '0';
	}

	// Return result with sign
	return res;
}

//creating the error massage that printed in stdout in case of error.
//can be one of 3 types of errors: problem with line, problem with specific parameter and parameter that have no defalut value and
//haven't been initialized.
//only the earliest error will appear.
void massageCreater(const char* filename, int typeOfError, const int lineNum, const char*
		parameterName, char* line, char* destination, char* name, char* value){

	char noParameter[LENGTH_OF_LINE+1] = "x";

	printf("File: %s\n", filename);
	printf("Line: %d\n", lineNum);
	if (typeOfError == 1){
		printf("Message: Invalid configuration line\n");
	}
	else if (typeOfError == 2){
		printf("Message: Invalid value - constraint not met\n");
	}

	else if(strcmp(parameterName, noParameter)){
		printf("Parameter %s is not set\n", parameterName);
	}

	spConfigDestroy(config);
	//	exit(0);

}

bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg){

	if (config == NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}

	if (config->spExtractionMode == true || config->spExtractionMode == false){
		*msg = SP_CONFIG_SUCCESS;
		return config->spExtractionMode;
	}

	return false;
}

bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG* msg){

	if (config == NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}

	if (config->spMinimalGUI == true || config->spMinimalGUI == false){
		*msg = SP_CONFIG_SUCCESS;
		return config->spMinimalGUI;
	}
	return false;
}

int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg){

	if (config == NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spNumOfImages;
}

int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg){

	if (config == NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spNumOfFeatures;
}

int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg){

	if (config == NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spPCADimension;
}

SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config,
		int index){

	if ( imagePath == NULL || config == NULL){
		return SP_CONFIG_INVALID_ARGUMENT;
	}

	if (index >= config->spNumOfImages){
		return SP_CONFIG_INDEX_OUT_OF_RANGE;
	}

	snprintf(imagePath, LENGTH_OF_LINE+1, "%s%s%d%s",  config->spImagesDirectory,
			config->spImagesPrefix, index, config->spImagesSuffix);


	return SP_CONFIG_SUCCESS;
}


SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config){

	if ( pcaPath == NULL || config == NULL){
		return SP_CONFIG_INVALID_ARGUMENT;
	}

	snprintf(pcaPath, LENGTH_OF_LINE+1, "%s%s",  config->spImagesDirectory,
			config->spPCAFilename);


	return SP_CONFIG_SUCCESS;
}

void spConfigDestroy(SPConfig config){

	if (config != NULL){
		free(config);//free allocation;
		config = NULL;
	}

}

SP_CONFIG_MSG getSpImagesDirectory(char* spImagesDirectory, const SPConfig config){ //TODO !!!

	if ( spImagesDirectory == NULL || config == NULL){
		return SP_CONFIG_INVALID_ARGUMENT;
	}

	snprintf(spImagesDirectory, LENGTH_OF_LINE+1, "%s",  config->spImagesDirectory);
	return SP_CONFIG_SUCCESS;

}

SP_CONFIG_MSG getSpImagesPrefix(char* spImagesPrefix, const SPConfig config){ //TODO !!!

	if ( spImagesPrefix == NULL || config == NULL){
		return SP_CONFIG_INVALID_ARGUMENT;
	}

	snprintf(spImagesPrefix, LENGTH_OF_LINE+1, "%s",  config->spImagesPrefix);
	return SP_CONFIG_SUCCESS;

}

int getSpNumOfSimilarImages (const SPConfig config,  SP_CONFIG_MSG* msg){ //TODO !!!
	if ( config == NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}

	*msg = SP_CONFIG_SUCCESS;
	return config->spNumOfSimilarImages;
}

spKDTreeSplitMethodEnum getSpKDTreeSplitMethod(const SPConfig config, SP_CONFIG_MSG* msg){ //TODO !!!

	if ( config == NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
	}

	*msg = SP_CONFIG_SUCCESS;
	return config->spKDTreeSplitMethod;

}


int getSpKNN (const SPConfig config,  SP_CONFIG_MSG* msg){ //TODO !!!
	if ( config == NULL){
			*msg = SP_CONFIG_INVALID_ARGUMENT;
			return -1;
		}

		*msg = SP_CONFIG_SUCCESS;
		return config->spKNN;
}






