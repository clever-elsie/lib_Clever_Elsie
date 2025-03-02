#ifndef ELSIE_BASIC_MATH
#define ELSIE_BASIC_MATH
#include <vector>
#include <limits>
#include <cstdint>
#include <concepts>
#include <atcoder/modint>
namespace elsie{
	using namespace std;
	using namespace atcoder;

	template<integral T,integral U>
	inline auto ceil(const T a,const U b){return(a+b-1)/b;}
	template<integral T,integral U>
	inline auto floor(const T a,const U b){return a/b-(a%b&&(a^b)<0);}

	int64_t safepow(int64_t a,uint64_t b){
		int64_t ret=1,k=a;
		double check=1,kcheck=a;
		while(b){
			if(b&1){
				check*=k;
				if(check>INT64_MAX)return INT64_MAX;
				if(check<INT64_MIN)return INT64_MIN;
				ret*=k;
			}
			kcheck*=k;
			if(kcheck>INT64_MAX)return INT64_MAX;
			if(kcheck<INT64_MIN)return INT64_MIN;
			k*=k;
			b>>=1;
		}
		return ret;
	}

	int64_t modpow(int64_t a,uint64_t b,uint64_t mod){
		__int128_t k=a%mod,ret=1;
		if(k<0)k+=mod;
		while(b){
			ret=(b&1?ret*k%mod:ret);
			k=(k*k)%mod;
			b>>=1;
		}
		return int64_t(ret);
	}

	template<class S>S gcd(S a,S b){
		if(b==0) return a;
		S r=1;if(a<0)a=-a;if(b<0)b=-b;
		while(r) r=a%b,a=b,b=r;
		return a;
	}

	template<class S>S lcm(S a,S b){return a/gcd(a,b)*b;}

	template<class S>S egcd(S a,S b,S&x,S&y){
		if(b==0){
			x=1,y=0;
			return a;
		}
		S d=egcd(b,a%b,y,x);
		y-=a/b*x;
		return d;
	}

	template<class T>T exgcd(T a,T b,T&x,T&y){
		auto assign=[&](T&s,T&t,T u,T v)->void {s=u,t=v;};
		x=1,y=0;
		T u=0,v=1;
		while(b){
			T k=a/b;
			T r=a%b;
			assign(a,b,b,r);
			assign(x,u,u,x-u*k);
			assign(y,v,v,y-v*k);
		}
		if(a<0)a=-a,x=-x,y=-y;
		return a;
	}

	template<class T>T mod_inv(T a,T m){
		T x,y,g=exgcd(a,m,x,y);
		if(g!=1)return -1;
		return(x%m+m)%m;
	}

	template<class T>T mod_inv_prime(T a,T p){ return modpow(a,p-2,p); }

	template<class mint=modint998244353>
	class combination{
		private:
			using tint = long long;
			vector<mint>invprod,prod;
			vector<tint>inv;
			const unsigned long long M;
		public:
		combination(int n=1):invprod(2,1),prod(2,1),inv(2,0),M(mint(-1).val()+1){
			inv[1]=1;
			if(n>1)PreCalc(n);
		}
		void PreCalc(int n){
			int presize=inv.size();
			if(presize>=n+1)return;
			inv.resize(n+1);
			prod.resize(n+1);
			invprod.resize(n+1);
			for(int i=presize;i<=n;i++){
				prod[i]=prod[i-1]*i;
				inv[i]=(((M/i)*(-inv[M%i]))+M)%M;
				invprod[i]=invprod[i-1]*inv[i];
			}
		}
		mint factorial(size_t n){
			PreCalc(n);
			return prod[n];
		}
		mint invfact(size_t n){
			PreCalc(n);
			return invprod[n];
		}
		mint P(size_t n,size_t k){
			if(n<k) return 0;
			PreCalc(n);
			return prod[n]*invprod[n-k];
		}
		mint C(size_t n,size_t k){
			if(n<k) return 0;
			PreCalc(n);
			return prod[n]*invprod[n-k]*invprod[k];
		}
		mint H(size_t n,size_t k){
			if(n==0) return mint(k!=0);
			PreCalc(n);
			return C(n+k-1,k);
		}
	};

	using u128=__uint128_t;
	using u64 = uint64_t;
	inline __uint128_t mul64to128(u64 a,u64 b){
		u64 hi,lo;
		asm("mulq %3" :"=a"(lo),"=d"(hi):"a"(a),"r"(b));
		return((__uint128_t)(hi)<<64)+lo;
	}
	inline pair<u128,u128>mul128(u128 a,u128 b){
		constexpr static u128 Lfilter=0xFFFF'FFFF'FFFF'FFFFull;
		u64 x1=a>>64,x0=a&Lfilter;
		u64 y1=b>>64,y0=b&Lfilter;
		u128 z2=mul64to128(x1,y1),z1=mul64to128(x1,y0)+mul64to128(x0,y1),z0=mul64to128(x0,y0);
		u128 lower=z0+(z1<<64);
		return{z2+(z1>>64)+(lower<z0),lower};
	}

	class barret32{
		uint64_t M,iM;
		public:
		barret32():M(0),iM(0){}
		barret32(uint32_t m):M(m),iM(uint64_t(-1)/m+1){}
		void set(uint32_t m){ M=m,iM=uint64_t(-1)/m+1; }
		uint32_t umod()const{ return M; }
		uint32_t mul(uint32_t a,uint32_t b)const{
			uint64_t z=(uint64_t)a*b;
			uint64_t x=((__uint128_t)z*iM>>64);
			uint64_t y=x*M;
			return(uint32_t)(z-y+(z<y?M:0));
		}
	};

	class barret64{
		__uint128_t M,iM;
		public:
		barret64():M(0),iM(0){}
		barret64(uint64_t m):M(m),iM(__uint128_t(-1)/M+1){}
		uint64_t umod(){ return M; }
		uint64_t mul(uint64_t a,uint64_t b){
			__uint128_t z=mul64to128(a,b);
			__uint128_t x=mul128(z,iM).first;
			__uint128_t y=x*M;
			return(uint64_t)(z-y+(z<y?M:0));
		}
	};
}
#endif