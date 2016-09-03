/*
 * main.cpp
 *
 *  Created on: 27 ·‡Â‚ 2016
 *      Author: Yom Tov
 */

#include <cstdio>
#include <cstdlib> //include c library#include "SPImageProc.h"
extern "C"
{ //include your own C source files
#include "SPConfig.h"
#include "KDTreeBuiler.h"
#include "SPKDArrays.h"
#include "SPLogger.h"
#include "SPFeatures.h"
#include <string.h>
}

#define PROGRAMNAME "SPCBIR"

using namespace sp;

int main(int args_num, char** args)
{

	FILE* configFile;
	SP_CONFIG_MSG msg;
	SPConfig config;
	int i,j,featIndex;
	int numOfImages;
	int numOfFeats;
	int featsFound;
	char* imagePath;
	SPPoint ** allImagesFeatsByImg;
	SPPoint * allImagesFeats;
	SPKDArray arr;
	KDTreeNode tree;
	SPPoint* quaryFeats;
	SPListElement* imagesFeatsMatchCount;
	SPBPQueue bpq;
	double val;

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
		return 0;
	}
	else
	{
		ImageProc pr = ImageProc(config);
		numOfImages = spConfigGetNumOfImages(config, &msg);
		numOfFeats = spConfigGetNumOfFeatures(config, &msg);

		imagesFeatsMatchCount= (SPListElement*)malloc(sizeof(SPListElement)* numOfImages);
		allImagesFeatsByImg = (SPPoint **) malloc(sizeof(SPPoint *) * numOfImages);
		allImagesFeats = (SPPoint *) malloc(sizeof(SPPoint) * numOfImages * numOfFeats);
		featIndex=0;
		imagePath = (char*) malloc(sizeof(char) * 1025);

		if (spConfigIsExtractionMode(config, &msg) == true)
		{
			for (i = 0; i < numOfImages; i++)
			{
				msg = spConfigGetImagePath(imagePath, config, i);
				printf("%s\n", imagePath);
				allImagesFeatsByImg[i] = pr.getImageFeatures(imagePath, i,
						&featsFound);
				saveFeaturesToFile(spConfigGetImagesDirectory(config),
						spConfigGetImagesPrefix(config), i, allImagesFeatsByImg[i],
						featsFound);

				for(j=0;j<featsFound;j++){
					allImagesFeats[featIndex]=allImagesFeatsByImg[i][j];
					featIndex++;
				}

				imagesFeatsMatchCount[i]= spListElementCreate(i,0);

			}
		}
		else
		{
			for (i = 0; i < numOfImages; i++)
			{
				msg = spConfigGetImagePath(imagePath, config, i);
				printf("%s\n", imagePath);
				allImagesFeatsByImg[i] = getFeaturesFromFile(
						spConfigGetImagesDirectory(config),
						spConfigGetImagesPrefix(config), i, &featsFound);


				for(j=0;j<featsFound;j++){
									allImagesFeats[featIndex]=allImagesFeatsByImg[i][j];
									featIndex++;
								}


				imagesFeatsMatchCount[i]= spListElementCreate(i,0);
			}
		}


		arr = init(allImagesFeats,featIndex);
		tree= CreateTreeFromArray(arr,config);


		printf("%s", "Please enter an image path:\n");
		scanf("%s", imagePath);
		while (strcmp(imagePath, "<>") != 0)
		{

			quaryFeats = pr.getImageFeatures(imagePath, i,&featsFound);

			for(i=0;i<featsFound;i++){

				 bpq = FindkNearestNeighbors(tree,quaryFeats[i],config);
				 for(j=0;j<spBPQueueSize(bpq);j++)
				 {
					 val = spListElementGetValue(imagesFeatsMatchCount[spListElementGetIndex(spBPQueuePeek(bpq))]);
					 val = val + 1.0;
					 spListElementSetValue(imagesFeatsMatchCount[spListElementGetIndex(spBPQueuePeek(bpq))],val);
					 spBPQueueDequeue(bpq);
				 }
				 spBPQueueDestroy(bpq);
			}

			for(i=0;i<featsFound;i++){
				spPointDestroy(quaryFeats[i]);

			}
			free(quaryFeats);

			printf("%s", "Please enter an image path:\n");
			scanf("%s", imagePath);
		}
	}

	printf("%s", "ExitingÖ\n");

	for(i=0;i<numOfImages;i++){
		spListElementDestroy(imagesFeatsMatchCount[i]);
		spPointDestroy(allImagesFeats[i]);
		for(j=0;j<numOfFeats;j++){
			spPointDestroy(allImagesFeatsByImg[i][j]);
		}
	}
	free(imagesFeatsMatchCount);
	free(allImagesFeats);
	free(allImagesFeatsByImg);

	SPKDArrayDestroy(arr);
	KDTreeDestroy(tree);
	free(configFile);
	free(imagePath);
	spConfigDestroy(config);
	return 0;
}

