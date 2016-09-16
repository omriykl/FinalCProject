#include "SPListElement.h"
#include "SPList.h"
#include "SPLogger.h"
#include "SPBPriorityQueue.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

struct sp_bp_queue_t {
	SPList list;
	int maxSize;
};

SPBPQueue spBPQueueCreate(int maxSize){
	SPBPQueue queue = (SPBPQueue) malloc(sizeof(*queue));
	if (queue == NULL){
		spLoggerPrintError("error allocation queue",__FILE__, __func__, __LINE__);
		return NULL;

	}
	else {
		queue->list = spListCreate();
		if (queue->list == NULL){
			spLoggerPrintError("error allocation list",__FILE__, __func__, __LINE__);
			free(queue);
			return NULL;
		}
		queue->maxSize = maxSize;

		return queue;
	}
}

SPBPQueue spBPQueueCopy(SPBPQueue source){
	if (source == NULL){
		spLoggerPrintWarning("source == NULL",__FILE__, __func__, __LINE__);
		return NULL;
	}

	SPBPQueue new = (SPBPQueue) malloc(sizeof(*new));
	if (new == NULL){
		spLoggerPrintError("error allocation new",__FILE__, __func__, __LINE__);
		return NULL;
	}
	else {
		new->list = spListCopy(source->list);
		if (new->list == NULL){
			spLoggerPrintError("error allocation list",__FILE__, __func__, __LINE__);
			free(new);
			return NULL;
		}
		new->maxSize = source->maxSize;

		return new;
	}
}

void spBPQueueDestroy(SPBPQueue source){
	if (source == NULL){
		spLoggerPrintWarning("source == NULL",__FILE__, __func__, __LINE__);
	    return;
	}
	spListDestroy(source->list);
	free(source);
}

void spBPQueueClear(SPBPQueue source){
	if (source == NULL)
		return;
	spListClear(source->list);
}

int spBPQueueSize(SPBPQueue source){
	if (source == NULL){
		spLoggerPrintWarning("source == NULL",__FILE__, __func__, __LINE__);
		return -1;
	}
	return spListGetSize(source->list);
}

int spBPQueueGetMaxSize(SPBPQueue source){
	if (source == NULL){
		spLoggerPrintWarning("source == NULL",__FILE__, __func__, __LINE__);
		return -1;
	}
	return source->maxSize;
}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element){
	SP_LIST_MSG msg;
	SPListElement cur;
	SPListElement first;

	if (source == NULL || element == NULL)
		return SP_BPQUEUE_INVALID_ARGUMENT;
	if (spBPQueueSize(source) == 0){
		msg = spListInsertFirst(source->list,element);
		if (msg == SP_LIST_OUT_OF_MEMORY)
			return SP_BPQUEUE_OUT_OF_MEMORY;
		return SP_BPQUEUE_SUCCESS;
	}

	if (spBPQueueSize(source) >= spBPQueueGetMaxSize(source))
	{
		cur = spListGetLast(source->list);
		if (spListElementGetValue(cur) < spListElementGetValue(element))
			return SP_BPQUEUE_FULL;
		spListRemoveCurrent(source->list);
	}

	first = spListGetFirst(source->list);

	if (spListElementGetValue(first) > spListElementGetValue(element)){
		msg = spListInsertFirst(source->list,element);
		if (msg == SP_LIST_OUT_OF_MEMORY)
			return SP_BPQUEUE_OUT_OF_MEMORY;
		return SP_BPQUEUE_SUCCESS;
	}

	cur = spListGetNext(source->list);
	while ((cur != NULL) && (spListElementGetValue(cur) < spListElementGetValue(element))){
		cur = spListGetNext(source->list);
	}
	if (cur == NULL)
	{
		msg = spListInsertLast(source->list,element);
		if (msg == SP_LIST_OUT_OF_MEMORY)
			return SP_BPQUEUE_OUT_OF_MEMORY;
		return SP_BPQUEUE_SUCCESS;
	}
	msg = spListInsertBeforeCurrent(source->list,element);
	if (msg == SP_LIST_OUT_OF_MEMORY)
		return SP_BPQUEUE_OUT_OF_MEMORY;
	return SP_BPQUEUE_SUCCESS;

}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source){
	if (source == NULL)
		return SP_BPQUEUE_INVALID_ARGUMENT;
	if (spBPQueueSize(source) <= 0)
		return SP_BPQUEUE_EMPTY;

	spListGetFirst(source->list);
	spListRemoveCurrent(source->list);
	return SP_BPQUEUE_SUCCESS;
}

SPListElement spBPQueuePeek(SPBPQueue source){
	SPListElement newElement;

	if (source == NULL || source->list == NULL){
		spLoggerPrintWarning("source == NULL or source->list == NULL",__FILE__, __func__, __LINE__);
		return NULL;
	}
	if (spBPQueueSize(source) <=0)
		return NULL;
	 newElement = spListElementCopy(spListGetFirst(source->list));
	 return newElement;
}

SPListElement spBPQueuePeekLast(SPBPQueue source){
	SPListElement newElement;

	if (source == NULL || source->list == NULL){
		spLoggerPrintWarning("source == NULL or source->list == NULL",__FILE__, __func__, __LINE__);
				return NULL;
			}

	if (spBPQueueSize(source) <=0)
		return NULL;
	 newElement = spListElementCopy(spListGetLast(source->list));
	 return newElement;
}

double spBPQueueMinValue(SPBPQueue source){
	if (source == NULL || source->list == NULL || spBPQueueSize(source) <=0){
		spLoggerPrintWarning("source == NULL or source->list == NULL",__FILE__, __func__, __LINE__);
		return -1;
	}

	return spListElementGetValue(spListGetFirst(source->list));
}

double spBPQueueMaxValue(SPBPQueue source){
	if (source == NULL || source->list == NULL || spBPQueueSize(source) <=0){
		spLoggerPrintWarning("source == NULL or source->list == NULL",__FILE__, __func__, __LINE__);
		return -1;
	}
	return spListElementGetValue(spListGetLast(source->list));
}

bool spBPQueueIsEmpty(SPBPQueue source){
	if (source == NULL || spBPQueueSize(source) <=0)
		return true;
	return false;
}

bool spBPQueueIsFull(SPBPQueue source){
	if (source == NULL)
		return false;
	if (spBPQueueSize(source) == spBPQueueGetMaxSize(source))
		return true;
	return false;
}


void SPBQUnitTest()
{
	SPBPQueue a = spBPQueueCreate(5);
	printf("%d\n",spBPQueueSize(a));
	SPListElement sp = spListElementCreate(1,4.5);
	spBPQueueEnqueue(a,sp);
	printf("%d\n",spBPQueueSize(a));

	sp = spListElementCreate(2,1.1);
	spBPQueueEnqueue(a,sp);
	printf("%d\n",spBPQueueSize(a));

	sp = spListElementCreate(3,1.5);
	spBPQueueEnqueue(a,sp);
	printf("%d\n",spBPQueueSize(a));

	sp = spListElementCreate(4,0.6);
	spBPQueueEnqueue(a,sp);
	printf("%d\n",spBPQueueSize(a));

	sp = spListElementCreate(5,0.1);
	spBPQueueEnqueue(a,sp);
	printf("%d\n",spBPQueueSize(a));

	sp = spListElementCreate(6,6.2);
	spBPQueueEnqueue(a,sp);
	printf("%d\n",spBPQueueSize(a));

//	spBPQueueDequeue(a);
//	spBPQueueDequeue(a);

	printf("%f\n",spListElementGetValue(spListGetLast(a->list)));
	printf("%f\n",spListElementGetValue(spListGetFirst(a->list)));

	printf("%d\n",spBPQueueSize(a));

	printf("%f\n",spBPQueueMinValue(a));
	printf("%d\n",spBPQueueIsEmpty(a));
	printf("%d\n",spBPQueueIsFull(a));
}
