#include "SPKDArrays.h"
#include "SPPoint.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

struct sp_kdarray_t {
	SPPoint* points;
	int** matrix;
	int numOfDims;
	int numOfPoints;
};

struct indexPlusVal
{
	int index;
	double value;
};

int cmpFuncByVals (const void * a, const void * b)
{
	struct indexPlusVal * a1 = (struct indexPlusVal *) a;
	struct indexPlusVal * b1 = (struct indexPlusVal *) b;

	if (a1->value > b1->value)
		return 1;
	return -1;
}

SPKDArray init(SPPoint* arr, int size)
{
	int i,j;
	int dim;
	SPKDArray kdArr = (SPKDArray) malloc(sizeof(*kdArr));
	if (kdArr == NULL)
		return NULL;
	kdArr->numOfPoints=size;
	kdArr->points = malloc(sizeof(SPPoint)*size);
	if (kdArr->points == NULL)
	{
		free(kdArr);
		return NULL;
	}

	for (i=0;i<size;i++)
	{
		kdArr->points[i] = (SPPoint) malloc(sizeof(SPPoint));
		//TODO: free all in case of null
		kdArr->points[i] = spPointCopy(arr[i]);
	}

	dim = spPointGetDimension(arr[0]);
	kdArr->numOfDims=dim;

	kdArr->matrix = (int**) malloc(dim*sizeof(int*));
	//TODO: check allocation
	for (i=0;i<dim;i++)
	{
		struct indexPlusVal* vals = malloc(sizeof(struct indexPlusVal)*size);
		for (j=0;j<size;j++)
		{
			vals[j].index = j;
			vals[j].value = spPointGetAxisCoor(arr[j],i);
		}

		qsort(vals,size,sizeof(struct indexPlusVal),cmpFuncByVals);

		kdArr->matrix[i] = (int*) malloc(sizeof(int)*size);
		//TODO: check allocation
		for (j=0;j<size;j++)
		{
			kdArr->matrix[i][j] = vals[j].index;
		}

		free(vals);

	}


}

SPKDArrayDestroy(SPKDArray arr){
	int i;
	for(i=0;i<arr->numOfPoints;i++){
		spPointDestroy(arr->points[i]);
	}
	free(arr->points);
	for(i=0;i<arr->numOfDims;i++){
		free(arr->matrix[i]);
	}
	free(arr->matrix);
}

void main()
{

}
