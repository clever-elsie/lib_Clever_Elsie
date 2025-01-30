#include <atcoder/lazysegtree>
#include<vector>
#include<cstddef>
using namespace std;
using namespace atcoder;
#define int long long

template<class type>class BIT{// 0-indexed [l,r)
	vector<type>v;
	size_t _size;
	type sum(int64_t r){
		type ret=0;
		for(;r;r-=r&-r) ret+=v[r-1];
		return ret;
	}
public:
	BIT(size_t size){
		size_t n=1;
		while(n<size)n*=2;
		_size=n, v.resize(n);
	}
	void add(size_t i,type val){ for(i++;i<=_size;i+=i&-i) v[i-1]+=val; }
	type sum(size_t l,size_t r){ return sum(r)-sum(l); }
	type get(size_t i){ return sum(i,i+1); }
	void set(size_t i,type t){ add(i,t-get(i)); }
};

template<class S,S(*op)(S,S),S(*e)()>class segtree{
protected: // 1-indexed [l,r)
	vector<S> v;
	size_t n,tl,tr;
	S prod(size_t l,size_t r,size_t idx){
		if(tl<=l&&r<=tr)return v[idx];
		size_t m=l+(r-l)/2;
		if((tr<=l||r<=tl)||!(tl<=m||m<=tr))return e();
		return op(prod(l,m,idx*2),prod(m,r,idx*2+1));
	}
public:
	segtree(int N):n(1){
		while(n<N)n<<=1;
		v.resize(n+n,e());
	}
	segtree(const vector<S>&V):n(1){
		while(n<V.size())n<<=1;
		v.resize(n+n,e());
		for(size_t i=n;auto&x:V)v[i++]=x;
		for(size_t i=n-1;i;i--)v[i]=op(v[2*i],v[2*i+1]);
	}
	void set(size_t p,S val){
		v[n+p]=val;
		for(size_t i=(n+p)>>1;i;i>>=1)
			v[i]=op(v[2*i],v[2*i+1]);
	}
	S get(size_t p){ return v.at(n+p);}
	S prod(size_t l,size_t r){
		tl=l,tr=r;
		return prod(0,n,1);
	}
};