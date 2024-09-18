#ifndef CLEVER_ELSIE_VECTOR
#define CLEVER_ELSIE_VECTOR
#include<assert.h>
#include<stdlib.h>
#include<stddef.h>
typedef int vdata_t;

typedef struct vector {
	vdata_t*ptr;
	size_t _size,_max_size;
} vector;

void v_init(vector*);
void v_free(vector*);
void v_alloc(vector*,size_t);
void v_clear(vector*);

void v_resize(vector*,size_t);
void v_reserve(vector*,size_t);
void v_shrink_to_fit(vector*,size_t);

int v_empty(vector*);
size_t v_size(vector*);
size_t v_max_size(vector*);
size_t v_capacity(vector*);

vdata_t* v_at(vector*,size_t);
vdata_t* v_front(vector*);
vdata_t* v_back(vector*);
void v_push_back(vector*,vdata_t);
void v_pop_back(vector*);
#endif