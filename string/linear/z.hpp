#ifndef ELSIE_Z_ALGORITHM
#define ELSIE_Z_ALGORITHM
#include <vector>
#include <cstdint>
namespace elsie{
template<class T>
std::vector<uint32_t>z_algorithm(const T&s){
  std::vector<uint32_t>r(s.size(),0);
  r[0]=s.size();
  std::size_t i=1,j=0;
  while(i<s.size()){
    while(i+j<s.size()&&s[j]==s[i+j])++j;
    r[i]=j;
    if(j){
      std::size_t k=1;
      while(i+k<s.size()&&k+r[k]<j)r[i+k]=r[k],++k;
      i+=k,j-=k;
    }else ++i;
  }
  return r;
}
}
#endif