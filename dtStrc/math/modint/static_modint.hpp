#ifndef ELSIE_STATIC_MINT
#define ELSIE_STATIC_MINT
#include <cstddef>
#include <cstdint>
#include <utility>
#include <concepts>
#include <math/basic_math.hpp>
namespace elsie{
    using namespace std;
    template<uint32_t M=998244353>
    class mint{
        private: int32_t x;
        public:
        mint():x(0){}
        mint(const mint&n):x(n.x){}
        template<integral T>mint(T val){ x=val%M+(val<0)*M; }
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
                swap(s,t);
                v-=k*u%M;
                swap(v,u);
            }
            return mint(v);
        }
        size_t mod(){ return M; }
        template<integral T>friend mint raw(T val){
            mint r;
            r.x=val;
            return r;
        }

        mint&operator=(const mint&a)&{x=a.x;return*this;}
        template<integral T>mint&operator=(const T&a)&{return*this=mint(a);}
        mint operator+(){return*this;}
        mint operator-(){return mint(-x);}
        mint&operator++(){if(++x==M)x=0;return*this;}
        mint&operator--(){if(--x==-1)x=M-1;return*this;}
        mint operator++(int32_t){mint r=*this;++*this;return r;}
        mint operator--(int32_t){mint r=*this;--*this;return r;}
        mint&operator+=(const mint&a){x+=a.x;if(x>=M)x-=M;return*this;}
        mint&operator-=(const mint&a){x-=a.x;if(x<0)x+=M;return*this;}
        mint&operator*=(const mint&a){x=x*a.x%M;return*this;}
        mint&operator/=(const mint&a){return*this*=a.inv();}
        template<integral T>mint&operator+=(const T&a){return*this+=mint(a);}
        template<integral T>mint&operator-=(const T&a){return*this-=mint(a);}
        template<integral T>mint&operator*=(const T&a){return*this*=mint(a);}
        template<integral T>mint&operator/=(const T&a){return*this*=mint(a).inv();}
        template<class T>friend mint operator+(const mint&a,const T&b){return mint(a.x)+=b;}
        template<class T>friend mint operator-(const mint&a,const T&b){return mint(a.x)-=b;}
        template<class T>friend mint operator*(const mint&a,const T&b){return mint(a.x)*=b;}
        template<class T>friend mint operator/(const mint&a,const T&b){return mint(a.x)/=b;}

        friend bool operator==(const mint&a,const mint&b){return a.x==b.x;}
        friend bool operator!=(const mint&a,const mint&b){return a.x!=b.x;}
        template<integral T>friend bool operator==(const mint&a,const T&b){return a.x==mint(b);}
        template<integral T>friend bool operator!=(const mint&a,const T&b){return a.x!=mint(b);}
        template<integral T>friend bool operator==(const T&a,const mint&b){return mint(a)==b.x;}
        template<integral T>friend bool operator!=(const T&a,const mint&b){return mint(a)!=b.x;}

        friend ostream&operator<<(ostream&os,const mint&a){return os<<a.x;}
        friend istream&operator>>(istream&is,const mint&a){
            int64_t v;is>>v;
            a=mint(v);
            return is;
        }
    };
}
#endif