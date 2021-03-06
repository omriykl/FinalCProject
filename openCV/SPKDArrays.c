#include "SPKDArrays.h"
#include "SPPoint.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "SPLogger.h"
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
	{
		spLoggerPrintError("error while allocation of kdArr",__FILE__, __func__, __LINE__);
        return NULL;
	}

	kdArr->numOfPoints=size;
	kdArr->points = (SPPoint*) malloc(sizeof(SPPoint)*size);
	if (kdArr->points == NULL)
	{
		spLoggerPrintError("error while allocation of points",__FILE__, __func__, __LINE__);
		free(kdArr);
		return NULL;
	}

	for (i=0;i<size;i++)
	{
		kdArr->points[i] = spPointCopy(arr[i]);
	}

	dim = spPointGetDimension(arr[0]);
	kdArr->numOfDims=dim;

	kdArr->matrix = (int**) malloc(dim*sizeof(int*));
	if(kdArr->matrix==NULL)
	{
		spLoggerPrintError("error while allocation of kdArr->matrix",__FILE__, __func__, __LINE__);
		SPKDArrayDestroy(kdArr);
		return NULL;
	}

	for (i=0;i<dim;i++)
	{
		vals = (struct indexPlusVal*) malloc(sizeof(struct indexPlusVal)*size);
		if(vals==NULL){
				spLoggerPrintError("error while allocation of vals",__FILE__, __func__, __LINE__);
				SPKDArrayDestroy(kdArr);
				return NULL;
			}
		for (j=0;j<size;j++)
		{
			vals[j].index = j;
			vals[j].value = spPointGetAxisCoor(arr[j],i);
		}

		qsort(vals,size,sizeof(struct indexPlusVal),cmpFuncByVals);

		kdArr->matrix[i] = (int*) malloc(sizeof(int)*size);
		if(kdArr->matrix[i]==NULL)
		{
			spLoggerPrintError("error while allocation of kdArr->matrix[i]",__FILE__, __func__, __LINE__);
			free(vals);
			SPKDArrayDestroy(kdArr);
			return NULL;
		}

		for (j=0;j<size;j++)
		{
			kdArr->matrix[i][j] = vals[j].index;
		}

		free(vals);
	}

	return kdArr;
}

void destroyPointsArr(SPPoint * points,int size)
{
	int i;
	for (i=0;i<size;i++)
		spPointDestroy(points[i]);
	free(points);
}

void freeDoubleIntArr(int** arr,int size)
{
	int i;
	for (i=0;i<size;i++)
		free(arr[i]);
}

SPKDArray * split(SPKDArray kdArr, int coor)
{
	SPKDArray left,right;
	SPKDArray * leftAndRight;
	int i,j,lIndex,rIndex;
	int middle = (kdArr->numOfPoints %2 == 0) ? (kdArr->numOfPoints / 2) : (kdArr->numOfPoints / 2)+1;
	SPPoint * leftPoints;
	SPPoint * rightPoints;
	int* x;
	int* map1;
	int* map2;
	int** matrixLeft;
	int** matrixRight;

	//start allocation of x,map1,map2
	x = (int*) malloc(sizeof(int) * kdArr->numOfPoints);
	if (x == NULL){
		spLoggerPrintError("error while allocation of x",__FILE__, __func__, __LINE__);
		return NULL;
	}

	map1 = (int*) malloc(sizeof(int) * kdArr->numOfPoints);
	if (map1 == NULL)
	{
		free(x);
		spLoggerPrintError("error while allocation of map1",__FILE__, __func__, __LINE__);
		return NULL;
	}
	map2 = (int*) malloc(sizeof(int) * kdArr->numOfPoints);
	if (map2 == NULL)
	{
		free(x);
		free(map1);
		spLoggerPrintError("error while allocation of map2",__FILE__, __func__, __LINE__);
		return NULL;
	}

	//calculation of x, which will hold 0 if index i of x is in left side, or 1 if it's in the right side
	for (i=0;i<middle;i++)
		x[kdArr->matrix[coor][i]] = 0;
	for (i=middle;i<kdArr->numOfPoints;i++)
		x[kdArr->matrix[coor][i]] = 1;

	//allocation of leftPoints and rightPoints
	leftPoints = (SPPoint*) malloc(sizeof(SPPoint)*middle);
	if (leftPoints == NULL)
	{
		free(x);
		free(map1);
		free(map2);
		spLoggerPrintError("error while allocation of leftPoints",__FILE__, __func__, __LINE__);
		return NULL;
	}
	rightPoints = (SPPoint*) malloc(sizeof(SPPoint)*((kdArr->numOfPoints) - middle));
	if (rightPoints == NULL)
	{
		free(x);
		free(map1);
		free(map2);
		free(leftPoints);
		spLoggerPrintError("error while allocation of rightPoints",__FILE__, __func__, __LINE__);
		return NULL;
	}


	//start filling leftPoints and rightPoints
	lIndex =0;
	rIndex = 0;
	for (i=0;i<kdArr->numOfPoints;i++)
	{
		if (x[i] == 0)
		{
			leftPoints[lIndex] = spPointCopy(kdArr->points[i]);
			map1[i] = lIndex;
			map2[i] = -1;
			lIndex++;
		}
		else
		{
			rightPoints[rIndex] = spPointCopy(kdArr->points[i]);
			map2[i] = rIndex;
			map1[i] = -1;
			rIndex++;
		}
	}

	matrixLeft = (int**)malloc(sizeof(int*)*kdArr->numOfDims);
	if (matrixLeft == NULL)
	{
		free(x);
		free(map1);
		free(map2);
		destroyPointsArr(leftPoints,middle);
		destroyPointsArr(rightPoints,((kdArr->numOfPoints) - middle));
		spLoggerPrintError("error while allocation of matrixLeft",__FILE__, __func__, __LINE__);
		return NULL;
	}
	matrixRight = (int**)malloc(sizeof(int*)*kdArr->numOfDims);
	if (matrixRight == NULL)
	{
		free(x);
		free(map1);
		free(map2);
		destroyPointsArr(leftPoints,middle);
		destroyPointsArr(rightPoints,((kdArr->numOfPoints) - middle));
		free(matrixLeft);
		spLoggerPrintError("error while allocation of matrixRight",__FILE__, __func__, __LINE__);
		return NULL;
	}

	for (i=0;i<kdArr->numOfDims;i++)
	{
		matrixLeft[i] = (int*) malloc(sizeof(int)*middle);
		if (matrixLeft[i] == NULL)
		{
			free(x);
			free(map1);
			free(map2);
			destroyPointsArr(leftPoints,middle);
			destroyPointsArr(rightPoints,((kdArr->numOfPoints) - middle));
			freeDoubleIntArr(matrixLeft,i);
			freeDoubleIntArr(matrixRight,i);
			free(matrixLeft);
			free(matrixRight);
			spLoggerPrintError("error while allocation of matrixLeft[i]",__FILE__, __func__, __LINE__);
			return NULL;
		}
		matrixRight[i] = (int*) malloc(sizeof(int)*(kdArr->numOfPoints - middle));
		if (matrixRight[i] == NULL)
		{
			free(x);
			free(map1);
			free(map2);
			destroyPointsArr(leftPoints,middle);
			destroyPointsArr(rightPoints,((kdArr->numOfPoints) - middle));
			freeDoubleIntArr(matrixLeft,i+1);
			freeDoubleIntArr(matrixRight,i);
			free(matrixLeft);
			free(matrixRight);
			spLoggerPrintError("error while allocation of matrixRight[i]",__FILE__, __func__, __LINE__);
			return NULL;
		}
		lIndex = 0;
		rIndex = 0;
		for (j=0;j<kdArr->numOfPoints;j++)
		{
			if (x[kdArr->matrix[i][j]] == 0)
			{
				matrixLeft[i][lIndex] = map1[kdArr->matrix[i][j]];
				lIndex++;
			}
			else
			{
				matrixRight[i][rIndex] = map2[kdArr->matrix[i][j]];
				rIndex++;
			}
		}
	}

	left = (SPKDArray) malloc(sizeof(*kdArr));
	if (left == NULL)
	{
		free(x);
		free(map1);
		free(map2);
		destroyPointsArr(leftPoints,middle);
		destroyPointsArr(rightPoints,((kdArr->numOfPoints) - middle));
		freeDoubleIntArr(matrixLeft,kdArr->numOfDims);
		freeDoubleIntArr(matrixRight,kdArr->numOfDims);
		free(matrixLeft);
		free(matrixRight);
		spLoggerPrintError("error while allocation of left",__FILE__, __func__, __LINE__);
		return NULL;
	}
	right = (SPKDArray) malloc(sizeof(*kdArr));
	if (right == NULL)
	{
		free(x);
		free(map1);
		free(map2);
		destroyPointsArr(leftPoints,middle);
		destroyPointsArr(rightPoints,((kdArr->numOfPoints) - middle));
		freeDoubleIntArr(matrixLeft,kdArr->numOfDims);
		freeDoubleIntArr(matrixRight,kdArr->numOfDims);
		free(matrixLeft);
		free(matrixRight);
		free(left);
		spLoggerPrintError("error while allocation of right",__FILE__, __func__, __LINE__);
		return NULL;
	}

	left->matrix=matrixLeft;
	left->points= leftPoints;
	left->numOfDims = kdArr->numOfDims;
	left->numOfPoints = middle;

	right->matrix=matrixRight;
	right->points=rightPoints;
	right->numOfPoints = kdArr->numOfPoints-middle;
	right->numOfDims = kdArr->numOfDims;

	leftAndRight = (SPKDArray*) malloc(sizeof(SPKDArray)*2);
	if (leftAndRight == NULL)
	{
		free(x);
		free(map1);
		free(map2);
		destroyPointsArr(leftPoints,middle);
		destroyPointsArr(rightPoints,((kdArr->numOfPoints) - middle));
		freeDoubleIntArr(matrixLeft,kdArr->numOfDims);
		freeDoubleIntArr(matrixRight,kdArr->numOfDims);
		free(matrixLeft);
		free(matrixRight);
		free(left);
		free(right);
		spLoggerPrintError("error while allocation of leftAndRight",__FILE__, __func__, __LINE__);
		return NULL;
	}

	leftAndRight[0] = left;
	leftAndRight[1] = right;

	free(x);
	free(map1);
	free(map2);

	return leftAndRight;

}
int SPKDArrayGetNumOfPoints(SPKDArray arr){
	if(arr==NULL){
		spLoggerPrintWarning("arr in null",__FILE__, __func__, __LINE__);
		return -1;
	}
	return arr->numOfPoints;
}
int SPKDArrayGetNumOfDims(SPKDArray arr){
	if(arr==NULL){
			spLoggerPrintWarning("arr in null",__FILE__, __func__, __LINE__);
			return -1;
		}
	return arr->numOfDims;
}

SPPoint* SPKDArrayGetpoints(SPKDArray arr){
	if(arr==NULL){
			spLoggerPrintWarning("arr in null",__FILE__, __func__, __LINE__);
			return NULL;
		}
	return arr->points;
}

int** SPKDArrayGetMatrix(SPKDArray arr){
	if(arr==NULL){
				spLoggerPrintWarning("arr in null",__FILE__, __func__, __LINE__);
				return NULL;
			}
	return arr->matrix;
}


SPPoint KDArrayGetTheMostRightPoint(SPKDArray arr, int coor){
	if(arr==NULL){
				spLoggerPrintWarning("arr in null",__FILE__, __func__, __LINE__);
				return NULL;
			}
	return arr->points[arr->matrix[coor][arr->numOfPoints -1]];
}


void SPKDArrayDestroy(SPKDArray arr){
	int i;
	if(arr==NULL){
		spLoggerPrintWarning("arr in null",__FILE__, __func__, __LINE__);
		return;
	}
	if(arr->points==NULL){
		spLoggerPrintWarning("points in null",__FILE__, __func__, __LINE__);
		free(arr);
		return;
	}
	for(i=0;i<arr->numOfPoints;i++){
		if(arr->points[i]==NULL){
				spLoggerPrintWarning("arr->points[i] in null",__FILE__, __func__, __LINE__);
			}
		else{
			spPointDestroy(arr->points[i]);
		}
	}
	free(arr->points);
	if(arr->matrix==NULL){
			spLoggerPrintWarning("matrix in null",__FILE__, __func__, __LINE__);
			free(arr);
			return;
		}
	for(i=0;i<arr->numOfDims;i++){
		if(arr->matrix[i]==NULL){
			spLoggerPrintWarning("arr->matrix[i] in null",__FILE__, __func__, __LINE__);
		}
		else{
			free(arr->matrix[i]);
		}
	}
	free(arr->matrix);
	free(arr);
}

void KDArrayUnitTest()
{
//	double allData[100][28];
//	SPPoint p[100];
//
	int i,j;
//
//	for (i=0;i<100;i++)
//	{
//		for (j=0;j<28;j++)
//		{
//			allData[i][j] = i*j;
//		}
//		p[i] = spPointCreate(allData[i],28,i);
//	}
//
//	for (i=0;i<100;i++)
//	{
//		printf("%f\n",allData[i][0]);
//	}
//
//	SPKDArray kdAr = init(p,100);

	double data1[2]={1,2};
	double data2[2]={123,70};
	double data3[2]={2,7};
	double data4[2]={9,11};
	double data5[2]={3,4};
	SPPoint p[5];
	p[0]= spPointCreate(data1,2,0);
	p[1]= spPointCreate(data2,2,1);
	p[2]= spPointCreate(data3,2,2);
	p[3]= spPointCreate(data4,2,3);
	p[4]= spPointCreate(data5,2,4);

	SPKDArray kdAr = init(p,5);



	for (i=0;i<2;i++)
	{
		printf("[");
		for (j=0;j<5;j++)
		{
			printf("%d,",kdAr->matrix[i][j]);
		}
		printf("]\n");
	}


	printf("\n\n");
	SPKDArray* kdArs = split(kdAr,0);

	printf("%f\n", spPointGetAxisCoor(KDArrayGetTheMostRightPoint(kdArs[0],1),0));

	for (i=0;i<2;i++)
	{
		printf("[");
		for (j=0;j<3;j++)
		{
			printf("%d,",kdArs[0]->matrix[i][j]);
		}
		printf("]\n");
	}

	printf("\n\n");

	for (i=0;i<2;i++)
	{
		printf("[");
		for (j=0;j<2;j++)
		{
			printf("%d,",kdArs[1]->matrix[i][j]);
		}
		printf("]\n");
	}

}
