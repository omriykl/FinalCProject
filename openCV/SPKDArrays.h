#ifndef SPKDARRAYS_H_
#define SPKDARRAYS_H_

#include "SPPoint.h"

/** Type for defining the KD Array **/
typedef struct sp_kdarray_t* SPKDArray;

SPKDArray init(SPPoint* arr, int size);

#endif /* SPKDARRAYS_H_ */
