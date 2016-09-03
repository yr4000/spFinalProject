/*
 * main_aux.h
 *
 *  Created on: 25 баев 2016
 *      Author: Yair
 */

#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

int compareIntBigToSmall(const void *a,const void* b);

KDTreeNode createTreeFromAllFeatures(SPConfig config,int numberOfImages);

int* getAppreanceOfImagesFeatures(SPConfig config,KDTreeNode tree,SPPoint* queryImageFeatures,int queryImageFeaturesNum,int numberOfImages);



#endif /* MAIN_AUX_H_ */
