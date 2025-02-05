#include<map>
#include<cstddef>
#include<cstdint>
#include<numeric>
#include<concepts>
#include<iostream>
#include<type_traits>
#include<unordered_map>
#ifndef ELSIE_datastructure_math
#define ELSIE_datastructure_math
namespace elsie{
using namespace std;

	class frac{
		using ul=uint64_t;
		bool s;
		ul n,d;
		frac*__internal_copy(frac*T,const frac&o){
			T->s=o.s,T->n=o.n,T->d=o.d;
			return T;
		}
	public:
		frac approx(){
			ul g;
			(n&&d)?(g=gcd(n,d),n/=g,d/=g):(n?n=0:d=1);
			return*this;
		}
		frac():s(false),n(0ULL),d(1ULL){}
		frac(bool S,ul N,ul D=1):s(S),n(N),d(D){}
		frac(const frac&f):s(f.s),n(f.n),d(f.d){}
		frac(frac*f):s(f->s),n(f->n),d(f->d){}
		template<unsigned_integral T>frac(T N,T D=1):n(N),d(D){s=false;}
		template<signed_integral T>frac(T N,T D=1){
			if(D==0)D=1;
			s=(((N<=0&&D<=0)||(N>=0&&D>=0))?0:1);
			n=(N<0?-N:N);d=(D<0?-D:D);
		}
		frac&operator+=(frac o){
			ul g=gcd(d,o.d),p=d/g,q=o.d/g;
			n*=q,o.n*=p;
			s=(s!=o.s&&(s?n>o.n:n<o.n));
			n=s!=o.s?(n>o.n?n-o.n:o.n-n):n+o.n;
			d*=q;
			return*this;
		}
		frac&operator-=(frac o){o.s=!o.s;*this+=o;return*this;}
		frac operator*=(const frac&o){
			return*this=frac(this)*o;}
		frac operator/=(const frac&o){
			return*this=frac(this)/o;}
		bool operator==(const frac &o) {
			ul g=gcd(this->d,o.d);
			return((this->s==o.s)&&(o.d/g*this->n==this->d/g*o.n));
		}
		bool operator!=(const frac&o){return!(*this==o);}
		bool operator>=(const frac&o){return!(*this<o);}
		bool operator<=(const frac&o){return!(*this>o);}
		bool operator>(const frac&o){
			ul g=gcd(this->d,o.d),l=this->n*o.d/g,r=this->d/g*o.n;
			return(this->s==o.s?o.s?l<r:l>r:o.s);
		}
		bool operator<(const frac &o) {
			ul g=gcd(this->d,o.d),l=this->n*o.d/g,r=this->d/g*o.n;
			return(this->s==o.s?o.s?l>r:l<r:this->s);
		}
		frac operator=(const frac&o){return*__internal_copy(this,o);}
		frac operator-()const{return frac(!(this->s),this->n,this->d);}
		frac operator+(const frac&o){
			frac ret;
			ul G=gcd(this->d,o.d),p=this->d/G,q=o.d/G;
			return frac(this->n*q+o.n*p,p*q*G).approx();
		}
		frac operator-(const frac&o){return*this+(-o);}
		frac operator*(const frac&o){
			if(this->n==0||o.n==0)return frac(0ll);
			else{
				ul g1=gcd(this->n,o.d),g2=gcd(this->d,o.n);
				return frac(this->s^o.s,this->n/g1*o.n/g2,this->d/g2*o.d/g1).approx();
			}
		}
		frac operator/(const frac&o){return*this*frac((o.s?-o.d:o.d),o.n);}
		friend ostream&operator<<(ostream&dst,frac&o){
			if(o.s)dst<<"-";
			dst<<o.n<<"/"<<o.d;
			return dst;
		}
	};

	template<size_t M=998244353>class modint{
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

	struct mex{
		map<int,int>v; // [l,r)
		unordered_map<int,int>c; // {val,cnt}
		mex(){v[-1]=0;}
		void add(int a){
			{ // count phase
				auto itr=c.find(a);
				if(itr!=c.end()){
					itr->second++;
					return;
				}else c[a]=1;
			}
			if(a<0)return;
			auto itr=v.upper_bound(a);
			itr--;
			if(a<itr->second)return;
			if(itr->second==a) itr->second++;
			else{
				v[a]=a+1;
				itr=v.find(a);
			}
			auto jtr=itr;
			jtr++;
			if(jtr==v.end())return;
			if(itr->second==jtr->first){
				itr->second=jtr->second;
				v.erase(jtr);
			}
		}
		void erase(int a){
			{ // count phase
				auto itr=c.find(a);
				if(itr==c.end())return;
				itr->second--;
				if(itr->second>0) return;
				c.erase(itr);
			}
			if(a<0)return;
			auto itr=v.upper_bound(a);
			itr--;
			if(itr->second-1==a){
				itr->second--;
				return;
			}
			v[a+1]=itr->second;
			itr->second=a;
		}
		int find(){return v[-1];}
		size_t count(int a){
			auto itr=c.find(a);
			return(itr==c.end()?0ull:itr->second);
		}
	};
}
#endif