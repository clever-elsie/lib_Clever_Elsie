// SPDX-License-Identifier: MIT
// Copyright (c) 2026 CleverElsie

#ifndef ELSIE_DAGSORT
#define ELSIE_DAGSORT
#include <vector>
#include <deque>
#include <set>
#include <utility>
namespace elsie{

template<class T>
std::vector<size_t>DAGsort(const std::vector<std::vector<T>>&edge){
  size_t n=edge.size();
  std::vector<std::set<T>>e(n);
  std::vector<size_t>deg(n,0);
  for(size_t i=0;i<n;i++)
    for(const auto&x:edge[i]){
      e[i].insert(x);
      ++deg[x];
    }
  size_t cnt=0;
  std::vector<size_t>ans(n,0);
  std::deque<size_t>q;
  for(size_t i=0;i<n;i++)
    if(!deg[i]){
      q.push_back(i);
      ans[i]=cnt++;
    }
  while(q.size()){
    size_t now=q.front();
    q.pop_front();
    for(const auto&to:e[now]){
      deg[to]--;
      if(deg[to]==0){
        q.push_back(to);
        ans[to]=cnt++;
      }
    }
  }
  return std::move(ans);
}
}
#endif