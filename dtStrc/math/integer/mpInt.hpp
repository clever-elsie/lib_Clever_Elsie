#ifndef ELSIE_MULTIPRECISION_INTEGER
#define ELSIE_MULTIPRECISION_INTEGER
#include <array>
#include <string>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <algorithm>
#include <functional>
#include <math/basic_math.hpp>
namespace elsie{
/**
 * mod_cntは1毎に9桁精度が増える．
 * mod_cnt ∈ [1,128]
 * 負の数は扱えません．
 */
template<std::size_t mod_cnt=8>
class mpInt{
  static_assert(1<=mod_cnt&&mod_cnt<=128,"mod_cnt must be in [1,128]");
  using it=uint32_t;
  using ut=uint64_t;
  constexpr static std::size_t max_mod=128;
  constexpr static it P_=1'000'000'000;
  constexpr static std::array<it,max_mod>mods{P_+7,P_+9,P_+21,P_+33,P_+87,P_+93,P_+97,P_+103,P_+123,P_+181,P_+207,P_+223,P_+241,P_+271,P_+289,P_+297,P_+321,P_+349,P_+363,P_+403,P_+409,P_+411,P_+427,P_+433,P_+439,P_+447,P_+453,P_+459,P_+483,P_+513,P_+531,P_+579,P_+607,P_+613,P_+637,P_+663,P_+711,P_+753,P_+787,P_+801,P_+829,P_+861,P_+871,P_+891,P_+901,P_+919,P_+931,P_+933,P_+993,P_+1011,P_+1021,P_+1053,P_+1087,P_+1099,P_+1137,P_+1161,P_+1203,P_+1213,P_+1237,P_+1263,P_+1269,P_+1273,P_+1279,P_+1311,P_+1329,P_+1333,P_+1351,P_+1371,P_+1393,P_+1413,P_+1447,P_+1449,P_+1491,P_+1501,P_+1531,P_+1537,P_+1539,P_+1581,P_+1617,P_+1621,P_+1633,P_+1647,P_+1663,P_+1677,P_+1699,P_+1759,P_+1773,P_+1789,P_+1791,P_+1801,P_+1803,P_+1819,P_+1857,P_+1887,P_+1917,P_+1927,P_+1957,P_+1963,P_+1969,P_+2043,P_+2089,P_+2103,P_+2139,P_+2149,P_+2161,P_+2173,P_+2187,P_+2193,P_+2233,P_+2239,P_+2277,P_+2307,P_+2359,P_+2361,P_+2431,P_+2449,P_+2457,P_+2499,P_+2571,P_+2581,P_+2607,P_+2631,P_+2637,P_+2649,P_+2667,P_+2727,P_+2791,P_+2803};
  private:
  std::array<it,mod_cnt>val;

  void u64toVal(ut x){
    for(it i=0;i<mod_cnt;++i)
      val[i]=x%mods[i];
  }
  bool is_zero()const{
    for(it i=0;i<mod_cnt;++i)
      if(val[i]!=0)return false;
    return true;
  }
  it garner_mod10(const std::array<it,mod_cnt>&b)const{
    auto mod_mul=[](it x,it y,it MOD){ return (ut)x*y%MOD; };
    constexpr it mod=10;
    std::array<it,mod_cnt+1>coeffs;
    std::array<it,mod_cnt+1>constants;
    coeffs.fill(1);
    constants.fill(0);
    for(it k=0;k<b.size();++k){
      it tmp=(b[k]<constants[k]?mods[k]:0)+b[k]-constants[k];
      it t=mod_mul(tmp,mod_inv<ut>(coeffs[k],mods[k]),mods[k]);
      for(it i=k+1;i<=mod_cnt;++i){
        const it MOD=(i==mod_cnt?mod:mods[i]);
        (constants[i]+=mod_mul(t,coeffs[i],MOD))%=MOD;
        coeffs[i]=mod_mul(coeffs[i],mods[k],MOD);
      }
    }
    return constants.back();
  }
  mpInt(it v,it unuse){ val.fill(v); }
  public:
  mpInt(){}
  mpInt(uint64_t x){ u64toVal(x); }
  mpInt(int64_t x){ u64toVal(x<0?~ut(x)+1:x); }
  mpInt(const mpInt&v):val(v.val){}
  mpInt(mpInt&&v):val(std::move(v.val)){}
  mpInt(const std::string&s){
    val.fill(0);
    if(s.size()!=0){
      bool sign=s[0]=='-';
      for(ut i=sign;i<s.size();++i)
        *this=(*this*raw(10))+raw(s[i]-'0');
      if(sign) *this=raw(0)-*this;
    }
  }
  static mpInt raw(it v){ return mpInt(v,0); }
  mpInt&operator=(mpInt&&v){
    this->val=std::move(v.val);
    return*this;
  }
  mpInt&operator=(const mpInt&v){
    this->val=v.val;
    return*this;
  }
  mpInt&operator=(int64_t x){return*this=mpInt(x);}
  mpInt&operator=(uint64_t x){return*this=mpInt(x);}
  mpInt&operator=(const std::string&s){return*this=mpInt(s);}
  std::string to_string(){
    mpInt x=*this;
    std::string ret;
    int cnt=0;
    while(!x.is_zero()){
      it b=garner_mod10(x.val);
      ret.push_back('0'+b);
      x=(x-raw(b))/raw(10);
    }
    if(ret.size()) std::reverse(ret.begin(),ret.end());
    else ret.push_back('0');
    return ret;
  }
  mpInt&operator+=(const mpInt&rhs){
    for(it i=0;i<mod_cnt;++i){
      val[i]+=rhs.val[i];
      if(val[i]>=mods[i])
        val[i]-=mods[i];
    }
    return*this;
  }
  mpInt&operator-=(const mpInt&rhs){
    for(it i=0;i<mod_cnt;++i)
      if(val[i]>=rhs.val[i])
        val[i]-=rhs.val[i];
      else val[i]+=mods[i]-rhs.val[i];
    return*this;
  }
  mpInt&operator*=(const mpInt&rhs){
    for(it i=0;i<mod_cnt;++i)
      val[i]=(ut)val[i]*rhs.val[i]%mods[i];
    return*this;
  }
  mpInt&operator/=(const mpInt&rhs){
    for(it i=0;i<mod_cnt;++i)
      val[i]=val[i]*mod_inv<int64_t>(rhs.val[i],mods[i])%mods[i];
    return*this;
  }
  mpInt&operator+=(int64_t x){return*this+=mpInt(x);}
  mpInt&operator-=(int64_t x){return*this-=mpInt(x);}
  mpInt&operator*=(int64_t x){return*this*=mpInt(x);}
  mpInt&operator/=(int64_t x){return*this/=mpInt(x);}
  friend bool operator!=(const mpInt&lhs,const mpInt&rhs){
    for(it i=0;i<mod_cnt;++i)
      if(lhs.val[i]!=rhs.val[i])
        return true;
    return false;
  }
  friend bool operator==(const mpInt&lhs,const mpInt&rhs){return!(lhs!=rhs);}
  friend bool operator!=(const mpInt&lhs,int64_t rhs){return lhs!=mpInt(rhs);}
  friend bool operator==(const mpInt&lhs,int64_t rhs){return lhs==mpInt(rhs);}
  friend bool operator!=(int64_t lhs,const mpInt&rhs){return mpInt(lhs)!=rhs;}
  friend bool operator==(int64_t lhs,const mpInt&rhs){return mpInt(lhs)==rhs;}
  friend mpInt operator+(const mpInt&lhs,const mpInt&rhs){return mpInt(lhs)+=rhs;}
  friend mpInt operator-(const mpInt&lhs,const mpInt&rhs){return mpInt(lhs)-=rhs;}
  friend mpInt operator*(const mpInt&lhs,const mpInt&rhs){return mpInt(lhs)*=rhs;}
  friend mpInt operator/(const mpInt&lhs,const mpInt&rhs){return mpInt(lhs)/=rhs;}
  friend mpInt operator+(int64_t lhs,const mpInt&rhs){return mpInt(lhs)+=rhs;}
  friend mpInt operator-(int64_t lhs,const mpInt&rhs){return mpInt(lhs)-=rhs;}
  friend mpInt operator*(int64_t lhs,const mpInt&rhs){return mpInt(lhs)*=rhs;}
  friend mpInt operator/(int64_t lhs,const mpInt&rhs){return mpInt(lhs)/=rhs;}
  friend mpInt operator+(const mpInt&lhs,int64_t rhs){return mpInt(rhs)+=lhs;}
  friend mpInt operator-(const mpInt&lhs,int64_t rhs){return mpInt(rhs)-=lhs;}
  friend mpInt operator*(const mpInt&lhs,int64_t rhs){return mpInt(rhs)*=lhs;}
  friend mpInt operator/(const mpInt&lhs,int64_t rhs){return mpInt(rhs)/=lhs;}
  private:
  friend struct std::hash<mpInt<mod_cnt>>;
};
}
namespace std{ // ハッシュ関数の特殊化
template<std::size_t mod_cnt>
struct hash<elsie::mpInt<mod_cnt>>{
  std::size_t operator()(const elsie::mpInt<mod_cnt>&obj)const{
    std::size_t res=0;
    for(const auto&val:obj.val)
      res^=std::hash<uint32_t>{}(val)+0x9E3779B9+(res<<6)+(res>>2);
    return res;
  }
};
}
#endif