#ifndef ELSIE_DIAMETER
#define ELSIE_DIAMETER
#include <algorithm>
#include <cstdint>
#include <list>
#include <vector>
namespace elsie{
using namespace std;
template<class S>using vc=vector<S>;
template<class S>using vv=vc<vc<S>>;
// e:cost,to
// ret:|diameter|,from,to
template<class S,class T>
pair<int32_t,pair<int32_t,int32_t>> diameter(vv<pair<S,T>>&edge,int start=0){
	auto farthest=[&](vv<pair<S,T>>&Edge,int Start)->pair<int32_t,int32_t> {
		int32_t inf=INT32_MAX;
		vc<int32_t>dist(Edge.size(),-1);
		list<int32_t>q;
		q.push_back(Start);
		dist[Start]=0;
		while(!q.empty()){
			int32_t here=q.front();q.pop_front();
			for(const auto&[to_cost,to_id]:Edge[here]){
				if(dist[to_id]==-1){
					dist[to_id]=dist[here]+to_cost;
					q.push_back(to_id);
				}
			}
		}
		vector<int32_t>::iterator res=max_element(begin(dist),end(dist));
		return {*res,res-dist.begin()};
	};
	auto st=farthest(edge,start).second;
	auto[cost,gl]=farthest(edge,st);
	return {cost,{st,gl}};
}

int32_t diameter(vv<int>&edge,int start=0){
	vv<pair<int32_t,int32_t>>weighted_edge(edge.size());
	for(size_t i=0;i<edge.size();++i){
		weighted_edge[i].resize(edge[i].size());
		for(size_t j=0;j<edge[i].size();++j)
			weighted_edge[i][j]={1,edge[i][j]};
	}
	return diameter(weighted_edge,start).first;
}
}
#endif