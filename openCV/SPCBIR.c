/*
 * SPCBIR.c
 *
 *  Created on: 20 ·‡Â‚ 2016
 *      Author: ‰ÈÏ‰
 */

#include "SPConfig.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define PROGRAMNAME "SPCBIR"


int main(int args_num, char** args)
{

	char* imagePath=malloc(sizeof(char)*1024);
	char* inputProgramName=malloc(sizeof(char)*1024);

	FILE* configFile;
	SP_CONFIG_MSG msg;
	SPConfig config;

	if(strcmp(args[1],"-c")!=0){
		printf("%s","Invalid command line : use -c <config_filename>\n");
		free(configFile);
		free(imagePath);
		return 0;
	}
	if(args_num<3){
		configFile = fopen("spcbir.config","r");
		if(configFile==NULL){
			printf("%s","The default configuration file spcbir.config couldnít be open\n");
			free(configFile);
			free(imagePath);
			return 0;

			}
	}
	else{
		configFile = fopen(args[2],"r");
		if(configFile==NULL){
				printf("%s %s %s","The configuration file ",args[2]," couldnít be open\n");
				free(configFile);
				free(imagePath);
				return 0;
		}
	}

	config= spConfigCreate(args[2],&msg);
	if(config==NULL){
		return 0;
		}

	printf("s%","Please enter an image path:\n");
	scanf("%s", imagePath);
	while(strcmp(imagePath,"<>")!=0){




		printf("s%","Please enter an image path:\n");
		scanf("%s", imagePath);
	}

	printf("s%","ExitingÖ\n");

	free(configFile);
	free(imagePath);
	spConfigDestroy(config);
	return 0;
}

