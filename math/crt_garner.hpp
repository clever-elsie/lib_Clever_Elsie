#ifndef ELSIE_CRT_GARNER
#define ELSIE_CRT_GARNER
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <numeric>
#include "math/basic_math.hpp"
namespace elsie{
// math/basic_math.cpp: elsie::exgcd();
template<std::integral T>std::pair<T,T>crt(const std::vector<T>&b,const std::vector<T>&m){
  T r=0,M=1;
  for(std::size_t i=0;i<b.size();++i){
    T p,q,d=exgcd(M,m[i],p,q);
    T qo=(b[i]-r)/d;
    T rem=(b[i]-r)%d;
    if(rem)return {0,0};
    T tmp=qo*p%(m[i]/d);
    r+=M*tmp;
    M*=m[i]/d;
  }return{(r%M+M)%M,M};
}

/**
 * any pair each of all m is gcd(m[i],m[j])=1
 */
template<class T,class U>U garner(const std::vector<T>&b,std::vector<T>&m,U mod)
requires std::is_integral_v<T>||std::same_as<T,__int128_t>||std::same_as<T,__uint128_t>{
  using int_s=std::conditional_t<(sizeof(U)>8),__int128_t,int64_t>;
  using vc=std::vector<int_s>;
  constexpr __uint128_t threshold=__uint128_t(1)<<64;
  m.push_back(mod);
  vc coeffs(m.size(),1),constants(m.size(),0);
  if(threshold>=mod){
    for(std::size_t k=0;k<b.size();++k){
      int_s tmp=b[k]>=constants[k]?b[k]-constants[k]:(b[k]+m[k])-constants[k];
      int_s t=tmp*mod_inv<int_s>(coeffs[k],m[k])%m[k];
      for(std::size_t i=k+1;i<m.size();++i){
        constants[i]=(t*coeffs[i]%m[i]+constants[i])%m[i];
        coeffs[i]=coeffs[i]*m[k]%m[i];
      }
    }
  }else{
    for(std::size_t k=0;k<b.size();++k){
      int_s tmp=b[k]>=constants[k]?b[k]-constants[k]:(b[k]+m[k])-constants[k];
      int_s t=tmp*mod_inv<int_s>(coeffs[k],m[k])%m[k];
      for(std::size_t i=k+1;i<m.size();++i){
        if constexpr(sizeof(T)>8){
          constants[i]=(constants[i]+rem256(mul128(t,coeffs[i]),m[i]))%m[i];
          coeffs[i]=rem256(mul128(coeffs[i],m[k]),m[i]);
        }else{
          constants[i]=(t*coeffs[i]%m[i]+constants[i])%m[i];
          coeffs[i]=coeffs[i]*m[k]%m[i];
        }
      }
    }
  }
  m.pop_back();
  return constants.back();
}

template<std::integral T>T decomposite_garner(std::vector<T>&b,std::vector<T>&m,T mod){
  std::size_t zerocnt=0;
  for(const auto&x:b)zerocnt+=x==0;
  long long ret=1;
  for(std::size_t i=0;i<b.size();++i){
    for(std::size_t j=0;j<i;++j){
      long long g=gcd(m[i],m[j]);
      if((b[i]-b[j])%g)return -1;
      m[i]/=g,m[j]/=g;
      long long gi=gcd(m[i],g),gj=g/gi;
      do{
        g=gcd(gi,gj);
        gi*=g,gj/=g;
      }while(g!=1);
      m[i]*=gi,m[j]*=gj;
      b[i]%=m[i],b[j]%=m[j];
    }
  }
  for(std::size_t i=0;i<b.size();++i)ret=(ret*m[i])%mod;
  if(zerocnt==b.size())return ret;
  return garner(b,m,mod);
}
}
#endif