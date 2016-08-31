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
	struct indexPlusVal* vals;
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
		vals = malloc(sizeof(struct indexPlusVal)*size);
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

	return kdArr;
}

SPKDArray * split(SPKDArray kdArr, int coor)
{
	SPKDArray left,right;
	SPKDArray * leftAndRight;
	int i,j;
	int middle = kdArr->numOfPoints / 2;
	SPPoint * leftPoints;
	SPPoint * rightPoints;
	int* x;
	int* map1;
	int* map2;

	//start allocation of x,map1,map2
	x = (int*) malloc(sizeof(int) * kdArr->numOfPoints);
	if (x == NULL)
		return NULL; //TODO: log error
	map1 = (int*) malloc(sizeof(int) * kdArr->numOfPoints);
	if (map1 == NULL)
	{
		free(x);
		return NULL; //TODO: log error
	}
	map2 = (int*) malloc(sizeof(int) * kdArr->numOfPoints);
	if (map2 == NULL)
	{
		free(x);
		free(map1);
		return NULL; //TODO: log error
	}

	//calculation of x, which will hold 0 if index i of x is in left side, or 1 if it's in thr right side
	for (i=0;i<middle;i++)
		x[kdArr->matrix[coor][i]] = 0;
	for (i=middle;i<kdArr->numOfPoints;i++)
		x[kdArr->matrix[coor][i]] = 1;

	for (i=0;i<kdArr->numOfPoints;i++)
		printf("%d = %d\n",i,x[i]);

	return NULL;

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
	double data1[3]={1.3,4.6,5.3};
	double data2[3]={5.3,5.6,5.1};
	SPPoint p[2];
	p[0]= spPointCreate(data1,3,0);
	p[1]= spPointCreate(data2,3,1);

	SPKDArray kdAr = init(p,2);

	int i,j;

	for (i=0;i<3;i++)
	{
		printf("[");
		for (j=0;j<2;j++)
		{
			printf("%d , ",kdAr->matrix[i][j]);
		}
		printf("]\n");
	}

	split(kdAr,0);

}
