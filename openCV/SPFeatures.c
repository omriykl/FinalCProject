#include "SPFeatures.h"
#include "SPPoint.h"
//#include "SPImageProc.h"
#include <stdlib.h>
#include <stdio.h>
#include "SPLogger.h"
#include <stdbool.h>
#include <assert.h>

void saveFeaturesToFile(char * imgDir,char * imgPre,int index,SPPoint * features,int numberOfFeats)
{
	//TODO: error handling
	char filePath[1024];
	int dim;
	double * data;
	int i;

	sprintf(filePath,"%s%s%d.feats",imgDir,imgPre,index);
	FILE * file = fopen(filePath,"wb+");

	if(file==NULL){
		spLoggerPrintError("error while reading file ","SPFeatures","saveFeaturesToFile",__LINE__);
		return;
	}

	//save the number of feats;
	fwrite(&numberOfFeats,sizeof(int),1,file);

	for (i=0;i<numberOfFeats;i++)
	{
		data = spPointGetData(features[i]);
		dim = spPointGetDimension(features[i]);
		fwrite(&dim,sizeof(int),1,file);
		fwrite(data,sizeof(double),dim,file);
		fflush(NULL);
	}
	if(fflush(file)==EOF){
		spLoggerPrintError("error while fflush file ","SPFeatures","saveFeaturesToFile",__LINE__);
				return;
	}
	if(fclose(file)==EOF){
			spLoggerPrintError("error while close file ","SPFeatures","saveFeaturesToFile",__LINE__);
					return;
		}

}

SPPoint * getFeaturesFromFile(char * imgDir,char * imgPre,int index, int * numberOfFeats)
{
	//TODO: error handling
	char filePath[1024];
	int dim;
	double * data;
	int i=0;
	SPPoint * feats;

	sprintf(filePath,"%s%s%d.feats",imgDir,imgPre,index);

	FILE * file = fopen(filePath,"rb");
	if(file==NULL){
			spLoggerPrintError("error while reading file ","SPFeatures","getFeaturesFromFile",__LINE__);
			return NULL;
		}

	//reads number of features
	fread(numberOfFeats,sizeof(int),1,file);

	feats = (SPPoint*) malloc(sizeof(SPPoint)*(*numberOfFeats));

	while (i < *numberOfFeats)
	{
		dim = 0;
		fread(&dim,4,1,file);
		data =(double*) malloc(sizeof(double)*dim);
		fread(data,sizeof(double),dim,file);
		feats[i] = spPointCreate(data,dim,index);
		i++;
	}

	return feats;
}

void main2()
{
//	double data1[100][28];
//	int i,j;
//
//	SPPoint *points = (SPPoint*) malloc(sizeof(SPPoint)*100);
//
//	for (i=0;i<100;i++)
//	{
//		for (j=0;j<28;j++)
//		{
//			data1[i][j] = i*j;
//		}
//		points[i] = spPointCreate(data1[i],28,0);
//	}
//
//	saveFeaturesToFile("images\\","img",400,points,100);
//
//	int num;
//
//	SPPoint * points2 = getFeaturesFromFile("images2\\","img",0,&num);
//
//	printf("\ngot %d",num);
//	printf("\npoint %d",spPointGetDimension(points2[0]));
//	printf("\npoint data %f",spPointGetAxisCoor(points2[5],5));

//	int num;
//	int dim;
//	double * data;
//
//	FILE * file = fopen("images2/img0.feats","r");
//
//	//reads number of features
//	fread(&num,sizeof(int),1,file);
//	fread(&dim,sizeof(int),1,file);
//
//	fseek(file,7,SEEK_SET);
//
//	data = (double*) malloc(sizeof(double)*200);
//
//	int d = fread(data,sizeof(double),200,file);
//
//	fflush(NULL);
//	printf("%d %d %f read total = %d\n",num,dim,data[0],d);
//
////	fseek(file,4,SEEK_SET);
//
//	dim=20;
//
//	fread(&dim,sizeof(int),1,file);
//
//	data = (double*) malloc(sizeof(double)*dim);
//
//	int read = fread(data,sizeof(double),dim,file);
//
//	printf("%d %d %f %d",num,dim,data[0],read);
//
//	fclose(file);

}
