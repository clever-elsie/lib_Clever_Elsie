#ifndef ELSIE_Z_ALGORITHM
#define ELSIE_Z_ALGORITHM
#include <vector>
#include <cstdint>
namespace elsie{
using namespace std;
template<class T>
vector<uint32_t>z_algorithm(const T&s){
  vector<uint32_t>r(s.size(),0);
  r[0]=s.size();
  size_t i=1,j=0;
  while(i<s.size()){
    while(i+j<s.size()&&s[j]==s[i+j])++j;
    r[i]=j;
    if(j){
      size_t k=1;
      while(i+k<s.size()&&k+r[k]<j)r[i+k]=r[k],++k;
      i+=k,j-=k;
    }else ++i;
  }
  return r;
}
}
#endif