#include<vector>
using namespace std;

template<class S,S(*op)(S,S),S(*e)()>
class segtree{
protected: // don't write
	vector<S> v;
	size_t n,tl,tr;
	S prod_internal(size_t l,size_t r,size_t idx){
		if(tr<=l||r<=tl)return e();
		if(tl<=l&&r<=tr)return v[idx];
		size_t mid=l+(r-l)/2;
		S ret=e();
		if(tl<=mid||mid<=tr){
			ret=op(ret,prod_internal(l,mid,idx*2));
			ret=op(ret,prod_internal(mid,r,idx*2+1));
		}
		return ret;
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
		return prod_internal(0, n, 1);
	}
};