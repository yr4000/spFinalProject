#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include "SPListElement.h"
#include "SPList.h"
#include "SPListElement.h"
#include <stdbool.h>
/**
 * SP Bounded Priority Queue summary
 *
 * 
 * SPBPriorityQueue Summary
 *
 * Implements a SPBPQueue type.
 * The elements of the SPBPQueue are of type SPListElement, please refer
 * to SPListElement.h for usage.
 *
 * The following functions are available:
 *
 *   spBPQueueCreate          - creates an empty queue with a given maximum capacity
 *   spBPQueueCopy            - creates a copy of a given queue
 *   spBPQueueDestroy         - frees all memory allocation associated with the queue
 *   spBPQueueClear           - removes all the elements in the queue
 *   spBPQueueSize            - returns the number of elements in the queue
 *   spBPQueueGetMaxSize      - returns the maximum capacity of the queue
 *   spBPQueueSetMaxSize      - updates the maximum capacity of the queue to the given size
 *   spBPQueueEnqueue         - Inserts a new copy (must be allocated) element to the queue
 *   spBPQueueDequeue         - removes the element with the lowest value
 *   spBPQueuePeek            - returns a new copy of the element with the lowest value
 *   spBPQueuePeekLast        – returns a new copy of the element with the highest value
 *   spBPQueueMinValue        - returns the minimum value in the queue
 *   spBPQueueMaxValue        - returns the maximum value in the queue
 *   spBPQueueIsEmpty         – returns true if the queue is empty
 *   spBPQueueIsFull          - returns true if the queue is full
 *   spBPRemoveMax            - removes the maximum element in the queue
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
 * spBPQueueCreate: Allocates a new SPBPQUueue.
 *
 * This function creates a new empty SPBPQueue with a given maximum capacity.
 * @param int maxSize- the maximum size of the SPBPQueue.
 * @return
 * Null - If memory allocations failed.
 * A new SPBPQueue in case of success.
 *
 */

SPBPQueue spBPQueueCreate(int maxSize);

/**
 * spBPQueueCopy: creates a copy of a given queue.
 *
 * The new copy of the SPBPQueue will contain all the elements from the source SPBPQueue in the same
 * order.
 *
 * @param SPBPQueue source - the target SPBPQueue to copy.
 * @return
 * NULL if a NULL was sent or a memory allocation failed.
 * A SPBPQueue containing the same elements with same order as SPBPQueue otherwise.
 */

SPBPQueue spBPQueueCopy(SPBPQueue source);

/**
 * 
 * spBPQueueDestroy: frees all memory allocation associated with the SPBPQueue.
 * Deallocates an existing SPBPQueue. Clears all elements by using the
 * free function.
 *
 * @param SPBPQueue source- the SPBPQueue that need to be deallocated.
 * If SPBPQueue is NULL, spBPQueueDestroy will do nothing.
 */

void spBPQueueDestroy(SPBPQueue source);

/**
 * spBPQueueClear: removes all the elements from the SPBPQueue.
 *
 * The elements are deallocated using the freeing function.
 * @param SPBPQueue source: we remove all element from this given SPBPQueue.
 * 
 */

void spBPQueueClear(SPBPQueue source);

/**
 * spBPQueueSize- returns the number of elements in the SPBPQueue.
 *
 * @param SPBPQueue source: The SPBPQueue which his size is requested.
 *
 * @return
 * -1 if a NULL pointer was sent or if the SPBPQueue is empty.
 * Otherwise we return the number of elements in the SPBPQueue.
 */

int spBPQueueSize(SPBPQueue source);

/**
 * spBPQueueGetMaxSize- returns the maximum capacity of the SPBPQueue.
 *
 * @param SPBPQueue source: The SPBPQueue which his maximum capacity of him
 * is requested.
 *
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise we return the maximum capacity of the SPBPQueue,
 * which is the field maxSize ofbSPBPQueue.
 */

int spBPQueueGetMaxSize(SPBPQueue source);

/**
 * spBPQueueEnqueue- inserts a new copy, which must be allocated, element to the queue.
 *
 * @param SPBPQueue source: The SPBPQueue which we would like to insert to a copy of
 * the new SPListElement element.
 * SPListElement element: the SPListElement element we would like to insert a copy of
 * him into the SPBPQueue source.
 *
 * @return
 * SP_BPQUEUE_INVALID_ARGUMENT if a NULL pointer was sent or that the SPListElement element
 * is null. we won't add it to the SPBPQueue source.
 *
 * SP_BPQUEUE_OUT_OF_MEMORY if the copy of the SPListElement element is NULL. we won't add
 * it to the SPBPQueue source.
 *
 * SP_BPQUEUE_SUCCESS: if the SPBPQueue source is empty. we add it to the SPBPQueue source.
 * if there is place in the queue and the element is smaller than the current, insert the copy of him before current.
 * if list is empty, insert the copy of SPListElement element as the first element.
 * if there is place in the queue and the element is bigger than the others, insert it in the end.
 * if the queue is full, remove the last one, and insert the element in the right place.
 * 
 * SP_BPQUEUE_FULL if the SPBPQueue source is full (his size is equal to MaxSize) and if
 * the SPListElement element is bigger then the last one. we won't add it to the SPBPQueue source.
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element);

/**
 * spBPQueueDequeu: removes the element with the lowest value.
 *
 * @param SPBPQueue source: The SPBPQueue which we would like to remove from him the element with lowest value.
 *
 * @return
 * SP_BPQUEUE_INVALID_ARGUMENT if a NULL pointer was sent. we won't remove any element.
 * SP_BPQUEUE_EMPTY if the SPBPQueue source is empty. (there is nothing to remove). we won't remove any element.
 * SP_BPQUEUE_SUCCESS if we can remove the element with the lowest value, which is the first one in SPBPQueue source.
 *
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source);

/**
 * spBPQueuePeek: returns a NEW COPY of the element with the lowest value.
 *
* @param SPBPQueue source: the SPBPQueue that his new copy of lowest value element is requested.
*
 * @return
 * NULL if a NULL pointer was sent or if the SPBPQueue is empty or the new copy of the SPBPQueue source
 * element is NULL.
 * otherwise we return the new copy of the lowest value element in the SPBPQueue source..
 */
SPListElement spBPQueuePeek(SPBPQueue source);

/**
 * spBPQueuePeekLast: returns a new copy of the element with the highest value.
 *
 * @param SPBPQueue source: the SPBPQueue that his new copy of highest value element is requested.
 *
 * @return
 * NULL if a NULL pointer was sent or if the SPBPQueue is empty or the new copy of the SPBPQueue source
 * element is NULL.
 * otherwise we return the new copy of the highest value element in the SPBPQueue source.
 */

SPListElement spBPQueuePeekLast(SPBPQueue source);

/**
 * spBPQueueMinValue: returns the minimum value in the queue.
 *
 * @param SPBPQueue source: the SPBPQueue that his minimum value element is requested.
 *
 * @return
 * -1.0 if a NULL pointer was sent or if the SPBPQueue is empty.
 * otherwise we return the minimum value element in the SPBPQueue source.
 */
double spBPQueueMinValue(SPBPQueue source);

/**
* spBPQueueMinValue: returns the maximum value in the queue.
*
 * @param SPBPQueue source: the SPBPQueue that his maximum value element is requested.
 *
 * @return
 * -1.0 if a NULL pointer was sent or if the SPBPQueue is empty.
 * otherwise we return the maximum value element in the SPBPQueue source.
 */

double spBPQueueMaxValue(SPBPQueue source);

/**
 * spBPQueueIsEmpty: returns true if the queue is empty.
 * @param SPBPQueue source: the SPBPQueue that we check if it is empty, which means: if it's
 *
 * size is equal to zero or bigger than zero.
 *
 * @return
 * false if a NULL pointer was sent.
 * otherwise we return true if SPBPQueue source's size is equal to zero, if the size is bigger than zero
 * we return false. (it means that the SPBPQueue contains at least one element).
 */

bool spBPQueueIsEmpty(SPBPQueue source);

/**
 * spBPQueueIsEmpty: returns true if the queue is full.
 *
 * @param SPBPQueue source: the SPBPQueue that we check if it is full, which means: if it's
 * size is equal to MaxSize or not.
 *
 * @return
 * false if a NULL pointer was sent.
 * otherwise we return true if SPBPQueue source's size is equal to MaxSize,
 * if the size is not equal to MaxSize  we return false. (it means that the SPBPQueue contains
 *  less than the MaxSize number of elements).
 */

bool spBPQueueIsFull(SPBPQueue source);

#endif
