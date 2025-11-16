#ifndef ELSIE_LCA
#define ELSIE_LCA
#include<iostream>
#include<vector>
#include<cstdint>
#include<concepts>
#include<type_traits>
#include<dtStrc/segtree/segtree.hpp>
namespace elsie{
static constexpr int LCA_INF = INT32_MAX;
struct idx_depth {
  int idx,depth;
  idx_depth(int i,int d):idx(i),depth(d){}
};
idx_depth LCA_OP(idx_depth a, idx_depth b){
  return a.depth<b.depth?a:b;
}
idx_depth LCA_E(){ return idx_depth(-1,LCA_INF); }
template<std::integral T>
class LCA{
  segtree<idx_depth,LCA_OP,LCA_E>seg;
  std::vector<std::vector<T>>e;
  std::vector<int>visit,vs1st,depth;
  void dfs(int now,int pre,int&cnt,int dep){
    depth[now]=std::min(depth[now],dep);
    vs1st[now]=visit.size();
    visit.push_back(now);
    for(const auto&to:e[now])if(to!=pre){
      cnt++;
      dfs(to,now,cnt,dep+1);
      visit.push_back(now);
    }
  }
  public:
  LCA(std::vector<std::vector<T>>edge,int root){
    e=std::move(edge);
    visit.reserve(e.size()*2);
    vs1st.resize(e.size(),LCA_INF);
    depth.resize(e.size(),LCA_INF);
    int cnt=0;
    dfs(root,-1,cnt,0);
    seg=segtree<idx_depth,LCA_OP,LCA_E>(visit.size());
    for(int i=0;i<visit.size();i++)
      seg.set(i,idx_depth(visit[i],depth[visit[i]]));
  }
  int get(int u,int v){
    int f=vs1st[u];
    int e=vs1st[v];
    if(f>e)std::swap(f,e);
    return seg.prod(f,e+1).idx;
  }
  int get_depth(int u){ return depth[u]; }
};
}
template<std::integral T>
using LCA = elsie::LCA<T>;
#endif