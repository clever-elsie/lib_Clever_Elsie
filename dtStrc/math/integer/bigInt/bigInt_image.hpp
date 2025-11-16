#ifndef ELSIE_BIG_INT
#define ELSIE_BIG_INT
#include <span>
#include <vector>
#include <string>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <concepts>
#include <iostream>
#include <type_traits>
namespace elsie{
class bigInt{
private:
template<class T>using vc=std::vector<T>;
using i32=int32_t; using u32=uint32_t;
using i64=int64_t; using u64=uint64_t;
using vu32=vc<u32>; using vu64=vc<u64>;
using cv32=const vc<u32>;
using cv64=const vc<u64>;
using sv64=std::span<const u64>;
using mv64=std::span<u64>;
private:
// dataの最後の要素は符号拡張のみで値ではない．
vu64 data;
bigInt(vu64&&vdata):data(std::move(vdata)){}
public:
bigInt():data(2,0){}
bigInt(bigInt&&b):data(std::move(b.data)){}
bigInt(const bigInt&b):data(b.data){}
bigInt(const std::string&s){
// 未実装
}
template<class T>
bigInt(T x) requires std::signed_integral<T> || std::same_as<std::remove_cvref_t<T>,__int128_t>
:data(2+(sizeof(T)==16),0){
  if constexpr(sizeof(T)<=8){
    data[0]=(u64)((i64)x);
    data[1]=(u64)(x>>(sizeof(T)*8-1)&1?-1:0);
  }else{
    data[0]=(u64)x;
    data[1]=(u64)(x>>64);
    data[2]=(u64)(u64(x>>127&1?-1:0));
  }
}
template<std::unsigned_integral T>
bigInt(T x) requires std::unsigned_integral<T> || std::same_as<std::remove_cvref_t<T>,__uint128_t>
:data(2+(sizeof(T)==16),0){
  data[0]=(u64)x;
  if constexpr(sizeof(T)>8) data[1]=(u64)(x>>64);
}
// 適切にコンストラクタを呼ぶためこれでよい
template<class T>
bigInt& operator=(T&&rhs)requires std::is_constructible_v<bigInt,std::decay_t<T>>{
  if constexpr(is_bigInt<T>){
    if constexpr(is_movable_bigInt<T>)
      data=std::move(rhs.data);
    else data=rhs.data;
  }else*this=bigInt(std::forward<T>(rhs));
  return*this;
}

std::string to_string()const; // interface
private:
template<class T> static constexpr
bool is_bigInt=std::is_same_v<std::remove_cvref_t<T>,bigInt>;

template<class T>static constexpr
bool is_copy_needed_bigInt=is_bigInt<T>&&(std::is_lvalue_reference_v<T>||std::is_const_v<std::remove_reference_t<T>>);

template<class T> static constexpr
bool is_movable_bigInt=is_bigInt<T>&&!is_copy_needed_bigInt<T>;

// a<b -1, a==b 0, a>b 1
inline i32 compare(cv64&a,cv64&b)const; // compare
inline void bit_rev(); // unary
inline void negate(); // unary
void shrink_to_fit(); // memory
void sign_expand(uint64_t len); // memory

template<bool is_sub> void adder(vu64&a,sv64&b); // add_sub
static constexpr std::array<std::size_t,3>Lp{64,250,100000};
void naive_mul(sv64 a,sv64 b,mv64 r); // mul
void karatsuba(sv64 a,sv64 b,mv64 r); // mul
void ntt_cnvlt(sv64 a,sv64 b,mv64 r); // mul
void multiply(cv64&a,cv64&b); // mul
public:
// arithmetic operation

bigInt operator+()const noexcept{return*this;}
bigInt operator-()const noexcept{
  vu64 d(data.size());
  __uint128_t carry=1;
  for(size_t i=0;i<d.size()-1;++i){
    carry+=~data[i];
    d[i]=(u64)carry;
    carry>>=64;
  }
  if(carry&&data.back()==0) d.back()=0; // 0
  else d.back()=~data.back();
  return bigInt(move(d));
}
template<class T>
bigInt& operator+=(T&&rhs)requires std::is_constructible_v<bigInt,std::decay_t<T>> {
  // bigInt以外では構築
  if constexpr(!is_bigInt<T>) return*this+=bigInt(std::forward<T>(rhs));
  if(rhs.data.size()>data.size()) // 自分のほうが短いときに場所を確保する．
    if constexpr (is_copy_needed_bigInt<T>) // rhsが左辺値なら拡張確定
      data.resize(rhs.data.size(),data.back()); // 符号拡張
    else std::swap(data,rhs.data); // movableなrhsが長いなら入れ替え
  adder<false>(data,sv64(rhs.data.begin(),rhs.data.end()));
  return*this;
}
template<class T>
bigInt& operator-=(T&&rhs)requires std::is_constructible_v<bigInt,std::decay_t<T>> {
  if constexpr(!is_bigInt<T>)return*this-=bigInt(std::forward<T>(rhs));
  if(rhs.data.size()>data.size()) // 自分のほうが短いときに場所を確保する．
    data.resize(rhs.data.size(),data.back()); // 符号拡張
  adder<true>(data,sv64(rhs.data.begin(),rhs.data.end()));
  return*this;
}
template<class T>
bigInt& operator*=(T&&rhs)requires std::is_constructible_v<bigInt,std::decay_t<T>> {
  if constexpr(!is_bigInt<T>)*this*=bigInt(std::forward<T>(rhs));
  else{
    size_t len=data.size()+rhs.data.size()-1;
    sign_expand(len);
    bigInt Rhs;
    if constexpr(is_movable_bigInt<T>(rhs))
      Rhs=std::move(rhs);
    else Rhs=rhs;
    Rhs.sign_expand(len);

    multiply(data,rhs.data);
  }
  return*this;
}
template<class T>
bigInt& operator/=(T&&rhs)requires std::is_constructible_v<bigInt,std::decay_t<T>> {
  return*this;
}
template<class T>
bigInt& operator%=(T&&rhs)requires std::is_constructible_v<bigInt,std::decay_t<T>> {
  return*this;
}
//前置
bigInt& operator++(){ return*this+=bigInt((u64)1ul); }
bigInt& operator--(){
  return*this-=bigInt((i64)(-1));
}
//後置
bigInt operator++(int32_t){
}
bigInt operator--(int32_t){
}

template<class S,class T>
friend bigInt operator+(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& std::is_constructible_v<bigInt,std::decay_t<T>>
&& std::is_constructible_v<bigInt,std::decay_t<T>> {
  if constexpr(is_bigInt<S>&&is_movable_bigInt<T>)
    return rhs+=std::forward<T>(lhs);
  else if constexpr(is_bigInt<T>&&is_movable_bigInt<S>)
    return lhs+=std::forward<S>(rhs);
  else return bigInt(std::forward<S>(lhs))+=std::forward<T>(rhs);
}
template<class S,class T>
friend bigInt operator-(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& std::is_constructible_v<bigInt,std::decay_t<T>>
&& std::is_constructible_v<bigInt,std::decay_t<T>> {
  if constexpr(is_bigInt<S>&&is_movable_bigInt<T>)
    return rhs-=std::forward<T>(lhs);
  else if constexpr(is_bigInt<T>&&is_movable_bigInt<S>)
    return lhs+=std::forward<S>(rhs);
  else return bigInt(std::forward<S>(lhs))+=std::forward<T>(rhs);
}
template<class S,class T>
friend bigInt operator*(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& std::is_constructible_v<bigInt,std::decay_t<T>>
&& std::is_constructible_v<bigInt,std::decay_t<T>> {
  return lhs; //dammy
}
template<class S,class T>
friend bigInt operator/(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& std::is_constructible_v<bigInt,std::decay_t<T>>
&& std::is_constructible_v<bigInt,std::decay_t<T>> {
  return lhs; // dammy
}
template<class S,class T>
friend bigInt operator%(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& std::is_constructible_v<bigInt,std::decay_t<T>>
&& std::is_constructible_v<bigInt,std::decay_t<T>> {
  return lhs; // dammy
}

// bit operation
bigInt operator~()const{
  bigInt r=*this;
  r.bit_rev();
  return r;
}
template<class T>
bigInt& operator<<=(T&&rhs)requires std::is_constructible_v<bigInt,std::decay_t<T>> {
  return*this;
}
template<class T>
bigInt& operator>>=(T&&rhs)requires std::is_constructible_v<bigInt,std::decay_t<T>> {
  return*this;
}
template<class T>
bigInt& operator&=(T&&rhs)requires std::is_constructible_v<bigInt,std::decay_t<T>> {
  return*this;
}
template<class T>
bigInt& operator|=(T&&rhs)requires std::is_constructible_v<bigInt,std::decay_t<T>> {
  return*this;
}
template<class T>
bigInt& operator^=(T&&rhs)requires std::is_constructible_v<bigInt,std::decay_t<T>> {
  return*this;
}

// logic operation
bool operator!()const noexcept{}
template<class S,class T>
friend bool operator<(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& std::is_constructible_v<bigInt,std::decay_t<T>>
&& std::is_constructible_v<bigInt,std::decay_t<T>> {
}
template<class S,class T>
friend bool operator>(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& std::is_constructible_v<bigInt,std::decay_t<T>>
&& std::is_constructible_v<bigInt,std::decay_t<T>> {
}
template<class S,class T>
friend bool operator==(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& std::is_constructible_v<bigInt,std::decay_t<T>>
&& std::is_constructible_v<bigInt,std::decay_t<T>> {
}
template<class S,class T>
friend bool operator!=(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& std::is_constructible_v<bigInt,std::decay_t<T>>
&& std::is_constructible_v<bigInt,std::decay_t<T>> {
}
template<class S,class T>
friend bool operator<=(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& std::is_constructible_v<bigInt,std::decay_t<T>>
&& std::is_constructible_v<bigInt,std::decay_t<T>> {
}
template<class S,class T>
friend bool operator>=(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& std::is_constructible_v<bigInt,std::decay_t<T>>
&& std::is_constructible_v<bigInt,std::decay_t<T>> {
}
template<class S,class T>
friend bool operator&&(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& std::is_constructible_v<bigInt,std::decay_t<T>>
&& std::is_constructible_v<bigInt,std::decay_t<T>> {
}


private: // friends
template<class Char,class Traits> friend struct std::basic_ostream;
template<class Char,class Traits> friend struct std::basic_istream;
};
} // namespace elsie

namespace std{ // 出力
template<class Char,class Traits>
std::basic_ostream<Char,Traits>&operator<<(std::basic_ostream<Char,Traits>&os,const elsie::bigInt&v){
}
template<class Char,class Traits>
std::basic_istream<Char,Traits>&operator<<(std::basic_istream<Char,Traits>&os,elsie::bigInt&v){
}
}
#endif