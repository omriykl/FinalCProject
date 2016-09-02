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
	kdArr->points = (SPPoint*) malloc(sizeof(SPPoint)*size);
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

	for (i=0;i<kdArr->numOfPoints;i++) //TODO: remove after testing
		printf("%d = %d\n",i,x[i]); //TODO: remove after testing

	//allocation of leftPoints and rightPoints
	leftPoints = (SPPoint*) malloc(sizeof(SPPoint)*middle);
	if (leftPoints == NULL)
	{
		free(x);
		free(map1);
		free(map2);
		return NULL; //TODO: log error
	}
	rightPoints = (SPPoint*) malloc(sizeof(SPPoint)*((kdArr->numOfPoints) - middle));
	if (leftPoints == NULL)
	{
		free(x);
		free(map1);
		free(map2);
		free(leftPoints);
		return NULL; //TODO: log error
	}

	//start filling leftPoints and rightPoints
	lIndex =0;
	rIndex = 0;
	for (i=0;i<kdArr->numOfPoints;i++)
	{
		if (x[i] == 0)
		{
			leftPoints[lIndex] = (SPPoint) malloc(sizeof(SPPoint)); //TODO: check alloc
			leftPoints[lIndex] = spPointCopy(kdArr->points[i]);
			map1[i] = lIndex;
			map2[i] = -1;
			lIndex++;
		}
		else
		{
			rightPoints[rIndex] = (SPPoint) malloc(sizeof(SPPoint)); //TODO: check alloc
			rightPoints[rIndex] = spPointCopy(kdArr->points[i]);
			map2[i] = rIndex;
			map1[i] = -1;
			rIndex++;
		}
	}

//	printf("\nThe left group: \n");
//	for (i=0;i<middle;i++)
//	{
//		printf("%f\n", spPointGetAxisCoor(leftPoints[i],coor));
//	}
//
//	printf("\nThe right group: \n");
//	for (i=0;i<(kdArr->numOfPoints - middle);i++)
//	{
//		printf("%f\n", spPointGetAxisCoor(rightPoints[i],coor));
//	}
//
//	printf("\nMap1:\n[");
//	for (i=0;i<kdArr->numOfPoints;i++)
//	{
//		printf("%d,",map1[i]);
//	}
//
//	printf("]\nMap2:\n[");
//	for (i=0;i<kdArr->numOfPoints;i++)
//	{
//		printf("%d,",map2[i]);
//	}

	matrixLeft = (int**)malloc(sizeof(int*)*kdArr->numOfDims);
	if (matrixLeft == NULL)
	{
		free(x);
		free(map1);
		free(map2);
		free(leftPoints);
		//TODO: free other stuff (points)
		return NULL; //TODO: log error
	}
	matrixRight = (int**)malloc(sizeof(int*)*kdArr->numOfDims);
	if (matrixLeft == NULL)
	{
		free(x);
		free(map1);
		free(map2);
		free(leftPoints);
		free(matrixLeft);
		//TODO: free other stuff (points)
		return NULL; //TODO: log error
	}

	for (i=0;i<kdArr->numOfDims;i++)
	{
		matrixLeft[i] = (int*) malloc(sizeof(int)*middle);
		if (matrixLeft[i] == NULL)
		{
			//TODO: free each matrixLeft[k], matrixRight[k] | k < i
			free(x);
			free(map1);
			free(map2);
			free(leftPoints);
			free(matrixLeft);
			free(matrixRight);
			//TODO: free other stuff (points)
			return NULL; //TODO: log error
		}
		matrixRight[i] = (int*) malloc(sizeof(int)*(kdArr->numOfPoints - middle));
		if (matrixRight[i] == NULL)
		{
			//TODO: free each matrixLeft[k] | k <= i, matrixRight[k] | k < i
			free(x);
			free(map1);
			free(map2);
			free(leftPoints);
			free(matrixLeft);
			free(matrixRight);
			//TODO: free other stuff (points)
			return NULL; //TODO: log error
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
//	printf("\n\n");
//	for (i=0;i<2;i++)
//	{
//		printf("[");
//		for (j=0;j<middle;j++)
//		{
//			printf("%d , ",matrixLeft[i][j]);
//		}
//		printf("]\n");
//	}
//
//	printf("\n\n");
//	for (i=0;i<2;i++)
//	{
//		printf("[");
//		for (j=0;j<kdArr->numOfPoints - middle;j++)
//		{
//			printf("%d , ",matrixRight[i][j]);
//		}
//		printf("]\n");
//	}

	left = (SPKDArray) malloc(sizeof(*kdArr));
	if (left == NULL)
	{
		free(x);
		free(map1);
		free(map2);
		free(leftPoints);
		free(matrixLeft);
		free(matrixRight);
		//TODO: free other stuff (points)
		return NULL; //TODO: log error
	}
	right = (SPKDArray) malloc(sizeof(*kdArr));
	if (right == NULL)
	{
		free(x);
		free(map1);
		free(map2);
		free(leftPoints);
		free(matrixLeft);
		free(matrixRight);
		free(left);
		//TODO: free other stuff (points)
		return NULL; //TODO: log error
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
		free(leftPoints);
		free(matrixLeft);
		free(matrixRight);
		free(left);
		free(right);
		//TODO: free other stuff (points)
		return NULL; //TODO: log error
	}

	leftAndRight[0] = left;
	leftAndRight[1] = right;

	free(x);
	free(map1);
	free(map2);

	return leftAndRight;

}
int SPKDArrayGetNumOfPoints(SPKDArray arr){
	return arr->numOfPoints;
}
int SPKDArrayGetNumOfDims(SPKDArray arr){
	return arr->numOfDims;
}

SPPoint* SPKDArrayGetpoints(SPKDArray arr){
	return arr->points;
}

int** SPKDArrayGetMatrix(SPKDArray arr){
	return arr->matrix;
}


void SPKDArrayDestroy(SPKDArray arr){
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

//void main()
//{
//	double data1[2]={1,2};
//	double data2[2]={123,70};
//	double data3[2]={2,7};
//	double data4[2]={9,11};
//	double data5[2]={3,4};
//	SPPoint p[5];
//	p[0]= spPointCreate(data1,2,0);
//	p[1]= spPointCreate(data2,2,1);
//	p[2]= spPointCreate(data3,2,2);
//	p[3]= spPointCreate(data4,2,3);
//	p[4]= spPointCreate(data5,2,4);
//
//	SPKDArray kdAr = init(p,5);
//
//	int i,j;
//
//	for (i=0;i<2;i++)
//	{
//		printf("[");
//		for (j=0;j<5;j++)
//		{
//			printf("%d , ",kdAr->matrix[i][j]);
//		}
//		printf("]\n");
//	}
//
//	printf("\n\n");
//	SPKDArray* kdArs = split(kdAr,0);
//
//	for (i=0;i<2;i++)
//	{
//		printf("[");
//		for (j=0;j<3;j++)
//		{
//			printf("%d , ",kdArs[0]->matrix[i][j]);
//		}
//		printf("]\n");
//	}
//
//	for (i=0;i<2;i++)
//	{
//		printf("[");
//		for (j=0;j<2;j++)
//		{
//			printf("%d , ",kdArs[1]->matrix[i][j]);
//		}
//		printf("]\n");
//	}
//
//}
