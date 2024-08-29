#ifndef CLEVER_ELSIE_HEAP_TREE
#define CLEVER_ELSIE_HEAP_TREE
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

typedef int heap_t;
/* comparison predicate */
	/* minimum heap */
#define HEAP_COMP(a,b) (a)<(b)
	/* maximum heap */
//#define HEAP_COMP(a,b) (a)>(b)

typedef struct heap_node{
	heap_t val;
	struct heap_node *par,*lch,*rch;
	size_t sub_size;
}heap_node;

typedef struct heap_tree{
	heap_node*root;
	size_t _size;
} heap_tree;

void heap_init(heap_tree*);
void heap_free(heap_tree*);
size_t heap_size(heap_tree*);

heap_t heap_top(heap_tree*);
void heap_pop(heap_tree*);
void heap_push(heap_tree*,heap_t);
#endif