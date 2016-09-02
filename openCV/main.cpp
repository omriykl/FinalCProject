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
#include "SPLogger.h"
#include "SPImageSearch.h"
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
	int i;
	int numOfImages;
	int numOfFeats;
	int featsFound;
	char* imagePath;
	SPPoint ** allImagesFeats;
	if (args_num < 2)
	{
		printf("%s", "Invalid command line : use -c <config_filename>\n");
		free(imagePath);
		return 0;
	}

	if (strcmp(args[1], "-c") != 0)
	{
		printf("%s", "Invalid command line : use -c <config_filename>\n");
		free(imagePath);
		return 0;
	}
	if (args_num < 3)
	{
		configFile = fopen("spcbir.config", "r");
		if (configFile == NULL)
		{
			printf("%s",
					"The default configuration file spcbir.config couldnít be open\n");
			free(configFile);
			free(imagePath);
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
			free(configFile);
			free(imagePath);
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

		allImagesFeats = (SPPoint **) malloc(sizeof(SPPoint *) * numOfImages);
		imagePath = (char*) malloc(sizeof(char) * 1025);

		if (spConfigIsExtractionMode(config, &msg) == true)
		{
			for (i = 0; i < numOfImages; i++)
			{
				msg = spConfigGetImagePath(imagePath, config, i);
				printf("%s\n", imagePath);
				allImagesFeats[i] = pr.getImageFeatures(imagePath, i,
						&featsFound);
				saveFeaturesToFile(spConfigGetImagesDirectory(config),
						spConfigGetImagesPrefix(config), i, allImagesFeats[i],
						featsFound);
			}
		}
		else
		{
			for (i = 0; i < numOfImages; i++)
			{
				msg = spConfigGetImagePath(imagePath, config, i);
				printf("%s\n", imagePath);
				allImagesFeats[i] = getFeaturesFromFile(
						spConfigGetImagesDirectory(config),
						spConfigGetImagesPrefix(config), i, &featsFound);
			}
		}

		printf("s%", "Please enter an image path:\n");
		scanf("%s", imagePath);
		while (strcmp(imagePath, "<>") != 0)
		{

			//TODO: ...
			GetSimilarImages(spConfigGetspNumOfSimilarImages(config),
					spConfigGetImagesDirectory(config), imagePath);

			printf("s%", "Please enter an image path:\n");
			scanf("%s", imagePath);
		}
	}

	printf("%s", "ExitingÖ\n");

	free(configFile);
	free(imagePath);
	spConfigDestroy(config);
	return 1;
}

