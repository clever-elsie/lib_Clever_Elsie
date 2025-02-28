#ifndef ELSIE_RBTREE
#define ELSIE_RBTREE
#include<any> // swap()
#include<cstdint>
#include<cstddef> // size_t
#include<utility> // forward(), move()
#include<compare>
#include<concepts>
#include<functional> // less<>
#include<type_traits>
#include<initializer_list>
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
		ip p,ch[2]; // ch0=L, ch1=R
		uint64_t time,size; // time: odd red, even black
		key_t key;
		val_t val;
		node(){}
		node(ip nil):key(),val(),time(0),size(1),p(nil){ ch[0]=ch[1]=nil; }
		node(key_t&&k,val_t&&v,uint64_t tm,ip P,ip L,ip R)
		:key(move(k)),val(move(v)),time(tm),size(1),p(P){ ch[0]=L,ch[1]=R; }
		node(key_t&&k,const val_t&v,uint64_t tm,ip P,ip L,ip R)
		:key(move(k)),val(v),time(tm),size(1),p(P){ ch[0]=L,ch[1]=R; }
		node(const key_t&k,val_t&&v,uint64_t tm,ip P,ip L,ip R)
		:key(k),val(move(v)),time(tm),size(1),p(P){ ch[0]=L,ch[1]=R; }
		node(const key_t&k,const val_t&v,uint64_t tm,ip P,ip L,ip R)
		:key(k),val(v),time(tm),size(1),p(P){ ch[0]=L,ch[1]=R; }
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
		root=nil=new node(nullptr);
		nil->p=nil->ch[0]=nil->ch[1]=nil;
		nil->size=0;
	}
	rbtree(rbtree&&v){ *this=move(v); }
	rbtree(const rbtree&v){ *this=v; }
	template<class T>
	rbtree(initializer_list<T>init)
		requires is_same_v<T,key_t>||is_same_v<T,pair<key_t,val_t>>
		:rbtree(){
		for(auto&&x:init)
			if constexpr (is_same_v<T,key_t>)
				this->insert(move(x),val_t());
			else this->insert(move(x));
	}
	~rbtree(){ reset(); }
	rbtree&operator=(rbtree&&rhs){
		reset();
		cur_size=rhs.cur_size, time=rhs.time;
		root=rhs.root, nil=rhs.nil, unused=rhs.unused;
		cmp=move(rhs.cmp);
		rhs.root=rhs.nil=new node(nullptr);
		rhs.nil->p=nil->ch[0]=nil->ch[1]=nil;
		rhs.nil->size=0;
		return*this;
	}
	rbtree&operator=(const rbtree&rhs){
		clear();
		for(auto itr=rhs.begin();itr!=rhs.end();++itr)
			this->insert(*itr);
		return*this;
	}
	struct iterator{
		iterator(rbtree*t,np node):tree(t),n(node){}
		iterator&operator=(const iterator&itr){
			n=itr.n;
			tree=itr.tree;
			return *this;
		}
		iterator&operator++(){
			this->n=this->tree->prev_next(this->n,true);
			return *this;
		}
		iterator operator++(int32_t){
			iterator cur=*this;
			this->n=this->tree->prev_next(this->n,true);
			return cur;
		}
		iterator&operator--(){
			this->n=this->tree->prev_next(this->n,false);
			return*this;
		}
		iterator operator--(int32_t){
			iterator cur=*this;
			this->n=this->tree->prev_next(this->n,false);
			return cur;
		}
		decltype(auto)operator*(){
			if constexpr(is_same_v<val_t,null_t>)
				return (const key_t&)(n->key);
			else return pair<const key_t&,val_t&>(n->key,n->val);
		}
		bool operator==(const iterator&itr)const{return this->n==itr.n;}
		bool operator!=(const iterator&itr)const{return this->n!=itr.n;}
		const key_t&f()const{return n->key;}
		val_t&s(){return n->val;}
		friend class rbtree;
		protected:
		np n;
		rbtree*tree;
	};
	struct reverse_iterator:public iterator{
		reverse_iterator(rbtree*t,np node):iterator(t,node){}
		reverse_iterator&operator++(){
			this->n=this->tree->prev_next(this->n,false);
			return *this;
		}
		reverse_iterator&operator--(){
			this->n=this->tree->prev_next(this->n,true);
			return*this;
		}
		reverse_iterator operator++(int32_t){
			reverse_iterator cur=*this;
			this->n=this->tree->prev_next(this->n,false);
			return cur;
		}
		reverse_iterator operator--(int32_t){
			reverse_iterator cur=*this;
			this->n=this->tree->prev_next(this->n,true);
			return cur;
		}
	};
	protected:
	np lower_bound(const node&tar)const{
		np cur=root,res=nil;
		while(cur!=nil){
			bool L=cmp(cur->key,tar.key);
			if(L==cmp(tar.key,cur->key)){
				if constexpr(allow_duplicate_keys){
					uint64_t ct=cur->time&filter_black,tt=tar.time&filter_black;
					if(ct==tt)return cur;
					else if(ct<tt)cur=cur->ch[1];
					else res=cur,cur=cur->ch[0];
				}else return cur;
			}else if(L)cur=cur->ch[1];
			else res=cur,cur=cur->ch[0];
		}
		return res;
	}
	np upper_bound(const node&tar)const{
		np cur=root,res=nil;
		while(cur!=nil){
			bool L=cmp(cur->key,tar.key);
			if(L||L==cmp(tar.key,cur->key)){
				if constexpr(allow_duplicate_keys)
					if(uint64_t ct=cur->time&filter_black,tt=tar.time&filter_black;ct>tt){
						res=cur,cur=cur->ch[0];
						continue;
					}
				cur=cur->ch[1];
			}else res=cur,cur=cur->ch[0];
		}
		return res;
	}
	np ordered_access(size_t idx){
		np cur=root;
		while(cur!=nil){
			size_t L=cur->ch[0]->size;
			if(L>idx)cur=cur->ch[0];
			else if(L<idx){
				cur=cur->ch[1];
				idx-=L+1;
			}else return cur;
		}
		return nil;
	}
	size_t order_of_node(np p){
		if(p==nil)return cur_size;
		size_t R=p->ch[0]->size;
		while(p!=root){
			if(p==p->p->ch[1])
				R+=1+p->p->ch[0]->size;
			p=p->p;
		}
		return R;
	}
	//! @param min_max_f: 0 min, 1 max
	np min_max(np p,bool min_max_f)const{
		if(!p||p==nil)return nil;
		for(;p&&p->ch[min_max_f]!=nil;p=p->ch[min_max_f]);
		return p;
	}
	//!@param prev_next_f: 0 prev, 1 next
	np prev_next(np x,bool prev_next_f)const{
		if(x==nil)return min_max(root,!prev_next_f);
		if(x->ch[prev_next_f]!=nil)return min_max(x->ch[prev_next_f],!prev_next_f);
		while(1){
			if(x==root)return nil;
			np y=x;
			x=x->p;
			if(x->ch[!prev_next_f]==y)break;
		}
		return x;
	}
	iterator find_wrapper(np p)const{
		np x=root;
		while(x!=nil){
			if(bool L=cmp(p->key,x->key);L==cmp(x->key,p->key)){
				if constexpr(allow_duplicate_keys){
					uint64_t pt=p->time&filter_black,xt=x->time&filter_black;
					if(pt==xt)break;
					if(pt<xt)x=x->ch[0];
					else x=x->ch[1];
				}else break;
			}else x=x->ch[!L];
		}
		return iterator(this,x);
	}
	iterator count_wrapper(auto&&key)const{
		if constexpr(allow_duplicate_keys){
			node P(forward(key),val_t(),0,nil,nil,nil);
			size_t R=order_of_key(lower_bound(P));
			P->time=UINT64_MAX;
			return R-order_of_key(lower_bound(P));
		}
		return nil!=find(key);
	}
	public:
	iterator find(const key_t&key)const{
		node tar(key,val_t(),0,nil,nil,nil);
		return find_wrapper(&tar);
	}
	iterator find(key_t&&key)const{
		node tar(move(key),val_t(),0,nil,nil,nil);
		return find_wrapper(&tar);
	}
	iterator lower_bound(const key_t&key)const{
		node tar(key,val_t(),0,nil,nil,nil);
		return iterator(this,lower_bound(tar));
	}
	iterator lower_bound(key_t&&key)const{
		node tar(move(key),val_t(),0,nil,nil,nil);
		return iterator(this,lower_bound(tar));
	}
	iterator upper_bound(const key_t&key)const{
		node tar(key,val_t(),UINT64_MAX,nil,nil,nil);
		return iterator(this,upper_bound(tar));
	}
	iterator upper_bound(key_t&&key)const{
		node tar(move(key),val_t(),UINT64_MAX,nil,nil,nil);
		return iterator(this,upper_bound(tar));
	}
	iterator find_by_order(int64_t idx)const{
		if(idx>=(int64_t)cur_size||-idx>(int64_t)cur_size) return iterator(this,nil);
		return iterator(this,ordered_access(idx>=0?idx:cur_size+idx));
	}
	size_t order_of_key(const key_t&key)const{
		node tar(key,val_t(),0,nil,nil,nil);
		return order_of_node(lower_bound(tar));
	}
	size_t order_of_key(key_t&&key)const{
		node tar(move(key),val_t(),0,nil,nil,nil);
		return order_of_node(lower_bound(tar));
	}
	size_t count(const key_t&key)const{ return count_wrapper(key); }
	size_t count(key_t&&key)const{ return count_wrapper(move(key)); }
	bool contains(key_t&&key)const{ return end()!=find(move(key)); }
	bool contains(const key_t&key)const{ return end()!=find(key); }
	protected:
	inline void calc_size(np x){ x->size=1+x->ch[0]->size+x->ch[1]->size; }
	//! @param left_right: 0 left, 1 right
	void rotation(np x,bool left_right){
		np y=x->ch[!left_right];
		x->ch[!left_right]=y->ch[left_right];
		y->p=x->p;
		if(y->ch[left_right]!=nil)y->ch[left_right]->p=x;
		if(x->p==nil)root=y;
		else x->p->ch[x==x->p->ch[1]]=y;
		y->ch[left_right]=x;
		x->p=y;
		y->size=x->size;
		calc_size(x);
	}
	void rb_insert_fixup(np z){
		while(z->p->time&filter_red){
			bool LR=z->p==z->p->p->ch[0];
			if(np y=z->p->p->ch[LR];y->time&filter_red){
				z->p->time&=filter_black,y->time&=filter_black;
				z->p->p->time|=filter_red,z=z->p->p;
			}else{
				if(z==z->p->ch[LR])rotation(z=z->p,!LR);
				z->p->time&=filter_black,z->p->p->time|=filter_red;
				rotation(z->p->p,LR);
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
			if(L==cmp(x->key,first))
				if constexpr(!allow_duplicate_keys){
					if constexpr(!is_same_v<val_t,null_t>)
						x->val=move(second);
					return iterator(this,x);
				}else x=x->ch[1];
			else x=x->ch[!L];
		}
		cur_size+=1;
		time+=unit_time;
		np z;
		if(unused==nullptr) z=new node(move(first),move(second),time|filter_red,y,nil,nil);
		else{
			z=unused;
			unused=unused->p;
			z->key=move(first),z->val=move(second);
			z->time=time|filter_red,z->ch[0]=z->ch[1]=nil;
			z->p=y,z->size=1;
		}
		if(y==nil) root=z;
		else y->ch[!cmp(z->key,y->key)]=z;
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
			bool LR=x==x->p->ch[0];
			np w=x->p->ch[LR];
			if(w->time&filter_red){
				w->time&=filter_black,x->p->time|=filter_red;
				rotation(x->p,!LR);
				w=x->p->ch[LR];
			}
			if(!((w->ch[0]->time&filter_red)||(w->ch[1]->time&filter_red)))
				w->time|=filter_red,x=x->p;
			else{
				if(!(w->ch[LR]->time&filter_red)){
					w->ch[!LR]->time&=filter_black,w->time|=filter_red;
					rotation(w,LR);
					w=x->p->ch[LR];
				}
				if(x->p->time&filter_red)w->time|=filter_red;
				else w->time&=filter_black;
				x->p->time&=filter_black,w->ch[LR]->time&=filter_black;
				rotation(x->p,!LR);
				x=root;
			}
		}
		x->time&=filter_black;
	}
	void erase(np z){
		auto transplant=[&](np u,np v){
			if(u->p==nil)root=v;
			else u->p->ch[u==u->p->ch[1]]=v;
			v->p=u->p;
		};
		np x,y=z,w=z->p;
		bool y_was_red=y->time&filter_red;
		if(z->ch[0]==nil) transplant(z,x=z->ch[1]);
		else if(z->ch[1]==nil) transplant(z,x=z->ch[0]);
		else{
			for(y=z->ch[1];1;y=y->ch[0]){
				y->size-=1;
				if(y->ch[0]==nil)break;
			}
			y_was_red=y->time&filter_red;
			x=y->ch[1];
			if(y!=z->ch[1]){
				transplant(y,y->ch[1]);
				y->ch[1]=z->ch[1],y->ch[1]->p=y;
			}else x->p=y;
			transplant(z,y);
			y->ch[0]=z->ch[0];
			y->ch[0]->p=y;
			if(z->time&filter_red)y->time|=filter_red;
			else y->time&=filter_black;
			calc_size(y);
		}
		if(w!=nil)for(;w!=nil;w=w->p)
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
		vector<np>st;
		st.reserve(cur_size);
		np c=min_max(root,false);
		while(c!=nil){
			st.push_back(c);
			c=prev_next(c,true);
		}
		for(const auto&x:st){
			x->p=unused;
			unused=x;
		}
		time=0;
		root=nil;
	}
	void reset(){
		clear();
		np x=unused;
		while(x!=nullptr){
			np dl=x;
			x=x->p;
			delete dl;
		}
		delete nil;
	}
	bool empty()const{return cur_size==0;}
	size_t size()const{return cur_size;}
	// iterator
	iterator begin()const{ return iterator(this,cur_size?min_max(root,false):nil); }
	iterator end()const{ return iterator(this,nil); }
	reverse_iterator rbegin()const{ return reverse_iterator(this,cur_size?min_max(root,true):nil); }
	reverse_iterator rend()const{ return reverse_iterator(this,nil); }
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
class varray:public rbtree<null_t,val_t,less<null_t>,true>{
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
			z->val=move(t),z->ch[0]=z->ch[1]=super::nil;
			z->size=1,z->time=super::time|super::filter_red;
			if(super::cur_size-1<=idx){
				y=super::min_max(super::root,true);
				z->p=y,y->ch[1]=z;
			}else{
				y=super::ordered_access(idx);
				if(y->ch[0]!=super::nil){
					y->ch[0]->p=z;
					z->ch[0]=y->ch[0];
				}
				z->p=y,y->ch[0]=z;
				super::calc_size(z);
			}
			for(typename super::np s=z->p;s!=super::nil;s=s->p)
				s->size+=1;
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