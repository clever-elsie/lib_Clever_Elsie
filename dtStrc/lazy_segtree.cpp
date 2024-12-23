#include <vector>
using namespace std;

template<class S,S(*op)(S,S),S(*e)(),
class F,S(*mapping)(F,S),F(*composition)(F,F),F(*id)()>
class lazy_segtree{
protected:
	vector<S> v;
	vector<F> lazy;
	size_t n,tl,tr;
	S prod_internal(size_t l,size_t r,size_t idx){
		if(tr<=l||r<=tl)return e();
		S ret=e();
		if(tl<=l&&r<=tr) ret=v[idx];
		else if(size_t mid=l+(r-l)/2;tl<=mid||mid<=tr){
			ret=op(ret,prod_internal(l,mid,idx*2));
			ret=op(ret,prod_internal(mid,r,idx*2+1));
		}
		for(;idx;idx>>=1)ret=mapping(lazy[idx],ret);
		return ret;
	}
	void apply_internal(size_t l,size_t r,size_t idx,F&app){
		if(tr<=l||r<=tl)return;
		if(tl<=l&&r<=tr){
			lazy[idx]=composition(app,lazy[idx]);
		}else if(size_t mid=l+(r-l)/2;tl<=mid||mid<=tr){
			apply_internal(l,mid,idx*2,app);
			apply_internal(mid,r,idx*2+1,app);
		}
	}
public:
	lazy_segtree(size_t N){
		while(n<N)n<<=1;
		v.resize(n+n,e());
		lazy.resize(n+n,id());
	}
	lazy_segtree(const vector<S>&V){
		while(n<V.size())n<<=1;
		lazy.resize(n+n,id());
		v.resize(n+n,e());
		for(size_t i=n;auto&x:V)v[i++]=x;
		for(size_t i=n-1;i;i--)v[i]=op(v[2*i],v[2*i+1]);
	}
	void set(size_t p,S val){
		v[n+p]=val;
		for(size_t i=(n+p)>>1;i;i>>=1)
			v[i]=op(v[2*i],v[2*i+1]);
	}
	S get(size_t p){
		size_t idx=n+p;
		S ret=v[idx];
		for(;idx;idx>>=1)ret=mapping(lazy[idx],ret);
		return ret;
	}
	S prod(size_t l,size_t r){
		tl=l,tr=r;
		return prod_internal(0,n,1);
	}
	void apply(size_t l,size_t r,F f){
		tl=l,tr=r;
		apply_internal(0,n,1,f);
	}
};
