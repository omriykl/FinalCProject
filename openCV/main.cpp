/*
 * main.cpp
 *
 *  Created on: 27 ·‡Â‚ 2016
 *      Author: Yom Tov
 */

#include <cstdio>
#include <cstdlib> //include c library
#include "SPImageProc.h"
extern "C"
{ //include your own C source files
#include "SPConfig.h"
#include "KDTreeBuiler.h"
#include "SPKDArrays.h"
#include "SPLogger.h"
#include "SPFeatures.h"
#include "SPListElement.h"
#include "SPBPriorityQueue.h"
#include <string.h>
}

#define PROGRAMNAME "SPCBIR"

using namespace sp;

int cmpFuncSPListElementByVals (const void * a, const void * b)
{
	 SPListElement * a1 = ( SPListElement *) a;
	 SPListElement * b1 = ( SPListElement *) b;

	 //sort from big to small then by lowest dim
	if (spListElementGetValue(*a1) > spListElementGetValue(*b1)){
		return -1;
	}
	else if(spListElementGetValue(*a1) == spListElementGetValue(*b1)){
		if(spListElementGetIndex(*a1) < spListElementGetIndex(*b1)){
			return -1;
		}
	}
	return 1;
}

void displayResults(SPConfig config,char *queryImg, char ** images,ImageProc pr)
{
	int i;
	SP_CONFIG_MSG msg;

	//TODO error handling
	if (spConfigMinimalGui(config,&msg))
	{
		for (i=0;i<spConfigGetspNumOfSimilarImages(config);i++)
		{
			pr.showImage(images[i]);
		}
	}
	else
	{
		printf("Best candidates for - %s - are:\n",queryImg);
		for (i=0;i<spConfigGetspNumOfSimilarImages(config);i++)
		{
			printf("%s\n",images[i]);
			fflush(NULL);
		}
	}

}


int main(int args_num, char** args)
{

	FILE* configFile;
	SP_CONFIG_MSG msg;
	SP_BPQUEUE_MSG bpqMsg;
	SPConfig config;
	int i,j,featIndex;
	int numOfImages;
	int numOfFeats;
	int featsFound;
	char* imagePath;
	int totalFeatsCounts=0;
	SPPoint ** allImagesFeatsByImg;
	SPPoint * allImagesFeats;
	SPKDArray arr;
	KDTreeNode tree;
	SP_LOGGER_MSG logger;
	SPPoint* quaryFeats;
	SPListElement* imagesFeatsMatchCount;
	SPListElement topOfTheBPQ;
	SPListElement tempElement;
	SPBPQueue bpq;
	double val;
	char ** imagesFound;

	if (args_num < 2)
	{
		printf("%s", "Invalid command line : use -c <config_filename>\n");
		return 0;
	}

	if (strcmp(args[1], "-c") != 0)
	{
		printf("%s", "Invalid command line : use -c <config_filename>\n");
		return 0;
	}
	if (args_num < 3)
	{
		configFile = fopen("spcbir.config", "r");
		if (configFile == NULL)
		{
			printf("%s",
					"The default configuration file spcbir.config couldnít be open\n");
			return 0;

		}
	}
	else
	{
		configFile = fopen(args[2], "r");
		if (configFile == NULL)
		{
			printf("%s %s %s", "The configuration file ", args[2],
					" couldnít be open\n");
			return 0;
		}
	}

	config = spConfigCreate(args[2], &msg);
	if (config == NULL)
	{
		printf("error while reading config file");

	}
	else if(msg!=SP_CONFIG_SUCCESS){

	}
	else{
		spConfigPrint(config);
		spLoggerPrintInfo("After Config Creation");


		//printf("%s", "After config:\n");
		//fflush(NULL);

		logger = spLoggerCreate(spConfigGetspLoggerFilename(config),spConfigGetspLoggerLevel(config));
		if(logger!=SP_LOGGER_SUCCESS){
			spLoggerPrintError("error reading spLoggerCreate",__FILE__, __func__, __LINE__);
		}

		ImageProc pr(config);

		//printf("%s", "After ImageProc :\n");
		spLoggerPrintInfo("After ImageProc Creation");

		//fflush(NULL);

		numOfImages = spConfigGetNumOfImages(config, &msg);
		if(msg!=SP_CONFIG_SUCCESS){
			spLoggerPrintError("error reading spConfigGetNumOfImages",__FILE__, __func__, __LINE__);
		}
		numOfFeats = spConfigGetNumOfFeatures(config, &msg);
		if(msg!=SP_CONFIG_SUCCESS){
			spLoggerPrintError("error reading spConfigGetNumOfFeatures",__FILE__, __func__, __LINE__);
				}

		imagesFeatsMatchCount= (SPListElement*)malloc(sizeof(SPListElement)* numOfImages);
		if(imagesFeatsMatchCount==NULL){
			spLoggerPrintError("error allocation imagesFeatsMatchCount",__FILE__, __func__, __LINE__);
		}
		allImagesFeatsByImg = (SPPoint **) malloc(sizeof(SPPoint *) * numOfImages);
		if(allImagesFeatsByImg==NULL){
					spLoggerPrintError("error allocation allImagesFeatsByImg",__FILE__, __func__, __LINE__);
				}
		allImagesFeats = (SPPoint *) malloc(sizeof(SPPoint) * numOfImages * numOfFeats);
		if(allImagesFeats==NULL){
					spLoggerPrintError("error allocation allImagesFeats",__FILE__, __func__, __LINE__);
				}
		featIndex=0;
		imagePath = (char*) malloc(sizeof(char) * 1025);
		if(imagePath==NULL){
					spLoggerPrintError("error allocation imagePath",__FILE__, __func__, __LINE__);
				}

		//printf("%s", "After imagePath:\n");
		spLoggerPrintInfo("After imagePath Creation");

		//fflush(NULL);
		if (spConfigIsExtractionMode(config, &msg) == true)
		{

			for (i = 0; i < numOfImages; i++)
			{
				msg = spConfigGetImagePath(imagePath, config, i);

				if(msg!=SP_CONFIG_SUCCESS){
						spLoggerPrintError("error reading spConfigGetImagePath",__FILE__, __func__, __LINE__);
							}


				allImagesFeatsByImg[i] = pr.getImageFeatures(imagePath, i,
						&featsFound);

				totalFeatsCounts+=featsFound;


				if(allImagesFeatsByImg[i]==NULL){
								spLoggerPrintError("error with allImagesFeatsByImg[i]",__FILE__, __func__, __LINE__);
							}

				saveFeaturesToFile(spConfigGetImagesDirectory(config),
						spConfigGetImagesPrefix(config), i, allImagesFeatsByImg[i],
						featsFound);

				if(totalFeatsCounts>(numOfImages * numOfFeats)){
					allImagesFeats = (SPPoint *) realloc(allImagesFeats,(sizeof(SPPoint) * numOfImages * numOfFeats)+featsFound);
				}

				for(j=0;j<featsFound;j++){
					allImagesFeats[featIndex]=allImagesFeatsByImg[i][j];
					featIndex++;
				}

				imagesFeatsMatchCount[i]= spListElementCreate(i,0);

				if(imagesFeatsMatchCount[i]==NULL){
								spLoggerPrintError("error allocation imagesFeatsMatchCount[i]",__FILE__, __func__, __LINE__);
							}
			}
		}
		else
		{
			if(msg!=SP_CONFIG_SUCCESS){
					spLoggerPrintError("error reading spConfigIsExtractionMode",__FILE__, __func__, __LINE__);
						}

			for (i = 0; i < numOfImages; i++)
			{
				msg = spConfigGetImagePath(imagePath, config, i);

				if(msg!=SP_CONFIG_SUCCESS){
						spLoggerPrintError("error reading spConfigGetImagePath",__FILE__, __func__, __LINE__);
							}

				allImagesFeatsByImg[i] = getFeaturesFromFile(
						spConfigGetImagesDirectory(config),
						spConfigGetImagesPrefix(config), i, &featsFound);

				totalFeatsCounts+=featsFound;

				if(allImagesFeatsByImg[i]==NULL){
										spLoggerPrintError("error with allImagesFeatsByImg[i]",__FILE__, __func__, __LINE__);
									}

				if(totalFeatsCounts>(numOfImages * numOfFeats)){
									allImagesFeats = (SPPoint *) realloc(allImagesFeats,(sizeof(SPPoint) * numOfImages * numOfFeats)+featsFound);
								}

				for(j=0;j<featsFound;j++){
									allImagesFeats[featIndex]=allImagesFeatsByImg[i][j];
									featIndex++;
								}


				imagesFeatsMatchCount[i]= spListElementCreate(i,0);

				if(imagesFeatsMatchCount[i]==NULL){
									spLoggerPrintError("error allocation imagesFeatsMatchCount[i]",__FILE__, __func__, __LINE__);
								}
			}
		}


		arr = init(allImagesFeats,featIndex);
		spLoggerPrintInfo("After arr Creation");

		//printf("%s", "After arr:\n");
			//	fflush(NULL);

		tree= CreateTreeFromArray(arr,config);

		spLoggerPrintInfo("After tree Creation");

		//printf("%s", "After tree:\n");
			//	fflush(NULL);

		printf("%s", "Please enter an image path:\n");
		fflush(NULL);
		scanf("%s", imagePath);
		fflush(NULL);

		while (strcmp(imagePath, "<>") != 0)
		{
			featsFound=0;
			quaryFeats = pr.getImageFeatures(imagePath, i,&featsFound);

			spLoggerPrintInfo("After quaryFeats Creation");

			printf("After quaryFeats : featfound=%d:\n",featsFound);
							fflush(NULL);


			for(i=0;i<featsFound;i++){

				 bpq = FindkNearestNeighbors(tree,quaryFeats[i],config);

				 //printf("bpq size is %d\n",spBPQueueSize(bpq));

				 for(j=0;j<spBPQueueSize(bpq);j++)
				 {
					 tempElement=spBPQueuePeek(bpq);
					 topOfTheBPQ=imagesFeatsMatchCount[spListElementGetIndex(tempElement)];

					 val = spListElementGetValue(topOfTheBPQ);
					 val = val + 1.0;
					 spListElementSetValue(topOfTheBPQ,val);

					 //printf("add 1 to value on index %d\n",spListElementGetIndex(topOfTheBPQ));
					 spListElementDestroy(tempElement);

					 bpqMsg =spBPQueueDequeue(bpq);
					 if(bpqMsg!=SP_BPQUEUE_SUCCESS){
					 					spLoggerPrintError("error with spBPQueueDequeue",__FILE__, __func__, __LINE__);
					 						}
				 }
				 //printf("bpq size is %d\n",spBPQueueSize(bpq));
				 spBPQueueDestroy(bpq);
			}

			spLoggerPrintInfo("Beofore qsort ");

			for(i=0;i<numOfImages;i++){
				printf("before sort image %d value=%f\n",spListElementGetIndex(imagesFeatsMatchCount[i]),spListElementGetValue(imagesFeatsMatchCount[i]));
			}

			qsort(imagesFeatsMatchCount,numOfImages,sizeof(SPListElement),cmpFuncSPListElementByVals);

			for(i=0;i<numOfImages;i++){
						printf("after sort image %d value=%f\n",spListElementGetIndex(imagesFeatsMatchCount[i]),spListElementGetValue(imagesFeatsMatchCount[i]));
					}

			spLoggerPrintInfo("After qsort ");

			printf("%s", "After qsort:\n");
			fflush(NULL);

			imagesFound = (char**) malloc(sizeof(char*)*spConfigGetspNumOfSimilarImages(config));

			for (i=0;i<spConfigGetspNumOfSimilarImages(config);i++)
			{
				imagesFound[i] = (char*) malloc(sizeof(char)*1025);
				spConfigGetImagePath(imagesFound[i],config,spListElementGetIndex(imagesFeatsMatchCount[i]));
			}

			displayResults(config,imagePath,imagesFound,pr);

			//TODO: free imagesFound

			for(i=0;i<featsFound;i++){
				spPointDestroy(quaryFeats[i]);
			}
			for(i=0;i<numOfImages;i++){
				spListElementSetValue(imagesFeatsMatchCount[i],0.0);
				spListElementSetIndex(imagesFeatsMatchCount[i],i);
			}
			for (i=0;i<spConfigGetspNumOfSimilarImages(config);i++)
			{
			  free(imagesFound[i]);
			}
			free(imagesFound);
			free(quaryFeats);


			printf("%s", "Please enter an image path:\n");
			fflush(NULL);
			scanf("%s", imagePath);
			fflush(NULL);

		}
	}

	printf("%s", "ExitingÖ\n");
	fflush(NULL);


	//free all
	for(i=0;i<numOfImages;i++){
		spListElementDestroy(imagesFeatsMatchCount[i]);
		for(j=0;j<numOfFeats;j++){
			spPointDestroy(allImagesFeatsByImg[i][j]);
		}
	}
	for(i=0;i<featIndex;i++){
		spPointDestroy(allImagesFeats[i]);
	}

	free(imagesFound);
	free(imagesFeatsMatchCount);
	free(allImagesFeats);
	free(allImagesFeatsByImg);

	SPKDArrayDestroy(arr);
	KDTreeDestroy(tree);
	free(configFile);
	free(imagePath);
	spConfigDestroy(config);
	spLoggerDestroy();
	return 0;
}

