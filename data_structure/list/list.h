#ifndef CLEVER_ELSIE_LIST
#define CLEVER_ELSIE_LIST
#include<stddef.h>
#include<assert.h>
#include<stdlib.h>
typedef int ldata_t;

typedef struct list_node{
	ldata_t val;
	struct list_node *prev,*next;
}list_node;

typedef list_node* list_iterator;

typedef struct list{
	list_iterator head,tail;
	size_t _size;
}list;

void l_init(list*);
void l_clear(list*);
void l_resize(list*,size_t);

int l_empty(list*);
size_t l_size(list*);

ldata_t l_view(list_iterator);
ldata_t* l_ptr(list_iterator);
ldata_t* l_front(list*);
ldata_t* l_back(list*);
void l_push_front(list*,ldata_t);
void l_pop_front(list*);
void l_push_back(list*,ldata_t);
void l_pop_back(list*);
list_iterator l_insert(list*l,list_iterator,ldata_t);
list_iterator l_insert_back(list*l,list_iterator,ldata_t);
list_iterator l_begin(list*);
list_iterator l_end(list*);
list_iterator l_prev(list_iterator);
list_iterator l_next(list_iterator);
#endif