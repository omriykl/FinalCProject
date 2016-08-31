#ifndef SPKDARRAYS_H_
#define SPKDARRAYS_H_

#include "SPPoint.h"

/** Type for defining the KD Array **/
typedef struct sp_kdarray_t* SPKDArray;

SPKDArray init(SPPoint* arr, int size);

SPKDArray * split(SPKDArray kdArr, int coor);

#endif /* SPKDARRAYS_H_ */
