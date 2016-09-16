#include "SPConfig.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct sp_config_t
{
	char * spImagesDirectory; // a parameter which contains the path of the images’ directory. This directory will contain all images in our database, such that given a query image, we will look for similar images in the directory spImagesDirectory. Constraint: the string contains no spaces.
	char * spImagesPrefix; // the images prefix. As we saw in assignment 2, all images in spImagesDirectory will start with this name. Constraint: the string contains no spaces.
	char * spImagesSuffix; // the images file format. This string represents the images format in spImagesDirecoty (all images in spImagesDirectory has the same file extension). Constraint: the string value is in the following set { .jpg , .png , .bmp , .gif }
	int spNumOfImages; // the number of images in spImagesDirectory. Constraint: positive integer.
	int spPCADimension; //we will use this value to reduce the dimension of the SIFT features from 128 to spPCADimension. Constraint: positive integer in the range [10 , 28].
	char * spPCAFilename; //the filename of the PCA file. Constraint: the string contains no spaces.
	int spNumOfFeatures; //the number of features which will be extracted per image. Constraint: positive integer.
	bool spExtractionMode; //a boolean variable which indicates if preprocessing must be done first or if the features are extracted from files (more on this later). Constraint: the value is in the following set { true , false}
	int spNumOfSimilarImages; //a positive integer which indicates the number of similar images which will be presented given a query image. That is if this parameter is set to 2, the most 2 similar images with respect to a query image will be presented. Constraint: spNumOfSimilarImages>0
	SP_SPLIT_METHOD spKDTreeSplitMethod; //a parameter which represents the cut method when the kd-tree is build (more later). Constraint: an enum which takes one of the following values {RANDOM, MAX_SPREAD, INCREMENTAL }
	int spKNN; //a positive integer which is used in the k nearest neighbor search algorithm (more on this later). Constraint: spKNN>0
	bool spMinimalGUI; //a boolean variable which indicates if minimal GUI is supported (more on this later). Constraint: the value is in the following set { true , false}
	int spLoggerLevel; //an integer which indicates the active level of the logger. The value 1 indicates error level, the value 2 indicates warning level, the value 3 indicates info level and 4 indicates debug level. Constraint: the value is in the following set { 1 , 2 , 3 , 4}
	char * spLoggerFilename; //the log file name. If the value is stdout , then the standard output will be used for printing. Otherwise, a log will be created with the name given by the parameter spLoggerFilename. Constraint: the string contains no spaces.
};

bool SetConfigValue(char * var, char * val, SPConfig spConfig,
		SP_CONFIG_MSG *msg)
{
	int i;
	if (strcmp(var, "spImagesDirectory") == 0)
	{
		i = 0;
		while (val[i] != '\0' && val[i] != '\r' && val[i] != '\n')
		{
			if (val[i] == ' ')
			{
				*msg = SP_CONFIG_INVALID_STRING;
				return false;
			}
			i++;
		}

		free(spConfig->spImagesDirectory);

		spConfig->spImagesDirectory = malloc(sizeof(char) * 1025);
		spConfig->spImagesDirectory[0] = '\0';

		strcpy(spConfig->spImagesDirectory, val);
	}
	else if (strcmp(var, "spImagesPrefix") == 0)
	{
		i = 0;
		while (val[i] != '\0' && val[i] != '\r' && val[i] != '\n')
		{
			if (val[i] == ' ')
			{
				*msg = SP_CONFIG_INVALID_STRING;
				return false;
			}
			i++;
		}
		free(spConfig->spImagesPrefix);
		spConfig->spImagesPrefix = malloc(sizeof(char) * 1025);
		spConfig->spImagesPrefix[0] = '\0';
		strcpy(spConfig->spImagesPrefix, val);

	}
	else if (strcmp(var, "spImagesSuffix") == 0)
	{
		i = 0;
		while (val[i] != '\0' && val[i] != '\r' && val[i] != '\n')
		{
			if (val[i] == ' ')
			{
				*msg = SP_CONFIG_INVALID_STRING;
				return false;
			}
			i++;
		}
		free(spConfig->spImagesSuffix);

		spConfig->spImagesSuffix = malloc(sizeof(char) * 1025);
		spConfig->spImagesSuffix[0] = '\0';
		strcpy(spConfig->spImagesSuffix, val);

	}
	else if (strcmp(var, "spLoggerFilename") == 0)
	{
		i = 0;
		while (val[i] != '\0' && val[i] != '\r' && val[i] != '\n')
		{
			if (val[i] == ' ')
			{
				*msg = SP_CONFIG_INVALID_STRING;
				return false;
			}
			i++;
		}
		free(spConfig->spLoggerFilename);

		spConfig->spLoggerFilename = malloc(sizeof(char) * 1025);
		spConfig->spLoggerFilename[0] = '\0';

		strcpy(spConfig->spLoggerFilename, val);

	}
	else if (strcmp(var, "spPCAFilename") == 0)
	{
		i = 0;
		while (val[i] != '\0' && val[i] != '\r' && val[i] != '\n')
		{
			if (val[i] == ' ')
			{
				*msg = SP_CONFIG_INVALID_STRING;
				return false;
			}
			i++;
		}
		free(spConfig->spPCAFilename);

		spConfig->spPCAFilename = malloc(sizeof(char) * 1025);
		spConfig->spPCAFilename[0] = '\0';

		strcpy(spConfig->spPCAFilename, val);

	}
	else if (strcmp(var, "spNumOfFeatures") == 0)
	{
		if (atoi(val) < 1)
		{
			*msg = SP_CONFIG_INVALID_INTEGER;
			return false;
		}
		spConfig->spNumOfFeatures = atoi(val);

	}
	else if (strcmp(var, "spNumOfImages") == 0)
	{
		if (atoi(val) < 1)
		{
			*msg = SP_CONFIG_INVALID_INTEGER;
			return false;
		}
		spConfig->spNumOfImages = atoi(val);
	}
	else if (strcmp(var, "spPCADimension") == 0)
	{
		if (atoi(val) < 10 || atoi(val) > 28)
		{
			*msg = SP_CONFIG_INVALID_INTEGER;
			return false;
		}
		spConfig->spPCADimension = atoi(val);

	}
	else if (strcmp(var, "spNumOfSimilarImages") == 0)
	{
		if (atoi(val) < 1)
		{
			*msg = SP_CONFIG_INVALID_INTEGER;
			return false;
		}
		spConfig->spNumOfSimilarImages = atoi(val);

	}
	else if (strcmp(var, "spKNN") == 0)
	{
		if (atoi(val) < 1)
		{
			*msg = SP_CONFIG_INVALID_INTEGER;
			return false;
		}
		spConfig->spKNN = atoi(val);

	}
	else if (strcmp(var, "spLoggerLevel") == 0)
	{
		i = atoi(val);
		if (i < 1 || i > 4)
		{
			*msg = SP_CONFIG_INVALID_INTEGER;
			return false;
		}
		spConfig->spLoggerLevel = atoi(val);

	}
	else if (strcmp(var, "spExtractionMode") == 0)
	{

		if (strcmp(val, "true") == 0)
		{
			spConfig->spExtractionMode = true;

		}
		else if (strcmp(val, "false") == 0)
		{
			spConfig->spExtractionMode = false;

		}
		else
		{
			*msg = SP_CONFIG_INVALID_ARGUMENT;
			return false;
		}

	}
	else if (strcmp(var, "spMinimalGUI") == 0)
	{

		if (strcmp(val, "true") == 0)
		{
			spConfig->spMinimalGUI = true;

		}
		else if (strcmp(val, "false") == 0)
		{
			spConfig->spMinimalGUI = false;

		}
		else
		{
			*msg = SP_CONFIG_INVALID_INTEGER;
			return false;
		}

	}
	else if (strcmp(var, "spKDTreeSplitMethod") == 0)
	{
		i = 0;
		while (val[i] != '\0' && val[i] != '\r' && val[i] != '\n')
		{
			if (val[i] == ' ')
				{
						*msg = SP_CONFIG_INVALID_STRING;
						return false;
			    }
		  i++;
		}

		if (strcmp(val, "RANDOM") == 0)
			spConfig->spKDTreeSplitMethod = RANDOM;
		else if (strcmp(val, "MAX_SPREAD") == 0)
			spConfig->spKDTreeSplitMethod = MAX_SPREAD;
		else if (strcmp(val, "INCREMENTAL") == 0)
			spConfig->spKDTreeSplitMethod = INCREMENTAL;
		else
		{
			*msg = SP_CONFIG_INVALID_STRING;
			return false;
		}

	}
	return true;

}

SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg)
{
	SPConfig spConfig = (SPConfig) malloc(sizeof(*spConfig));
	char *line = malloc(sizeof(char) * 1025);
	char *var;
	char *value;
	int varOrValue = 0; //0=nothing yet, 1= var, 2=value;
	int varIndex = 0;
	int valIndex = 0;
	int i;
	bool setResult;
	FILE* configFile;
	int countLine = 0;


	spConfig->spPCADimension = 20;
	spConfig->spNumOfFeatures = 100;
	spConfig->spExtractionMode = true;
	spConfig->spMinimalGUI = false;
	spConfig->spNumOfSimilarImages = 1;
	spConfig->spKNN = 1;
	spConfig->spKDTreeSplitMethod = MAX_SPREAD;
	spConfig->spLoggerLevel = 3;
	spConfig->spNumOfImages = -1;

	if (line == NULL || spConfig == NULL)
	{
		*msg = SP_CONFIG_ALLOC_FAIL;
		return NULL;
	}

	if (filename == NULL)
	{
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}

	configFile = fopen(filename, "r");
	if (configFile == NULL)
	{
		*msg = SP_CONFIG_CANNOT_OPEN_FILE;
		return NULL;
	}

	while (fgets(line, 1024, configFile) != NULL)
	{

		if (line[0] != '#')
		{
			varIndex = 0;
			valIndex = 0;
			i = 0;
			varOrValue = 0;
			var= malloc(sizeof(char) * 1025);
			value= malloc(sizeof(char) * 1025);
			if (var == NULL || value == NULL)
				{
					*msg = SP_CONFIG_ALLOC_FAIL;
					return NULL;
				}

			var[0] = '\0';
			value[0] = '\0';

			while (line[i] != '\n')
			{
				if (line[i] != ' ')
				{
					if (varOrValue == 0)
					{
						varOrValue = 1;

						var[varIndex] = line[i];
						varIndex++;
					}
					else if (varOrValue == 1)
					{
						if (line[i] == '=')
						{
							varOrValue = 2;
							var[varIndex] = '\0';
						}
						else if (line[i] == ' ')
						{
							while (line[i + 1] == ' '){
								i++;
							}

							if (line[i + 1] != '=')
							{
								*msg = SP_CONFIG_INVALID_STRING;
								return NULL;
							}

						}
						else
						{
							var[varIndex] = line[i];
							varIndex++;
						}

					}
					else if (varOrValue == 2)
					{
						if (line[i] == '\n')
						{
							value[valIndex] = '\0';
							break;
						}
						else if (line[i] == ' ')
						{
							while (line[i + 1] == ' ')
								i++;
							if (line[i + 1] != '\n')
							{
								*msg = SP_CONFIG_INVALID_STRING;
								return NULL;
							}

							else
							{
								value[valIndex] = '\0';
								break;
							}
						}
						else
						{
							value[valIndex] = line[i];
							valIndex++;
						}
					}

				}
				i++;
			}
			if (varOrValue != 2 && varOrValue != 0)
			{
				printf(
						"File: %s\n Line: %d\n Message: Invalid configuration line\n",
						filename, countLine);
			}
			else{
				value[valIndex] = '\0';
				setResult = SetConfigValue(var, value, spConfig, msg);

						if (setResult == false)
						{
							printf(
									"File: %s\n Line: %d\n Message: Invalid value - constraint not met\n",
									filename, countLine);
							return NULL;
						}
			}
			free(var);
			free(value);
		}
		countLine++;

	}

	//check if all set
	if (spConfig->spImagesDirectory == NULL)
	{
		*msg = SP_CONFIG_MISSING_DIR;
		printf("File: %s\n Line: %d\n Message: Parameter %s is not set\n",
				filename, countLine, "spImagesDirectory");
		return NULL;
	}
	else if (spConfig->spImagesPrefix == NULL)
	{
		*msg = SP_CONFIG_MISSING_PREFIX;
		printf("File: %s\n Line: %d\n Message: Parameter %s is not set\n",
				filename, countLine, "spImagesPrefix");
		return NULL;
	}
	else if (spConfig->spImagesSuffix == NULL)
	{
		*msg = SP_CONFIG_MISSING_SUFFIX;
		printf("File: %s\n Line: %d\n Message: Parameter %s is not set\n",
				filename, countLine, "spImagesSuffix");
		return NULL;
	}
	else if (spConfig->spNumOfImages == -1)
	{
		*msg = SP_CONFIG_MISSING_NUM_IMAGES;
		printf("File: %s\n Line: %d\n Message: Parameter %s is not set\n",
				filename, countLine, "spNumOfImages");
		return NULL;
	}
	*msg = SP_CONFIG_SUCCESS;

	free(line);


	if(spConfig->spPCAFilename==NULL){
		spConfig->spPCAFilename = "pca.yml";
	}
	if(spConfig->spLoggerFilename==NULL || strcmp(spConfig->spLoggerFilename,"stdout")==0){
		spConfig->spLoggerFilename = NULL;
	}


	return spConfig;

}
bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg)
{
	if (config == NULL)
	{
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	else
	{
		*msg = SP_CONFIG_SUCCESS;
		return config->spExtractionMode;
	}
}

bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG* msg)
{
	if (config == NULL)
	{
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	else
	{
		*msg = SP_CONFIG_SUCCESS;
		return config->spMinimalGUI;
	}

}
int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg)
{
	if (config == NULL)
	{
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	else
	{
		*msg = SP_CONFIG_SUCCESS;
		return config->spNumOfImages;
	}
}

int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg)
{
	if (config == NULL)
	{
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	else
	{
		*msg = SP_CONFIG_SUCCESS;
		return (int) config->spNumOfFeatures;
	}

}

int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg)
{
	if (config == NULL)
	{
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	else
	{
		*msg = SP_CONFIG_SUCCESS;
		return config->spPCADimension;
	}

}

SP_SPLIT_METHOD spConfigeGetSplitMethod(SPConfig config)
{
	if (config == NULL)
		return -1;
	return config->spKDTreeSplitMethod;
}

int spConfigGetspKNN(SPConfig config)
{
	if (config == NULL)
		return -1;
	return config->spKNN;
}

char* spConfigGetImagesDirectory(SPConfig config)
{
	if (config == NULL)
		return NULL;
	return config->spImagesDirectory;
}

int spConfigGetspNumOfSimilarImages(SPConfig config)
{
	if (config == NULL)
		return -1;
	return config->spNumOfSimilarImages;
}

SP_LOGGER_LEVEL spConfigGetspLoggerLevel(SPConfig config)
{
	if (config == NULL)
		return -1;
	return config->spLoggerLevel;
}

char* spConfigGetspLoggerFilename(SPConfig config)
{
	if (config == NULL)
		return NULL;
	return config->spLoggerFilename;
}

SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config,
		int index)
{
	char str[15];
	if (imagePath == NULL || config == NULL)
	{
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	else if (index >= config->spNumOfImages)
	{
		return SP_CONFIG_INDEX_OUT_OF_RANGE;
	}
	else
	{
		imagePath[0] = '\0';
		strcat(imagePath, config->spImagesDirectory);
		strcat(imagePath, config->spImagesPrefix);
		sprintf(str, "%d", index);
		strcat(imagePath, str);
		strcat(imagePath, config->spImagesSuffix);
		return SP_CONFIG_SUCCESS;

	}

}

SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config)
{
	if (pcaPath == NULL || config == NULL)
	{
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	else
	{
		strcat(pcaPath, config->spImagesDirectory);
		strcat(pcaPath, config->spPCAFilename);
		return SP_CONFIG_SUCCESS;

	}
}

char* spConfigGetImagesPrefix(SPConfig config)
{
	if (config == NULL)
		return NULL;
	return config->spImagesPrefix;
}

void spConfigDestroy(SPConfig config)
{
	if (config != NULL)
	{
		free(config->spImagesDirectory);
		free(config->spImagesPrefix);
		free(config->spImagesSuffix);
		free(config->spLoggerFilename);
		free(config->spPCAFilename);
		free(config);
	}
}

void spConfigPrint(SPConfig co)
{
	printf("spImagesDirectory = %s\n", co->spImagesDirectory);
	printf("spImagesPrefix = %s\n", co->spImagesPrefix);
	printf("spImagesSuffix = %s\n", co->spImagesSuffix);
	printf(" = %s\n", co->spLoggerFilename);
	printf(" = %s\n", co->spPCAFilename);
	printf(" = %d\n", co->spKNN);
	printf(" = %d\n", co->spLoggerLevel);
	printf(" = %d\n", co->spNumOfFeatures);
	printf(" = %d\n", co->spNumOfImages);
	printf(" = %d\n", co->spNumOfSimilarImages);
	printf(" = %d\n", co->spPCADimension);
	printf(" = %d\n", co->spMinimalGUI);
	printf(" = %d\n", co->spExtractionMode);

}

//int main()
//{
//	SP_CONFIG_MSG msg;
//	spConfigCreate("conf.config",&msg);
//	printf("endmain\n");
//	return 0;
//}
