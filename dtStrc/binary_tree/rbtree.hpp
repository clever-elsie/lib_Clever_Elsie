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

struct null_t{
	null_t(){}
	friend bool operator<(const null_t&lhs,const null_t&rhs){ return true; }
	friend bool operator==(const null_t&lhs,const null_t&rhs){ return true; }
};

template<class key_t,class val_t,class Cmp=less<key_t>,bool allow_duplicate_keys=false>
class rbtree{
	protected:
	struct node{
		using ip=node*;
		ip l,r,p;
		uint64_t time,size; // time: odd red, even black
		key_t key;
		val_t val;
		node(){}
		node(ip nil):key(),val(),time(0),size(1),l(nil),r(nil),p(nil){}
		node(key_t&&k,val_t&&v,uint64_t tm,ip P,ip L,ip R):key(move(k)),val(move(v)),time(tm),size(1),p(P),l(L),r(R){}
		node(key_t&&k,const val_t&v,uint64_t tm,ip P,ip L,ip R):key(move(k)),val(v),time(tm),size(1),p(P),l(L),r(R){}
		node(const key_t&k,val_t&&v,uint64_t tm,ip P,ip L,ip R):key(k),val(move(v)),time(tm),size(1),p(P),l(L),r(R){}
		node(const key_t&k,const val_t&v,uint64_t tm,ip P,ip L,ip R):key(k),val(v),time(tm),size(1),p(P),l(L),r(R){}
		friend auto operator<=>(const node&lhs,const node&rhs){
			Cmp cmp;
			bool L=cmp(lhs.key,rhs.key),R=cmp(rhs.key,lhs.key);
			if(L==R)return lhs.time<=>rhs.time;
			return L?strong_ordering::less:strong_ordering::greater;
		}
	};
	using np=node*;
	using vp=pair<key_t,val_t>;
	public:
	using value_type=pair<const key_t,val_t>;

	protected:
	constexpr static uint64_t unit_time=2;
	constexpr static uint64_t filter_red=1;
	constexpr static uint64_t filter_black=0xFFFF'FFFF'FFFF'FFFEul;
	size_t cur_size,time;
	np root,nil,unused;
	Cmp cmp;
	public:
	rbtree():cur_size(0),time(0),unused(nullptr){
		nil=new node(nullptr);
		nil->p=nil->l=nil->r=nil;
		nil->size=0;
		root=nil;
	}
	//~rbtree(){ clear(); }
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
				return (const key_t&)(n->key);
			else return pair<const key_t&,val_t&>(n->key,n->val);
		}
		bool operator==(const iterator&itr){ return this->n==itr.n; }
		bool operator!=(const iterator&itr){ return this->n!=itr.n; }
		const key_t&f(){return n->key;}
		val_t&s(){return n->val;}
		friend class rbtree;
		protected:
		np n;
		rbtree*tree;
	};
	struct reverse_iterator:public iterator{
		reverse_iterator(rbtree*t,np node):iterator(t,node){}
		reverse_iterator&operator++(){
			this->n=this->tree->prev(this->n);
			return *this;
		}
		reverse_iterator&operator--(){
			this->n=this->tree->next(this->n);
			return*this;
		}
		reverse_iterator operator++(int32_t){
			reverse_iterator cur=*this;
			this->n=this->tree->prev(this->n);
			return cur;
		}
		reverse_iterator operator--(int32_t){
			reverse_iterator cur=*this;
			this->n=this->tree->next(this->n);
			return cur;
		}
	};
	protected:
	np lower_bound(const node&tar)const{
		np cur=root,res=nil;
		while(cur!=nil){
			auto s=*cur<=>tar;
			if(s<0)cur=cur->r;
			else if(s>0){
				res=cur;
				cur=cur->l;
			}else return cur;
		}
		return res;
	}
	np upper_bound(const node&tar)const{
		np cur=root,res=nil;
		while(cur!=nil){
			auto s=*cur<=>tar;
			if(s<=0)cur=cur->r;
			else if(s>0){
				res=cur;
				cur=cur->l;
			}
		}
		return res;
	}
	np ordered_access(size_t idx){
		np cur=root;
		while(cur!=nil){
			size_t L=cur->l->size;
			if(L==idx)return cur;
			if(L>idx)cur=cur->l;
			else {
				cur=cur->r;
				idx-=L+1;
			}
		}
		return nil;
	}
	size_t order_of_node(np p){
		size_t R=0;
		if(p==nil)return cur_size;
		if(p->l!=nil)R+=p->l->size;
		while(p->p!=nil){
			if(p==p->p->r){
				++R;
				if(p->p->l!=nil)R+=p->p->l->size;
			}
			p=p->p;
		}
		return R;
	}
	np minimum(np p){
		if(!p||p==nil)return nil;
		for(;p&&p->l!=nil;p=p->l);
		return p;
	}
	np maximum(np p){
		if(!p||p==nil)return nil;
		for(;p&&p->r!=nil;p=p->r);
		return p;
	}
	np next(np x){
		if(x==nil)return minimum(root);
		if(x->r!=nil)return minimum(x->r);
		while(1){
			if(x==root)return nil;
			np y=x;
			x=x->p;
			if(x->l==y)break;
		}
		return x;
	}
	np prev(np x){
		if(x==nil)return maximum(root);
		if(x->l!=nil)return maximum(x->l);
		while(1){
			if(x==root)return nil;
			np y=x;
			x=x->p;
			if(x->r==y)break;
		}
		return x;
	}
	iterator find_wrapper(np p){
		np lb=lower_bound(*p);
		if(lb!=nil&&cmp(lb->key,p->key)==cmp(p->key,lb->key))
			return iterator(this,lb);
		else return iterator(this,nil);
	}
	iterator count_wrapper(auto&&key){
		if constexpr(allow_duplicate_keys){
			node P(forward(key),val_t(),0,nil,nil,nil);
			size_t R=order_of_key(lower_bound(P));
			P->time=UINT64_MAX;
			return R-order_of_key(lower_bound(P));
		}
		return nil!=find(key);
	}
	public:
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
		return iterator(this,lower_bound(tar));
	}
	iterator lower_bound(key_t&&key){
		node tar(move(key),val_t(),0,nil,nil,nil);
		return iterator(this,lower_bound(tar));
	}
	iterator upper_bound(const key_t&key){
		node tar(key,val_t(),UINT64_MAX,nil,nil,nil);
		return iterator(this,upper_bound(tar));
	}
	iterator upper_bound(key_t&&key){
		node tar(move(key),val_t(),UINT64_MAX,nil,nil,nil);
		return iterator(this,upper_bound(tar));
	}
	iterator find_by_order(int64_t idx){
		if(idx>=(int64_t)cur_size||-idx>(int64_t)cur_size) return iterator(this,nil);
		return iterator(this,ordered_access(idx>=0?idx:cur_size+idx));
	}
	size_t order_of_key(const key_t&key){
		node tar(key,val_t(),0,nil,nil,nil);
		return order_of_node(lower_bound(tar));
	}
	size_t order_of_key(key_t&&key){
		node tar(move(key),val_t(),0,nil,nil,nil);
		return order_of_node(lower_bound(tar));
	}
	size_t count(const key_t&key){ return find_wrapper(key); }
	size_t count(key_t&&key){ return find_wrapper(move(key)); }
	bool contains(const key_t&key){ return end()!=find(key); }
	protected:
	void calc_size(np x){
		x->size=1+x->l->size+x->r->size;
	}
	void left_rotation(np x){
		np y=x->r;
		x->r=y->l,y->p=x->p;
		if(y->l!=nil)y->l->p=x;
		if(x->p==nil)root=y;
		else if(x==x->p->l)x->p->l=y;
		else x->p->r=y;
		y->l=x,x->p=y;
		y->size=x->size;
		calc_size(x);
	}
	void right_rotation(np x){
		np y=x->l;
		x->l=y->r,y->p=x->p;
		if(y->r!=nil)y->r->p=x;
		if(x->p==nil)root=y;
		else if(x==x->p->l)x->p->l=y;
		else x->p->r=y;
		y->r=x,x->p=y;
		y->size=x->size;
		calc_size(x);
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
	iterator insert_wrapper(vp&&t){
		auto&[first,second]=t;
		np x=root,y=nil;
		while(x!=nil){
			y=x;
			bool L=cmp(first,x->key);
			bool R=cmp(x->key,first);
			if(L==R)
				if constexpr(allow_duplicate_keys) x=x->r;
				else{
					if constexpr(!is_same_v<val_t,null_t>)
						x->val=move(second);
					return iterator(this,x);
				}
			else x=L?x->l:x->r;
		}
		cur_size+=1;
		time+=unit_time;
		np z;
		if(unused==nullptr) z=new node(move(first),move(second),time|filter_red,y,nil,nil);
		else{
			z=unused;
			unused=unused->p;
			z->key=move(first),z->val=move(second);
			z->time=time|filter_red,z->l=z->r=nil;
			z->p=y,z->size=1;
		}
		if(y==nil) root=z;
		else if(*z<*y) y->l=z;
		else y->r=z;
		for(np s=y;s!=nil;s=s->p)
			s->size+=1;
		rb_insert_fixup(z);
		return iterator(this,z);
	}
	public:
	iterator insert(value_type&&val){return insert_wrapper(move(val));}
	iterator insert(const value_type&val){return insert_wrapper(vp(val));}
	iterator insert(const key_t&key,const val_t&val){return insert_wrapper(vp(key,val));}
	iterator insert(const key_t&key,val_t&&val){return insert_wrapper(vp(key,move(val)));}
	iterator insert(key_t&&key,const val_t&val){return insert_wrapper(vp(move(key),val));}
	iterator insert(key_t&&key,val_t&&val){return insert_wrapper(vp(move(key),move(val)));}
	protected:
	void rb_delete_fixup(np x){
		while(x!=root&&!(x->time&filter_red)){
			if(x==x->p->l){
				np w=x->p->r;
				if(w->time&filter_red){
					w->time&=filter_black,x->p->time|=filter_red;
					left_rotation(x->p);
					w=x->p->r;
				}
				if(!((w->l->time&filter_red)||(w->r->time&filter_red))) //w.l=w.r=black
					w->time|=filter_red,x=x->p;
				else{
					if(!(w->r->time&filter_red)){
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
				if(!((w->r->time&filter_red)||(w->l->time&filter_red)))
					w->time|=filter_red,x=x->p;
				else{
					if(!(w->l->time&filter_red)){
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
		};
		np x,y=z,w=z->p;
		bool y_was_red=y->time&filter_red;
		if(z->l==nil) transplant(z,x=z->r);
		else if(z->r==nil) transplant(z,x=z->l);
		else{
			for(y=z->r;1;y=y->l){
				y->size-=1;
				if(y->l==nil)break;
			}
			y_was_red=y->time&filter_red;
			x=y->r;
			if(y!=z->r){
				transplant(y,y->r);
				y->r=z->r,y->r->p=y;
			}else x->p=y;
			transplant(z,y);
			y->l=z->l;
			y->l->p=y;
			if(z->time&filter_red)y->time|=filter_red;
			else y->time&=filter_black;
			calc_size(y);
		}
		if(w!=nil)
		for(;w!=nil;w=w->p)
			w->size-=1;
		if(!y_was_red)rb_delete_fixup(x);
		z->p=unused;
		unused=z;
	}
	public:
	iterator erase(iterator&itr){
		if(itr.n!=nil){
			cur_size-=1;
			np nx=next(itr.n);
			erase(itr.n);
			itr.n=nx;
			return itr;
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
		root=nil;
		unused=nullptr;
		vector<np>st;
		st.reserve(cur_size);
		np c=minimum(root);
		while(c!=nil){
			st.push_back(c);
			c=next(c);
		}
		for(const auto&x:st){
			x->p=unused;
			unused=x;
		}
	}
	bool empty(){return cur_size==0;}
	size_t size(){return cur_size;}
	// iterator
	iterator begin(){ return iterator(this,cur_size?minimum(root):nil); }
	iterator end(){ return iterator(this,nil); }
	reverse_iterator rbegin(){ return reverse_iterator(this,cur_size?maximum(root):nil); }
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

template<class val_t>
class vector:public rbtree<null_t,val_t,less<null_t>,true>{
	protected:
	using super=rbtree<null_t,val_t,less<null_t>,true>;
	typename super::iterator insert_key_null(size_t idx,val_t&&t){
		if(super::cur_size==0)return super::insert_wrapper(typename super::vp(null_t(),move(t)));
		else{
			super::cur_size+=1;
			super::time+=super::unit_time;
			typename super::np y,z;
			if(super::unused==nullptr)z=new super::node(super::nil);
			else{
				z=super::unused;
				super::unused=z->p;
			}
			z->val=move(t),z->l=super::nil,z->r=super::nil;
			z->time=super::time|super::filter_red;
			if(super::cur_size-1<=idx){
				y=super::maximum(super::root);
				z->p=y,y->r=z;
			}else{
				y=super::ordered_access(idx);
				if(y->l!=super::nil){
					y->l->p=z;
					z->l=y->l;
				}
				z->p=y,y->l=z;
			}
			for(typename super::np s=z;s!=super::nil;s=s->p)
				super::calc_size(s);
			super::rb_insert_fixup(z);
			return typename super::iterator(this,z);
		}
	}
	public:
	using iterator=super::iterator;
	iterator insert(int64_t idx,const val_t&v){ return insert_key_null(idx<0?super::cur_size+idx:idx,v); }
	iterator insert(int64_t idx,val_t&&v){ return insert_key_null(idx<0?super::cur_size+idx:idx,move(v)); }
	iterator erase(int64_t idx){ return super::erase(super::find_by_order(idx)); }
	val_t& get(int64_t idx){ return super::find_by_order(idx).s(); }
};

}
#endif