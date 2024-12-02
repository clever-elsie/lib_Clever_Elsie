#include <map>

struct mex{
	std::map<int,int>v; // [l,r)
	std::map<int,int>c; // {val,cnt}
	mex(){
		v[-1]=0;
	}
	void add(int a){
		{ // count phase
			auto itr=c.find(a);
			if(itr!=c.end()){
				itr->second++;
				return;
			}else c[a]=1;
		}
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
		auto itr=v.upper_bound(a);
		itr--;
		if(itr->second-1==a){
			itr->second--;
			return;
		}
		v[a+1]=itr->second;
		itr->second=a;
	}
	int find(){
		return v[-1];
	}
};