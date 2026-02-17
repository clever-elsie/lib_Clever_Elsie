// SPDX-License-Identifier: MIT
// Copyright (c) 2026 CleverElsie

#ifndef ELSIE_BASE_P_INTEGER
#define ELSIE_BASE_P_INTEGER
#include <vector>
#include <compare>
#include <cstddef>
#include <cstdint>
#include <type_traits>
namespace elsie{

using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::size_t;
using std::int32_t;

template<size_t p,class T>
struct int_base_base{
  static_assert(p>2,"int base p is expect p>2.");
  std::vector<T> data_;
  size_t v;
  void calc_base_p(){
    if(v==0){
      data_.assign(1,0);
      return;
    }
    size_t t=v;
    while(t){
      data_.push_back(t%p);
      t/=p;
    }
  }
  void incl(){
    bool c=true;
    ++v;
    for(auto&x:data_){
      if(x==p-1) x=0;
      else{
        ++x;
        c=false;
        break;
      }
    }
    if(c)data_.push_back(1);
  }
  public:
  int_base_base():data_(1,0),v(0){}
  int_base_base(int_base_base&&)=default;
  int_base_base(const int_base_base&)=default;
  int_base_base& operator=(int_base_base&&)=default;
  int_base_base& operator=(const int_base_base&)=default;

  int_base_base(size_t n):data_(),v(n){ calc_base_p(); }
  int_base_base& operator=(size_t n){
    v=n;
    data_.clear();
    calc_base_p();
  }
  
  T operator[](size_t idx)const{
    if(idx>=data_.size()) return 0;
    return data_[idx];
  }
  int_base_base& operator++(){
    incl();
    return*this;
  }
  int_base_base operator++(int32_t){
    int_base_base ret=*this;
    incl();
    return ret;
  }
  int_base_base& operator--(){
    assert(v);
    calc_base_p();
    return*this;
  }
  int_base_base operator--(int32_t){
    assert(v);
    int_base_base ret=*this;
    calc_base_p();
    return ret;
  }
  friend auto operator<=>(const int_base_base&lhs,const int_base_base&rhs){ return lhs.v<=>rhs.v; }
};

template<size_t p>
using int_base=
int_base_base<p,
  std::conditional_t<(p<=UINT8_MAX),
    uint8_t,
    std::conditional_t<(p<=UINT16_MAX),
      uint16_t,
      std::conditional_t<(p<=UINT32_MAX),
        uint32_t,
        uint64_t
      >
    >
  >
>;

} // namespace elsie
#endif
