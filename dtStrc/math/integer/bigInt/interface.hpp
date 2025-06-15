#ifndef ELSIE_BIGINT_INTERFACE
#define ELSIE_BIGINT_INTERFACE
#include "dtStrc/math/integer/bigInt/bigInt_image.hpp"
namespace elsie{
string bigInt::to_string()const{
  bigInt cp=*this;
  constexpr uint64_t base=1'000'000'000;
  vector<uint32_t>digits;
  bool neg=cp.data.back();
  if(neg) cp=move(-cp);
  cp.data.pop_back();
  if(cp.data.size()==1&&cp.data[0]==0)return string("0");
  while(cp.data.size()){
    uint64_t remainder=0;
    for(int32_t i=(int32_t)cp.data.size()-1;i>=0;--i){
      __uint128_t cur=__uint128_t(remainder)<<64|cp.data[i];
      cp.data[i]=cur/base;
      remainder=cur%base;
    }
    digits.push_back((uint32_t)remainder);
    while(cp.data.size()&&cp.data.back()==0)cp.data.pop_back();
  }
  string r=std::to_string(digits.back());
  return r;
}
}// namespace elsie
#endif