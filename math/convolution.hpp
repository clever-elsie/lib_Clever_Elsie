#ifndef ELSIE_CONVOLUTION
#define ELSIE_CONVOLUTION
#include <chrono>
#include<iostream>
#include <array>
#include <vector>
#include <cstdint>
#include <variant>
#include <algorithm>
#include <type_traits>
#include <math/prime_factor.hpp>
namespace elsie{
using namespace std;
template<uint32_t M=998244353>
class convolution_fast{
static_assert(is_prime(M),"convolution_fast requires that M is prime");
private:
using i32 = int32_t; using u32 = uint32_t;
using i64 = int64_t; using u64 = uint64_t;
static inline u32 modpow(u32 a,u32 b){
  u32 r=1;
  while(b){
    if(b&1)r=(u64)r*a%M;
    a=(u64)a*a%M;
    b>>=1;
  }
  return r;
}
struct ntt_info{
  static constexpr i32 e2=countr_zero(M-1);
  array<u32,e2+1>r,i;
  array<u32,max(0,e2-1)>r2,i2;
  array<u32,max(0,e2-2)>r3,i3;
  ntt_info(){
    r[e2]=convolution_fast<M>::modpow(primitive_root(M),(M-1)>>e2);
    i[e2]=mod_inv<__int128_t>(r[e2],M);
    for(i32 j=e2-1;j>=0;--j){
      r[j]=(u64)r[j+1]*r[j+1]%M;
      i[j]=(u64)i[j+1]*i[j+1]%M;
    }
    u32 p=1,ip=1;
    for(i32 j=0;j<=e2-2;++j){
      r2[j]=(u64)r[j+2]*p%M;
      i2[j]=(u64)i[j+2]*ip%M;
      p=(u64)p*i[j+2]%M;
      ip=(u64)ip*r[j+2]%M;
    }
    p=1,ip=1;
    for(i32 j=0;j<=e2-3;++j){
      r3[j]=(u64)r[j+3]*p%M;
      i3[j]=(u64)i[j+3]*ip%M;
      p=(u64)p*i[j+3]%M;
      ip=(u64)ip*r[j+3]%M;
    }
  }
};
ntt_info info=ntt_info();
void ntt(vector<u32>&a){
  u32 len=0,n=static_cast<u32>(a.size()),h=countr_zero(n);
  while(len<h){
    if(h-len==1){
    u32 p=1u<<(h-len-1),rot=1;
    for(u32 s=0;s<(1u<<len);++s){
      u32 offs=s<<(h-len);
      for(u32 i=0;i<p;++i){
        u32 l=a[i+offs],r=(u64)a[i+offs+p]*rot%M;
        a[i+offs]=(u64)l+r-((u64)l+r>=M?M:0);
        a[i+offs+p]=(l>=r?l-r:(M-r)+l);
      }
      if(s+1!=(1u<<len)) rot=(u64)rot*info.r2[countr_zero(~s)]%M;
    }
    ++len;
  }else{
    u32 rot=1,imag=info.r[2],p=1u<<(h-len-2);
    for(u32 s=0;s<(1u<<len);++s){
      u32 rot2=(u64)rot*rot%M,rot3=(u64)rot2*rot%M;
      u32 offs=s<<(h-len);
      for(u32 i=0;i<p;++i){
        u32 a0=a[i+offs],a1=(u64)a[i+offs+p]*rot%M;
        u32 a2=(u64)a[i+offs+2*p]*rot2%M;
        u32 a3=(u64)a[i+offs+3*p]*rot3%M;
        u32 a1_a3imag=u64(a1>=a3?a1-a3:(M-a3)+a1)*imag%M;
        u32 a02=(u64)a0+a2-((u64)a0+a2>=M?M:0);
        u32 a13=(u64)a1+a3-((u64)a1+a3>=M?M:0);
        u32 a0n2=(a0>=a2?a0-a2:(M-a2)+a0);
        a[i+offs]=(u64)a02+a13-((u64)a02+a13>=M?M:0);
        a[i+offs+p]=(a02>=a13?a02-a13:(M-a13)+a02);
        a[i+offs+2*p]=(u64)a0n2+a1_a3imag-((u64)a0n2+a1_a3imag>=M?M:0);
        a[i+offs+3*p]=(a0n2>=a1_a3imag?a0n2-a1_a3imag:(M-a1_a3imag)+a0n2);
      }
      if(s+1!=(1u<<len))
        rot=(u64)rot*info.r3[countr_zero(~s)]%M;
    }
    len+=2;
  }
  }
}
void ntt_inv(vector<u32>&a){
  u32 n=static_cast<u32>(a.size()),h=countr_zero(n);
  i32 len=h;
  while(len){
    if(len==1){
      u32 p=1u<<(h-len),rot=1;
      for(u32 s=0;s<(1u<<len-1);++s){
        u32 offs=s<<(h+1-len);
        for(u32 i=0;i<p;++i){
          u32 l=a[i+offs],r=a[i+offs+p];
          a[i+offs]=(u64)l+r-((u64)l+r>=M?M:0);
          a[i+offs+p]=u64(l>=r?l-r:(M-r)+l)*rot%M;
        }
        if(s+1!=(1u<<len-1)) rot=(u64)rot*info.i2[countr_zero(~s)]%M;
      }
      --len;
    }else{
      u32 p=1u<<(h-len),rot=1,imag=info.i[2];
      for(u32 s=0;s<(1u<<len-2);++s){
        u32 rot2=(u64)rot*rot%M,rot3=(u64)rot*rot2%M;
        u32 offs=s<<(h+2-len);
        for(u32 i=0;i<p;++i){
          u32 a0=a[i+offs],a1=a[i+offs+p],a2=a[i+offs+2*p],a3=a[i+offs+3*p];
          u32 a2_a3imag=(u64)(a2>=a3?a2-a3:(M-a3)+a2)*imag%M;
          u32 a01=(u64)a0+a1-((u64)a0+a1>=M?M:0);
          u32 a23=(u64)a2+a3-((u64)a2+a3>=M?M:0);
          u32 a0n1=(a0>=a1?a0-a1:(M-a1)+a0);
          a[i+offs]=(u64)a01+a23-((u64)a01+a23>=M?M:0);
          a[i+offs+p]=((u64)a0n1+a2_a3imag-((u64)a0n1+a2_a3imag>=M?M:0))*rot%M;
          a[i+offs+2*p]=(u64)(a01>=a23?a01-a23:(M-a23)+a01)*rot2%M;
          a[i+offs+3*p]=(u64)(a0n1>=a2_a3imag?a0n1-a2_a3imag:(M-a2_a3imag)+a0n1)*rot3%M;
        }
        if(s+1!=(1u<<len-2))rot=(u64)rot*info.i3[countr_zero(~s)]%M;
      }
      len-=2;
    }
  }
}
void conv_naive(vector<u32>&&a,vector<u32>&&b,vector<u32>&c){
  if(b.size()>a.size())swap(a,b);
  for(u32 i=0;i<a.size();++i)
  for(u32 j=0;j<b.size();++j){
    u32 tmp=(u64)a[i]*b[j]%M;
    c[i+j]=(u64)c[i+j]+tmp-((u64)c[i+j]+tmp>=M?M:0);
  }
}
public:
convolution_fast():info(){}
template<bool is_Zmod=true>
vector<u32>prod_move(vector<u32>&&a,vector<u32>&&b){
  if(a.empty()||b.empty())return{0};
  size_t m=a.size()+b.size()-1;
  if(min(a.size(),b.size())<=60){
    vector<u32>c(m,0);
    conv_naive(move(a),move(b),c);
    return c;
  }
  size_t u=1ull<<(sizeof(size_t)*8-1-countl_zero(m)+(popcount(m)!=1));
  size_t sz=countr_zero(u);
  if constexpr(!is_Zmod) for(auto&x:a) x%=M;
  a.resize(u,0),ntt(a);
  if constexpr(!is_Zmod) for(auto&x:b) x%=M;
  b.resize(u,0),ntt(b);
  for(u32 i=0;i<u;++i) a[i]=(u64)a[i]*b[i]%M;
  ntt_inv(a);
  a.resize(m);
  u32 invN=modpow(u,M-2);
  for(auto&&x:a)x=(u64)invN*x%M;
  return a;
}
template<bool is_Zmod=true>
vector<u32>prod_amove(vector<u32>&&a,vector<u32>b){ return prod_move<is_Zmod>(move(a),move(b)); }
template<bool is_Zmod=true>
vector<u32>prod_bmove(vector<u32>a,vector<u32>&&b){ return prod_move<is_Zmod>(move(a),move(b)); }
template<bool is_Zmod=true>
vector<u32>prod(vector<u32>a,vector<u32>b){ return prod_move<is_Zmod>(move(a),move(b)); }
};

template<class T,T M=998244353,size_t psz=2>
class convolution{
  static_assert(1<=psz&&psz<=11,"Even if you use a good prime as M, you still need to satisfy the condition 1<=psz<=11.");
  static constexpr bool f_prime=is_prime<T>(M);
  using u32 = uint32_t;
  static constexpr array<u32,11>mods{
    2'885'681'153u/*43 * 2^26 +1*/,2'717'908'993u/*81 * 2^25 +1*/,
    2'483'027'969u/*37 * 2^26 +1*/,2'281'701'377u/*17 * 2^27 +1*/,
    2'113'929'217u/*63 * 2^25 +1*/,2'013'265'921u/*15 * 2^27 +1*/,
    1'811'939'329u/*27 * 2^26 +1*/,1'711'276'033u/*51 * 2^25 +1*/,
    1'107'296'257u/*33 * 2^25 +1*/,  469'762'049u/* 7 * 2^26 +1*/,
      167'772'161u/* 5 * 2^25 +1*/
  };
  template<size_t... I>
  static constexpr array<T,psz+1>make_mods_s(index_sequence<I...>){ return{{mods[I]...,M}}; }
  static constexpr array<T,psz+1>mods_s=make_mods_s(make_index_sequence<psz>{});
  T garner_impl(const array<u32,psz>&b)const
  requires is_integral_v<T>||same_as<T,__int128_t>||same_as<T,__uint128_t>{
    using int_s=conditional_t<(sizeof(T)>8),__int128_t,int64_t>;
    using vc=vector<int_s>;
    constexpr __uint128_t threshold=__uint128_t(1)<<64;
    array<int_s,psz+1>coeffs,constants;
    coeffs.fill(1); constants.fill(0);
    if constexpr(M<=threshold){
      for(size_t k=0;k<b.size();++k){
        int_s tmp=b[k]>=constants[k]?b[k]-constants[k]:((int_s)b[k]+mods_s[k])-constants[k];
        int_s t=tmp*mod_inv<int_s>(coeffs[k],mods_s[k])%mods_s[k];
        for(size_t i=k+1;i<=b.size();++i){
          constants[i]=(t*coeffs[i]%mods_s[i]+constants[i])%mods_s[i];
          coeffs[i]=coeffs[i]*mods_s[k]%mods_s[i];
        }
      }
    }else{
      for(size_t k=0;k<b.size();++k){
        int_s tmp=b[k]>=constants[k]?b[k]-constants[k]:(b[k]+mods_s[k])-constants[k];
        int_s t=tmp*mod_inv<int_s>(coeffs[k],mods_s[k])%mods_s[k];
        for(size_t i=k+1;i<=b.size();++i){
          constants[i]=(constants[i]+rem256(mul128(t,coeffs[i]),mods_s[i]))%mods_s[i];
          coeffs[i]=rem256(mul128(coeffs[i],mods_s[k]),mods_s[i]);
        }
      }
    }
    return constants.back();
  }
  template<u32 mod,bool is_Zmod> vector<u32>mod_vec(const vector<T>&a)const{
    vector<u32>r(a.size());
    if constexpr(is_Zmod){
      for(auto rtr=r.begin(),atr=a.begin();atr!=a.end();++atr,++rtr)
        *rtr=*atr;
    }else{
      for(auto rtr=r.begin(),atr=a.begin();atr!=a.end();++atr,++rtr){
        T m=*atr%mod;
        if constexpr(is_signed_v<T>||same_as<T,__int128_t>)
          if(m<0)m+=mod;
        *rtr=static_cast<u32>(m);
      }
    }
    return r;
  }
  template<bool is_Zmod,size_t... I>
  vector<T>prod_impl(const vector<T>&a,const vector<T>&b,const size_t len,index_sequence<I...>)const{
    vector<vector<u32>>r={convolution_fast<mods_s[I]>().prod_move(mod_vec<mods_s[I],is_Zmod>(a),mod_vec<mods[I],is_Zmod>(b))...};
    vector<T>ret(len);
    array<u32,psz>q;
    for(size_t i=0;i<len;++i){
      for(uint32_t j=0;j<psz;++j)
        q[j]=r[j][i];
      ret[i]=garner_impl(q);
    }
    return ret;
  }
  public:
  convolution(){}
  template<bool is_Zmod=false>
  vector<T>prod(const vector<T>&a,const vector<T>&b)const{
    const size_t len=a.size()+b.size()-1;
    const size_t step=63-countl_zero(len)+(popcount(len)!=1);
    size_t max_step=0;
    if constexpr(sizeof(T)>8) for(T p=M-1;(p&1==0);p>>=1)++max_step;
    else max_step=countr_zero(uint64_t(M)-1);
    if constexpr(f_prime&&same_as<T,u32>)if(max_step>=step)
      return convolution_fast<M>().template prod<is_Zmod>(a,b);
    return prod_impl<is_Zmod>(a,b,len,make_index_sequence<psz>{});
  }
};
}
#endif