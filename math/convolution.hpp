#ifndef ELSIE_CONVOLUTION
#define ELSIE_CONVOLUTION
#include <chrono>
#include <iostream>
#include <array>
#include <vector>
#include <cstdint>
#include <variant>
#include <algorithm>
#include <type_traits>
#include <utility>
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

template<size_t M=998244353,size_t mod_cnt=5>
class convolution{
  using i32=int32_t; using u32=uint32_t;
  using i64=int64_t; using u64=uint64_t;
  static constexpr size_t max_mods=11;
  static_assert(mod_cnt<=5);
  static constexpr array<u32,max_mods>mods_list{
    2'717'908'993u/*81 * 2^25 +1*/, 2'113'929'217u/*63 * 2^25 +1*/,
    1'711'276'033u/*51 * 2^25 +1*/, 1'107'296'257u/*33 * 2^25 +1*/,
      167'772'161u/* 5 * 2^25 +1*/
  };
  static constexpr array<u32,max_mods>primitive_root_list{ 1100, 206, 947, 199, 124 };
  class mint_ntt{
    friend convolution;
    using i32=int32_t; using u32=uint32_t;
    using i64=int64_t; using u64=uint64_t;
    template<size_t... I> static constexpr array<u32,mod_cnt> make_mods(index_sequence<I...>){ return {mods_list[I]...}; }
    private:
    static constexpr array<u32,mod_cnt> mods=make_mods(make_index_sequence<mod_cnt>{});
    array<u32,mod_cnt>val;

    template<size_t... I> constexpr mint_ntt& add_impl(const mint_ntt&rhs,index_sequence<I...>){
      (...,add_impl_unit<I>(rhs));
      return*this;
    }
    template<size_t I> constexpr void add_impl_unit(const mint_ntt&rhs){
      u64 tmp=(static_cast<u64>(val[I])+rhs.val[I])%mods[I];
      if(tmp>=mods[I])tmp-=mods[I];
      val[I]=static_cast<u32>(tmp);
    }
    template<size_t... I> constexpr mint_ntt& sub_impl(const mint_ntt&rhs,index_sequence<I...>){
      (...,(val[I]=val[I]>=rhs.val[I]?val[I]-rhs.val[I]:mods[I]-rhs.val[I]+val[I]));
      return*this;
    }
    template<size_t... I> constexpr mint_ntt& mul_impl(const mint_ntt&rhs,index_sequence<I...>){
      (...,(val[I]=static_cast<u32>(static_cast<u64>(val[I])*rhs.val[I]%mods[I])));
      return*this;
    }
    template<size_t... I> constexpr mint_ntt& div_impl(const mint_ntt&rhs,index_sequence<I...>){
      (...,(val[I]=static_cast<u32>(static_cast<u64>(val[I])*modinv<I>(rhs.val[I])%mods[I])));
      return*this;
    }
    template<size_t... I> static constexpr mint_ntt negate_impl(const mint_ntt&v,index_sequence<I...>){
      mint_ntt res(v);
      (...,(res.val[I]=(mods[I]-v.val[I])%mods[I]));
      return res;
    }
    template<size_t... I> constexpr void assign(u64 x,index_sequence<I...>){
      (...,(val[I]=x%mods[I]));
    }
    public:
    ~mint_ntt()=default;
    constexpr mint_ntt()=default;
    constexpr mint_ntt(mint_ntt&&)=default;
    constexpr mint_ntt(const mint_ntt&)=default;
    constexpr mint_ntt(u64 x){ assign(x,make_index_sequence<mod_cnt>{}); }
    constexpr mint_ntt& operator=(mint_ntt&&)=default;
    constexpr mint_ntt& operator=(const mint_ntt&)=default;
    constexpr mint_ntt& operator=(u64 x){
      assign(x,make_index_sequence<mod_cnt>{});
      return *this;
    }

    constexpr mint_ntt& operator+=(const mint_ntt&rhs){ return add_impl(rhs,make_index_sequence<mod_cnt>{}); }  
    constexpr mint_ntt& operator-=(const mint_ntt&rhs){ return sub_impl(rhs,make_index_sequence<mod_cnt>{}); }
    constexpr mint_ntt& operator*=(const mint_ntt&rhs){ return mul_impl(rhs,make_index_sequence<mod_cnt>{}); }
    constexpr mint_ntt& operator/=(const mint_ntt&rhs){ return div_impl(rhs,make_index_sequence<mod_cnt>{}); }

    private:
    template<size_t... k>
    constexpr void outer_value(array<u64,mod_cnt+1>&coeffs,array<u64,mod_cnt+1>&constants,index_sequence<k...>)const{
      ((outer_step<k>(coeffs,constants)),...);
    }
    template<i32 k>
    constexpr void outer_step(array<u64,mod_cnt+1>&coeffs,array<u64,mod_cnt+1>&constants)const{
      u64 tmp = val[k]>=constants[k]? val[k]-constants[k] : val[k]+mods[k]-constants[k];
      u32 t = tmp*modinv<k>(coeffs[k])%mods[k];
      inner_value<k>(coeffs,constants,t,make_index_sequence<mod_cnt-k-1>{});
      if constexpr(M!=0){ // M==0 then assume M==2^64
        constants[mod_cnt]=(t*coeffs[mod_cnt]%M+constants[mod_cnt])%M;
        coeffs[mod_cnt]=coeffs[mod_cnt]*mods[k]%M;
      }else{ // if M is 0, assume M = 2^64
        constants[mod_cnt]=t*coeffs[mod_cnt]+constants[mod_cnt];
        coeffs[mod_cnt]=coeffs[mod_cnt]*mods[k];
      }
    }
    template<i32 k,size_t... I>
    constexpr void inner_value(array<u64,mod_cnt+1>&coeffs,array<u64,mod_cnt+1>&constants,const u32 t,index_sequence<I...>)const{
      ((inner_step<k,I>(coeffs,constants,t)),...);
    }
    template<i32 k,i32 I>
    constexpr void inner_step(array<u64,mod_cnt+1>&coeffs,array<u64,mod_cnt+1>&constants,const u32 t)const{
      constexpr size_t i=k+1+I;
      constants[i]=(t*coeffs[i]%mods[i]+constants[i])%mods[i];
      coeffs[i]=coeffs[i]*mods[k]%mods[i];
    }
    public:
    constexpr u64 value()const{
      array<u64,mod_cnt+1>coeffs,constants;
      coeffs.fill(1); constants.fill(0);
      outer_value(coeffs,constants,make_index_sequence<mod_cnt>{});
      return constants.back();
    }
    template<size_t I> static constexpr u32 modinv(u32 a){
      i64 x=1,u=0,b=mods[I];
      while(b){
        i64 k=a/b,r=a%b,tmp=u;
        a=b,b=r;
        u=x-u*k;
        x=tmp;
      }
      if(a<0)a=-a,x=-x;
      if(a!=1)return -1;
      return(x%mods[I]+mods[I])%mods[I];
    }
    template<size_t I> static constexpr u32 modpow(u32 a, u32 b){
      u64 r=1,k=a;
      for(;b;b>>=1){
        if(b&1)r=r*k%mods[I];
        k=k*k%mods[I];
      }
      return r;
    }
    private:
    template<size_t... I> constexpr bool eq_impl(const mint_ntt&rhs,index_sequence<I...>)const{
      return (...&&(val[I]==rhs.val[I]));
    }
    public:
    friend constexpr bool operator!=(const mint_ntt&lhs,const mint_ntt&rhs){ return !lhs.eq_impl(rhs,make_index_sequence<mod_cnt>{}); }
    friend constexpr bool operator==(const mint_ntt&lhs,const mint_ntt&rhs){ return lhs.eq_impl(rhs,make_index_sequence<mod_cnt>{}); }
    friend constexpr mint_ntt operator+(const mint_ntt&lhs,const mint_ntt&rhs){return mint_ntt(lhs)+=rhs;}
    friend constexpr mint_ntt operator-(const mint_ntt&lhs,const mint_ntt&rhs){return mint_ntt(lhs)-=rhs;}
    friend constexpr mint_ntt operator*(const mint_ntt&lhs,const mint_ntt&rhs){return mint_ntt(lhs)*=rhs;}
    friend constexpr mint_ntt operator/(const mint_ntt&lhs,const mint_ntt&rhs){return mint_ntt(lhs)/=rhs;}
  };
  struct ntt_info{
    static constexpr i32 e2=25;
    array<mint_ntt,e2+1>root,iroot;
    array<mint_ntt,e2-1>root2,iroot2;
    array<mint_ntt,e2-2>root3,iroot3;
    private:
    template<size_t... I>
    constexpr void init_root(index_sequence<I...>){
      (...,(root[e2].val[I]=mint_ntt::template modpow<I>(primitive_root_list[I],(convolution::mods_list[I]-1)>>e2)));
      (...,(iroot[e2].val[I]=mint_ntt::template modinv<I>(root[e2].val[I])));
    }
    public:
    constexpr ntt_info(){
      init_root(make_index_sequence<mod_cnt>{});
      for(i32 i=e2-1;i>=0;--i){
        root[i]=root[i+1]*root[i+1];
        iroot[i]=iroot[i+1]*iroot[i+1];
      }
      mint_ntt p(1),ip(1);
      for(i32 i=0;i<=e2-2;++i){
        root2[i]=root[i+2]*p;
        iroot2[i]=iroot[i+2]*ip;
        p*=iroot[i+2];
        ip*=root[i+2];
      }
      p=ip=1;
      for(i32 i=0;i<=e2-3;++i){
        root3[i]=root[i+3]*p;
        iroot3[i]=iroot[i+3]*ip;
        p*=iroot[i+3];
        ip*=root[i+3];
      }
    }
  };
  constexpr static ntt_info info=ntt_info();

  void ntt(vector<mint_ntt>&a)const{
    u32 len=0,n=static_cast<u32>(a.size()),h=countr_zero(n);
    while(len<h){
      if(h-len==1){
        u32 p=1u<<(h-len-1);
        mint_ntt rot(1);
        for(u32 s=0;s<(1u<<len);++s){
          for(u32 i=0,offs=s<<(h-len);i<p;++i){
            mint_ntt l=a[i+offs],r=a[i+offs+p]*rot;
            a[i+offs]=l+r;
            a[i+offs+p]=l-r;
          }
          if(s+1!=(1u<<len)) rot*=info.root2[countr_zero(~s)];
        }
        ++len;
      }else{
        mint_ntt rot(1),imag(info.root[2]);
        u32 p=1u<<(h-len-2);
        for(u32 s=0;s<(1u<<len);++s){
          mint_ntt rot2=rot*rot,rot3=rot2*rot;
          for(u32 i=0,offs=s<<(h-len);i<p;++i){
            mint_ntt&a0=a[i+offs];
            mint_ntt a1=a[i+offs+p]*rot;
            mint_ntt a2=a[i+offs+2*p]*rot2;
            mint_ntt a3=a[i+offs+3*p]*rot3;
            mint_ntt a1_a3imag=(a1-a3)*imag;
            mint_ntt a02=a0+a2,a13=a1+a3;
            mint_ntt a0n2=a0-a2;
            a[i+offs]=a02+a13;
            a[i+offs+p]=a02-a13;
            a[i+offs+2*p]=a0n2+a1_a3imag;
            a[i+offs+3*p]=a0n2-a1_a3imag;
          }
          if(s+1!=(1u<<len)) rot*=info.root3[countr_zero(~s)];
        }
        len+=2;
      }
    }
  }
  void inv_ntt(vector<mint_ntt>&a)const{
    u32 n=static_cast<u32>(a.size()),h=countr_zero(n);
    i32 len=h;
    while(len){
      if(len==1){
        u32 p=1u<<(h-len);
        mint_ntt rot(1);
        for(u32 s=0;s<(1u<<len-1);++s){
          for(u32 i=0,offs=s<<(h+1-len);i<p;++i){
            mint_ntt l=a[i+offs];
            mint_ntt&r=a[i+offs+p];
            a[i+offs]+=r;
            a[i+offs+p]=(l-r)*rot;
          }
          if(s+1!=(1u<<len-1)) rot*=info.iroot2[countr_zero(~s)];
        }
        --len;
      }else{
        u32 p=1u<<(h-len);
        mint_ntt rot(1),imag(info.iroot[2]);
        for(u32 s=0;s<(1u<<len-2);++s){
          mint_ntt rot2(rot*rot),rot3(rot*rot2);
          for(u32 i=0,offs=s<<(h+2-len);i<p;++i){
            mint_ntt&a0=a[i+offs];
            mint_ntt&a1=a[i+offs+p];
            mint_ntt&a2=a[i+offs+2*p];
            mint_ntt&a3=a[i+offs+3*p];
            mint_ntt a2_a3imag=(a2-a3)*imag;
            mint_ntt a01=a0+a1,a23=a2+a3;
            mint_ntt a0n1=a0-a1;
            a[i+offs]=a01+a23;
            a[i+offs+p]=(a0n1+a2_a3imag)*rot;
            a[i+offs+2*p]=(a01-a23)*rot2;
            a[i+offs+3*p]=(a0n1-a2_a3imag)*rot3;
          }
          if(s+1!=(1u<<len-2))rot*=info.iroot3[countr_zero(~s)];
        }
        len-=2;
      }
    }
  }

  template<unsigned_integral T>
  vector<uint64_t> convolution_naive(const vector<T>&a,const vector<T>&b)const{
    size_t s=a.size()+b.size()-1;
    vector<uint64_t> c(s,0);
    const auto&p=(a.size()<b.size()?a:b);
    const auto&q=(a.size()<b.size()?b:a);
    for(size_t i=0;i<p.size();++i)
      for(size_t j=0;j<q.size();++j)
        if constexpr(M) c[i+j]=(c[i+j]+p[i]*q[j]%M)%M;
        else c[i+j]+=p[i]*q[j];
    return c;
  }
public:
  using mint=mint_ntt;
  convolution()=default;
  template<unsigned_integral T>
  vector<uint64_t> prod(const vector<T>&lhs,const vector<T>&rhs)const{
    u32 n=lhs.size(),m=rhs.size();
    u32 nm=n+m-1;
    if(std::min(n,m)<=60) return convolution_naive(lhs,rhs);
    u32 e=31-countl_zero(nm)+(popcount(nm)!=1);
    u32 p=1u<<e;
    vector<mint> a(p,0),b(p,0);
    for(size_t i=0;i<lhs.size();++i) a[i]=lhs[i];
    for(size_t i=0;i<rhs.size();++i) b[i]=rhs[i];
    auto r=prod(std::move(a),std::move(b));
    r.resize(nm);
    return r;
  }
  private:
  template<size_t ...I>
  mint_ntt calc_invk(size_t len,index_sequence<I...>)const{
    mint_ntt v;
    (...,(v.val[I]=mint_ntt::template modinv<I>(len)));
    return v;
  }
  public:
  vector<uint64_t> prod(vector<mint>&&lhs,vector<mint>&&rhs)const{
    assert(lhs.size()==rhs.size() && popcount(lhs.size())==1);
    ntt(lhs);
    ntt(rhs);
    for(size_t i=0;i<lhs.size();++i)
      lhs[i]*=rhs[i];
    inv_ntt(lhs);
    vector<uint64_t>ret(lhs.size());
    mint_ntt invk=calc_invk(lhs.size(),make_index_sequence<mod_cnt>{});
    for(size_t i=0;i<lhs.size();++i)
      ret[i]=(lhs[i]*=invk).value();
    return ret;
  }
};
}
#endif