#ifndef ELSIE_ROLLINGHASH
#define ELSIE_ROLLINGHASH
#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <set>
namespace elsie{
using namespace std;
template<size_t psz>
class rollingHash{
	protected:
	using ll = long long;
	using vl = vector<ll>;
	using str = string;
	const ll mods[3]={(ll)((1LL<<61)-1),998244353,(ll)(1e9+7)};
	ll b[psz];
	using arl = array<ll,psz>;
	str src;
	ll trans[256][psz];
	vector<arl>thash;
	private:
	vector<arl>hash;
	protected:
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
	void make_hash(const str&s,vector<arl>&h){
		h.resize(s.size()+1);
		h[0].fill(0ll);
		for(ll i=0;i<s.size();i++)
			for(int j=0;j<psz;j++)
				h[i+1][j]=((mul(h[i][j],b[j],mods[j])+trans[s[i]][j])%mods[j]+mods[j])%mods[j];
	}
	void gen_rand(){
		random_device seed;
		mt19937_64 gen(seed());
		for(int i=0;i<psz;i++)do{
			b[i]=gen()%mods[i];
		}while(b[i]<1e3);
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
	pair<arl,size_t>hassing(const str&tar){
		arl h;
		make_hash(tar,thash);
		for(int i=0;i<psz;i++)h[i]=thash.back()[i];
		return move(make_pair(move(h),tar.size()));
	}
	/**
	 * @brief return all matched string's first character's index in src
	 * @brief verified with AOJ https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/14/ALDS1_14_B
	 */
	vector<size_t>find(const arl&h,const size_t len){
		vector<size_t>ret;
		for(ll j=len;j<src.size()+1;j++){
			bool fls=false;
			for(int i=0;i<psz;i++)
				if(fls=h[i]!=hash_value(j-len,j,i))break;
			if(!fls) ret.push_back(j-len);
		}
		return move(ret);
	}
	vector<size_t>find(const str&tar){ return find(hassing(tar)); }
	vector<size_t>find(const size_t len,const arl&h){return find(h,len);}
	vector<size_t>find(const pair<arl,size_t>&hashed){ return find(hashed.first,hashed.second); }
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

class rolHashSeg{
private: // don't write
	using ll = long long;
	using S = pair<ll,ll>;
	vector<S> v;
	array<ll,256>trans;
	size_t n,tl,tr;
	ll B,mod;
	ll modpow(ll a,ll b){
		__uint128_t k=a%mod,ret=1;
		while(b){
			if(b&1)ret=(ret*k)%mod;
			k=k*k%mod;
			b>>=1;
		}
		return ll(ret);
	}
	ll mul(ll a,ll b){ return ll(__int128_t(a)*b%mod); }
	S op(S a,S b){ return S((mul(a.first,modpow(B,b.second))+b.first)%mod,a.second+b.second); }
	S e(){return S(0,0);}
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
	ll from0(size_t r) {
		tl = 0, tr = r;
		return prod_internal(0, n, 1).first;
	}
public:
	rolHashSeg(){}
	rolHashSeg(const string&s,int b,int MOD):n(1),B(b),mod(MOD){
		size_t N=s.size()+1;
		while(n<N)n<<=1;
		v.resize(n+n,e());
		random_device seed;
		mt19937_64 gen(seed());
		uniform_int_distribution<ll>(1ll,(1ll<<61)-1);
		for(int i=0;i<256;i++) trans[i]=gen()%mod;
		for(size_t i=n,j=0;j<s.size();) v[++i]={trans[s[j++]],1};
		for(size_t i=n;i;i--){
			int ch1=i<<1,ch2=ch1+1;
			v[i]=op(v[ch1],v[ch2]);
		}
	}
	void set(size_t p,char val){
		v[n+p+1].first=trans[val];
		for(size_t i=(n+p+1)>>1;i;i>>=1){
			int ch1=i<<1,ch2=ch1+1;
			v[i]=op(v[ch1],v[ch2]);
		}
	}
	ll prod(size_t l,size_t r){
		ll L=from0(l+1);
		ll R=from0(r+1);
		ll len = r-l;
		return ((R-mul(L,modpow(B,len)))%mod+mod)%mod;
	}
	ll hash_value(const string&t){
		ll ret=0;
		for(const auto&x:t)
			ret=(mul(x,B)+trans[x])%mod;
		return ret;
	}
};

template<size_t psz>
class dynamicRollingHash{
	using ll = long long;
	const ll mods[3]={ll(1ll<<61)-1,998244353,1'000'000'007};
	rolHashSeg seg[psz];
	ll n;
	public:
	dynamicRollingHash(const string&s){
		size_t b=0;
		n=s.size();
		random_device seed;
		mt19937_64 gen(seed());
		for(int i=0;i<psz;i++){
			do{
				b=gen()%mods[i];
			}while(b<1e3);
			seg[i]=rolHashSeg(s,b,mods[i]);
		}
	}
	vector<size_t>find(const string&t){
		std::set<size_t>st;
		ll hash=seg[0].hash_value(t);
		ll len=t.size();
		for(int i=len;i<n+1;i++){
			ll key = seg[0].prod(i-len,i);
			if(key==hash)st.insert(i);
		}
		for(int j=1;j<psz;j++){
			hash=seg[j].hash_value(t);
			for(auto itr=st.begin();itr!=st.end();){
				ll key=seg[j].prod(*itr-len,*itr);
				if(key!=hash)itr=st.erase(itr);
				else itr++;
			}
		}
		vector<size_t>ret;
		for(const auto&x:st)ret.push_back(x);
		return move(ret);
	}
	bool same(int l,int r,int L,int R){
		if(r-l!=R-L)return false;
		bool all=true;
		for(int j=0;j<psz;j++)
			if(seg[j].prod(l,r)!=seg[j].prod(L,R))
				return false;
		return true;
	}
	void set(size_t idx,char c){
		for(int i=0;i<psz;i++)
			seg[i].set(idx,c);
	}
};
}
#endif