#include<any> // swap()
#include<cstdint>
#include<cstddef> // size_t
#include<utility> // forward(), move()
#include<compare>
#include<concepts>
#include<functional> // less<>
#include<type_traits>
#include<initializer_list>
#ifndef ELSIE_RBTREE
#define ELSIE_RBTREE
namespace elsie{
using namespace std;

struct null_t{ null_t(){} };

template<class key_t,class val_t,class cmp=less<key_t>,bool allow_duplicate_keys=false>
class rbtree{
	static_assert(!is_same_v<key_t,null_t>);
	protected:
	struct node{
		using ip=node*;
		ip l,r,p;
		uint64_t time,size; // time: odd red, even black, second bit is reverse flag
		const key_t key;
		val_t val;
		node(ip nil):key(),val(),time(0),size(1),l(nil),r(nil),p(nil){}
		node(key_t&&k,val_t&&v,int32_t tm,ip P,ip L,ip R):key(move(k)),val(move(v)),time(tm),size(0),p(P),l(L),r(R){}
		node(key_t&&k,const val_t&v,int32_t tm,ip P,ip L,ip R):key(move(k)),val(v),time(tm),size(0),p(P),l(L),r(R){}
		node(const key_t&k,val_t&&v,int32_t tm,ip P,ip L,ip R):key(k),val(move(v)),time(tm),size(0),p(P),l(L),r(R){}
		node(const key_t&k,const val_t&v,int32_t tm,ip P,ip L,ip R):key(k),val(v),time(tm),size(0),p(P),l(L),r(R){}
		auto operator<=>(const node&other){
			bool L=cmp(key,other.key),R=cmp(other.key,key);
			if(L==R)return time<=>other.time;
			return L?strong_ordering::less:strong_ordering::greater;
		}
	};
	using np=node*;
	using vp=pair<key_t,val_t>;
	public:
	using value_type=pair<const key_t,val_t>;

	protected:
	constexpr static uint64_t filter_red=1;
	constexpr static uint64_t filter_black=0xFFFF'FFFF'FFFF'FFFEul;
	constexpr static uint64_t unit_time=2;
	size_t cur_size,time;
	np root,bs,es,nil;

	np lower_bound(const node&tar){
		np cur=root,res=nil;
		while(cur!=nil){
			auto s=*cur<=>tar;
			if(s<=0){
				if(res==nil||s==0) res=cur;
				cur=cur->l;
			}else cur=cur->r;
		}
		return cur;
	}
	np ordered_access(size_t idx){
		np cur=root;
		while(cur!=nil){
			size_t L=1;
			if(cur->l!=nil)
				L+=cur->l;
			if(L==idx)return cur;
			if(L>idx)cur=cur->l;
			else {
				cur=cur->r;
				idx-=L;
			}
		}
		return nil;
	}
	size_t order_of_node(np p){
		size_t R=0;
		while(p!=nil){
			R+=1+(p->l!=nil?p->l->size:0);
			p=p->p;
		}
		return R;
	}
	public:
	np minimum(np p){
		for(;p->l!=nil;p=p->l);
		return p;
	}
	np maximum(np p){
		for(;p->r!=nil;p=p->r);
		return p;
	}
	np next(np x){
		if(x->r!=nil)return maximum(x->r);
		while(1){
			if(x==root)return nil;
			np y=x;
			x=x->p;
			if(x->l==y)break;
		}
		return x;
	}
	np prev(np x){
		if(x->l!=nil)return minimum(x->l);
		while(1){
			if(x==root)return nil;
			np y=x;
			x=x->p;
			if(x->r==y)break;
		}
	}
	protected:
	void calc_size(np x){ x->size=1+(x->l!=nil?x->l->size:0)+(x->r!=nil?x->r->size:0); }
	np left_rotation(np x){
		np y=x->r;
		x->r=y->l,y->p=x->p;
		if(y->l!=nil)y->l->p=x;
		if(x->p==nil)root=y;
		else if(x==x->p->l)x->p->l=y;
		else x->p->r=y;
		y->l=x,x->p=y;
		calc_size(x);
		calc_size(y);
	}
	np right_rotation(np x){
		np y=x->l;
		x->l=y->r,y->p=x->p;
		if(y->r!=nil)y->r->p=x;
		if(x->p==nil)root=y;
		else if(x==x->p->l)x->p->l=y;
		else x->p->r=y;
		y->r=x,x->p=y;
		calc_size(x);
		calc_size(y);
	}
	void rb_insert_fixup(np z){
		while(z->p->time&filter_red){
			if(z->p==z->p->p->l){
				if(np y=z->p->p->r;y->time&filter_red){
					z->p->time&=filter_black,y->time&=filter_black;
					z->p->p->time|=filter_red,z=z->p->p;
				}else{
					if(z==z->p->r)left_rotation(z=z->p);
					z->p->time&=filter_black,z->p->p->time|=filter_red;
					right_rotation(z->p->p);
				}
			}else{
				if(np y=z->p->p->l;y->time&filter_red){
					z->p->time&=filter_black,y->time&=filter_black;
					z->p->p->time|=filter_red,z=z->p->p;
				}else{
					if(z==z->p->l)right_rotation(z=z->p);
					z->p->time&=filter_black,z->p->p->time|=filter_red;
					left_rotation(z->p->p);
				}
			}
		}
		root->time&=filter_black;
	}
	np insert(np z){
		np x=root,y=nil;
		while(x!=nil){
			y=x;
			if(*z<*x)x=x->l;
			else x=x->r;
		}
		if constexpr(!allow_duplicate_keys)
			if(cmp(y->key,z->key)==cmp(z->key,y->key)){
				if constexpr(!is_same_v<val_t,null_t>)
					y->val=z->val;
				return y;
			}
		z->p=y;
		if(y==nil){
			root=y;
		}else if(*z<*y) y->l=z;
		else y->r=z;
		rb_insert_fixup(z);
		return z;
	}

	void rb_delete_fixup(np x){
		while(x!=root&&(x->time&filter_red)==0){
			if(x==x->p->l){ // x is left child
				np w=x->p->r;
				if(w->time&filter_red){
					w->time&=filter_black,x->p->time|=filter_red;
					left_rotation(x->p);
					w=x->p->r;
				}
				if((w->l->time&filter_red)==0&&(w->r->time&filter_red)==0)
					w->time|=filter_red,x=x->p;
				else{
					if((w->r->time&filter_red)==0){
						w->l->time&=filter_black,w->time|=filter_red;
						right_rotation(w);
						w=x->p->r;
					}
					if(x->p->time&filter_red)w->time|=filter_red;
					else w->time&=filter_black;
					x->p->time&=filter_black,w->r->time&=filter_black;
					left_rotation(x->p);
					x=root;
				}
			}else{ // x is right child
				np w=x->p->l;
				if(w->time&filter_red){
					w->time&=filter_black,x->p->time|=filter_red;
					right_rotation(x->p);
					w=x->p->l;
				}
				if((w->r->time&filter_red)==0&&(w->l->time&filter_red)==0)
					w->time|=filter_red,x=x->p;
				else{
					if((w->l->time&filter_red)==0){
						w->r->time&=filter_black,w->time|=filter_red;
						left_rotation(w);
						w=x->p->l;
					}
					if(x->p->time&filter_red)w->time|=filter_red;
					else w->time&=filter_black;
					x->p->time&=filter_black,w->l->time&=filter_black;
					right_rotation(x->p);
					x=root;
				}
			}
		}
		x->time&=filter_black;
	}

	void erase(np z){
		auto transplant=[&](np u,np v){
			if(u->p==nil)root=v;
			else if(u==u->p->l)u->p->l=v;
			else u->p->r=v;
			v->p=u->p;
			calc_size(v);
			calc_size(v->p);
		};
		np x,y=z,w=nil,v=nil;
		bool y_was_red=y->time&filter_red;
		if(z->l==nil){
			transplant(z,x=z->r);
		}else if(z->r==nil){
			transplant(z,x=z->l);
		}else{
			for(y=y->r;y->l!=nil;y=y->l)
				y->size-=1;
			y_was_red=y->time&filter_red;
			x=y->r;
			if(y!=z->r){
				w=y->p;
				transplant(y,y->r);
				y->r=z->r,y->r->p=y;
			}else x->p=y;
			transplant(z,y);
			y->l=z->l;
			y->l->p=y;
			v=y;
			if(z->time&filter_red)y->time|=filter_red;
			else y->time&=filter_black;
		}
		for(np s=x;1;s=s->p){
			calc_size(s);
			if(s==root)break;
		}
		if(!y_was_red)rb_delete_fixup(x);
		if(z->p!=nil) z->p->size-=1;
		delete z;
	}
	public://iterator
	struct iterator{
		iterator(rbtree*t,np node):tree(t),n(node){}
		iterator&operator=(const iterator&itr){
			n=itr.n;
			tree=itr.tree;
			return *this;
		}
		iterator&operator++(){
			this->n=this->tree->next(this->n);
			return *this;
		}
		iterator operator++(int32_t){
			iterator cur=*this;
			this->n=this->tree->next(this->n);
			return cur;
		}
		iterator&operator--(){
			this->n=this->tree->prev(this->n);
			return*this;
		}
		iterator operator--(int32_t){
			iterator cur=*this;
			this->n=this->tree->prev(this->n);
			return cur;
		}
		auto operator*(){
			if constexpr(is_same_v<val_t,null_t>)
				return (const key_t&)n->key;
			else return pair<const key_t&,val_t&>(n->key,n->val);
		}
		bool operator==(const iterator&itr){ return this->n==itr.n; }
		bool operator!=(const iterator&itr){ return this->n!=itr.n; }
		const key_t&f(){return n->key;}
		val_t&s(){return n->val;}
		protected:
		np n;
		rbtree*tree;
	};

	struct reverse_iterator:public iterator{
		reverse_iterator(rbtree*t,np node):iterator(t,node){}
		iterator&operator++(){ return *this=this->tree->prev(this->n); }
		iterator operator++(int32_t){
			iterator cur=*this;
			*this=this->tree->prev(this->n);
			return cur;
		}
		iterator&operator--(){ return *this=this->tree->next(this->n); }
		iterator operator--(int32_t){
			iterator cur=*this;
			*this=this->tree->next(this->n);
			return cur;
		}
	};
	protected:// wrapper
	// auto -> const value_type& or value_type&&
	iterator insert_wrapper(auto&&x){
		time+=unit_time;
		cur_size+=1;
		if constexpr(allow_duplicate_keys)
			return iterator(this,insert(new node(forward(x.first),forward(x.second),time|filter_red,nil,nil,nil)));
		else{
			node p=new node(forward(x.first),forward(x.second),0,nil,nil,nil);
			np lb=lower_bound(&p);
			if(cmp(x.first,lb->key)==cmp(lb->key,x.first)){
				cur_size-=1;
				lb->val=move(p->val);
				delete p;
			}else{
				p->time=time|filter_red;
				lb=insert(p);
			}
			return iterator(this,lb);
		}
	}
	iterator find_wrapper(np p){
		np lb=lower_bound(p);
		if(cmp(lb->key,p->key)==cmp(p->key,lb->key))
			return iterator(this,lb);
		else return iterator(this,nil);
	}
	iterator count_wrapper(auto&&key){
		if constexpr(allow_duplicate_keys){
			node P(forward(key),val_t(),0,nil,nil,nil);
			size_t R=order_of_key(lower_bound(&P));
			P->time=UINT64_MAX;
			return R-order_of_key(lower_bound(&P));
		}
		return nil!=find(key);
	}
	public:
	// fundamental manipulation
	rbtree():cur_size(0),time(0){
		nil=new node(nullptr);
		root=nil;
	}
	// access
	iterator find(const key_t&key){
		node tar(key,val_t(),0,nil,nil,nil);
		return find_wrapper(&tar);
	}
	iterator find(key_t&&key){
		node tar(move(key),val_t(),0,nil,nil,nil);
		return find_wrapper(&tar);
	}
	iterator lower_bound(const key_t&key){
		node tar(key,val_t(),0,nil,nil,nil);
		return iterator(this,lower_bound(&tar));
	}
	iterator lower_bound(key_t&&key){
		node tar(move(key),val_t(),0,nil,nil,nil);
		return iterator(this,lower_bound(&tar));
	}
	iterator upper_bound(const key_t&key){
		node tar(key,val_t(),UINT64_MAX,nil,nil,nil);
		return iterator(this,lower_bound(&tar));
	}
	iterator upper_bound(key_t&&key){
		node tar(move(key),val_t(),UINT64_MAX,nil,nil,nil);
		return iterator(this,lower_bound(&tar));
	}
	iterator find_by_order(int64_t idx){
		if(idx>=cur_size||-idx>cur_size)return iterator(this,nil);
		return iterator(this,ordered_access(idx>=0?idx:cur_size+idx));
	}
	size_t order_of_key(const key_t&key){
		node tar(key,val_t(),0,nil,nil,nil);
		return order_of_node(lower_bound(&tar));
	}
	size_t order_of_key(key_t&&key){
		node tar(move(key),val_t(),0,nil,nil,nil);
		return order_of_node(lower_bound(&tar));
	}
	size_t count(const key_t&key){ return find_wrapper(key); }
	size_t count(key_t&&key){ return find_wrapper(move(key)); }
	bool contains(const key_t&key){ return nil!=find(key); }
	// change
	iterator insert(value_type&&val){return insert_wrapper(move(val));}
	iterator insert(const value_type&val){return insert_wrapper(val);}
	iterator insert(const key_t&key,const val_t&val){return insert_wrapper(vp(key,val));}
	iterator insert(const key_t&key,val_t&&val){return insert_wrapper(vp(key,move(val)));}
	iterator insert(key_t&&key,const val_t&val){return insert_wrapper(vp(move(key),val));}
	iterator insert(key_t&&key,val_t&&val){return insert_wrapper(vp(move(key),move(val)));}
	iterator erase(iterator&itr){
		if(itr!=nil){
			cur_size-=1;
			np nx=next(itr.n);
			erase(itr.n);
			return itr.n=nx;
		}
		return iterator(this,nil);
	}
	iterator erase(iterator&&itr){ return erase(itr); }
	iterator erase(const key_t&key){ return erase(find(key)); }
	iterator erase(key_t&&key){ return erase(find(move(key))); }
	// memory
	void clear(){
		if(root==nil)return;
		time=0;
		vector<np>st;
		np c=minimum(root);
		while(c!=nil){
			st.push_back(c);
			c=next(c);
		}
		for(auto&x:st)delete x;
	}
	bool empty(){return cur_size==0;}
	size_t size(){return cur_size;}
	// iterator
	iterator begin(){
		if(cur_size) return iterator(this,minimum(root));
		return iterator(this,nil);
	}
	iterator end(){ return iterator(this,nil); }
	reverse_iterator rbegin(){
		if(cur_size) return reverse_iterator(this,maximum(root));
		return iterator(this,nil);
	}
	reverse_iterator rend(){ return reverse_iterator(this,nil); }
};

template<class key_t,class cmp=less<key_t>,bool allow_duplicate_keys=true>
class multiset:public rbtree<key_t,null_t,cmp,allow_duplicate_keys>{
	using super=rbtree<key_t,null_t,cmp,allow_duplicate_keys>;
	public:
	using iterator=typename super::iterator;
	const key_t&operator[](int64_t idx){ return *super::find_by_order(idx); }
	iterator insert(const key_t&key){ return super::insert(key,null_t()); }
	iterator insert(key_t&&key){ return super::insert(move(key),null_t()); }
};

template<class key_t,class cmp=less<key_t>>
using set=multiset<key_t,cmp,false>;

template<class key_t,class val_t,class cmp=less<key_t>>
using multimap=rbtree<key_t,val_t,cmp,true>;

template<class key_t,class val_t,class cmp=less<key_t>>
class map:public rbtree<key_t,val_t,cmp,false>{
	public:
	using iterator=typename rbtree<key_t,val_t,cmp,false>::iterator;
	val_t&operator[](const key_t&key){
		iterator p=this->find(key);
		if(p.n!=this->nil)return p.n->val;
		return this->emplace(key,val_t()).n->val;
	}
};

}
#endif