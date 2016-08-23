#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include "SPListElement.h"
#include <stdbool.h>
/**
 * SP Bounded Priority Queue summary
 *
 *
 * Implements a priority queue container type.
 * The elements of the queue are of type SPListElement, please refer
 * to SPListElement.h for usage.
 * There is a fixed upper bound on the number of elements that can be stored in the BPQ.
 * Whenever a new element is added to the queue, if the queue is at capacity,
 * the element with the highest priority value is ejected from the queue.
 *
 * The following functions are available:
 *
 * Create- creates an empty queue with a given maximum capacity
 * Copy- creates a copy of a given queue
 * Destroy- frees all memory allocation associated with the queue
 * Clear- removes all the elements in the queue
 * GetSize- returns the number of elements in the queue
 * GetMaxSize- returns the maximum capacity of the queue
 * Enqueue- Inserts a NEW COPY (must be allocated) element to the queue
 * Dequeue- removes the element with the lowest value
 * Peek- returns a NEW COPY of the element with the lowest value
 * PeekLast – returns a NEW COPY of the element with the highest value
 * MinValue- returns the minimum value in the queue
 * MaxValue- returns the maximum value in the queue
 * IsEmpty – returns true if the queue is empty
 * IsFull- returns true if the queue is full
 *
 */


/** type used to define Bounded priority queue **/
typedef struct sp_bp_queue_t* SPBPQueue;

/** type for error reporting **/
typedef enum sp_bp_queue_msg_t {
	SP_BPQUEUE_OUT_OF_MEMORY,
	SP_BPQUEUE_FULL,
	SP_BPQUEUE_EMPTY,
	SP_BPQUEUE_INVALID_ARGUMENT,
	SP_BPQUEUE_SUCCESS
} SP_BPQUEUE_MSG;


/**
 * Allocates a new SBPQueue.
 *
 * This function creates an empty queue with a given maximum capacity.
 * @param maxSize - maximum capacity of the queue
 * @return
 * 	NULL - If allocations failed.
 * 	A new SBPQueue in case of success.
 */
SPBPQueue spBPQueueCreate(int maxSize);

/**
 * Creates a copy of target SBPQueue.
 *
 * The new copy will contain all the elements from the source SBPQueue in the same
 * order. the iterator state will not change.
 *
 * @param source The target SPBPQueue to copy
 * @return
 * NULL if a NULL was sent or a memory allocation failed.
 * A SPBPQueue containing the same elements with same order as source otherwise.
 */
SPBPQueue spBPQueueCopy(SPBPQueue source);

/**
 * Deallocates an existing SPBPQueue. Clears all elements by using the
 * stored free function.
 *
 * @param source Target SPBPQueue to be deallocated. If list is NULL nothing will be
 * done
 */
void spBPQueueDestroy(SPBPQueue source);

/**
 * Removes all elements from target source. The state of the current element will not be defined afterwards.
 *
 * The elements are deallocated using the stored freeing function
 * @param source Target SPBPQueue to remove all element from.If list is NULL nothing will be
 * done
 */
void spBPQueueClear(SPBPQueue source);

/**
 * Returns the number of elements in a source. the iterator state will not change.
 *
 * @param source The target SPBPQueue which size is requested.
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise the number of elements in the SPBPQueue.
 */
int spBPQueueSize(SPBPQueue source);

 /**
 * Returns the maximum capacity of the queue. the iterator state will not change.
 *
 * @param source The target SPBPQueue which maximum capacity is requested.
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise the maximum capacity of the SPBPQueue.
 */

int spBPQueueGetMaxSize(SPBPQueue source);

/**
 * Adds a new element to the source, the new element will be ordered by value.
 *
 * If the queue is at capacity, the element with the highest priority value is ejected from the queue.
 *
 * @param source The SPBPQueue for which to add an element in its start
 * @param element The element to insert. A copy of the element will be
 * inserted
 * @return
 * SP_BPQUEUE_INVALID_ARGUMENT if a NULL was sent as source or as element
 * SP_BPQUEUE_OUT_OF_MEMORY if an allocation failed
 * SP_BPQUEUE_FULL the source was full,and the element has not been inserted
 *                 because it was the element with the highest priority value in the queue.
 * SP_BPQUEUE_SUCCESS the element has been inserted successfully
 */

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element);

/**
 * Removes the element with the lowest value
 *
 * @param source The SPBPQueue from which element needs to be removed
 * @return
 * SP_BPQUEUE_INVALID_ARGUMENT - if a NULL pointer was sent.
 * SP_BPQUEUE_EMPTY- if source is empty
 * SP_BPQUEUE_SUCCESS - Otherwise.
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source);

/**
 * Returns a NEW COPY of the element with the lowest value in source
 * Items in source will not be changed
 *
 * @param source The SPBPQueue from which element needs to be peeked
 * @return
 * NULL if a NULL pointer was sent or if an allocation failed.
 * Otherwise a NEW COPY of the element with the lowest value in source
 */
SPListElement spBPQueuePeek(SPBPQueue source);

/**
 * Returns a NEW COPY of the element with the highest value in source
 * Items in source will not be changed
 *
 * @param source The SPBPQueue from which element needs to be peeked
 * @return
 * NULL if a NULL pointer was sent or if an allocation failed.
 * Otherwise a NEW COPY of the element with the highest value in source
 */
SPListElement spBPQueuePeekLast(SPBPQueue source);

/**
 * Returns the minimum value in the SPBPQueue source
 * Items in source will not be changed
 *
 * @param source The SPBPQueue from which value needs to be pulled from
 * @return
 * -1 if a NULL pointer was Or source is empty.
 * Otherwise the minimum value in the SPBPQueue source
 */
double spBPQueueMinValue(SPBPQueue source);

/**
 * Returns the maximum value in the SPBPQueue source
 * Items in source will not be changed
 *
 * @param source The SPBPQueue from which value needs to be pulled from
 * @return
 * -1 if a NULL pointer was Or source is empty.
 * Otherwise the maximum value in the SPBPQueue source
 */
double spBPQueueMaxValue(SPBPQueue source);

/**
 * Returns true if the queue is empty
 * Items in source will not be changed
 *
 * @param source The SPBPQueue that needs to be checked
 * @return true if the queue is empty
 * Otherwise false
 */
bool spBPQueueIsEmpty(SPBPQueue source);

/**
 * Returns true if the queue is full
 * Items in source will not be changed
 *
 * @param source The SPBPQueue that needs to be checked
 * @return true if the queue is full
 * Otherwise false
 */
bool spBPQueueIsFull(SPBPQueue source);

#endif
