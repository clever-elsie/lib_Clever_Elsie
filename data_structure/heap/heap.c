#include"heap.h"

/* internal functions */
void print_ch(heap_node*itr){
	printf(":: %d -> ",itr->val);
	if(itr->lch!=NULL) printf("%d ",itr->lch->val);
	if(itr->rch!=NULL) printf(" %d",itr->rch->val);
	printf("\n");
}
void check_swap(heap_node*par,heap_node*ch,int i){
	const char *ed[]={"swap","swaped"};
	printf("%s %d->%d\n",ed[i],par->val,ch->val);
	print_ch(par);
	print_ch(ch);
}
void swap_nodes(heap_tree*h,heap_node*par,heap_node*ch){
	//check_swap(par,ch,0);
	heap_node*g=par->par;
	if(g==NULL) h->root=ch;
	else{
		if(g->lch==par) g->lch=ch;
		else g->rch=ch;
	}
	heap_node*lch=ch->lch,*rch=ch->rch;
	// 兄弟を自分に繋ぎなおす
	if(ch!=par->lch){
	if(par->lch!=NULL)par->lch->par=ch;
	}else if(par->rch!=NULL) par->rch->par=ch;
	if(lch!=NULL)lch->par=par;
	if(rch!=NULL)rch->par=par;
	ch->par=g;
	ch->lch=par;
	ch->rch=(par->lch!=ch?par->lch:par->rch);
	par->par=ch;
	par->lch=lch,par->rch=rch;
	par->sub_size=(par->lch!=NULL?par->lch->sub_size:0)+(par->rch!=NULL?par->rch->sub_size:0)+1;
	ch->sub_size=(ch->lch!=NULL?ch->lch->sub_size:0)+(ch->rch!=NULL?ch->rch->sub_size:0)+1;
	//check_swap(ch,par,1);
}
heap_node*make_null_node(){
	heap_node*ptr=(heap_node*)malloc(sizeof(heap_node));
	ptr->par=ptr->lch=ptr->rch=NULL;
	ptr->sub_size=1;
	return ptr;
}
heap_node*heap_get_entry(heap_tree*h){
	heap_node*ptr=h->root;
	if(ptr==NULL){
		heap_node*ptr=make_null_node();
		h->root=ptr;
		return ptr;
	}
	ptr->sub_size++;
	while(1){
		if(ptr->lch!=NULL&&ptr->rch!=NULL){
			if(ptr->lch->sub_size<ptr->rch->sub_size)
				ptr=ptr->lch;
			else ptr=ptr->rch;
			continue;
		}
		heap_node*next=make_null_node();
		next->par=ptr;
		if(ptr->lch==NULL) ptr->lch=next;
		else ptr->rch=next;
		ptr=next;
		break;
	}
	return ptr;
}
void back_propagation(heap_tree*h,heap_node*ptr){
	heap_node* par=ptr->par;
	while(par!=NULL){
		if(HEAP_COMP(ptr->val,par->val)){
			swap_nodes(h,par,ptr);
			par=ptr->par;
		}else {
			ptr=par;
			par=par->par;
		}
	}
}
heap_node*heap_get_leaf(heap_tree*h,int subt){
	if(h->root==NULL)return NULL;
	heap_node*leaf=h->root;
	while(leaf->lch!=NULL||leaf->rch!=NULL){
		leaf->sub_size-=subt;
		if(leaf->lch!=NULL&&leaf->rch!=NULL)
			if(leaf->lch->sub_size > leaf->rch->sub_size)
				leaf = leaf->lch;
			else leaf=leaf->rch;
		else leaf=(leaf->lch!=NULL?leaf->lch:leaf->rch);
	}
	return leaf;
}
void front_propagetion(heap_tree*h,heap_node*entry){
	if(entry==NULL) return;
	int bf=(entry->lch==NULL?0:1)+(entry->rch==NULL?0:2);
	if(bf)
	if(bf<3){
		heap_node*ch=(bf==1?entry->lch:entry->rch);
		if(HEAP_COMP(ch->val,entry->val)){
			swap_nodes(h,entry,ch);
			front_propagetion(h,ch);
		}
	}else{
		int lr=(HEAP_COMP(entry->lch->val,entry->rch->val)?1:0);
		heap_node*ch=(lr?entry->lch:entry->rch);
		if(HEAP_COMP(ch->val,entry->val)){
			swap_nodes(h,entry,ch);
			front_propagetion(h,ch);
		}else{
			ch=(lr?entry->rch:entry->rch);
			if(HEAP_COMP(ch->val,entry->val)){
				swap_nodes(h,entry,ch);
				front_propagetion(h,ch);
			}
		}
	}
}
void free_dfs(heap_node*itr){
	if(itr->lch!=NULL)free_dfs(itr->lch);
	if(itr->rch!=NULL)free_dfs(itr->rch);
	free(itr);
}
/* end internal functions */

void heap_init(heap_tree*h){ h->root=NULL;h->_size=0; }
void heap_free(heap_tree*h){
	free_dfs(h->root);
	h->_size=0;
	h->root=NULL;
}
size_t heap_size(heap_tree*h){ return h->_size; }

heap_t heap_top(heap_tree*h){ return (h->root==NULL?(heap_t)0:h->root->val);}

void heap_push(heap_tree*h,heap_t val){
	h->_size++;
	heap_node*entry=heap_get_entry(h);
	entry->val=val;
	back_propagation(h,entry);
}

void heap_pop(heap_tree*h){
	heap_node*leaf=heap_get_leaf(h,1);
	if(leaf==NULL)return;
	h->_size--;
	if(h->root->lch!=NULL||h->root->rch!=NULL){
		// leafをleaf->parから分離
		if(leaf->par->lch==leaf) leaf->par->lch=NULL;
		else leaf->par->rch=NULL;
		leaf->par=NULL;
		// leafにrootの子を付け替え
		if(leaf!=h->root->lch){
			leaf->lch=h->root->lch;
			if(leaf->lch!=NULL) leaf->lch->par=leaf;
		}else leaf->lch=NULL;
		if(leaf!=h->root->rch){
			leaf->rch=h->root->rch;
			if(leaf->rch!=NULL) leaf->rch->par=leaf;
		}else leaf->rch=NULL;
		free(h->root);
		h->root=leaf;
		front_propagetion(h,leaf);
		//back_propagation(h,heap_get_leaf(h,0));
	}else{
		free(leaf);
		h->root=NULL;
	}
}