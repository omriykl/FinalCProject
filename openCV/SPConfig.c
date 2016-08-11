#include "SPConfig.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

struct sp_config_t
{
	char * spImagesDirectory; // a parameter which contains the path of the images’ directory. This directory will contain all images in our database, such that given a query image, we will look for similar images in the directory spImagesDirectory. Constraint: the string contains no spaces.
	char * spImagesPrefix; // the images prefix. As we saw in assignment 2, all images in spImagesDirectory will start with this name. Constraint: the string contains no spaces.
	char * spImagesSuffix; // the images file format. This string represents the images format in spImagesDirecoty (all images in spImagesDirectory has the same file extension). Constraint: the string value is in the following set { .jpg , .png , .bmp , .gif }
	int spNumOfImages; // the number of images in spImagesDirectory. Constraint: positive integer.
	int spPCADimension; //we will use this value to reduce the dimension of the SIFT features from 128 to spPCADimension. Constraint: positive integer in the range [10 , 28].
	char * spPCAFilename; //the filename of the PCA file. Constraint: the string contains no spaces.
	char * spNumOfFeatures; //the number of features which will be extracted per image. Constraint: positive integer.
	bool spExtractionMode; //a boolean variable which indicates if preprocessing must be done first or if the features are extracted from files (more on this later). Constraint: the value is in the following set { true , false}
	int spNumOfSimilarImages; //a positive integer which indicates the number of similar images which will be presented given a query image. That is if this parameter is set to 2, the most 2 similar images with respect to a query image will be presented. Constraint: spNumOfSimilarImages>0
	SP_SPLIT_METHOD spKDTreeSplitMethod; //a parameter which represents the cut method when the kd-tree is build (more later). Constraint: an enum which takes one of the following values {RANDOM, MAX_SPREAD, INCREMENTAL }
	int spKNN; //a positive integer which is used in the k nearest neighbor search algorithm (more on this later). Constraint: spKNN>0
	bool spMinimalGUI; //a boolean variable which indicates if minimal GUI is supported (more on this later). Constraint: the value is in the following set { true , false}
	int spLoggerLevel; //an integer which indicates the active level of the logger. The value 1 indicates error level, the value 2 indicates warning level, the value 3 indicates info level and 4 indicates debug level. Constraint: the value is in the following set { 1 , 2 , 3 , 4}
	char * spLoggerFilename; //the log file name. If the value is stdout , then the standard output will be used for printing. Otherwise, a log will be created with the name given by the parameter spLoggerFilename. Constraint: the string contains no spaces.
};


void SetConfigValue(char * var, char * val){
	if(strcmp(var,"spImagesDirectory")==0){
		printf("%s = %s",var,val);
	}
	//TODO ALL THE REST
}

SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg)
{
	int i;
	FILE* configFile = fopen(filename,"r");
	char line[1024];
	char var[1024],value[1024];
	int varOrValue=0; //0=nothig yet, 1= var, 2=value;
    int varIndex,valIndex;

	while (fgets(line,1024,configFile) != NULL)
	{
		varIndex=0;valIndex=0;
		if(line[0]!='#')
		{
			for(i=0;i<2014;i++)
			{
				if(line[i]!=' ')
				{
					if(varOrValue==0)
					{
						varOrValue=1;

						var[varIndex]=line[i];
						varIndex++;
					}
					else if(varOrValue==1)
					{
						if(line[i]=='=')
						{
							varOrValue=2;
							var[varIndex]='\0';
						}
						else if(line[i]==' ')
						{
							while(line[i+1]==' ')
								i++;
							if(line[i+1]!='=')
								*msg= SP_CONFIG_INVALID_ARGUMENT;

						}
						else
						{
							var[varIndex]=line[i];
							varIndex++;
						}

					}
					else if(varOrValue==2){
						if(line[i]=='\n')
										{
										value[valIndex]='\0';
										break;
										}
										else if(line[i]==' ')
										{
											while(line[i+1]==' ')
												i++;
											if(line[i+1]!='\n')
												*msg= SP_CONFIG_INVALID_ARGUMENT;
											else{
												value[valIndex]='\0';
												break;
											}
										}
										else
										{
											value[valIndex]=line[i];
											valIndex++;
										}
					}


				}
			}

			SetConfigValue(var,value);
		}
		//printf("%s",line);
	}
}


void main()
{
	SP_CONFIG_MSG msg;
	spConfigCreate("conf.config",&msg);
}
