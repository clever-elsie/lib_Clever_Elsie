#ifndef ELSIE_BIGINT_MEMORY
#define ELSIE_BIGINT_MEMORY
#include "dtStrc/math/integer/bigInt/bigInt_image.hpp"
namespace elsie{
void bigInt::shrink_to_fit(){
  while(data.size()>2){
    if(data.back()==data[data.size()-2])
      data.pop_back();
    else break;
  }
}

void bigInt::sign_expand(uint64_t len){
  if(data.size()<=len) return;
  data.resize(len);
}
}
#endif