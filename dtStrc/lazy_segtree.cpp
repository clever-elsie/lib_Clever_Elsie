#include <iostream>
#include <cstdint>
#include <vector>
using namespace std;

namespace elsie{
template<class S,S(*op)(S,S),S(*e)(),
class F,S(*mapping)(F,S),F(*composition)(F,F),F(*id)()>
class lazy_segtree{
protected:
	vector<S>v;
	vector<F>lazy;
	size_t n,tl,tr;
	F app_new;
	S prod(size_t l,size_t r,size_t idx){
		if(tl<=l&&r<=tr)return v[idx];
		if(tr<=l||r<=tl)return e();
		size_t m=(l+r)/2;
		return op(prod(l,m,2*idx),prod(m,r,2*idx+1));
	}
	void update(size_t idx,F app){
		if(2*idx+1<v.size())v[idx]=mapping(lazy[idx],op(v[2*idx],v[2*idx+1]));
		else v[idx]=mapping(app,v[idx]);
	}
	void apply(size_t l,size_t r,size_t idx,F app){
		if(tr<=l||r<=tl){ // out of range
			lazy[idx]=composition(app,lazy[idx]);
			update(idx,app);
		}if(tl<=l&&r<=tr){ // completely included
			lazy[idx]=composition(app,lazy[idx]);
			lazy[idx]=composition(app_new,lazy[idx]);
			update(idx,app);
		}else{
			size_t m=(l+r)/2;
			app=composition(app,lazy[idx]);
			apply(l,m,idx*2,lazy[idx]);
			apply(m,r,idx*2+1,lazy[idx]);
			lazy[idx]=id();
			update(idx,id());
		}
	}
public:
	lazy_segtree(size_t N):n(1){
		while(n<N)n*=2;
		v.resize(n*2,e());
		lazy.resize(n*2,id());
	}
	lazy_segtree(const vector<S>&V):lazy_segtree(V.size()){
	}
	void set(size_t p,S val){
		size_t idx=n+p;
		v[idx]=val;
		vector<size_t>rp;
		for(idx>>=1;idx;idx>>=1)rp.push_back(idx);
		for(int32_t i=(int32_t)rp.size()-1;i>=0;--i){
			lazy[2*rp[i]]=composition(lazy[rp[i]],lazy[2*rp[i]]);
			lazy[2*rp[i]+1]=composition(lazy[rp[i]],lazy[2*rp[i]+1]);
			update(2*rp[i],lazy[rp[i]]);
			update(2*rp[i]+1,lazy[rp[i]]);
			lazy[rp[i]]=id();
		}
		lazy[n+p]=id();
		for(size_t i=0;i<rp.size();++i)
			v[rp[i]]=mapping(lazy[rp[i]],op(v[2*rp[i]],v[2*rp[i]+1]));
	}
	S get(size_t p){
		size_t idx=n+p;
		S r=v[idx];
		for(idx>>=1;idx;idx>>=1)
			r=mapping(lazy[idx],r);
		return r;
	}
	S prod(size_t l,size_t r){
		tl=l,tr=r;
		return prod(0,n,1);
	}
	void apply(size_t l,size_t r,const F&f){
		tl=l,tr=r,app_new=f;
		apply(0,n,1,f);
	}
	template<class T>
	void print(vector<T>&a){
		int st=1;
		int cnt=0;
		while(st<v.size()){
			int nx=2*st;
			for(int i=0;i<countr_zero(n)-cnt;i++)
				cout<<"  ";
			for(int i=st;i<nx;i++)
				cout<<a[i]<<' ';
			cout<<endl;
			st=nx;
			++cnt;
		}
	}
	void print(){
		print(v);
		print(lazy);
		cout<<endl;
	}
};
}