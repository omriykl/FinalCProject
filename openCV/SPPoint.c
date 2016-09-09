#include "SPPoint.h"
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

struct sp_point_t {
	int index;
	int dim;
	double *data;
};

SPPoint spPointCreate(double* data, int dim, int index)
{
	SPPoint point;
	if(dim<=0 || index<0 || data==NULL){
		spLoggerPrintWarning("dim<=0 || index<0 || data==NULL","SPPoint","spPointCreate",__LINE__);
		return NULL;
	}
	point = (SPPoint) malloc(sizeof(*point));
	if (point == NULL){
		spLoggerPrintError("error while allocation of point","SPPoint","spPointCreate",__LINE__);
		return NULL;
	}


	point->data = (double*) calloc(dim,sizeof(double));
	if (point->data == NULL){
		spLoggerPrintError("error while allocation of data","SPPoint","spPointCreate",__LINE__);
		free(point);
		return NULL;
	}

	for (int i = 0; i< dim;i++)
		point->data[i] = data[i];
	point->index = index;
	point->dim = dim;

	return point;
}

SPPoint spPointCopy(SPPoint source)
{
	assert(source != NULL);

	SPPoint pointCp = (SPPoint) malloc(sizeof(*pointCp));
	if (pointCp == NULL){
		spLoggerPrintError("error while allocation of pointCp","SPPoint","spPointCopy",__LINE__);
        return NULL;
	}


	pointCp->data = (double*) calloc(source->dim,sizeof(double));
	if (pointCp->data == NULL){
		spLoggerPrintError("error while allocation of data","SPPoint","spPointCopy",__LINE__);
		spPointDestroy(pointCp);
		return NULL;
	}

	for (int i = 0; i< source->dim;i++)
		pointCp->data[i] = source->data[i];
	pointCp->index = source->index;
	pointCp->dim = source->dim;

	return pointCp;
}

void spPointDestroy(SPPoint point)
{
	if (point != NULL){
		free(point->data);
		free(point);
	}
}

int spPointGetDimension(SPPoint point)
{
	assert(point != NULL);
	return point->dim;
}

int spPointGetIndex(SPPoint point)
{
	assert(point != NULL);
	return point->index;
}

double spPointGetAxisCoor(SPPoint point, int axis)
{
	assert(point!=NULL && axis < point->dim);
	return point->data[axis];
}

double* spPointGetData(SPPoint point)
{
	assert(point!=NULL);
	double *data = (double*) calloc(point->dim,sizeof(double));
	if (data == NULL)
	{
		return NULL;
	}
	for (int i = 0; i< point->dim;i++)
		data[i] = point->data[i];
	return data;
}

double spPointL2SquaredDistance(SPPoint p, SPPoint q)
{
	assert(p!=NULL && q!=NULL && (p->dim == q->dim));

	double dist=0;
	int i;

	for(i=0;i<p->dim;i++)
	{
		dist+= ( (p->data[i]-q->data[i])*(p->data[i]-q->data[i]) );
	}
	return dist;
}

