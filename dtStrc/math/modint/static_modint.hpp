#ifndef ELSIE_STATIC_MINT
#define ELSIE_STATIC_MINT
#include <cstddef>
#include <cstdint>
#include <utility>
#include <concepts>
#include <math/basic_math.hpp>
namespace elsie{
  template<class T>
  concept ints=std::is_integral_v<T>||std::same_as<T,__int128_t>||std::same_as<T,__uint128_t>;
  template<uint32_t M=998244353>
  class mint{
    public: // static
    constexpr static std::size_t mod(){ return M; }
    template<std::integral T>
    static mint raw(T val){
      mint r;
      r.x=val;
      return r;
    }
    private: uint32_t x; // field
    public:
    mint():x(0){}
    mint(const mint&n):x(n.x){}
    template<ints T>mint(T val){
      if constexpr(std::is_signed_v<T>||std::same_as<T,__int128_t>){
        T v=val%M;
        if(v<0)v+=M;
        x=v;
      }else x=val%M;
    }

    uint32_t val()const{ return x; }
    mint pow(uint64_t n)const{
      mint v=*this,r=1;
      for(;n;v*=v,n>>=1)if(n&1)r*=x;
      return r;
    }
    mint inv()const{
      uint32_t s=x,t=M,v=1,u=0;
      while(t){
        uint32_t k=s/t;
        s-=k*t%M;
        std::swap(s,t);
        v-=k*u%M;
        std::swap(v,u);
      }
      return mint(v);
    }

    mint&operator=(const mint&a){x=a.x;return*this;}
    template<ints T>mint&operator=(const T&a){return*this=mint(a);}
    mint operator+(){return*this;}
    mint operator-(){return mint(M-x);}
    mint&operator++(){if(x++==M-1)x=0;return*this;}
    mint&operator--(){if(x--==0)x=M-1;return*this;}
    mint operator++(int32_t){mint r=*this;++*this;return r;}
    mint operator--(int32_t){mint r=*this;--*this;return r;}
    template<class T>
    mint&operator+=(T a) requires std::is_constructible_v<mint,T> {
      if constexpr(std::same_as<T,mint<M>>) x=(uint64_t)x+a.x-((uint64_t)x+a.x>=M?M:0);
      else *this+=mint(a);
      return*this;
    }
    template<class T>
    mint&operator-=(T a) requires std::is_constructible_v<mint,T> {
      if constexpr(std::same_as<T,mint<M>>) x=(x>=a.x?x-a.x:x+(M-a.x));
      else*this-=mint(a);
      return*this;
    }
    template<class T>
    mint&operator*=(T a) requires std::is_constructible_v<mint,T> {
      if constexpr(std::same_as<T,mint<M>>) x=(uint64_t)x*a.x%M;
      else*this*=mint(a);
      return*this;
    }
    template<class T>
    mint&operator/=(mint a) requires std::is_constructible_v<mint,T> {
      if constexpr(std::same_as<T,mint<M>>) return*this*=a.inv();
      else return*this*=mint(a).inv();
    }
    template<class T>friend mint operator+(mint a,T b) requires std::is_constructible_v<mint,T> {
      if constexpr(std::same_as<T,mint>) return a+=b;
      else return a+=mint(b);
    }
    template<class T>friend mint operator-(mint a,T b) requires std::is_constructible_v<mint,T> {
      if constexpr(std::same_as<T,mint>) return a-=b;
      else return a-=mint(b);
    }
    template<class T>friend mint operator*(mint a,T b) requires std::is_constructible_v<mint,T> {
      if constexpr(std::same_as<T,mint>) return a*=b;
      else return a*=mint(b);
    }
    template<class T>friend mint operator/(mint a,T b) requires std::is_constructible_v<mint,T> {
      if constexpr(std::same_as<T,mint>) return a*=b.inv();
      return a*=mint(b).inv();
    }
    template<ints T>friend mint operator+(T a,mint b){ return b+=mint(a); }
    template<ints T>friend mint operator-(T a,mint b){
      if constexpr(std::same_as<T,mint<M>>) return a-=b;
      else return mint(a)-=b;
    }
    template<ints T>friend mint operator*(T a,mint b){ return b*=a; }
    template<ints T>friend mint operator/(T a,mint b){ return a*mint(b).inv(); }

    friend bool operator==(mint a,mint b){return a.x==b.x;}
    friend bool operator!=(mint a,mint b){return a.x!=b.x;}
    template<ints T>friend bool operator==(mint a,T b){return a.x==mint(b).x;}
    template<ints T>friend bool operator!=(mint a,T b){return a.x!=mint(b).x;}
    template<ints T>friend bool operator==(T a,mint b){return mint(a).x==b.x;}
    template<ints T>friend bool operator!=(T a,mint b){return mint(a).x!=b.x;}

		template<class CharT,class Trait>
    friend std::basic_ostream<CharT,Trait>&operator<<(std::basic_ostream<CharT,Trait>&os,mint a){
			return os<<a.x;
		}
		template<class CharT,class Trait>
    friend std::basic_istream<CharT,Trait>&operator>>(std::basic_istream<CharT,Trait>&is,mint&a){
      int64_t v;is>>v;
      a=mint(v);
      return is;
    }
  };
}
#endif