#include "SPFeatures.h"
#include "SPPoint.h"
//#include "SPImageProc.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

void main ()
{
	double dt[2]={1.3,4.6};
	SPPoint point = spPointCreate(dt,2,2);

	FILE * file = fopen("feat.txt","w+");
	int dim = spPointGetDimension(point);
	fwrite(&dim,sizeof(int),1,file);

	fseek(file,0,SEEK_SET);

	int dimBack[1];
	fread(dimBack,4,1,file);

	printf("back = %d",*dimBack);
}
