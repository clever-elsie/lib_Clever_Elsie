#ifndef ELSIE_BIGINT_COMPARE
#define ELSIE_BIGINT_COMPARE
#include "dtStrc/math/integer/bigInt/bigInt_image.hpp"
namespace elsie{

int32_t bigInt::compare(cv64&a,cv64&b)const{
  // .back()は符号
  if(a.back()!=b.back()) return a.back()?-1:1;
  if(a.size()!=b.size()) return a.size()<b.size()?-1:1;
  // 負の数のとき，判定は逆転する
  i32 ret=a.back()?1:-1;
  for(u64 i=a.size()-2;i>=0;--i){
    if(a[i]==b[i])continue;
    return a[i]<b[i]?ret:-ret;
  }
  return 0;
}

} // namespace elsie
#endif