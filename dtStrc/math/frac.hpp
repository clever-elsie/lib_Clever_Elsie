#ifndef ELSIE_FRAC
#define ELSIE_FRAC
#include <cstdint>
#include <numeric>
#include <concepts>
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
            if(d==0) n=0,d=1;
            else if(n==0) d=1;
            else{
                ul g=gcd(n,d);
                n/=g,d/=g;
            }
            return*this;
        }
        frac():s(false),n(0ULL),d(1ULL){}
        frac(bool S,ul N,ul D=1):s(S),n(N),d(D){}
        frac(const frac&f):s(f.s),n(f.n),d(f.d){}
        frac(frac*f):s(f->s),n(f->n),d(f->d){}
        template<unsigned_integral T>frac(T N,T D=1):s(0),n(N),d(D){}
        template<signed_integral T>frac(T N,T D=1){
            if(D==0)D=1;
            s=(((N<=0&&D<=0)||(N>=0&&D>=0))?0:1);
            n=(N<0?-N:N);d=(D<0?-D:D);
        }
        frac&operator+=(frac o){
            if(s==o.s)*this=frac(s,n*o.d+o.n*d,d*o.d).approx();
            else{
                if(s)swap(*this,o);
                ul ad=n*o.d,bc=o.n*d;
                if(ad>=bc) *this=frac(0,ad-bc,d*o.d).approx();
                else *this=frac(1,bc-ad,d*o.d).approx();
            }
            return*this;
        }
        frac&operator-=(frac o){o.s=!o.s;*this+=o;return*this;}
        frac operator*=(const frac&o){
            frac s(n,o.d),t(o.n,d);
            s.approx(),t.approx();
            n=s.n*t.n, d=s.d*t.d;
            this->s=this->s^o.s;
            return*this;
        }
        frac operator/=(const frac&o){
            return*this*=frac(o.s,o.d,o.n);
        }
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
        friend frac operator+(const frac&lhs,const frac&rhs){ return frac(lhs)+=rhs;}
        friend frac operator-(const frac&lhs,const frac&rhs){ return frac(lhs)-=rhs;}
        friend frac operator*(const frac&lhs,const frac&rhs){ return frac(lhs)*=rhs;}
        friend frac operator/(const frac&lhs,const frac&rhs){ return frac(lhs)/=rhs;}
        friend ostream&operator<<(ostream&dst,const frac&o){
            if(o.s)dst<<"-";
            if(o.d>1) dst<<o.n<<"/"<<o.d;
            else dst<<o.n;
            return dst;
        }
    };
}
#endif