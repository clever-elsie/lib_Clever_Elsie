#ifndef ELSIE_DYNAMIC_modint32
#define ELSIE_DYNAMIC_modint32
#include <cstddef>
#include <cstdint>
#include <utility>
#include <concepts>
#include <math/basic_math.hpp>
namespace elsie{
  template<int64_t id=-1>
  class modint32{
    private:
    int32_t x;
    static barret32 bt;
    static uint32_t M;
    public:
    modint32():x(0){}
    modint32(const modint32&n):x(n.x){}
    template<std::integral T>modint32(T val){ x=val%M+(val<0)*M; }
    friend void set_mod(uint32_t m){ bt.set(m),M=m; }
    modint32 val()const{ return x; }
    modint32 pow(uint64_t n)const{
      modint32 v=*this,r=1;
      for(;n;v*=v,n>>=1)if(n&1)r*=x;
      return r;
    }
    modint32 inv()const{
      uint32_t s=x,t=M,v=1,u=0;
      while(t){
        uint32_t k=s/t;
        s-=bt.mul(k,t);
        std::swap(s,t);
        v-=bt.mul(k,u);
        std::swap(v,u);
      }
      return modint32(v);
    }
    std::size_t mod(){ return bt.umod(); }
    template<std::integral T>friend modint32 raw(T val){
      modint32 r;
      r.x=val;
      return r;
    }

    modint32&operator=(const modint32&a)&{x=a.x;return*this;}
    template<std::integral T>modint32&operator=(const T&a)&{return*this=modint32(a);}
    modint32 operator+(){return*this;}
    modint32 operator-(){return modint32(-x);}
    modint32&operator++(){if(++x==M)x=0;return*this;}
    modint32&operator--(){if(--x==-1)x=M-1;return*this;}
    modint32 operator++(int32_t){modint32 r=*this;++*this;return r;}
    modint32 operator--(int32_t){modint32 r=*this;--*this;return r;}
    modint32&operator+=(const modint32&a){x+=a.x;if(x>=M)x-=M;return*this;}
    modint32&operator-=(const modint32&a){x-=a.x;if(x<0)x+=M;return*this;}
    modint32&operator*=(const modint32&a){x=bt.mul(x,a.x);return*this;}
    modint32&operator/=(const modint32&a){return*this*=a.inv();}
    template<std::integral T>modint32&operator+=(const T&a){return*this+=modint32(a);}
    template<std::integral T>modint32&operator-=(const T&a){return*this-=modint32(a);}
    template<std::integral T>modint32&operator*=(const T&a){return*this*=modint32(a);}
    template<std::integral T>modint32&operator/=(const T&a){return*this*=modint32(a).inv();}
    template<class T>friend modint32 operator+(const modint32&a,const T&b){return modint32(a.x)+=b;}
    template<class T>friend modint32 operator-(const modint32&a,const T&b){return modint32(a.x)-=b;}
    template<class T>friend modint32 operator*(const modint32&a,const T&b){return modint32(a.x)*=b;}
    template<class T>friend modint32 operator/(const modint32&a,const T&b){return modint32(a.x)/=b;}

    friend bool operator==(const modint32&a,const modint32&b){return a.x==b.x;}
    friend bool operator!=(const modint32&a,const modint32&b){return a.x!=b.x;}
    template<std::integral T>friend bool operator==(const modint32&a,const T&b){return a.x==modint32(b);}
    template<std::integral T>friend bool operator!=(const modint32&a,const T&b){return a.x!=modint32(b);}
    template<std::integral T>friend bool operator==(const T&a,const modint32&b){return modint32(a)==b.x;}
    template<std::integral T>friend bool operator!=(const T&a,const modint32&b){return modint32(a)!=b.x;}

    friend std::ostream&operator<<(std::ostream&os,const modint32&a){return os<<a.x;}
    friend std::istream&operator>>(std::istream&is,const modint32&a){
      int64_t v;is>>v;
      a=modint32(v);
      return is;
    }
  };
}
#endif