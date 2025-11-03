#ifndef ELSIE_BASIC_MATH
#define ELSIE_BASIC_MATH
#include <vector>
#include <limits>
#include <cstdint>
#include <concepts>
#include <iterator>
#include <dtStrc/math/modint/static_modint.hpp>
namespace elsie{

template<std::integral T,std::integral U>
inline auto ceil(const T a,const U b){return(a+b-1)/b;}
template<std::integral T,std::integral U>
inline auto floor(const T a,const U b){return a/b-(a%b&&(a^b)<0);}

// 比較回数 3n/2回 素朴な実装は2n回 ループ変数の比較はcpuが分岐予測できるため無視
template<std::random_access_iterator Iterator>
std::pair<Iterator, Iterator>
minmax_element(Iterator first, const Iterator last){
  const size_t n=last-first;
  if(n==0) return {first,first};
  Iterator min,max;
  if(n&1) min=first,max=first++; // odd number of elements
  else{
    if(*first<*first+1) min=first,max=first+1;
    else min=first+1,max=first;
    first+=2;
  }
  for(;first!=last;first+=2){
    auto next=first+1;
    if(*first<*next){
      if(*first<*min) min=first;
      if(*next>*max) max=next;
    }else{
      if(*next<*min) min=next;
      if(*first>*max) max=first;
    }
  }
  return {min,max};
}

template<class Iterator>
requires (!std::random_access_iterator<Iterator>)
std::pair<Iterator, Iterator>
minmax_element(Iterator first, Iterator last){
  if(first==last) return {first,first};
  Iterator min,max;
  if(auto prev=first++;prev==last) return {prev,prev};
  else{
    if(*prev<*first) min=prev,max=first;
    else min=first,max=prev;
    first++;
  }
  for(;first!=last;first++){
    auto prev=first++;
    if(first!=last){
      if(*prev<*first){
        if(*prev<*min) min=prev;
        if(*first>*max) max=first;
      }else{
        if(*first<*min) min=first;
        if(*prev>*max) max=prev;
      }
    }else{
      if(*prev<*min) min=prev;
      else if(*prev>*max) max=prev;
    }
  }
  return {min,max};
}

/**
 * first==lastはUB
 */
template<class Iterator>
std::pair<std::iter_value_t<Iterator>, std::iter_value_t<Iterator>>
minmax(Iterator first, Iterator last){
  auto [min,max]=minmax_element(first,last);
  return {*min,*max};
}

// std::ceil(std::sqrtl(x:u64))が64bit精度で40%高速なので，long double 80bitならそちらを使うべき
template<bool internal_invocate=false>
inline uint64_t sqrt_ceil(uint64_t x){
  if constexpr(!internal_invocate){
    // sqrt_floorからの呼出しではここをスキップする
    if(x>uint64_t(UINT32_MAX)*UINT32_MAX)return 1ull<<32;
    else if(x<2)return x!=0;
  }
  uint64_t nx,y=std::min<uint64_t>((1ULL<<((64-std::countl_zero(x))>>1)+1)-1,UINT32_MAX);
  y-=(y*y-x)/(y<<1);
  y-=(y*y-x)/(y<<1);
  y-=(y*y-x)/(y<<1);
  y-=(y*y-x)/(y<<1);
  y-=(y*y-x)/(y<<1);
  nx=y-(y*y-x)/(y<<1);
  while(nx!=y){
    y=nx;
    nx=y-(y*y-x)/(y<<1);
    if(nx==y)break;
  }
  return y+(y*y<x?1:(y-1)*(y-1)>=x?-1:0);
}
inline uint64_t sqrt_floor(uint64_t x){
  if((uint64_t)UINT32_MAX*UINT32_MAX<=x)return UINT32_MAX;
  else if(x<4)return x!=0;
  uint64_t y=sqrt_ceil<true>(x);
  return y-(y*y!=x);
}

inline __uint128_t mul64to128(uint64_t a,uint64_t b){
  uint64_t hi,lo;
  asm("mulq %3" :"=a"(lo),"=d"(hi):"a"(a),"r"(b));
  return((__uint128_t)(hi)<<64)+lo;
}
inline pair<__uint128_t,__uint128_t>mul128(__uint128_t a,__uint128_t b){
  constexpr static __uint128_t Lfilter=0xFFFF'FFFF'FFFF'FFFFull;
  uint64_t x1=a>>64,x0=a&Lfilter;
  uint64_t y1=b>>64,y0=b&Lfilter;
  __uint128_t z2=mul64to128(x1,y1),z1=mul64to128(x1,y0)+mul64to128(x0,y1),z0=mul64to128(x0,y0);
  __uint128_t lower=z0+(z1<<64);
  return{z2+(z1>>64)+(lower<z0),lower};
}
inline __uint128_t rem256(pair<__uint128_t,__uint128_t>x,__uint128_t mod){
  __uint128_t rem32=(__uint128_t(1)<<32)%mod;
  __uint128_t rem64=rem32*rem32%mod;
  __uint128_t rem128=rem64*rem64%mod;
  auto&&[a,b]=x;
  a%=mod,b%=mod;
  return(a*rem128%mod+b)%mod;
}

int64_t safepow(int64_t a,uint64_t b){
  int64_t ret=1,k=a;
  double check=1,kcheck=a;
  while(b){
    if(b&1){
      check*=k;
      if(check>INT64_MAX)return INT64_MAX;
      if(check<INT64_MIN)return INT64_MIN;
      ret*=k;
    }
    kcheck*=k;
    if(kcheck>INT64_MAX)return INT64_MAX;
    if(kcheck<INT64_MIN)return INT64_MIN;
    k*=k;
    b>>=1;
  }
  return ret;
}

template<std::integral T,class U=std::make_unsigned_t<T>>
constexpr T modpow(T a,U b,U mod){
  if constexpr(std::is_same_v<U,__uint128_t>){ // mod=2^64専用
    constexpr __uint128_t filter=uint64_t(-1);
    a%=mod;
    __uint128_t ret=1;
    if(a<0)a+=mod;
    while(b){
      ret=(b&1?ret*a&filter:ret);
      a=a*a&filter;
      b>>=1;
    }
    return uint64_t(ret&filter);
  }else{
    __int128_t k=a%mod,ret=1;
    if(k<0)k+=mod;
    while(b){
      ret=(b&1?ret*k%mod:ret);
      k=(k*k)%mod;
      b>>=1;
    }
    return int64_t(ret);
  }
}

template<class S>S gcd(S a,S b){
  if(b==0) return a;
  S r=1;if(a<0)a=-a;if(b<0)b=-b;
  while(r) r=a%b,a=b,b=r;
  return a;
}

template<class S>S lcm(S a,S b){return a/gcd(a,b)*b;}

template<class S>S egcd(S a,S b,S&x,S&y){
  if(b==0){
    x=1,y=0;
    return a;
  }
  S d=egcd(b,a%b,y,x);
  y-=a/b*x;
  return d;
}

template<class T>T exgcd(T a,T b,T&x,T&y){
  auto assign=[&](T&s,T&t,T u,T v)->void {s=u,t=v;};
  x=1,y=0;
  T u=0,v=1;
  while(b){
    T k=a/b,r=a%b;
    assign(a,b,b,r);
    assign(x,u,u,x-u*k);
    assign(y,v,v,y-v*k);
  }
  if constexpr(is_signed_v<T>)
    if(a<0)a=-a,x=-x,y=-y;
  return a;
}

template<class T>T mod_inv(T a,T m){
  T x,y,g=exgcd(a,m,x,y);
  if(g!=1)return -1;
  return(x%m+m)%m;
}

template<class T>T mod_inv_prime(T a,T p){ return modpow(a,p-2,p); }

template<class mint=mint<998244353u>>
class combination{
  private:
    using tint = long long;
    vector<mint>invprod,prod;
    vector<tint>inv;
    const unsigned long long M;
  public:
  combination(int64_t n=1):invprod(2,1),prod(2,1),inv(2,0),M(mint(-1).val()+1){
    inv[1]=1;
    if(n>1)PreCalc(n);
  }
  void PreCalc(int64_t n){
    size_t presize=inv.size();
    if(presize>=n+1)return;
    inv.resize(n+1);
    prod.resize(n+1);
    invprod.resize(n+1);
    for(int64_t i=presize;i<=n;++i){
      prod[i]=prod[i-1]*i;
      inv[i]=(((M/i)*(-inv[M%i]))+M)%M;
      invprod[i]=invprod[i-1]*inv[i];
    }
  }
  mint factorial(size_t n){
    PreCalc(n);
    return prod[n];
  }
  mint invfact(size_t n){
    PreCalc(n);
    return invprod[n];
  }
  mint P(size_t n,size_t k){
    if(n<k) return 0;
    PreCalc(n);
    return prod[n]*invprod[n-k];
  }
  mint C(size_t n,size_t k){
    if(n<k) return 0;
    PreCalc(n);
    return prod[n]*invprod[n-k]*invprod[k];
  }
  mint H(size_t n,size_t k){
    if(n==0) return mint(k!=0);
    PreCalc(n);
    return C(n+k-1,k);
  }
};

class barret32{
  uint64_t M,iM;
  public:
  barret32():M(0),iM(0){}
  barret32(uint32_t m):M(m),iM(uint64_t(-1)/m+1){}
  void set(uint32_t m){ M=m,iM=uint64_t(-1)/m+1; }
  uint32_t umod()const{ return M; }
  uint32_t mul(uint32_t a,uint32_t b)const{
    uint64_t z=(uint64_t)a*b;
    uint64_t x=((__uint128_t)z*iM>>64);
    uint64_t y=x*M;
    return(uint32_t)(z-y+(z<y?M:0));
  }
};

class barret64{
  __uint128_t M,iM;
  public:
  barret64():M(0),iM(0){}
  barret64(uint64_t m):M(m),iM(__uint128_t(-1)/M+1){}
  uint64_t umod(){ return M; }
  uint64_t mul(uint64_t a,uint64_t b){
    __uint128_t z=mul64to128(a,b);
    __uint128_t x=mul128(z,iM).first;
    __uint128_t y=x*M;
    return(uint64_t)(z-y+(z<y?M:0));
  }
};
}
#endif