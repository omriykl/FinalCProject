#include "SPListElement.h"
#include "SPLogger.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct sp_list_element_t {
	int index;
	double value;
};

SPListElement spListElementCreate(int index, double value) {
	SPListElement temp = NULL;
	if(index < 0 || value <0.0){
		spLoggerPrintWarning("index < 0 || value <0.0",__FILE__, __func__, __LINE__);
		return NULL;
	}
	temp = (SPListElement) malloc(sizeof(*temp));
	if (temp == NULL) { //Allocation Fails
		spLoggerPrintError("error while allocation of temp",__FILE__, __func__, __LINE__);
		return NULL;
	}
	temp->index = index;
	temp->value = value;
	return temp;
}

SPListElement spListElementCopy(SPListElement data) {
	SPListElement elementCopy = NULL;
	if (data == NULL) {
		spLoggerPrintWarning("data == NULL",__FILE__, __func__, __LINE__);
		return NULL;
	}
	elementCopy = (SPListElement) malloc(sizeof(*elementCopy));
	if (elementCopy == NULL) {
		spLoggerPrintError("error while allocation of elementCopy",__FILE__, __func__, __LINE__);
		return NULL;
	}
	elementCopy->index = data->index;
	elementCopy->value = data->value;
	return elementCopy;
}

void spListElementDestroy(SPListElement data) {
	if (data == NULL) {
		spLoggerPrintWarning("data == NULL",__FILE__, __func__, __LINE__);
		return;
	}
	free(data);
}

SP_ELEMENT_MSG spListElementSetIndex(SPListElement data, int index) {
	if (data == NULL || index <0) {
		return SP_ELEMENT_INVALID_ARGUMENT;
	}
	data->index = index;
	return SP_ELEMENT_SUCCESS;
}

int spListElementGetIndex(SPListElement data) {
	if(data == NULL){
		spLoggerPrintWarning("data == NULL",__FILE__, __func__, __LINE__);
		return -1;
	}
	return data->index;
}

SP_ELEMENT_MSG spListElementSetValue(SPListElement data, double newValue) {
	if (data == NULL || newValue < 0) {
		return SP_ELEMENT_INVALID_ARGUMENT;
	}
	data->value = newValue;
	return SP_ELEMENT_SUCCESS;
}

double spListElementGetValue(SPListElement data) {
	if(data == NULL){
		spLoggerPrintWarning("data == NULL",__FILE__, __func__, __LINE__);
		return -1.0;
	}
	return data->value;
}

int spListElementCompare(SPListElement e1, SPListElement e2){
	assert(e1!=NULL && e2!=NULL);
	if(e1->value == e2->value){
		return e1->index - e2->index;
	}else if(e1->value > e2->value){
		return 1;
	}
	return -1;
}
