/*
 * Extracted.h
 *
 *  Created on: 22 баев 2016
 *      Author: Yair
 */

#ifndef EXTRACTED_H_
#define EXTRACTED_H_

#include "SPConfig.h"
#include "SPPoint.h"

typedef enum sp_extracted_msg{
	SP_EXTRACT_FAILED_TO_OPEN_FILE,
	SP_EXTRACT_FAILED_GENERATE_FILEPATH,
	SP_EXTRACT_FAILED_TO_WRITE_TO_FILE,
	SP_EXTRACT_INVALID_ARGUMENT,
	SP_EXTRACT_FILE_CONTENT_DOESNT_MATCH_THE_FORMAT,
	SP_EXTRACT_SUCCESS
}SP_EXTRACTED_MSG;

/*initialise ext from pictures. we need to the data in config to do it right.
* this function will also write the data in SPPoint to the file.
*this function gets an array of points, the images index, a config object and
*the number of features (the size of the array), and generates a feat file with the
*data inside the points array.
*pre: arr!=NULL, index>=0, config!=NULL,numOfFeatures>=0
*pro: sends success message and create feat file.
*/
SP_EXTRACTED_MSG initExtractionMode(SPPoint* arr, int index, SPConfig config,int numOfFeatues);

/*this function extract the data from a feat file of the relevant image to the index,
* into the unintialised SPPoint* arr.
* here we write the data from a file to the matrix acording to config.
*/
SP_EXTRACTED_MSG initNonExtractionMode(SPPoint** arr,int index, SPConfig config,int* numberOfFeatures);

//this function will make the desired file name
char* getFeatsFileName(SPConfig config, int index);

#endif /* EXTRACTED_H_ */
