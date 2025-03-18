#ifndef ELSIE_BIG_INT
#define ELSIE_BIG_INT
#include <span>
#include <vector>
#include <string>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <concepts>
#include <iostream>
#include <type_traits>
namespace elsie{
using namespace std;
class bigInt{
private:
template<class T>using vc=vector<T>;
using i32=int32_t; using u32=uint32_t;
using i64=int64_t; using u64=uint64_t;
using vu32=vc<u32>; using vu64=vc<u64>;
using cv32=const vc<u32>;
using cv64=const vc<u64>;
using sv64=span<const u64>;
public:
// dataの最後の要素は符号拡張のみで値ではない．
vu64 data;
bigInt(vu64&&vdata):data(vdata){}
public:
bigInt():data(2,0){}
bigInt(bigInt&&b):data(move(b.data)){}
bigInt(const bigInt&b):data(b.data){}
bigInt(const string&s){}
template<class T>
bigInt(T x) requires is_integral_v<T> || same_as<remove_cvref_t<T>,__int128_t>
:data(2+(sizeof(T)==16),0){
    if constexpr(sizeof(T)<=8){
        data[0]=(u64)((i64)x);
        data[1]=(u64)(x>>(sizeof(T)*8-1)&1?-1:0);
    }else{
        data[0]=(u64)x;
        data[1]=(u64)(x>>64);
        data[2]=(u64)(u64(x>>127&1?-1:0));
    }
}
template<unsigned_integral T>
bigInt(T x) requires is_integral_v<T> || same_as<remove_cvref_t<T>,__uint128_t>
:data(2+(sizeof(T)==16),0){
    data[0]=(u64)x;
    if constexpr(sizeof(T)>8) data[1]=(u64)(x>>64);
}
// 適切にコンストラクタを呼ぶためこれでよい
template<class T>
bigInt& operator=(T&&rhs)requires is_constructible_v<bigInt,decay_t<T>>{
    if constexpr(is_bigInt<T>){
        if constexpr(is_movable_bigInt<T>)
            data=move(rhs.data);
        else data=rhs.data;
    }else*this=bigInt(forward<T>(rhs));
    return*this;
}

string to_string()const{
    bigInt cp=*this;
    constexpr uint64_t base=1'000'000'000;
    vector<uint32_t>digits;
    bool neg=cp.data.back();
    if(neg) cp=move(-cp);
    cp.data.pop_back();
    if(cp.data.size()==1&&cp.data[0]==0)return string("0");
    while(cp.data.size()){
        uint64_t remainder=0;
        for(int32_t i=(int32_t)cp.data.size()-1;i>=0;--i){
            __uint128_t cur=__uint128_t(remainder)<<64|cp.data[i];
            cp.data[i]=cur/base;
            remainder=cur%base;
        }
        digits.push_back((uint32_t)remainder);
        while(cp.data.size()&&cp.data.back()==0)cp.data.pop_back();
    }
    string r=std::to_string(digits.back());
    return r;
}
private:
template<class T> static constexpr
bool is_bigInt=is_same_v<remove_cvref_t<T>,bigInt>;

template<class T>static constexpr
bool is_copy_needed_bigInt=is_bigInt<T>&&(is_lvalue_reference_v<T>||is_const_v<remove_reference_t<T>>);

template<class T> static constexpr
bool is_movable_bigInt=is_bigInt<T>&&!is_copy_needed_bigInt<T>;

// a<b -1, a==b 0, a>b 1
inline i32 compare(cv64&a,cv64&b)const{
    // .back()は符号
    if(a.back()!=b.back()) return a.back()?-1:1;
    if(a.size()!=b.size()) return a.size()<b.size()?-1:1;
    // 負の数のとき，判定は逆転する
    i32 ret=a.back()?1:-1;
    for(u64 i=a.size()-2;i>=0;--i){
        if(a[i]==b[i])continue;
        return a[i]<b[i]?ret:-ret;
    }
    return 0;
}
inline void bit_rev(){ for(auto&x:data)x=~x; }
inline void negate(){
    bool sign=data.back();
    bit_rev();
    data.back()=sign?uint64_t(-1)-1:1;
    data.push_back(~sign);
    *this+=bigInt(1ul);
    shrink_to_fit();
}
void shrink_to_fit(){
    while(data.size()>2){
        if(data.back()==data[data.size()-2])
            data.pop_back();
        else break;
    }
}
private:
template<bool is_sub>
void adder(vu64&a,sv64&b){
    uint64_t carry=is_sub,sign; // 桁溢れを保持 減算のとき~x+1の1
    // |a=lhs.data|>=|b=rhs.data|より，短い方の長さを参照する
    // 最後は符号なので飛ばす．
    for(uint64_t i=0;i<b.size()-1;++i){
        __uint128_t add;
        if constexpr(is_sub) add=(__uint128_t)a[i]+(~b[i])+carry;
        else add=(__uint128_t)a[i]+b[i]+carry;
        a[i]=(u64)add;
        carry=(u64)(add>>64);
    }
    // 符号拡張
    if constexpr(is_sub)sign=~b.back();
    else sign=b.back();
    for(uint64_t i=rhs.data.size()-1;i<data.size()-1;++i){
        __uint128_t add=(__uint128_t)data[i]+sign+carry;
        data[i]=(uint64_t)add;
        carry=(uint64_t)(add>>64);
    }
    sign=is_sub?a.back()==sign:a.back()!=sign;
    if(sign){ // 符号
        // 異符号の加算, 同符号の減算では元のbitより情報量が増えることはない．
        // -1->0で桁溢れが発生するため，桁あふれするならば正の数である．
        // そうでないとき，負の数である．
        data.back()=carry?0:~(uint64_t)0;
    }else{
        // 同符号で桁溢れしたならば精度が足りない．
        // carryをそのまま加算し，符号拡張する
        data.back()+=carry;
        data.push_back(sign);
    }
    shrink_to_fit();
}
static constexpr array<size_t,3>Lp{64,80,250};
void naive_mul(sv64 a,sv64 b,sv64 r){
    sv64 s=a,t=b;
    if(a.size()<b.size())swap(s,t);
    for(size_t i=0;i<s.size();++i){
        __uint128_t carry=0;
        for(size_t j=0;j<t.size();++j){
            u64 hi,lo;
            asm("mulq %3" :"=a"(lo),"=d"(hi):"a"(s[i]),"r"(t[j]));
            carry+=__uint128_t(lo)+r[i+j];
            r[i+j]=(u64)carry;
            carry>>=64;
            carry+=__uint128_t(hi)+r[i+j+1];
            r[i+j+1]=(u64)carry;
            carry>>=64;
        }
    }
}
void karatsuba(sv64 a,sv64 b,sv64 r){
    const size_t msize=min(a.size(),b.size());
    if(msize<Lp[0]) return naive_mul(a,b,r);
    const size_t hsize=msize>>1,wsize=hsize<<1;
    auto split=[&](sv64&s){ return{sv64(s.begin()+hsize,s.end()),sv64(s.begin(),s.begin()+hsize)}; };
    auto[ahi,alow]=split(a);
    auto[bhi,blow]=split(b);
    sv64 low(r.begin(),wsize),hi(r.begin()+wsize,r.end());
    karatsuba(alow,blow,low);
    karatsuba(ahi,bhi,hi);
    auto sub=[&](sv64 Hi,sv64 Lo)-> vu64 {
        // 未実装
    };
    bigInt mid(sub(ahi,alow));
    mid*=bigInt(sub(bhi,blow));
    // mid-(hi+low)して，r-mid
    __uint128_t carry=1;
    if(mid.data.size()-1<hi.size()) mid.data.resize(hi.size(),mid.data.back()); // 符号拡張
    for(size_t i=0;i<hi.size();++i){
        carry+=mid[i]+(~(__uint128_t(hi[i])+low[i]))+carry;
        mid[i]=(u64)carry;
        carry>>=64;
    }
    for(size_t i=hi.size();carry>0;++i){
        carry+=__uint128_t(mid[i])+carry;
        mid[i]=(u64)carry;
        carry>>=64;
    }
    carry=1;
    for(size_t i=0;i<mid.size();++i){
        carry+=__uint128_t(r[i+hsize])+(~mid[i])+carry;
        r[i+hsize]=(u64)carry;
        carry>>=64;
    }
    for(size_t i=hsize+mid.size();carry>0;++i){
        carry+=__uint128_t(r[i]);
        r[i]=(u64)carry;
        carry>>=64;
    }
    return r;
}
void ntt_cnvlt(sv64 a,sv64 b,sv64 r){}
void multiply(cv64&a,cv64&b){
    // 符号を除いた配列サイズ
    const size_t m=min<size_t>(a.size(),b.size())-1;
    // 新しい配列のサイズ
    const size_t n=a.size()+b.size()-1;
    // unsignedで処理
    sv64 s(a.begin(),a.end()-1),t(b.begin(),b.end()-1);
    vu64 r(n,0);
    sv64 rspan(r.begin(),r.end()-1);
         if(m<=Lp[0]) naive_mul(s,t,rspan); // O(n^2)
    else if(m<=Lp[1]) karatsuba(s,t,rspan); // O(n^{1.58})
    else if(m<=Lp[2]) toomcook3(s,t,rspan); // O(n^{1.46})
    else              ntt_cnvlt(s,t,rspan); // O(n\lg n)
    r.push_back(0);
}
public:
// arithmetic operation

bigInt operator+()const noexcept{return*this;}
bigInt operator-()const noexcept{
    vu64 d(data.size());
    __uint128_t carry=1;
    for(size_t i=0;i<d.size()-1;++i){
        carry+=~data[i];
        d[i]=(u64)carry;
        carry>>=64;
    }
    if(carry&&data.back()==0) d.back()=0; // 0
    else d.back()=~data.back();
    return bigInt(move(d));
}
template<class T>
bigInt& operator+=(T&&rhs)requires is_constructible_v<bigInt,decay_t<T>> {
    // bigInt以外では構築
    if constexpr(!is_bigInt<T>) return*this+=bigInt(forward<T>(rhs));
    if(rhs.data.size()>data.size()) // 自分のほうが短いときに場所を確保する．
        if constexpr (is_copy_needed_bigInt<T>) // rhsが左辺値なら拡張確定
            data.resize(rhs.data.size(),data.back()); // 符号拡張
        else swap(data,rhs.data); // movableなrhsが長いなら入れ替え
    adder<false>(data,sv64(rhs.data.begin(),rhs.data.end()));
    return*this;
}
template<class T>
bigInt& operator-=(T&&rhs)requires is_constructible_v<bigInt,decay_t<T>> {
    if constexpr(!is_bigInt<T>)return*this-=bigInt(forward<T>(rhs));
    if(rhs.data.size()>data.size()) // 自分のほうが短いときに場所を確保する．
        data.resize(rhs.data.size(),data.back()); // 符号拡張
    adder<true>(data,sv64(rhs.data.begin(),rhs.data.end()));
    return*this;
}
template<class T>
bigInt& operator*=(T&&rhs)requires is_constructible_v<bigInt,decay_t<T>> {
    if constexpr(!is_bigInt<T>)*this*=bigInt(forward<T>(rhs));
    else{
        if(data.back()==rhs.data.back()){ // 同符号
            multiply(data,rhs.data);
            shrink_to_fit();
        }else{ // 異符号
            if(data.back()){
                negate();
                *this*=forward<T>(rhs);
            }else{
                if constexpr(is_movable_bigInt<T>){
                    rhs.negate();
                    *this*=forward<T>(rhs);
                }else*this*=-rhs;
                negate();
            }
        }
        return*this;
    }
}
template<class T>
bigInt& operator/=(T&&rhs)requires is_constructible_v<bigInt,decay_t<T>> {
    return*this;
}
template<class T>
bigInt& operator%=(T&&rhs)requires is_constructible_v<bigInt,decay_t<T>> {
    return*this;
}
//前置
bigInt& operator++(){ return*this+=bigInt((u64)1ul); }
bigInt& operator--(){
    return*this-=bigInt((i64)(-1));
}
//後置
bigInt operator++(int32_t){
}
bigInt operator--(int32_t){
}

template<class S,class T>
friend bigInt operator+(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& is_constructible_v<bigInt,decay_t<T>>
&& is_constructible_v<bigInt,decay_t<T>> {
    if constexpr(is_bigInt<S>&&is_movable_bigInt<T>)
        return rhs+=forward<T>(lhs);
    else if constexpr(is_bigInt<T>&&is_movable_bigInt<S>)
        return lhs+=forward<S>(rhs);
    else return bigInt(forward<S>(lhs))+=forward<T>(rhs);
}
template<class S,class T>
friend bigInt operator-(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& is_constructible_v<bigInt,decay_t<T>>
&& is_constructible_v<bigInt,decay_t<T>> {
    if constexpr(is_bigInt<S>&&is_movable_bigInt<T>)
        return rhs-=forward<T>(lhs);
    else if constexpr(is_bigInt<T>&&is_movable_bigInt<S>)
        return lhs+=forward<S>(rhs);
    else return bigInt(forward<S>(lhs))+=forward<T>(rhs);
}
template<class S,class T>
friend bigInt operator*(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& is_constructible_v<bigInt,decay_t<T>>
&& is_constructible_v<bigInt,decay_t<T>> {
    return lhs; //dammy
}
template<class S,class T>
friend bigInt operator/(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& is_constructible_v<bigInt,decay_t<T>>
&& is_constructible_v<bigInt,decay_t<T>> {
    return lhs; // dammy
}
template<class S,class T>
friend bigInt operator%(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& is_constructible_v<bigInt,decay_t<T>>
&& is_constructible_v<bigInt,decay_t<T>> {
    return lhs; // dammy
}

// bit operation
bigInt operator~()const{
    bigInt r=*this;
    r.bit_rev();
    return r;
}
template<class T>
bigInt& operator<<=(T&&rhs)requires is_constructible_v<bigInt,decay_t<T>> {
    return*this;
}
template<class T>
bigInt& operator>>=(T&&rhs)requires is_constructible_v<bigInt,decay_t<T>> {
    return*this;
}
template<class T>
bigInt& operator&=(T&&rhs)requires is_constructible_v<bigInt,decay_t<T>> {
    return*this;
}
template<class T>
bigInt& operator|=(T&&rhs)requires is_constructible_v<bigInt,decay_t<T>> {
    return*this;
}
template<class T>
bigInt& operator^=(T&&rhs)requires is_constructible_v<bigInt,decay_t<T>> {
    return*this;
}

// logic operation
bool operator!()const noexcept{}
template<class S,class T>
friend bool operator<(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& is_constructible_v<bigInt,decay_t<T>>
&& is_constructible_v<bigInt,decay_t<T>> {
}
template<class S,class T>
friend bool operator>(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& is_constructible_v<bigInt,decay_t<T>>
&& is_constructible_v<bigInt,decay_t<T>> {
}
template<class S,class T>
friend bool operator==(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& is_constructible_v<bigInt,decay_t<T>>
&& is_constructible_v<bigInt,decay_t<T>> {
}
template<class S,class T>
friend bool operator!=(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& is_constructible_v<bigInt,decay_t<T>>
&& is_constructible_v<bigInt,decay_t<T>> {
}
template<class S,class T>
friend bool operator<=(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& is_constructible_v<bigInt,decay_t<T>>
&& is_constructible_v<bigInt,decay_t<T>> {
}
template<class S,class T>
friend bool operator>=(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& is_constructible_v<bigInt,decay_t<T>>
&& is_constructible_v<bigInt,decay_t<T>> {
}
template<class S,class T>
friend bool operator&&(S&&lhs,T&&rhs)
requires (is_bigInt<S>||is_bigInt<T>)
&& is_constructible_v<bigInt,decay_t<T>>
&& is_constructible_v<bigInt,decay_t<T>> {
}


private: // friends
template<class Char,class Traits> friend struct std::basic_ostream<Char,Traits>;
template<class Char,class Traits> friend struct std::basic_istream<Char,Traits>;
};
} // namespace elsie

namespace std{ // 出力
template<class Char,class Traits>
std::basic_ostream<Char,Traits>&operator<<(std::basic_ostream<Char,Traits>&os,const elsie::bigInt&v){
}
template<class Char,class Traits>
std::basic_istream<Char,Traits>&operator<<(std::basic_istream<Char,Traits>&os,elsie::bigInt&v){
}
}
#endif