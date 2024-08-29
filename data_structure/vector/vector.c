#include"vector.h"

void v_init(vector*v){ v_alloc(v,0); }
void v_free(vector*v){ free(v->ptr); }
void v_alloc(vector*v,size_t size) {
	static int vector_init_flag=0;
	if(vector_init_flag){
		if(size<=v->_size||size<=v->_max_size) v->_size=size;
		else{
			size_t new_size=v->_max_size;
			while(new_size<=size)new_size*=2;
			v_reserve(v,new_size);
			v->_size=size;
		}
	}else{
		v->_size=size;
		v->_max_size=0;
		v_reserve(v,size>1024?size*2:1024);
		vector_init_flag=1;
	}
}
void v_clear(vector*v){ v->_size=0; }

void v_resize(vector*v,size_t size) {
	vdata_t*new_ptr=(vdata_t*)realloc(v->ptr,size);
	assert(new_ptr!=NULL);
	if(new_ptr!=v->ptr)v->ptr=new_ptr;
	if(v->_size>size) v->_size=size;
	v->_max_size=size;
}
void v_reserve(vector*v,size_t size){ if(v->_max_size<size+v->_size) v_resize(v,v->_size+size); }
void v_shrink_to_fit(vector*v,size_t size){ if(v->_max_size>size+v->_size) v_reserve(v,v->_size+size); }

int v_empty(vector *v) { return (v->_size == 0 ? 0 : 1); }
size_t v_size(vector *v) { return v->_size; }
size_t v_max_size(vector *v) { return v->_max_size; }
size_t v_capacity(vector *v) { return v->_max_size - v->_size; }

vdata_t* v_at(vector *v, size_t i) { return (v->_size <= i ? NULL : &v->ptr[i]); }
vdata_t* v_front(vector*v){return (v->_size?&v->ptr[0]:NULL);}
vdata_t* v_back(vector*v){return (v->_size?&v->ptr[v->_size-1]:NULL);}
void v_push_back(vector*v,vdata_t val){
	if(v->_max_size-v->_size==0) v_resize(v,v->_max_size*2);
	v->ptr[v->_size++]=val;
}
void v_pop_back(vector*v){if(v->_size)v->_size--;}