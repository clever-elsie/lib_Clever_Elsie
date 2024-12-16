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
	constexpr static ll psz = 3;
	constexpr static ll mods[psz]={998244353,(ll)(1e9+7),(ll)((1LL<<61)-1)};
	ll b[psz]={0};
	str src;
	ll trans[256][psz];
	vector<array<ll,psz>>hash,thash;
	ll mul(ll a,ll b,ll mod){return ll(__int128_t(a)*b%mod);}
	ll modpow(ll a,ll b,ll mod){
		__uint128_t k=(a%mod+mod)%mod,ret=1;
		while(b){
			ret=b&1?ret*k%mod:ret;
			k=k*k%mod;
			b>>=1;
		}
		return ll(ret);
	}
	ll spow(ll i,ll p){ return modpow(b[i],p,mods[i]); }
	void make_hash(const str&s,vector<array<ll,psz>>&h){
		h.resize(s.size()+1);
		h[0].fill(0ll);
		for(ll i=0;i<s.size();i++)
			for(int j=0;j<psz;j++)
				h[i+1][j]=((mul(h[i][j],b[j],mods[j])+trans[s[i]][j])%mods[j]+mods[j])%mods[j];
	}
	void gen_rand(){
		random_device seed;
		mt19937_64 gen(seed());
		for(int i=0;i<psz;i++)
			while(b[i]<1e3)
				b[i]=gen()%mods[i];
		for(int i=0;i<256;i++)
		for(int j=0;j<psz;j++)
			trans[i][j]=gen()%mods[j];
	}
	ll hash_value(ll l,ll r,ll i){
		ll ret=hash[r][i];
		ll L=mul(hash[l][i],spow(i,r-l),mods[i]);
		ret=(ret-L)%mods[i];
		ret=(ret+mods[i])%mods[i];
		return ret;
	}
	public:
	rollingHash():src(""){gen_rand();}
	rollingHash(const str&SRC):src(SRC){gen_rand(),make_hash(src,hash);}
	void set(const str&SRC){src=SRC,make_hash(src,hash);}
	/**
	 * @brief return all matched string's first character's index in src
	 * @brief verified with AOJ https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/14/ALDS1_14_B
	 */
	vector<size_t>find(const str&tar){
		const size_t len = tar.size();
		vector<size_t> ret;
		array<ll,psz>h;
		make_hash(tar,thash);
		for(int i=0;i<psz;i++)h[i]=thash.back()[i];
		for(ll j=len;j<src.size()+1;j++){
			bool fls=false;
			for(int i=0;i<psz;i++)
				if(fls=h[i]!=hash_value(j-len,j,i))break;
			if(!fls) ret.push_back(j-len);
		}
		return move(ret);
	}
	/**
	 * @brief [l,r)==[L,R)
	 * @brief verified with AtCoder https://atcoder.jp/contests/tessoku-book/submissions/me
	 */
	bool same(int l,int r,int L,int R){
		if(r-l!=R-L)return false;
		ll n=R-L;
		for(int i=0;i<psz;i++)
			if(hash_value(l,r,i)!=hash_value(L,R,i))
				return false;
		return true;
	}
};