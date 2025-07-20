#ifndef ELSIE_MEX
#define ELSIE_MEX
#include <map>
#include <cstddef>
#include <unordered_map>
#include <type_traits>
#include <string>
#include <cassert>
#include <initializer_list>
namespace elsie{

template<class T>
struct mex_traits;

template<class traits>
concept MEX_TRAITS=requires(traits x){
  typename traits::value_type;
  {traits::LOWER_BOUND}->std::convertible_to<typename traits::value_type>;
  {traits::LOWER_LIMIT}->std::convertible_to<typename traits::value_type>;
  {traits::pred}->std::invocable<typename traits::value_type,typename traits::value_type>;
  {traits::prev(traits::LOWER_LIMIT)}->std::convertible_to<typename traits::value_type>;
  {traits::next(traits::LOWER_BOUND)}->std::convertible_to<typename traits::value_type>;
};


template<std::signed_integral INT>
struct mex_traits<INT>{
  using value_type=INT;
  constexpr static INT LOWER_BOUND=-1;
  constexpr static INT LOWER_LIMIT=0;
  constexpr static std::less<INT> pred{};
  constexpr static INT prev(INT x){ return x-1; }
  constexpr static INT next(INT x){ return x+1; }
};

template<>
struct mex_traits<std::string>{
  using value_type=std::string;
  constexpr static std::string LOWER_BOUND="";
  constexpr static std::string LOWER_LIMIT="a";
  constexpr static struct{
    constexpr bool operator()(const std::string&lhs,const std::string&rhs)const{
      return lhs.size()!=rhs.size()?lhs.size()<rhs.size():lhs<rhs;
    }
  } pred{};
  constexpr static std::string prev(const std::string&v){
    assert(v.size());
    std::string ret(v);
    ssize_t i=v.size()-1;
    for(;i>=0;--i){
      if(ret[i]=='a') ret[i]='z';
      else{
        --ret[i];
        break;
      }
    }
    if(i==-1) ret.pop_back();
    return ret;
  }
  constexpr static std::string next(const std::string&v){
    std::string ret(v);
    size_t i=0;
    for(;i<ret.size();++i){
      if(ret[i]=='z') ret[i]='a';
      else{
        ++ret[i];
        break;
      }
    }
    if(i==ret.size()) ret.push_back('a');
    return ret;
  }
};


template<class T,class hash=std::hash<T>,class traits=mex_traits<T>>
requires MEX_TRAITS<traits>
struct mex{
  static_assert(traits::pred(traits::LOWER_BOUND,traits::LOWER_LIMIT));
  static_assert(traits::LOWER_BOUND==traits::prev(traits::LOWER_LIMIT));
  static_assert(traits::next(traits::LOWER_BOUND)==traits::LOWER_LIMIT);
  std::map<T,T,decltype(traits::pred)>v; // [l,r)
  std::unordered_map<T,size_t,hash>c; // {val,cnt}
  mex():v(traits::pred),c(){
    v[traits::LOWER_BOUND]=traits::next(traits::LOWER_BOUND);
  }
  mex(std::initializer_list<T>init):mex(){
    for(auto itr:init) add(itr);
  }
  mex(mex&&)=default;
  mex(const mex&)=default;
  ~mex()=default;
  mex& operator=(mex&&)=default;
  mex& operator=(const mex&)=default;
  void add(const T&value){
    { // count phase
      if(auto itr=c.find(value);itr!=c.end()){
        ++(itr->second);
        return;
      }else c[value]=1;
    }
    if(traits::pred(value,traits::LOWER_LIMIT))return;
    auto itr=v.upper_bound(value);
    --itr;
    if(traits::pred(value,itr->second))return;
    if(itr->second==value) itr->second=traits::next(itr->second);
    else{
      v[value]=traits::next(value);
      itr=v.find(value);
    }
    auto jtr=itr;
    ++jtr;
    if(jtr==v.end())return;
    if(itr->second==jtr->first){
      itr->second=jtr->second;
      v.erase(jtr);
    }
  }
  void erase(const T&value){
    { // count phase
      auto itr=c.find(value);
      if(itr==c.end())return;
      --(itr->second);
      if(itr->second>0) return;
      c.erase(itr);
    }
    if(traits::pred(value,traits::LOWER_LIMIT))return;
    auto itr=v.upper_bound(value);
    --itr;
    if(itr->second==traits::next(value)){
      itr->second=traits::prev(itr->second);
      return;
    }
    v[traits::next(value)]=itr->second;
    itr->second=value;
  }
  const T& find()const{return v.begin()->second;}
  size_t count(const T&a){
    auto itr=c.find(a);
    return(itr==c.end()?0ull:itr->second);
  }
};

}
#endif