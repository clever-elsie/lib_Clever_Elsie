#include <cstdint>
#include <iostream>
#ifndef ELSIE_MODINT
#define ELSIE_MODINT
namespace elsie{
using namespace std;
template<size_t M=998244353>
class modint{
	private:
	long long x;
	public:
	modint():x(0){}
	template<integral T>
	modint(T val){
		val%=M;
		x=val+(val<0)*M;
	}
	operator size_t(){ return x;}
	modint&operator=(const modint&a)&{this->x=a.x;return *this;}
	template<integral T>modint&operator=(const T&a)&{return*(this=modint(a));}
	modint operator+(){return*this;}
	modint operator-(){return modint(-x);}
	modint&operator++(){if(++x==M)x=0;return*this;}
	modint&operator--(){if(--x==-1)x=M-1;return*this;}
	modint operator++(int32_t){modint r=*this;++*this;return r;}
	modint operator--(int32_t){modint r=*this;--*this;return r;}
	modint&operator+=(const modint&a){x+=a.x;if(x>=M)x-=M;return*this;}
	modint&operator-=(const modint&a){x-=a.x;if(x<0)x+=M;return*this;}
	modint&operator*=(const modint&a){x=(x*a.x)%M;return*this;}
	modint&operator/=(const modint&a){return*this*=a.inv();}
	template<integral T>modint&operator+=(const T&a){x+=a;if(x>=M)x-=M;return*this;}
	template<integral T>modint&operator-=(const T&a){x-=a;if(x<0)x+=M;return*this;}
	template<integral T>modint&operator*=(const T&a){x=(x*a)%M;return*this;}
	template<integral T>modint&operator/=(const T&a){return*this*=modint(a).inv();}
	template<class T>friend modint operator+(const modint&a,const T&b){return modint(a.x)+=b;}
	template<class T>friend modint operator-(const modint&a,const T&b){return modint(a.x)-=b;}
	template<class T>friend modint operator*(const modint&a,const T&b){return modint(a.x)*=b;}
	template<class T>friend modint operator/(const modint&a,const T&b){return modint(a.x)/=b;}
	friend bool operator==(const modint&a,const modint&b){return a.x==b.x;}
	friend bool operator!=(const modint&a,const modint&b){return a.x!=b.x;}
	template<integral T>friend bool operator==(const modint&a,const T&b){return a.x==b;}
	template<integral T>friend bool operator!=(const modint&a,const T&b){return a.x!=b;}
	template<integral T>friend bool operator==(const T&a,const modint&b){return a==b.x;}
	template<integral T>friend bool operator!=(const T&a,const modint&b){return a!=b.x;}
	friend ostream&operator<<(ostream&os,const modint&a){return os<<a.x;}
	friend istream&operator>>(istream&is,const modint&a){
		long long v;is>>v;
		a=modint(v);
		return is;
	}
	modint val()const{ return x; }
	modint pow(long long n)const{
		if(n<0)return pow(-n).inv();
		modint v=*this,r=1;
		for(;n;v*=v,n>>=1)if(n&1)r*=x;
		return r;
	}
	modint inv()const{
		long long s=x,t=M,v=1,u=0;
		while(t){
			long long k=s/t;
			s-=k*t;
			swap(s,t);
			v-=k*u;
			swap(v,u);
		}
		return modint(v);
	}
};
}
#endif