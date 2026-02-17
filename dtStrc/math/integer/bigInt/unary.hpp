// SPDX-License-Identifier: MIT
// Copyright (c) 2026 CleverElsie

#ifndef ELSIE_BIGINT_UNARY
#define ELSIE_BIGINT_UNARY
#include "dtStrc/math/integer/bigInt/bigInt_image.hpp"
namespace elsie{

void bigInt::bit_rev(){ for(auto&x:data)x=~x; }

void bigInt::negate(){
  bool sign=data.back();
  bit_rev();
  data.back()=sign?uint64_t(-1)-1:1;
  data.push_back(~sign);
  *this+=bigInt(1ul);
  shrink_to_fit();
}
} // namespace elsie
#endif