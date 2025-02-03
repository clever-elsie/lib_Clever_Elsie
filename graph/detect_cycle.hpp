#include <functional>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <cstddef>
#ifndef ELSIE_DETECT_CYCLE
#define ELSIE_DETECT_CYCLE
namespace elsie{
using namespace std;
template<class S>using vc=vector<S>;
template<class S>using vv=vc<vc<S>>;
template<class S>vc<int32_t>find_cycle_directed(const vv<S>&e){
	size_t n=e.size();
	vc<bool> seen(n,false);
	set<int32_t>now_path;
	vc<int32_t>cycle(0);cycle.reserve(n);
	auto dfs=[&](auto dfs,int32_t now)->int {
		for(const auto&to:e[now]){
			if(now_path.contains(to)){
				cycle.push_back(to);
				cycle.push_back(now);
				return to;
			}else if(!seen[to]){
				now_path.insert(to);
				seen[to]=true;
				int v=dfs(dfs,to);
				if(v!=-1){
					if(v<-1)return v;
					cycle.push_back(now);
					return (v==now?-2:v);
				}
			}
		}
		now_path.erase(now);
		return -1;
	};
	for(int32_t i=0;i<n;++i)if(!seen[i]){
		now_path.clear();
		now_path.insert(i);
		seen[i]=true;
		dfs(dfs,i);
		if(cycle.size())break;
	}
	if(cycle.size())reverse(cycle.begin(),cycle.end());
	return move(cycle);
}

// e_id[id]={from,to}
template<class S,class T,class U>
vc<int32_t>reconstruct_edge(const vc<U>&v,const vc<map<S,T>>&e_id){
	vc<int32_t>ans(v.size()-1);
	for(int32_t i=0;i+1<v.size();++i)
		ans[i]=e_id[v[i]].find(v[i+1])->second;
	return move(ans);
}
}
#endif