#include "SPFeatures.h"
#include "SPPoint.h"
//#include "SPImageProc.h"
#include <stdlib.h>
#include <stdio.h>
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
	FILE * file = fopen(filePath,"w+");

	for (i=0;i<numberOfFeats;i++)
	{
		data = spPointGetData(features[i]);
		dim = spPointGetDimension(features[i]);
		fwrite(&dim,sizeof(int),1,file);
		fwrite(data,sizeof(double),dim,file);
	}
	fflush(file);
	fclose(file);
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

	FILE * file = fopen(filePath,"r");

	while (fread(&dim,4,1,file) > 0)
	{
		data = calloc(dim,sizeof(double));
		fread(data,sizeof(double),dim,file);
		feats = realloc(feats,sizeof(*feats)*(i+1));
		feats[i] = spPointCreate(data,dim,i);
		i++;
	}

	*numberOfFeats = i;
	return feats;
}

//void main ()
//{
//	double dt[2]={1.3,4.6};
//	double dt3[3]={1.3,4.6,5.7};
//	SPPoint p[2];
//	p[0] = spPointCreate(dt,2,0);
//	p[1] = spPointCreate(dt3,3,1);


//	saveFeaturesToFile("images\\","img",4,p,2);

//	int num;

//	SPPoint * points = getFeaturesFromFile("images\\","img",4,&num);

//	printf("\ngot %d",num);
//	printf("\npoint %d",spPointGetDimension(points[1]));
//	printf("\npoint data %f",spPointGetAxisCoor(points[1],2));
//}
