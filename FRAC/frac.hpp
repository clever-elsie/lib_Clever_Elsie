#include"header_std.hpp"

using namespace std;
class frac {
	using l = signed long long;
	using ul = unsigned long long;
	bool s;
	ul n, d;
	ul egcd(ul a, ul b) const {
		ul r = 1, g = a;
		if (a < b) a = b, b = g;
		while (r) r = a % b, (r) ? a = b, b = r : 0;
		return b;
	}
	inline ul __internal_abs(l a){ return (a>=0?a:-a); }
	frac*__internal_copy(frac*T,const frac&o){
		T->s=o.s;
		T->n=o.n;
		T->d=o.d;
		return T;
	}
public:
	frac approx() {
		if (n == 0 || d == 0)
			if (n == 0) d = 1;
			else n = 0;
		else {
			ul g = egcd(n, d);
			n = n / g, d = d / g;
		}
		return *this;
	}
	frac() : s(false), n(0ULL), d(1ULL) {}
	frac(bool S,ul N,ul D=1ull):s(S),n(N),d(D){};
	frac(ul N, ul D = 1ULL) : n(N), d(D) { s = false; }
	frac(const frac &f) : s(f.s), n(f.n), d(f.d) {}
	frac(const frac *f) : s(f->s), n(f->n), d(f->d) {}
	frac(frac::l N, frac::l D = 1LL) {
		if (D == 0) D = 1;
		s = (((N < 0 && D < 0) || (N >= 0 && D >= 0)) ? 0 : 1);
		n = __internal_abs(N);
		d = __internal_abs(D);
	}
	bool operator==(const frac &o) {
		ul g = egcd(this->d, o.d);
		return ((this->s == o.s) && (this->n * o.d / g == this->d / g * o.n));
	}
	bool operator==(const ul o) { return *this == frac(o); }
	bool operator==(const l o) { return *this == frac(o); }
	bool operator!=(const frac &o) { return !(*this == o); }
	bool operator!=(const ul o) { return !(*this == o); }
	bool operator!=(const l o) { return !(*this == o); }
	bool operator>=(const frac &o) { return !(*this < o); }
	bool operator>=(const ul o) { return *this >= frac(o); }
	bool operator>=(const l o) { return *this >= frac(o); }
	bool operator<=(const frac &o) { return !(*this > o); }
	bool operator<=(const ul o) { return *this <= frac(o); }
	bool operator<=(const l o) { return *this <= frac(o); }
	bool operator>(const frac &o) {
		ul g = egcd(this->d, o.d);
		ul l=this->n*o.d/g, r=this->d/g*o.n;
		if(this->s==o.s) return (o.s?l<r:l>r);
		else return o.s;
	}
	bool operator>(const ul o) { return *this>frac(o); }
	bool operator>(const l o) { return *this > frac(o); }
	bool operator<(const frac &o) {
		ul g = egcd(this->d, o.d);
		ul l=this->n*o.d/g, r=this->d/g*o.n;
		if(this->s==o.s) return (o.s?l>r:l<r);
		else return this->s;
	}
	bool operator<(const ul o) { return *this < frac(o); }
	bool operator<(const l o) { return *this < frac(o); }
	frac operator=(const frac &o) { return *__internal_copy(this,o); }
	frac operator=(const ul o) { return *this=frac(o); }
	frac operator=(const l o) { return *this=frac(o); }
	frac operator-()const{ return frac(!(this->s),this->n,this->d);}
	frac operator+(const frac &o) const {
		frac ret;
		ul G = egcd(this->d, o.d);
		ul p=this->d/G,q=o.d/G;
		return frac(this->n*q+o.n*p,p*q*G).approx();
	}
	frac operator+(const ul o) const { return *this + frac(o); }
	frac operator+(const l o) const { return *this + frac(o); }
	frac operator-(const frac &o) { return *this + (-o); }
	frac operator-(const ul o) { return *this + frac(true,o); }
	frac operator-(const l o) { return *this + frac(-o); }
	frac operator*(const frac &o) {
		if (this->n == 0 || o.n == 0) return frac(0ll);
		else {
			ul g1 = egcd(this->n, o.d), g2 = egcd(this->d, o.n);
			return frac(this->s ^ o.s,
				this->n / g1 * o.n / g2,
				this->d / g2 * o.d / g1).approx();
		}
	}
	frac operator*(const ul o) { return *this * frac(o); }
	frac operator*(const l o) { return *this * frac(o); }
	frac operator/(const frac &o) { return *this * frac((o.s?-o.d:o.d),o.n); }
	frac operator/(const ul o) { return *this * frac(1ull,o); }
	frac operator/(const l o) { return *this * frac(1ll,o); }
	frac operator+=(const frac &o) { return *this = *this + o; }
	frac operator+=(const ul o) { return *this = *this + o; }
	frac operator+=(const l o) { return *this = *this + o; }
	frac operator-=(const frac &o) { return *this = *this - o; }
	frac operator-=(const ul o) { return *this = *this - o; }
	frac operator-=(const l o) { return *this = *this - o; }
	frac operator*=(const frac &o) { return *this = *this * o; }
	frac operator*=(const ul o) { return *this = *this * o; }
	frac operator*=(const l o) { return *this = *this * o; }
	frac operator/=(const frac &o) { return *this = *this / o; }
	frac operator/=(const ul o) { return *this = *this / o; }
	frac operator/=(const l o) { return *this = *this / o; }
};