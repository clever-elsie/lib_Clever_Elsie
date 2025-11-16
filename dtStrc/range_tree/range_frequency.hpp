#ifndef ELSIE_RANGE_TREE
#define ELSIE_RANGE_TREE
#include <vector>
#include <cassert>
#include <cstdint>
#include <algorithm>
#include <unordered_map>
#include <dtStrc/binary_tree/rbtree.hpp>
namespace elsie{

  template<class S>class range_frequency{
    using it=int32_t;
    std::unordered_map<S,elsie::set<it>>v_idx;
    std::vector<S>va;
    public:
    range_frequency():va(0){}
    range_frequency(it n,const S&v):va(n,v){ for(it i=0;i<n;i++)v_idx[S()].insert(i); }
    range_frequency(const std::vector<S>&v):va(v){ for(std::size_t i=0;i<v.size();i++) v_idx[v[i]].insert(i); }
    void set(it idx,const S&v){
      v_idx[va[idx]].erase(idx);
      if(v_idx[va[idx]].size()==0)
        v_idx.erase(va[idx]);
      v_idx[v].insert(idx);
      va[idx]=v;
    }
    it cnt(it r,const S&x)const{
      if(auto itr=v_idx.find(x);itr!=v_idx.end()){
        auto jtr=itr->second.lower_bound(r);
        return(jtr==itr->second.end()?
          itr->second.size():
          itr->second.order_of_key(r));
      }return 0;
    }
    it cnt(it l,it r,const S&x)const{return cnt(r,x)-cnt(l,x); }
  };

  template<class S>class range_frequency_static{
    using it=int32_t;
    std::unordered_map<S,std::vector<it>>v_idx;
    public:
    range_frequency_static(){}
    range_frequency_static(const std::vector<S> &v){
      for(it i=0;i<v.size();i++)
        v_idx[v[i]].push_back(i);
    }
    it cnt(it r,const S&x)const{
      if(auto itr=v_idx.find(x);itr!=v_idx.end())
        return std::lower_bound(itr->second.begin(),itr->second.end(),r)-itr->second.begin();
      return 0;
    }
    it cnt(it l,it r,const S&x)const{return cnt(r,x)-cnt(l,x); }
  };
}
#endif