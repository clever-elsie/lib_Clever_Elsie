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
	namespace rbtree_internal{
		template<class key_t,class val_t,class lazy_t,class cmp,bool allow_duplicate_keys=std::false>
		struct rbnode{
			using ip=rbnode*;
			ip l,r,p,nx,pr;
			uint64_t time,size; // odd red, even black
			const key_t key;
			val_t val,data;
			lazy_t lz;
			rbnode(ip nil):key(),val(),lz(),time(0),size(1),
				l(nil),r(nil),p(nil),nx(nil),pr(nil){}
			rbnode(const key_t&k,const val_t&v,const lazy_t&Lz,int32_t tm,
				ip P,ip L,ip R,ip Pr,ip Nx)
				:key(k),val(v),lz(Lz),
				time(tm),size(0),
				p(P),l(L),r(R),pr(Pr),nx(Nx){}
			auto operator<=>(const rbnode&other){
				bool L=cmp(key,other.key),R=cmp(other.key,key);
				if(L==R)return time<=>other.time;
				return L?strong_ordering::less:strong_ordering::greater;
			}
		};
		template<class key_t,class val_t,class lazy_t,class cmp,bool allow_duplicate_keys=std::false>
		struct rbiterator{
			rbiterator(rbnode*node):n(node){}
			key_t&first;
			val_t&second;
			private:rbnode*n;
		};
		template<class key_t,class val_t,class lazy_t,class cmp,bool allow_duplicate_keys=std::false>
		struct rbreverseiterator{
			rbreverseiterator(rbnode*node):n(node){}
			key_t&first;
			val_t&second;
			private:rbnode*n;
		};
	}

struct null_t{ null_t(){} };

template<class key_t,class val_t,class op,class e,
	class lazy_t,class mapping,
	class composition,class id,class cmp=less<key_t>,bool allow_duplicate_keys=false>
class master_rbtree{
	static_assert(!is_same_v<key_t,null_t>);
	static_assert(is_same_v<val_t,null_t>||(is_convertible_v(decltype(op),function<val_t(val_t,val_t)>)&&is_convertible_v(decltype(e),function<val_t()>)));
	static_assert(is_same_v<lazy_t,null_t>||(is_convertible_v(decltype(mapping),function<val_t(lazy_t,val_t)>)&&is_convertible_v(decltype(composition),function<lazy_t(lazy_t,lazy_t)>)&&is_convertible_v(decltype(id),function<lazy_t()>)));

	public:
	using value_type=pair<const key_t,val_t>;
	using iterator=rbtree_internal::rbiterator<key_t,val_t,lazy_t,cmp,allow_duplicate_keys>;
	using reverse_iterator=rbtree_internal::rbreverseiterator<key_t,val_t,lazy_t,cmp,allow_duplicate_keys>;

	protected:
	const static uint64_t filter_red=1;
	const static uint64_t filter_black=0xFFFF'FFFF'FFFF'FFFEul;
	using node=rbtree_internal::rbnode<key_t,val_t,lazy_t,cmp,allow_duplicate_keys>;
	using np=node*;
	size_t cur_size,time;
	np root,bs,nil;
	/* 実装完了時に消すコメント
		lower_boundを探索の基本として使う
		lower_bound({key,0})がfind及びlower_bound
		lower_bound({key,UINT64_MAX})がupper_bound
		findではkeyが異なるときに落とす処理を加える． */
	np lower_bound(const node&tar){
		if(root==nil)return nil;
		np cur=root,nx;
		while(1){
			auto s=tar<=>*cur;
			if(s<0)nx=cur->l;else if(s>0)nx=cur->r;else break;
			if(nx!=nil)cur=nx;
			else{
				if(s<0)return cur->nx;
				else return nil;
			}
		}
		return cur;
	}
	np find(np p){
		np f=lower_bound(*p);
		return f!=nil&&*f==*p?f:nil;
	}
	np ordered_access(int64_t idx){
	}
	np minimum(np p){
		for(;p->l!=nil;p=p->l);
		return p;
	}
	np maximum(np p){
		for(;p->r!=nil;p=p->r);
		return p;
	}
	// nodeの中の属性を書き換える必要がある
	void node_apply(np x,lazy_t*f){
		x->val=mapping(*f,x->val);
		x->data=mapping(*f,x->data);
		x->lz=composition(*f,x->lz);
	}
	void push_apply(np x){
		node_apply(x,&x->lz);
		if(x->l!=nil) node_apply(x->l,&x->lz);
		if(x->r!=nil) node_apply(x->r,&x->lz);
		x->lz=id();
	}
	void update_prod(np x){ x->data=op(op(x->l!=nil?x->l->data,x->data),x->r->data); }
	np left_rotation(np x){
		np y=x->r;
		if constexpr(!is_same_v<lazy_t,null_t>)
			push_apply(x),push_apply(y);
		x->r=y->l,y->p=x->p;
		if(y->l!=nil)y->l->p=x;
		if(x->p==nil)root=y;
		else if(x==x->p->l)x->p->l=y;
		else x->p->r=y;
		y->l=x,x->p=y;
		swap(x->size,y->size);
		if constexpr(!is_same_v<val_t,null_t>)
			update_prod(x),update_prod(y);
	}
	np right_rotation(np x){
		np y=x->l;
		if constexpr(!is_same_v<lazy_t,null_t>)
			push_apply(x),push_apply(y);
		x->l=y->r,y->p=x->p;
		if(y->r!=nil)y->r->p=x;
		if(x->p==nil)root=y;
		else if(x==x->p->l)x->p->l=y;
		else x->p->r=y;
		y->r=x,x->p=y;
		swap(x->size,y->size);
		if constexpr(!is_same_v<val_t,null_t>)
			update_prod(x),update_prod(y);
	}
	void resolve_delay(np z){
		vector<np>pst;
	}
	void ascend_prod(np z){
	}
	// ここまで
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
		if(root==nil){
			root=z;
			z->pr=bs,z->nx=es;
			cur_size=1;
			return root;
		}
		np x=lower_bound(*z);
		if(x->pr==nil){
			if constexpr(!is_same_v<lazy_t,null_t>)
				resolve_delay(x);
			x->l=z,z->pr=x->pr;
			z->p=x,z->nx=x,x->pr=z;
			if(z->pr!=nil)z->pr->nx=z;
		}else{
			x=x->pr;
			if constexpr(!is_same_v<lazy_t,null_t>)
				resolve_delay(x);
			x->r=z,z->pr=x,z->nx=x->nx;
			z->p=x,x->nx=z;
			if(z->nx!=nil)z->nx->pr=z;
		}
		if constexpr(!allow_duplicate_keys)
			if(cmp(x->key,z->key)==cmp(z->key,x->key)){
				if constexpr(!is_same_v<val_t,null_t>){
					x->val=z->val;
					ascend_prod(x);
				}
				return x;
			}
		rb_insert_fixup(z);
		if constexpr(!is_same_v<val_t,null_t>)
			ascend_prod(z);
		return z;
	}

	np rb_delete_fixup(np x){
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

	np erase(np z){
		auto transplant=[](np u,np v){
			if(u->p==nil)root=v;
			else if(u==u->p->l)u->p->l=v;
			else u->p->r=v;
			v->p=u->p;
		};
		np x,y=z;
		bool y_was_red=y->time&filter_red;
		if(z->l==nil){
			x=z->r;
			transplant(z,z->r);
		}else if(z->r==nil){
			x=z->l;
			transplant(z,z->l);
		}else{
			y=minimum(z->r);
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
		}
		if(!y_was_red)rb_delete_fixup(x);
		delete z;
	}
	public:
	// fundamental manipulation
	master_rbtree():cur_size(0),time(0){
		nil=new node(nullptr);
		bs=new node(nil);
		bs->pr=bs,bs->nx=nil;
		nil->pr=bs,nil->nx=nil;
		root=nil;
	}
	// access
	virtual iterator find(const key_t&key)=0;
	virtual iterator lower_bound(const key_t&key)=0;
	virtual iterator upper_bound(const key_t&key)=0;
	virtual iterator find_by_order(int64_t idx)=0;
	virtual size_t order_of_key(const key_t&key)=0;
	virtual const val_t&operator[](const key_t&)=0;
	virtual size_t count(const key_t&)=0;
	virtual size_t contains(const key_t&)=0;
	// change
	virtual iterator insert(value_type&&x)=0;
	virtual iterator emplace(key_t&&key,val_t&&val)=0;
	virtual iterator erase(const key_t&key)=0;
	virtual iterator erase(iterator&itr)=0;
	// memory
	void clear(){}
	bool empty(){}
	size_t size(){}
	// iterator
	virtual iterator begin()=0;
	virtual iterator end()=0;
	virtual iterator rbegin()=0;
	virtual iterator rend()=0;
protected:
	virtual val_t prod(const key_t&l,const key_t&r)=0;
	virtual void apply(const key_t&l,const key_t&r,const lazy_t&lazy)=0;
};

template<class key_t,class cmp=less<key_t>>
class multiset:public master_rbtree<key_t,null_t,null_t,null_t,null_t,null_t,null_t,null_t,cmp,true>{};

template<class key_t,class cmp=less<key_t>>
class set:public master_rbtree<key_t,null_t,null_t,null_t,null_t,null_t,null_t,null_t,cmp,false>{};

template<class key_t,class val_t,class cmp=less<key_t>>
class multimap:public master_rbtree<key_t,val_t,null_t,null_t,null_t,null_t,null_t,null_t,cmp,true>{};

template<class key_t,class val_t,class cmp=less<key_t>>
class map:public master_rbtree<key_t,val_t,null_t,null_t,null_t,null_t,null_t,null_t,cmp,false>{};

template<class key_t,class val_t,class op,class e,class cmp=less<key_t>>
class multimap_monoid:public master_rbtree<key_t,val_t,op,e,null_t,null_t,null_t,null_t,cmp,true>{};

template<class key_t,class val_t,class op,class e,class cmp=less<key_t>>
class map_monoid:public master_rbtree<key_t,val_t,op,e,null_t,null_t,null_t,null_t,cmp,false>{};

template<class key_t,class val_t,class op,class e,class lazy_t,class mapping,class composition,class id,class cmp=less<key_t>>
class lazymultimap:public master_rbtree<key_t,val_t,op,e,lazy_t,mapping,composition,id,cmp,true>{};

template<class key_t,class val_t,class op,class e,class lazy_t,class mapping,class composition,class id,class cmp=less<key_t>>
class lazymap:public master_rbtree<key_t,val_t,op,e,lazy_t,mapping,composition,id,cmp,false>{};

}
#endif