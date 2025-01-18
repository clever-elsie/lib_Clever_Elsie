#include <vector>
#include <cstdint>
using namespace std;

template<integral T>T exgcd(T a,T b,T&x,T&y){
	auto eq=[](T&s,T&t,T u,T v)->void {s=u,t=v;};
	x=1,y=0;
	T u=0,v=1;
	while(b){
		T k=a/b;
		eq(x,u,u,x-k*u);
		eq(y,v,v,y-k*v);
		eq(a,b,b,a%b);
	}return a;
}

template<integral T>T mod_inv(T a,T m){
	T x,y,g=exgcd(a,m,x,y);
	return(g!=1?-1:x+(x<0?m:0));
}

template<integral T>pair<T,T>crt(const vector<T>&b,const vector<T>&m){
	T r=0,M=1;
	for(size_t i=0;i<b.size();++i){
		T p,q,d=exgcd(M,m[i],p,q);
		T qo=(b[i]-r)/d;
		T rem=(b[i]-r)%d;
		if(rem)return {0,-1};
		T tmp=qo*p%(m[i]/d);
		r+=M*tmp;
		M*=m[i]/d;
	}return{(r%M+M)%M,M};
}

template<integral T>T garner(vector<T>&b,vector<T>&m,T mod){// any pair each of all m is gcd(m[i],m[j])=1
	using vc=vector<T>;
	auto mod_add=[](int64_t x,int64_t y,int64_t MOD){ return ((x+y)%MOD+MOD)%MOD; };
	auto mod_mul=[](int64_t x,int64_t y,int64_t MOD){ return ((x%MOD)*(y%MOD))%MOD; };
	m.push_back(mod);
	vc coeffs(m.size(),1);
	vc constants(m.size(),0);
	for(size_t k=0;k<b.size();++k){
		T t=mod_mul(mod_add(b[k],-constants[k],m[k]),mod_inv<T>(coeffs[k],m[k]),m[k]);
		for(size_t i=k+1;i<m.size();++i){
			constants[i]=mod_add(constants[i],mod_mul(t,coeffs[i],m[i]),m[i]);
			coeffs[i]=mod_mul(coeffs[i],m[k],m[i]);
		}
	}
	return constants.back();
}