#ifndef ELSIE_MST
#define ELSIE_MST
#include <vector>
#include <queue>
#include <algorithm>
#include <utility>
#include "dtStrc/unionFind.hpp"
namespace elsie{
using namespace std;
template<class f>using vc=vector<f>;
template<class f>using vv=vc<vc<f>>;
template<class f>using pqg=priority_queue<f,vc<f>,greater<f>>;
// vector<cost,{from,to}> |E|!=n
template<class S,class T,class U>
size_t kruskal(const int n,vc<pair<S,pair<T,U>>>&e){
  unionFind uf(n);
  sort(e.begin(),e.end());
  size_t ans=0;
  for(const auto&[cost,ft]:e){
    auto [u,v]=ft;
    if(!uf.same(u,v)){
      uf.unite(u,v);
      ans+=cost;
    }
  }
  return ans;
}
template<class S,class T,class U>
pair<size_t,vc<int32_t>>kruskal(const int n,const vc<pair<S,pair<T,U>>>&e){ // cost, from, to
  unionFind uf(n);
  vc<pair<S,pair<int32_t,pair<T,U>>>>p(e.size());
  for(int i=0;i<e.size();i++) // cost, id, from, to
    p[i]=make_pair(e[i].first,make_pair(i,e[i].second));
  sort(p.begin(),p.end());
  size_t ans=0;
  vc<int32_t> ansv;
  for(const auto&[cost,ift]:p){
    auto [id,ft]=ift;
    auto [u,v]=ft;
    if(!uf.same(u,v)){
      uf.unite(u,v);
      ans+=cost;
      ansv.push_back(id);
    }
  }
  return {ans,ansv};
}

// v[from]={cost,to}
template<class S,class T>
size_t prim(const vv<pair<S,T>>&e){
  size_t ans=0;
  vc<bool>seen(e.size(),0);
  seen[0]=1;
  pqg<pair<S,T>>q;
  for(const auto&edge:e[0])q.push(edge);
  while(q.size()){
    auto[cost,to]=q.top();q.pop();
    if(seen[to])continue;
    seen[to]=1;
    ans+=cost;
    for(const auto&edge:e[to])q.push(edge);
  }
  return ans;
}
// v[from]={cost,to,id}
template<class S,class T,class U>
pair<size_t,vc<int32_t>>prim(const vv<pair<pair<S,T>,U>>&e){
  size_t ans=0;
  vc<int32_t>ansv;
  vc<bool>seen(e.size(),0);
  seen[0]=1;
  pqg<pair<pair<S,T>,int>>q;// cost,to,id
  for(const auto&edge:e[0])q.push(edge);
  while(q.size()){
    auto [E,id]=q.top();q.pop();
    auto [cost,to]=E;
    if(seen[to])continue;
    seen[to]=1;
    ans+=cost;
    ansv.push_back(id);
    for(const auto&edge:e[to])q.push(edge);
  }
  return {ans,ansv};
}
}
#endif