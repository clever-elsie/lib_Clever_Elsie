#ifndef ELSIE_DETECT_CYCLE
#define ELSIE_DETECT_CYCLE
#include <functional>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <cstddef>
namespace elsie{

template<class S>
std::vector<int32_t>find_cycle_directed(const std::vector<std::vector<S>>&e){
  size_t n=e.size();
  std::vector<bool> seen(n,false);
  std::set<int32_t>now_path;
  std::vector<int32_t>cycle(0);cycle.reserve(n);
  auto dfs=[&](auto dfs,int32_t now)->int32_t {
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
  if(cycle.size())std::reverse(cycle.begin(),cycle.end());
  return std::move(cycle);
}

// e_id[id]={from,to}
template<class S,class T,class U>
std::vector<int32_t>reconstruct_edge(const std::vector<U>&v,const std::vector<std::map<S,T>>&e_id){
  std::vector<int32_t>ans(v.size()-1);
  for(int32_t i=0;i+1<v.size();++i)
    ans[i]=e_id[v[i]].find(v[i+1])->second;
  return std::move(ans);
}
}
#endif