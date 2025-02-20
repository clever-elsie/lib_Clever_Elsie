#ifndef ELSIE_CONVOLUTION
#define ELSIE_CONVOLUTION
#include <vector>
#include <cstdint>
#include <math/basic_math.hpp>
#include <math/prime_factor.hpp>
namespace elsie{
using namespace std;
template<size_t M=998244353,auto op=[](const int64_t&a,const int64_t&b){return a*b;},auto e=[](){return 0ll;}>
class convolution{
	private:
	using ll = int64_t;
	using it = int32_t;
	vector<ll> root,invroot;
	template<bool sign>
	void ntt(vector<it>&s,int id){
		if(id==0)return;
		it N=1<<id-1;
		vector<it>f(N),g(N);
		for(int i=0;i<N;i++){
			f[i]=s[i<<1];
			g[i]=s[i<<1|1];
		}
		ntt<sign>(f,id-1);
		ntt<sign>(g,id-1);
		ll z,p=1;
		if constexpr(sign)z=invroot[id];
		else z=root[id];
		for(int i=0;i<N;i++){
			s[i]=(f[i]+p*g[i]%M)%M;
			s[i+N]=(f[i]-p*g[i]%M+M)%M;
			p=p*z%M;
		}
	}
	public:
	convolution(){}
	vector<it>prod(vector<it>a,vector<it>b){
		if(a.empty()||b.empty()) return vector<it>(0);
		it u=1,sz=0,m=a.size()+b.size()-1;
		while(u<m)u<<=1,sz++;
		root.resize(sz+1),invroot.resize(sz+1);
		root[sz]=modpow(primitive_root(M),M-1>>sz,M);
		invroot[sz]=modpow(root[sz],M-2,M);
		for(int i=sz-1;i>0;i--){
			root[i]=root[i+1]*root[i+1]%M;
			invroot[i]=invroot[i+1]*invroot[i+1]%M;
		}
		a.resize(u,e()),b.resize(u,e());
		vector<it>ret(m,e());
		ntt<false>(a,sz),ntt<false>(b,sz);
		for(int i=0;i<u;i++)a[i]=op(a[i],b[i])%M;
		ntt<true>(a,sz);
		ll invN=modpow(u,M-2,M);
		for(int i=0;i<m;i++)ret[i]=invN*a[i]%M;
		return ret;
	}
};
}
#endif