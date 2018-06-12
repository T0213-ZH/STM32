#include "queue.h"
#include "../../hal/header.h"

#include <stdio.h>
#include <stdlib.h>

struct element{
	int num;
	
	LIST_ENTRY(element) elements;
};

void list_test(void){

	LIST_HEAD(listh, element) head;
	struct element *nl, *np;
	
	int i=0;
	
	LIST_INIT(&head);
	for(i=0; i<4; i++){
		
		nl = (struct element*) malloc (sizeof(struct element));
		nl->num = i;
		
		LIST_INSERT_HEAD(&head, nl, elements);
	}
	
	for(np=head.lh_first; np != NULL; np=np->elements.le_next){
		printf("%d  ", np->num);
	}	
}



struct QUEUE_ITEM{

	int value;
	TAILQ_ENTRY(QUEUE_ITEM) entries;
};

void queue_test(void){

	TAILQ_HEAD(, QUEUE_ITEM) head;	
	struct QUEUE_ITEM *item;
	struct QUEUE_ITEM *temp_item;
	
	int i;
	
	TAILQ_INIT(&head);
	for(i=0; i<4; i++){
		item = (struct QUEUE_ITEM *) malloc (sizeof(struct QUEUE_ITEM));
		item->value = i;
		TAILQ_INSERT_TAIL(&head, item, entries);
	}
		
	for(temp_item = TAILQ_FIRST(&head); temp_item != NULL; temp_item = TAILQ_NEXT(temp_item, entries)){
		printf("%d  ", temp_item->value);
	}
}
