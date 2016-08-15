#ifndef SPFEATURES_H_
#define SPFEATURES_H_

#include "SPPoint.h"

/**
 * saves the features of an image in a file "imgDir+imgPre+index.feats"
 * the data will saved in a binary file
 *
 */
void saveFeaturesToFile(char * imgDir,char * imgPre,int index,SPPoint * features,int numberOfFeats);

/**
 * get the features of an image from the file "imgDir+imgPre+index.feats"
 *
 * @return
 * array of features
 */
SPPoint * getFeaturesFromFile(char * imgDir,char * imgPre,int index, int * numberOfFeats);


#endif /* SPFEATURES_H_ */
