#include <utility> // forward(), move()
#include <any> // swap()
#include <functional> // less<>
#include <initializer_list>
#include <cstdint>
#include <cstddef> // size_t

template<class key_t,class val_t,class cmp=std::less<key_t>>
class tree{
	protected:
	using szt=std::size_t;
	using std::uint64_t;
	using value_type=std::pair<const key_t,val_t>;
	constexpr static std::nullptr_t nl = nullptr;
	constexpr static std::int8_t NIL=0, RED=1, BLK=2;
	public:
	union cs{
		uint64_t raw;
		struct{
			uint64_t s:62; // size
			uint64_t c:2; // color
		};
		cs(std::int8_t C,uint64_t S):c(C),s(S){}
		cs(uint64_t r):raw(r){}cs(cs c):cs(c.raw){}
	};
	class iterator{
		private:
		using ip=iterator*;
		ip l,r,p,nxt,prv;
		value_type*v;
		cs attr;
		iterator(value_type*V=nl,ip P=nl,ip pr=nl,ip nx=nl,ip L=nl,ip R=nl,cs att=cs(BLK,1))
			:v(V),p(P),prv(pr),nxt(nx),l(L),r(R),attr(att){}
		public:
		iterator&operator=(const iterator&o){return*this=iterator(o.v,o.p,o.prv,o.nxt,o.l,o.r);}
		bool operator==(iterator o){}
		bool operator!=(iterator o){}
		iterator&operator++(){}
		iterator operator++(std::int32_t){}
		iterator&operator--(){}
		iterator operator--(std::int32_t){}
	};
	class reverse_iterator{
		iterator p;
		public:
		iterator&operator=(const iterator&p){ return*this=iterator(p.v,p.par,p.prv,p.nxt,p.l,p.r,p.h,p.sz); }
		bool operator==(iterator p){}
		bool operator!=(iterator p){}
		iterator&operator++(){}
		iterator operator++(int32_t){}
		iterator&operator--(){}
		iterator operator--(int32_t){}
	};
	protected:
	constexpr static iterator nil; // nil_sentinel
	size_t cur_size;
	iterator*root,*bsent,*esent;//begin_sentinel,end_sentinel
	iterator insert(value_type*p){
		if(root==nullptr){
			root=new iterator(p,nl,bsent,esent);/*no LR*/
			cur_size=1;
			return*root;
		}
		iterator*itr=new iterator(p);
	}
	public:
	// constructor
	tree():root(nullptr),cur_size(0){
		bsent
	}
	tree(std::initializer_list<value_type>init):tree(){

	}
	// access
	iterator find(const key_t&key){}
	bool contains(const key_t&key){return find(key)!=end();}
	size_t count(const key_t&key){return contains(key);}
	val_t&operator[](const key_t&key){
		iterator itr=find(key);
		if(itr==end())itr=emplace(key,val_t());
		return itr->v->first;
	}
	iterator lower_bound(const key_t&key){}
	iterator upper_bound(const key_t&key){}
	size_t order_of_key(const key_t&key){}
	iterator find_by_order(size_t idx){}
	// update
	iterator insert(value_type&&x){
		value_type*p=new value_type(std::forward(x));
		return insert(p);
	}
	iterator emplace(key_t&&key,val_t&&val){
		value_type*p=new value_type(std::forward(key),std::forward(val));
		return insert(p);
	}
	iterator erase(const key_t&key){
		iterator p=find(key),r=p;
		if(p==end())return end();
		++r;erase(p);
		return r;
	}
	iterator erase(iterator&itr){
		if(itr==end())return end();
		iterator r=itr;
		++r;

		return r;
	}
	void clear(){
		iterator*p=bsent->nxt;
		while(p!=esent){
			delete p->v;
			iterator*q=p;
			p=p->nxt;
			delete q;
		}
		root=nullptr,cur_size=0,bsent->nxt=esent,esent->prv=bsent;
	}
	friend void swap(tree&x){}
	void merge(tree&x){}
	//iterator
	iterator begin(){return(first!=nullptr?*first:iterator());}
	constexpr iterator end(){return iterator();}
	reverse_iterator rbegin(){}
	reverse_iterator rend(){}
	//memory
	bool empty(){return cur_size==0;}
	size_t size(){return cur_size;}
};
