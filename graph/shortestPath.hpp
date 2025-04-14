#ifndef ELSIE_SHORTEST_PATH
#define ELSIE_SHORTEST_PATH
#include <vector>
#include <queue>
#include <algorithm>
#include <utility>
#include <cstdint>
#include <limits>
namespace elsie{
using namespace std;
template<class f>using vc=vector<f>;
template<class f>using vv=vc<vc<f>>;
template<class f>using pqg=priority_queue<f,vc<f>,greater<f>>;

// weighted graph's dijkstra
template<class ll>
vc<ll>dijkstra(const vv<pair<ll,ll>>&edge,ll start=0){
  constexpr size_t int_width=sizeof(int);
  vc<ll>dist(edge.size(),1ll<<int_width*8-int_width/2);//16->15, 32->30, 64->60
  pqg<pair<ll,int32_t>>q;
  dist[start]=0;
  q.emplace(dist[start],start);
  while(!q.empty()) {
    auto[now_dist,now]=q.top();q.pop();
    if(now_dist>dist[now])continue;
    for(const auto&[e_cost,to]:edge[now]){
      if(e_cost+now_dist<dist[to]){
        dist[to]=e_cost+now_dist;
        q.emplace(dist[to],to);
      }
    }
  }
  return move(dist);
}
// [dist,path]
template<class ll>pair<vc<ll>,vc<int32_t>> dijkstra(const vv<pair<ll,ll>>&edge,int start,int t){
  constexpr size_t int_width=sizeof(ll);
  vc<ll>dist(edge.size(),1ll<<int_width*8-int_width/2);//16->15, 32->30, 64->60
  vc<int32_t>pre(edge.size(),-1);
  pqg<pair<ll,int32_t>>q;
  dist[start]=0;
  q.emplace(dist[start],start);
  while (!q.empty()) {
    auto[now_dist,now]=q.top();q.pop();
    if(now_dist>dist[now])continue;
    for(const auto&[e_cost,to]:edge[now]){
      if(e_cost+now_dist<dist[to]){
        dist[to]=e_cost+now_dist;
        pre[to]=now;
        q.emplace(dist[to],to);
      }
    }
  }
  vc<int32_t>p(1,t);
  while(t!=start){
    t=pre[t];
    p.push_back(t);
  }
  reverse(p.begin(),p.end());
  return {dist,p};
}

// weight matrix dijkstra
template<class ll>vc<ll>dijkstra(const vv<ll>&edge,int start=0){
  constexpr size_t int_width=sizeof(int);
  vc<ll> dist(edge.size(),1ll<<int_width*8-int_width/2);
  pqg<pair<ll,int32_t>>q;
  dist[start]=0;
  q.emplace(dist[start],start);
  while(q.size()){
    auto[cost,now]=q.top();q.pop();
    if(cost>dist[now])continue;
    for(int32_t to=0;to<edge.size();++to){
      if(ll next_cost=dist[now]+edge[now][to];next_cost<dist[to]){
        dist[to]=next_cost;
        q.emplace(dist[to],to);
      }
    }
  }
  return dist;
}

/**
 * @return dist:vc, has_negative_cycle:bool
 */
template<class ll>pair<vc<ll>,bool>
BF(vv<pair<ll,ll>>&e,int start=0){
  constexpr ll inf=numeric_limits<ll>::max();
  bool has_renew;
  vc<ll>dist(e.size(),inf);
  dist[start]=0;
  for(int _=0;_<e.size();++_){
    has_renew=false;
    for(int i=0;i<e.size();++i)
    for(const auto&[cost,to]:e[i])
      if(dist[i]<inf&&dist[i]+cost<dist[to])
        has_renew=1,
        dist[to]=dist[i]+cost;
  }
  return {move(dist),has_renew};
}

template<class ll>void WF(vv<ll>&e)noexcept(false){
  size_t n=e.size();
  for(size_t k=0;k<n;++k)
  for(size_t i=0;i<n;++i)
  for(size_t j=0;j<n;++j)
    e[i][j]=min(e[i][j],e[i][k]+e[k][j]);
  for(size_t i=0;i<n;++i)
    if(e[i][i]<0)throw(int64_t)(-1);
}
}
#endif