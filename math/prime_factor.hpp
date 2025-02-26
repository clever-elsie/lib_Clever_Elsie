#ifndef ELSIE_PRIMITIVE_ROOT
#define ELSIE_PRIMITIVE_ROOT
#include <map>
#include <cmath>
#include <deque>
#include <random>
#include <vector>
#include <cstdint>
#include <numeric>
#include <unordered_set>
#include <atcoder/modint>
#include <math/basic_math.hpp>
namespace elsie{
	using namespace std;
	// O(\sqrt{n})
	constexpr uint64_t eular_totient(uint64_t n){
		uint64_t r=n;
		for(uint64_t i=2;i*i<=n;++i){
			if(n%i==0){
				r-=r/i;
				while(n%i==0)n/=i;
			}
		}
		if(n>1)r-=r/n;
		return r;
	}

	vector<uint64_t>quotients(uint64_t n){
		using u64=uint64_t;
		vector<u64>ret;
		for(int x=n;x>0;){
			u64 q=n/x;
			ret.push_back(q);
			x=n/(q+1);
		}
		return move(ret);
	}

	/**
	 * miller rabin
	 */
	constexpr bool is_prime(uint64_t p){
		if(p<=1)return false;
		for(const auto&s:{2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101}){
			if(p==s)return true;
			if(p%s==0)return false;
		}
		uint64_t d=(p-1)>>countr_zero(p-1);
		auto ok=[&](uint64_t a)-> bool {
			uint64_t y=modpow(a,d,p);
			uint64_t t=d;
			while(y!=1&&y!=p-1&&t!=p-1){
				y=__uint128_t(y)*y%p;
				t<<=1;
			}
			return y==p-1||t%2;
		};
		if(p<(1ull<<32)){
			for(uint64_t a:{2,7,61})
				if(!ok(a))return false;
		}else{
			for(uint64_t a:{2,325,9375,28178,450775,9780504,1795265022}){
				if(p<=a)return true;
				if(!ok(a))return false;
			}
		}
		return true;
	}

	/**
	 * pollard's rho O(n^{1/4})
	*/
	vector<uint64_t>factorize(uint64_t m){
		if(m==1)return {};
		if(is_prime(m))return{m};
		auto pollard_rho=[&](auto pollard_rho,uint64_t n)->uint64_t {
			if(~n&1)return 2;
			if(is_prime(n))return n;
			uint64_t x,y,ys,g,q,r,k,m=uint64_t(pow<double>(double(n),1.0/8.0))+1;
			for(uint64_t c=1;c<n;++c){
				auto f=[&](__uint128_t a){return(a*a+c)%n;};
				y=k=0;g=q=r=1;
				while(g==1){
					x=y;
					while(k<3*r/4)y=f(y),++k;
					while(k<r&&g==1){
						ys=y;
						for(uint64_t _=0;_<m&&_+k<r;++_)
							y=f(y),q=__uint128_t(q)*(x>y?x-y:y-x)%n;
						g=gcd(q,n);
						k+=m;
					}
					k=r;
					r<<=1;
				}
				if(g==n){
					g=1,y=ys;
					while(g==1) y=f(y),g=gcd(x>y?x-y:y-x,n);
				}
				if(g==n)continue;
				if(is_prime(g))return g;
				else if(is_prime(n/g))return n/g;
				else return pollard_rho(pollard_rho,g);
			}
			return 0;
		};
		vector<uint64_t>ret;
		ret.reserve(64);
		while(m>1&&!is_prime(m))
			for(uint64_t p=pollard_rho(pollard_rho,m);m%p==0;m/=p)
				ret.push_back(p);
		if(m>1)ret.push_back(m);
		sort(begin(ret),end(ret));
		return ret;
	}

	int64_t primitive_root(uint64_t n){
		if(n==2)return 1;
		if(n==3)return 2;
		vector<uint64_t> factor=factorize(n-1);
		factor.erase(unique(begin(factor),end(factor)),end(factor));
		random_device seed;
		mt19937_64 engine(seed());
		uniform_int_distribution<uint64_t>rng(2,n-2);
		for(uint64_t g=1,ok=1;1;g=rng(engine),ok=1){
			for(const auto&q:factor)
				if(modpow(g,(n-1)/q,n)==1){ok=0;break;}
			if(ok)return g;
		}
		return -1;
	}
}
#endif