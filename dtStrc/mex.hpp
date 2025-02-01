#include <cstddef>
#include <map>
#include <unordered_map>
#ifndef ELSIE_MEX
#define ELSIE_MEX
namespace elsie{
	using namespace std;
struct mex{
	map<int,int>v; // [l,r)
	unordered_map<int,int>c; // {val,cnt}
	mex(){v[-1]=0;}
	void add(int a){
		{ // count phase
			auto itr=c.find(a);
			if(itr!=c.end()){
				itr->second++;
				return;
			}else c[a]=1;
		}
		if(a<0)return;
		auto itr=v.upper_bound(a);
		itr--;
		if(a<itr->second)return;
		if(itr->second==a) itr->second++;
		else{
			v[a]=a+1;
			itr=v.find(a);
		}
		auto jtr=itr;
		jtr++;
		if(jtr==v.end())return;
		if(itr->second==jtr->first){
			itr->second=jtr->second;
			v.erase(jtr);
		}
	}
	void erase(int a){
		{ // count phase
			auto itr=c.find(a);
			if(itr==c.end())return;
			itr->second--;
			if(itr->second>0) return;
			c.erase(itr);
		}
		if(a<0)return;
		auto itr=v.upper_bound(a);
		itr--;
		if(itr->second-1==a){
			itr->second--;
			return;
		}
		v[a+1]=itr->second;
		itr->second=a;
	}
	int find()const{return v[-1];}
	size_t count(int a){
		auto itr=c.find(a);
		return(itr==c.end()?0ull:itr->se);
	}
};
}
#endif