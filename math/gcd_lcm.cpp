#include <concepts>
#include <type_traits>
#include "power.cpp"
using namespace std;

int gcd_(int a,int b){
	if(b==0) return a;
	int r=1;if(a<0)a=-a;if(b<0)b=-b;
	while(r) r=a%b,a=b,b=r;
	return a;
}
int lcm_(int a,int b){ return a/gcd_(a,b)*b; }

int egcd(int a,int b,int&x,int&y){
	if(b==0){
		x=1,y=0;
		return a;
	}
	int d=egcd(b,a%b,y,x);
	y-=a/b * x;
	return d;
}

template<integral T>T exgcd(T a,T b,T&x,T&y){
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

template<integral T>T mod_inv(T a,T m){
	T x,y,g=exgcd(a,m,x,y);
	if(g!=1)return -1;
	return(x%m+m)%m;
}

// ./power.cpp::modpow
template<integral T>T mod_inv(T a,T p){
	return modpow(a,p-2,p);
}