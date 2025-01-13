#include <vector>
#include <cstdint>
using namespace std;

template<size_t M=998244353,auto op=[](const int64_t&a,const int64_t&b){return a*b;},auto e=[](){return 0ll;}>
class convolution{
	private:
	using ll = int64_t;
	using it = int32_t;
	vector<ll> root,invroot;
	ll modpow(ll a,it b,it mod){
		ll k=a%mod,ret=1;
		if(k<0)k+=mod;
		while(b){
			if(b&1)ret=ret*k%mod;
			k=(k*k)%mod;
			b>>=1;
		}
		return ret;
	}
	constexpr it find_primitive_root(){
		vector<it>factor;{
			it p=M-1;
			for(ll i=2;i<=p;i++)
				if(p%i==0){
					factor.push_back(i);
					while(p%i==0)p/=i;
				}
		}
		for(it g=2;g<M;g++){
			bool ok=true;
			for(const auto&q:factor){
				if(modpow(g,(M-1)/q,M)==1){
					ok=false;
					break;
				}
			}
			if(ok)return g;
		}
		return -1;
	}
	void ntt(vector<it>&s,bool sign,int id){
		if(id==0)return;
		it N=1<<id-1;
		vector<it>f(N),g(N);
		for(int i=0;i<N;i++){
			f[i]=s[i<<1];
			g[i]=s[i<<1|1];
		}
		ntt(f,sign,id-1);
		ntt(g,sign,id-1);
		ll z=(sign?invroot:root)[id],p=1;
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
		root[sz]=modpow(find_primitive_root(),M-1>>sz,M);
		invroot[sz]=modpow(root[sz],M-2,M);
		for(int i=sz-1;i>0;i--){
			root[i]=root[i+1]*root[i+1]%M;
			invroot[i]=invroot[i+1]*invroot[i+1]%M;
		}
		a.resize(u,e()),b.resize(u,e());
		vector<it>ret(m,e());
		ntt(a,false,sz),ntt(b,false,sz);
		for(int i=0;i<u;i++)a[i]=op(a[i],b[i])%M;
		ntt(a,1,sz);
		ll invN=modpow(u,M-2,M);
		for(int i=0;i<m;i++)ret[i]=invN*a[i]%M;
		return ret;
	}
};