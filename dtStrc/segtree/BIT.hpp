// SPDX-License-Identifier: MIT
// Copyright (c) 2026 CleverElsie

#ifndef ELSIE_SEGTREE
#define ELSIE_SEGTREE
#include<vector>
#include<cstdint>
#include<cstddef>
#include<utility>
namespace elsie{
  template<class type>class BIT{// 0-indexed [l,r)
    std::vector<type>v;
    type sum(int64_t r){
      type ret=0;
      for(;r;r-=r&-r) ret+=v[r-1];
      return ret;
    }
    void setup(auto&&v_){
      v=std::forward<std::vector<type>>(v_);
      v.resize(std::bit_ceil(v.size()),type());
    }
  public:
    BIT():v(0){}
    BIT(std::size_t size){ setup(std::vector<type>(size,type())); }
    BIT(const std::vector<type>&v_){ setup(v_); }
    BIT(std::vector<type>&&v_){ setup(std::move(v_)); }
    BIT(const BIT&b){ setup(std::vector<type>(b.v));}
    BIT(BIT&&b){ setup(std::move(b.v));}
    void add(std::size_t i,type val){ for(i++;i<=v.size();i+=i&-i) v[i-1]+=val; }
    type sum(std::size_t l,std::size_t r){ return sum(r)-sum(l); }
    type get(std::size_t i){ return sum(i,i+1); }
    void set(std::size_t i,type t){ add(i,t-get(i)); }
    BIT&operator=(const BIT&b){
      v=b.v;
      return*this;
    }
    BIT&operator=(BIT&&b){
      v=std::move(b.v);
      return*this;
    }
  };
}
#endif
