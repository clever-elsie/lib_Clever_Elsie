#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <vector>
using namespace std;

class rollingHash{
	using ll = long long;
	using vl = vector<ll>;
	using str = string;
	constexpr static ll psz = 4;
	constexpr static ll mods[psz]={998244353,998244383,(ll)(1e9+7),(ll)((1LL<<61)-1)};
	ll b[psz]={0};
	array<array<ll,256>,psz>trans;
	array<vl,psz>hash,thash;
	str src;
	ll mul(ll a,ll b,ll mod){
		__int128_t s=a;
		s=(s*b)%mod;
		return s;
	}
	ll modpow(ll a,ll b,ll mod){
		__uint128_t k=a%mod,ret=1;
		if(k<0)k+=mod;
		while(b){
			if(b&1)ret=(ret*k)%mod;
			k=(k*k)%mod;
			b>>=1;
		}
		return ll(ret);
	}
	ll spow(ll i,ll p){ return modpow(b[i],p,mods[i]); }
	void make_hash(const str&s,array<vl,psz>&h){
		for(int i=0;i<psz;i++){
			h[i].resize(s.size()+1,0ull);
			for(int j=0;j<s.size();j++){
				h[i][j+1]=(mul(h[i][j],b[i],mods[i])+trans[i][s[j]])%mods[i];
				if(h[i][j+1]<0)h[i][j+1]+=mods[i];
			}
		}
	}
	void make_rand(){
		random_device seed;
		mt19937_64 gen(seed());
		for(int i=0;i<psz;i++){
			while(b[i]<1e3) b[i]=gen()%mods[i];
			for(int j=0;j<256;j++)
				trans[i][j]=gen()%mods[i];
		}
	}
	public:
	rollingHash(const string&SRC):src(SRC){make_rand(),make_hash(src,hash);}
	rollingHash(string &&SRC):src(SRC){make_rand(),make_hash(src,hash);}
	void set(const string&SRC){src=SRC,make_hash(src,hash);}
	void set(string&&SRC){src=move(SRC),make_hash(src,hash);}

	vector<size_t>find(const string&tar){
		vector<size_t> ret;
		make_hash(tar,thash);
		array<ll,psz>h;
		const size_t len = tar.size();
		for(int i=0;i<psz;i++)h[i]=thash[i].back();
		for(ll j=len;j<src.size()+1;j++){
			bool all=true;
			for(int i=0;i<psz;i++){
				ll key=(hash[i][j]-mul(hash[i][j-len],spow(i,len),mods[i]))%mods[i];
				if(key<0)key+=mods[i];
				if(key!=h[i]){
					all=false;
					break;
				}
			}
			if(all) ret.push_back(j-len);
		}
		return move(ret);
	}
};