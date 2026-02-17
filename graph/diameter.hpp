// SPDX-License-Identifier: MIT
// Copyright (c) 2026 CleverElsie

#ifndef ELSIE_DIAMETER
#define ELSIE_DIAMETER
#include <algorithm>
#include <cstdint>
#include <list>
#include <vector>
namespace elsie{
template<class S>using vc=std::vector<S>;
template<class S>using vv=vc<vc<S>>;
// e:cost,to
// ret:|diameter|,from,to
template<class S,class T>
std::pair<int32_t,std::pair<int32_t,int32_t>> diameter(vv<std::pair<S,T>>&edge,int start=0){
  auto farthest=[&](vv<std::pair<S,T>>&Edge,int Start)->std::pair<int32_t,int32_t> {
    int32_t inf=INT32_MAX;
    vc<int32_t>dist(Edge.size(),-1);
    std::list<int32_t>q;
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
    std::vector<int32_t>::iterator res=std::max_element(dist.begin(),dist.end());
    return {*res,res-dist.begin()};
  };
  auto st=farthest(edge,start).second;
  auto[cost,gl]=farthest(edge,st);
  return {cost,{st,gl}};
}

int32_t diameter(vv<int>&edge,int start=0){
  vv<std::pair<int32_t,int32_t>>weighted_edge(edge.size());
  for(std::size_t i=0;i<edge.size();++i){
    weighted_edge[i].resize(edge[i].size());
    for(std::size_t j=0;j<edge[i].size();++j)
      weighted_edge[i][j]={1,edge[i][j]};
  }
  return diameter(weighted_edge,start).first;
}
}
#endif