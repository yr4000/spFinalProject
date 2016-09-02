/*
 * SPBPriorityQueue.c
 *
 *  Created on: 24 במאי 2016
 *      Author: Yair
 */

#include <stdio.h>
#include <stdlib.h>
#include "SPBPriorityQueue.h"
#include "SPList.h"
#include "SPListElement.h"

struct sp_bp_queue_t {
	int maxSize;
	SPList myList;
};

SPBPQueue spBPQueueCreate(int maxSize){
	if(maxSize<0){
		return NULL;
	}
	SPBPQueue new = (SPBPQueue)malloc(sizeof(*new));
	if(new==NULL){
		return NULL;
	}
	new->maxSize=maxSize;
	new->myList = spListCreate();
	if(new->myList==NULL){
		return NULL;
	}
	return new;
}

SPBPQueue spBPQueueCopy(SPBPQueue source){
	if(source==NULL){
		return NULL;
	}
	SPBPQueue copy = (SPBPQueue)malloc(sizeof(*copy));
	if(copy==NULL){
		return NULL;
	}
	copy->maxSize=source->maxSize;
	copy->myList=spListCopy(source->myList);
	if(copy->myList==NULL){
		return NULL;
	}
	return copy;
}

void spBPQueueDestroy(SPBPQueue source){
	if(source!=NULL){
		spListDestroy(source->myList);
		free(source);
	}
}

void spBPQueueClear(SPBPQueue source){
	if(source!=NULL && source->myList!=NULL){
		spListClear(source->myList);
	}
}

int spBPQueueSize(SPBPQueue source){
	if(source==NULL || source->myList==NULL){
		return -1;
	}
	return spListGetSize(source->myList);
}

int spBPQueueGetMaxSize(SPBPQueue source){
	return source==NULL ? -1 : source->maxSize;
}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element){
	if(source==NULL||element==NULL){
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	//insert if list is empty
	if(spBPQueueSize(source)==0){
		spListInsertFirst(source->myList,element);
		return SP_BPQUEUE_SUCCESS;
	}
	//checks if the value of the element is bigger then the last one
	if(spBPQueueGetMaxSize(source)==spBPQueueSize(source) &&
			spListElementCompare(element,source->myList->tail->previous->data)>=0 ){
		return SP_BPQUEUE_FULL;
	}
	//runs over the list elements
	SP_LIST_FOREACH(SPListElement,e2,source->myList){
		//if the element is smaller then the current, insert him before it.
		if(spListElementCompare(element,e2)<0){
			spListInsertBeforeCurrent(source->myList,element);
			break;
		}
		if(source->myList->current->next==source->myList->tail){
			spListInsertLast(source->myList,element);
			break;
		}
	}
	//if the queue is full, remove the last one.
	if(spBPQueueSize(source)>source->maxSize){
		source->myList->current=source->myList->tail->previous; //jump to the last node
		spListRemoveCurrent(source->myList);
	}
	return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source){
	if(source==NULL){
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	if(spBPQueueIsEmpty(source)){
		return SP_BPQUEUE_EMPTY;
	}
	source->myList->current = source->myList->head->next; //current points to the first element
	spListRemoveCurrent(source->myList);
	return SP_BPQUEUE_SUCCESS;
}

SPListElement spBPQueuePeek(SPBPQueue source){
	if(source==NULL) return NULL;
	if(spBPQueueIsEmpty(source)) return NULL;
	SPListElement copyE = spListElementCopy(source->myList->head->next->data);
	if(copyE==NULL) return NULL;
	return copyE;
}

SPListElement spBPQueuePeekLast(SPBPQueue source){
	if(source==NULL) return NULL;
	if(spBPQueueIsEmpty(source)) return NULL;
	SPListElement copyE = spListElementCopy(source->myList->tail->previous->data);
	if(copyE==NULL) return NULL;
	return copyE;
}

double spBPQueueMinValue(SPBPQueue source){
	if(source==NULL) return -1.0;
	if(spBPQueueIsEmpty(source)) return -1.0;
	return spListElementGetValue(source->myList->head->next->data);
}

double spBPQueueMaxValue(SPBPQueue source){
	if(source==NULL) return -1.0;
	if(spBPQueueIsEmpty(source)) return -1.0;
	return spListElementGetValue(source->myList->tail->previous->data);
}

bool spBPQueueIsEmpty(SPBPQueue source){
	if(source==NULL) return false;
	return (spListGetSize(source->myList)==0);
}

bool spBPQueueIsFull(SPBPQueue source){
	if(source==NULL) return false;
	return (spListGetSize(source->myList)==source->maxSize);
}
