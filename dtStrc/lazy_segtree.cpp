#include <iostream>
#include <cstdint>
#include <vector>
using namespace std;

template <class S> struct sum_lst { S val; size_t size; sum_lst(S a, size_t b) : val(a), size(b) {} };
template <class S, class F> using add_sum_lst = lazy_segtree<sum_lst<S>, [](sum_lst<S> a, sum_lst<S> b) -> sum_lst<S> { return sum_lst<S>(a.val + b.val, a.size + b.size); }, []() -> sum_lst<S> { return sum_lst<S>((S)0, 0ull); }, F, [](F f, sum_lst<S> x) -> sum_lst<S> { return sum_lst<S>(x.val + f * x.size, x.size); }, [](F f, F g) { return g + f; }, []() -> F { return 0; }>;

template <class S, class F> using add_max_lst = lazy_segtree<S, [](S a, S b) { return (a > b ? a : b); }, []() -> S { return -(1ll << 62); }, F, [](F f, S x) { return x + f; }, [](F f, F g) { return g + f; }, []() -> F { return 0; }>;
template <class S, class F> using add_min_lst = lazy_segtree<S, [](S a, S b) { return (a < b ? a : b); }, []() -> S { return 1ll << 62; }, F, [](F f, S x) { return x + f; }, [](F f, F g) { return g + f; }, []() -> F { return 0; }>;

template <class S, class F>using ch_max_lst = lazy_segtree<S, [](S a, S b) { return (a > b ? a : b); }, []() -> S { return -(1ll << 62); }, F, [](F f, S x) -> S { return (f == -(1ll << 62) ? x : f); }, [](F f, F g) -> F { return (f == -(1ll << 62) ? g : f); }, []() -> F { return -(1ll << 62); }>;
template <class S, class F>using ch_min_lst = lazy_segtree<S, [](S a, S b) { return (a < b ? a : b); }, []() -> S { return 1ll << 62; }, F, [](F f, S x) -> S { return (f == (1ll << 62) ? x : f); }, [](F f, F g) -> F { return (f == (1ll << 62) ? g : f); }, []() -> F { return 1ll << 62; }>;
template <class S, class F>using ch_sum_lst = lazy_segtree<sum_lst<S>, [](sum_lst<S> a, sum_lst<S> b) -> sum_lst<S> { return sum_lst<S>(a.val + b.val, a.size + b.size); }, []() -> sum_lst<S> { return sum_lst<S>((S)0, 0ull); }, F, [](F f, sum_lst<S> x) -> sum_lst<S> { if (f != (1ll << 62)) x.val = f * x.size; return x; }, [](F f, F g) { return (f == (1ll << 62) ? g : f); }, []() -> F { return 1ll << 62; }>;

template<class S,S(*op)(S,S),S(*e)(),
class F,S(*mapping)(F,S),F(*composition)(F,F),F(*id)()>
class lazy_segtree{
protected:
	using sz_t=uint32_t;
	vector<S>data;
	vector<F>lazy;
	sz_t sz,h;
	void update(sz_t idx){data[idx]=op(data[2*idx],data[2*idx+1]);}
	void all_apply(sz_t idx,F f){
		data[idx]=mapping(f,data[idx]);
		if(idx<sz)lazy[idx]=composition(f,lazy[idx]);
	}
	void push(sz_t idx){
		all_apply(2*idx,lazy[idx]);
		all_apply(2*idx+1,lazy[idx]);
		lazy[idx]=id();
	}
public:
	lazy_segtree(size_t N=0):lazy_segtree(vector<S>(N,e())){}
	lazy_segtree(const vector<S>&v){
		sz=1ll<<(63-countl_zero(v.size())+(popcount(v.size())>1?1:0));
		h=countr_zero(sz);
		data=vector<S>(2*sz,e());
		for(sz_t i=0;i<v.size();++i)data[i+sz]=v[i];
		for(sz_t i=sz-1;i>0;--i)update(i);
		lazy=vector<F>(sz,id());

	}
	void set(sz_t p,S val){
		p+=sz;
		for(sz_t i=h;i>=1;--i)push(p>>i);
		data[p]=val;
		for(sz_t i=1;i<=h;++i)update(p>>i);
	}
	S get(sz_t p){
		p+=sz;
		for(sz_t i=h;i>=1;--i)push(p>>i);
		return data[p];
	}
	S prod(sz_t l,sz_t r){
		if(l==r)return e();
		l+=sz,r+=sz;
		for(sz_t i=h;i>=1;--i){
			if(((l>>i)<<i)!=l)push(l>>i);
			if(((r>>i)<<i)!=r)push((r-1)>>i);
		}
		S sl=e(),sr=e();
		while(l<r){
			if(l&1)sl=op(sl,data[l++]);
			if(r&1)sr=op(data[--r],sr);
			l>>=1,r>>=1;
		}
		return op(sl,sr);
	}
	S all_prod(){return data[1];}
	void apply(sz_t l,sz_t r,F f){
		if(l==r)return;
		l+=sz,r+=sz;
		for(sz_t i=h;i>=1;--i){
			if(((l>>i)<<i)!=l)push(l>>i);
			if(((r>>i)<<i)!=r)push((r-1)>>i);
		}
		sz_t L=l,R=r;
		while(L<R){
			if(L&1)all_apply(L++,f);
			if(R&1)all_apply(--R,f);
			L>>=1,R>>=1;
		}
		for(sz_t i=1;i<=h;++i){
			if(((l>>i)<<i)!=l)update(l>>i);
			if(((r>>i)<<i)!=r)update((r-1)>>i);
		}
	}
};