/*
 * main_aux.h
 *
 */

#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

#include "SPPoint.h"
#include "SPConfig.h"
#include "KDTree.h"

SPConfig createConfigFromFilePath(int argc, char* argv[]);

int compareIntBigToSmall(const void *a,const void* b);

KDTreeNode createTreeFromAllFeatures(SPConfig config,int numberOfImages);

int* getAppreanceOfImagesFeatures(SPConfig config,KDTreeNode tree,SPPoint* queryImageFeatures,int queryImageFeaturesNum,int numberOfImages);



#endif /* MAIN_AUX_H_ */
