#include"list.h"

void l_init(list*l){ l->head=l->tail=NULL, l->_size=0; }
void l_clear(list*l){
	while(l->head!=NULL){
		list_iterator ptr=l->head;
		l->head=l->head->next;
		free(ptr);
	}
	l_init(l);
}
void l_resize(list*l,size_t size){
	while(l->_size>size) l_pop_back(l);
	while(l->_size<size){
		list_node* ptr=(list_node*)malloc(sizeof(list_node));
		ptr->next=NULL;
		ptr->prev=l->tail;
		l->tail->next=ptr;
	}
}

int l_empty(list*l){ return (l->_size?1:0); }
size_t l_size(list*l){ return l->_size; }

ldata_t l_view(list_iterator itr) { return itr->val; }
ldata_t *l_ptr(list_iterator itr) { return &itr->val; }
ldata_t* l_front(list*l){ return &l->head->val; }
ldata_t* l_back(list*l){ return &l->tail->val; }
void l_push_front(list*l,ldata_t val){
	list_node* ptr=(list_node*)malloc(sizeof(list_node));
	ptr->val=val, ptr->prev=NULL, ptr->next=l->head;
	if(l->head!=NULL) l->head->prev=ptr;
	else l->head=l->tail=ptr;
	l->head=ptr;
	l->_size++;
}
void l_pop_front(list*l){
	list_node*ptr=l->head;
	if(ptr==NULL) return;
	l->head=ptr->next;
	free(ptr);
	if(l->head!=NULL)l->head->prev=NULL;
	l->_size--;
}
void l_push_back(list*l,ldata_t val){
	list_node* ptr=(list_node*)malloc(sizeof(list_node));
	ptr->val=val, ptr->prev=l->tail, ptr->next=NULL;
	if(l->tail!=NULL) l->tail->next=ptr;
	else l->tail=l->head=ptr;
	l->tail=ptr;
	l->_size++;
}
void l_pop_back(list*l){
	list_node*ptr=l->tail;
	if(ptr==NULL)return;
	l->tail=ptr->prev;
	free(ptr);
	if(l->tail!=NULL)l->tail->next=NULL;
	l->_size--;
}
list_iterator l_insert(list*l,list_iterator itr,ldata_t val){
	list_node*ptr=(list_node*)malloc(sizeof(list_node));
	ptr->val=val,ptr->next=itr, l->_size++;
	if(itr!=NULL){
		itr->prev->next=ptr;
		ptr->prev=itr->prev;
		itr->prev=ptr;
	} else {
		ptr->prev=NULL;
		if(l->head==NULL) l->head=l->tail=ptr;
		else {
			free(ptr);
			l_push_front(l,val);
			l->_size--;
		}
	}
	return ptr;
	}
list_iterator l_insert_back(list*l,list_iterator itr,ldata_t val){
	list_node*ptr=(list_node*)malloc(sizeof(list_node));
	ptr->val=val,ptr->prev=itr,l->_size++;
	if(itr!=NULL){
		ptr->next=itr->next;
		itr->next->prev=ptr;
		itr->next=ptr;
	}else{
		ptr->next=NULL;
		if(l->head==NULL) l->head=l->tail=ptr;
		else {
			free(ptr);
			l_push_back(l,val);
			l->_size--;
		}
	}
	return ptr;
}
list_iterator l_begin(list*l){ return l->head; }
list_iterator l_end(list*l){ return l->tail; }
list_iterator l_prev(list_iterator itr){
	if(itr==NULL)return NULL;
	return itr->prev;
}
list_iterator l_next(list_iterator itr){
	if(itr==NULL)return NULL;
	return itr->next;
}