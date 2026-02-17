// SPDX-License-Identifier: MIT
// Copyright (c) 2026 CleverElsie

#ifndef ELSIE_BIGINT_ADD_SUB
#define ELSIE_BIGINT_ADD_SUB
#include "dtStrc/math/integer/bigInt/bigInt_image.hpp"
namespace elsie{

template<bool is_sub>
void bigInt::adder(vu64&a,sv64&b){
  uint64_t carry=is_sub,sign; // 桁溢れを保持 減算のとき~x+1の1
  // |a=lhs.data|>=|b=rhs.data|より，短い方の長さを参照する
  // 最後は符号なので飛ばす．
  for(uint64_t i=0;i<b.size()-1;++i){
    __uint128_t add;
    if constexpr(is_sub) add=(__uint128_t)a[i]+(~b[i])+carry;
    else add=(__uint128_t)a[i]+b[i]+carry;
    a[i]=(u64)add;
    carry=(u64)(add>>64);
  }
  // 符号拡張
  if constexpr(is_sub)sign=~b.back();
  else sign=b.back();
  for(uint64_t i=rhs.data.size()-1;i<data.size()-1;++i){
    __uint128_t add=(__uint128_t)data[i]+sign+carry;
    data[i]=(uint64_t)add;
    carry=(uint64_t)(add>>64);
  }
  sign=is_sub?a.back()==sign:a.back()!=sign;
  if(sign){ // 符号
    // 異符号の加算, 同符号の減算では元のbitより情報量が増えることはない．
    // -1->0で桁溢れが発生するため，桁あふれするならば正の数である．
    // そうでないとき，負の数である．
    data.back()=carry?0:~(uint64_t)0;
  }else{
    // 同符号で桁溢れしたならば精度が足りない．
    // carryをそのまま加算し，符号拡張する
    data.back()+=carry;
    data.push_back(sign);
  }
  shrink_to_fit();
}
  
}// namespace elsie
#endif