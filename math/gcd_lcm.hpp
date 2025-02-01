#include <concepts>
#include <type_traits>
#include "math/power.cpp"
#ifndef ELSIE_GCD_LCM
#define ELSIE_GCD_LCM
namespace elsie{
using namespace std;
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
		T k=a/b;
		T r=a%b;
		assign(a,b,b,r);
		assign(x,u,u,x-u*k);
		assign(y,v,v,y-v*k);
	}
	if(a<0)a=-a,x=-x,y=-y;
	return a;
}

template<class T>T mod_inv(T a,T m){
	T x,y,g=exgcd(a,m,x,y);
	if(g!=1)return -1;
	return(x%m+m)%m;
}

template<class T>T mod_inv_prime(T a,T p){
	return modpow(a,p-2,p);
}
}
#endif