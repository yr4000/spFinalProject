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

typedef struct sp_extracted *Extracted;

struct sp_extracted{
	SPPoint* features;
};

// create a new Extracted object
Extracted createExtracted(int numOfPoints);

//initialise ext from pictures. we need to the data in config to do it right.
// this function will also write the data in SPPoint to the file.
//TODO: problem - will it always rewrite? remember for each picture 1 file
SP_EXTRACTED_MSG initExtractionMode(SPPoint* arr, int index, SPConfig config,int numOfFeatues);

//here we write the data from a file to the matrix acording to config.
//maybe better save the number of feats in the file itself.
SP_EXTRACTED_MSG initNonExtractionMode(SPPoint* arr,int index, SPConfig config);

//this function will make the desired file name
char* getFeatsFileName(SPConfig config, int index);

#endif /* EXTRACTED_H_ */
