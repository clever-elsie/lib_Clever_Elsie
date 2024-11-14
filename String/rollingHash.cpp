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
	constexpr static ll psz = 6;
	constexpr static ll mods[psz]={998244353,998244383,(ll)(1e9+7),(ll)(1e9+9),(ll)(1e9+21),(ll)(1e9+33)};
	ll b[psz]={0};
	array<array<ll,256>,psz>trans;
	array<vector<ll>,psz>hash,thash;
	string src;
	ll spow(int i,int p){
		ll r=1,s=b[i];
		while(p){
			if(p&1) r=(r*s)%mods[i];
			p>>=1;
			s*=s;
		}
		return r;
	}
	void make_hash(const string&s,array<vector<ll>,psz>&h){
		for(int i=0;i<psz;i++){
			h[i].resize(s.size()+1,0ull);
			for(int j=0;j<s.size();j++){
				h[i][j+1]=(h[i][j]*b[i]+trans[i][s[j]])%mods[i];
				if(h[i][j+1]<0)h[i][j+1]+=mods[i];
			}
		}
	}
	void make_rand(){
		random_device seed;
		mt19937 gen(seed());
		for(int i=0;i<psz;i++){
			while(b[i]<1e3) b[i]=gen()%mods[i];
			for(int j=0;j<256;j++)
				trans[i][j]=gen()%mods[i];
		}
	}
	public:
	rollingHash(const string&SRC):src(SRC){
		make_rand();
		make_hash(src,hash);
	}
	rollingHash(string &&SRC){
		src=move(SRC);
		make_rand();
		make_hash(src,hash);
	}
	void set(const string&SRC){
		src=SRC;
		make_hash(src,hash);
	}
	void set(string&&SRC){
		src=move(SRC);
		make_hash(src,hash);
	}
	vector<size_t>find(const string&tar){
		vector<size_t> ret;
		make_hash(tar,thash);
		array<ll,psz>h;
		const size_t len = tar.size();
		for(int i=0;i<psz;i++)h[i]=thash[i].back();
		for(size_t j=len;j<src.size()+1;j++){
			bool all=true;
			for(int i=0;i<psz;i++){
				int key=(hash[i][j]-hash[i][j-len]*spow(i,len)%mods[i])%mods[i];
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
	void putv(){
		for(int i=0;i<psz;i++){
			for(int j=0;j<thash[i].size();j++)
				cout<<thash[i][j]<<" \n"[j==thash[i].size()-1];
			for(int j=0;j<hash[i].size();j++)
				cout<<hash[i][j]<<" \n"[j==hash[i].size()-1];
		}
	}
};