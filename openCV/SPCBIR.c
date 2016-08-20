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


int main()
{
	char* str=malloc(sizeof(char)*1024);

	char* configName=malloc(sizeof(char)*1024);
	char* imagePath=malloc(sizeof(char)*1024);
	char* inputProgramName=malloc(sizeof(char)*1024);

	char* inputC=malloc(sizeof(char)*1024);
	FILE* configFile;
	SP_CONFIG_MSG msg;
	SPConfig config;

	int i=0;
	int readStatus=0;


	scanf("%s", str);
	while(str[i]!='\n'){
		if(readStatus==0){
			if(str[i]==' '){
				readStatus=1;
				inputProgramName[i]='\0';
			}
			inputProgramName[i]=str[i];
		}
		else if(readStatus==1){
			if(str[i]==' '){
				readStatus=2;
				inputC[i]='\0';
			}
			inputC[i]=str[i];
		}
		else if(readStatus==2){
			if(str[i]==' ' || str[i]=='\0'){
				configName[i]='\0';

			}
			configName[i]=str[i];
		}
	i++;
	}
	configName[i]='\0';


	if(strcmp(inputProgramName,PROGRAMNAME)!=0){
		printf("%s","Invalid command line : use -c <config_filename>\n");
		free(str);
		free(configName);
		free(inputC);
		free(configFile);
		free(imagePath);
		return 0;
	}
	if(strcmp(inputC,"-c")!=0){
		printf("%s","Invalid command line : use -c <config_filename>\n");
		free(str);
		free(configName);
		free(inputC);
		free(configFile);
		free(imagePath);
		return 0;
	}
	if(readStatus<2){
		configName="spcbir.config";
		configFile = fopen(configName,"r");
		if(configFile==NULL){
			printf("%s","The default configuration file spcbir.config couldnít be open\n");
			free(str);
			free(configName);
			free(inputC);
			free(configFile);
			free(imagePath);
			return 0;

			}
	}
	else{
		configFile = fopen(configName,"r");
		if(configFile==NULL){
				printf("%s %s %s","The configuration file ",configName," couldnít be open\n");
				free(str);
				free(configName);
				free(inputC);
				free(configFile);
				free(imagePath);
				return 0;
		}
	}

	config= spConfigCreate(configName,&msg);
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
	free(str);
	free(configName);
	free(inputC);
	free(configFile);
	free(imagePath);
	spConfigDestroy(config);
	return 0;
}

