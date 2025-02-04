#include<any> // swap()
#include<cstdint>
#include<cstddef> // size_t
#include<utility> // forward(), move()
#include<functional> // less<>
#include<type_traits>
#include<initializer_list>
#ifndef ELSIE_RBTREE
#define ELSIE_RBTREE
namespace elsie{
using namespace std;
	namespace rbtree_internal{
		template<class key_t,class val_t,class lazy_t,class cmp>
		struct rbnode{
			using ip=rbnode*;
			ip l,r,p,nx,pr;
			uint64_t time; // odd red, even black
			const key_t key;
			val_t val;
			lazy_t lz;
			rbnode(const key_t&k,const val_t&v,const lazy_t&Lz,int32_t tm,
				ip P,ip L,ip R,ip Pr,ip Nx)
				:key(k),val(v),lz(Lz),time(tm),p(P),l(L),r(R),pr(Pr),nx(Nx){}
			bool operator<(const rbnode&other){
				bool L=cmp(key,other.key);
				return(L!=cmp(other.key,key)?L:time<other.time);
			}
			private:
			// 遅延解消関数を書く.
		};
		template<class key_t,class val_t,class lazy_t,class cmp>
		struct rbiterator{
			rbiterator(rbnode*node):n(node){}
			key_t&first;
			val_t&second;
			private:rbnode*n;
		};
	}

struct null_t{};

template<class key_t,class val_t,class op,class e,
	class lazy_t,class mapping,
	class composition,class id,class cmp=less<key_t>>
class master_rbtree{
	static_assert(is_convertible_v(decltype(op),function<val_t(val_t,val_t)>));
	static_assert(is_convertible_v(decltype(e),function<val_t()>));
	static_assert(is_convertible_v(decltype(mapping),function<val_t(lazy_t,val_t)>));
	static_assert(is_convertible_v(decltype(composition),function<lazy_t(lazy_t,lazy_t)>));
	static_assert(is_convertible_v(decltype(id),function<lazy_t()>));

	public:
	using value_type=pair<const key_t,val_t>;
	using iterator=rbtree_internal::rbiterator<key_t,val_t,lazy_t,cmp>;

	protected:
	using node=rbtree_internal::rbnode<key_t,val_t,lazy_t,cmp>;
	constexpr static nullptr_t nl=nullptr;
	constexpr static node nil; // nil_sentinel
	size_t cur_size,time;
	node*root,*bsent,*esent;//begin_sentinel,end_sentinel
	/*
		実装完了時に消すコメント
		lower_boundを探索の基本として使う
		lower_bound({key,0})がfind及びlower_bound
		lower_bound({key,UINT64_MAX})がupper_bound
		findではkeyが異なるときに落とす処理を加える．
	*/
	node*lower_bound(const node&tar){
		if(root==nl)return nl;
		node*cur=root,*nx;
		while(1){
			if(tar<*cur)nx=cur->l;else if(*cur<tar)nx=cur->r;else break;
			if(nx!=nl)cur=nx;else break;
		}
		return cur;
	}
	node*find(node*p){
		node*f=lower_bound(*p);
		if((*f<*p)==(*p<*f))return f;
		else nl;
	}
	node*right_rotation(node*cur){
	}
	node*left_rotation(node*cur){
	}
	node*insert(node*p){
		if(root==nl){
			root=new node();/*no LR*/
			cur_size=1;
			return*root;
		}
		node*p=new node(p);
	}
	node*erase(node*p){

	}

	public:
	friend void resolve_delay(master_rbtree&tree,node*p){}
	// fundamental manipulation
	tree():root(nullptr),cur_size(0),time(0){
		nil=new node();
		bsent=new node();
		esent=new node();
	}
	tree(initializer_list<value_type>init):tree(){}
	iterator find(const key_t&key){}
	iterator lower_bound(const key_t&key){}
	iterator upper_bound(const key_t&key){}
	iterator find_by_order(size_t idx){}
	size_t order_of_key(const key_t&key){}
	iterator insert(value_type&&x){}
	iterator emplace(key_t&&key,val_t&&val){}
	iterator erase(const key_t&key){}
	iterator erase(iterator&itr){}
};

template<class key_t,class cmp=less<key_t>>
class treeset:public master_rbtree<key_t,null_t,null_t,null_t,null_t,null_t,null_t,null_t,cmp>{

};

template<class key_t,class val_t,class cmp=less<key_t>>
class treemap:public master_rbtree<key_t,val_t,null_t,null_t,null_t,null_t,null_t,null_t,cmp>{

};

template<class key_t,class val_t,class op,class e,class cmp=less<key_t>>
class treemap_monoid:public master_rbtree<key_t,val_t,op,e,null_t,null_t,null_t,null_t,cmp>{

};
}
#endif