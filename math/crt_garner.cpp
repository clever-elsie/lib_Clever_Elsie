#include <vector>
#include <cstdint>
#include <unordered_map>
#include <numeric>
#include "gcd_lcm.cpp"
#ifndef ELSIE_CRT_GARNER
#define ELSIE_CRT_GARNER
namespace elsie{
using namespace std;
// gcd_lcm.cpp::exgcd();
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
	auto mod_mul=[](int64_t x,int64_t y,int64_t MOD){ return (MOD+((x%MOD)*(y%MOD))%MOD)%MOD; };
	m.push_back(mod);
	vc coeffs(m.size(),1);
	vc constants(m.size(),0);
	for(size_t k=0;k<b.size();++k){
		T t=mod_mul(b[k]-constants[k],mod_inv<T>(coeffs[k],m[k]),m[k]);
		for(size_t i=k+1;i<m.size();++i){
			(constants[i]+=mod_mul(t,coeffs[i],m[i]))%=m[i];
			coeffs[i]=mod_mul(coeffs[i],m[k],m[i]);
		}
	}
	m.pop_back();
	return constants.back();
}

template<integral T>T decomposite_garner(vector<T>&b,vector<T>&m,T mod)noexcept(false){
	size_t zerocnt=0;
	for(const auto&x:b)zerocnt+=x==0;
	long long ret=1;
	for(size_t i=0;i<b.size();++i){
		for(size_t j=0;j<i;++j){
			long long g=gcd(m[i],m[j]);
			if((b[i]-b[j])%g)return -1;
			m[i]/=g,m[j]/=g;
			long long gi=gcd(m[i],g),gj=g/gi;
			do{
				g=gcd(gi,gj);
				gi*=g,gj/=g;
			}while(g!=1);
			m[i]*=gi,m[j]*=gj;
			b[i]%=m[i],b[j]%=m[j];
		}
	}
	for(size_t i=0;i<b.size();++i)ret=(ret*m[i])%mod;
	if(zerocnt==b.size())return ret;
	return garner(b,m,mod);
}
}
#endif