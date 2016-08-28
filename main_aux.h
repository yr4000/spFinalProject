/*
 * main_aux.h
 *
 *  Created on: 25 баев 2016
 *      Author: Yair
 */

#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

int compareIntBigToSmall(const void *a,const void* b);

int* getAppreanceOfImagesFeatures(SPConfig config,SP_CONFIG_MSG* msg,SPPoint* queryImageFeatures,int queryImageFeaturesNum,SPPoint* arr);



#endif /* MAIN_AUX_H_ */
