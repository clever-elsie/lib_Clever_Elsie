#ifndef ELSIE_BIG_INT
#define ELSIE_BIG_INT
#include <vector>
#include <string>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <concepts>
#include <type_traits>
namespace elsie{
using namespace std;
class bigInt;
class bigInt{
    private:
    template<class T>using vc=vector<T>;
    using i32=int32_t; using u32=uint32_t;
    using i64=int64_t; using u64=uint64_t;
    using vu32=vc<u32>; using vu64=vc<u64>;
    using cv32=const vc<u32>;
    using cv64=const vc<u64>;
    public:
    // dataの最後の要素は符号拡張のみで値ではない．
    vu64 data;
    public:
    bigInt():data(2,0){}
    bigInt(bigInt&&b):data(move(b.data)){}
    bigInt(const bigInt&b):data(b.data){}
    bigInt(const string&s){}
    template<class T>
    bigInt(T x) requires is_integral_v<T> || same_as<remove_cvref_t<T>,__int128_t>
    :data(2+(sizeof(T)==16),0){
        if constexpr(sizeof(T)<=8) data[0]=(uint64_t)((int64_t)x);
        else{
            data[0]=(uint64_t)x;
            data[1]=(uint64_t)(x>>64);
            data[2]=(uint64_t)(int64_t(x>>127&1?-1:0));
        }
    }
    template<unsigned_integral T>
    bigInt(T x) requires is_integral_v<T> || same_as<remove_cvref_t<T>,__uint128_t>
    :data(2+(sizeof(T)==16),0){
        if constexpr(sizeof(T)<=8) data[0]=x;
        else{
            data[0]=(uint64_t)x;
            data[1]=(uint64_t)(x>>64);
        }
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
    vu32 naive_mul(cv64&a,cv64&b,const size_t n){
        // nは符号除外の配列サイズ，符号を付加してn+1
        cv64&s=a; cv64&t=b;
        if(a.size()<b.size())swap(s,t);
        vu64 r(n+1,0);
        for(size_t i=0;i<s.size()-1;++i){
            __uint128_t carry=0;
            for(size_t j=0;j<t.size()-1;++j){
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
    vu32 karatsuba(cv64&a,cv64&b,const size_t n){}
    vu32 toom_cook(cv64&a,cv64&b,const size_t n){}
    vu32 ntt_cnvlt(cv64&a,cv64&b,const size_t n){}
    vu32 multiply(cv64&a,cv64&b){
        constexpr array<size_t,3>Lp{64,80,250};
        // 符号を除いた配列サイズ
        const size_t n=a.size()+b.size()-2;
        if(n<=Lp[1]){ // 2回の比較で関数を決定できるためif文をnestedに
            if(n<=Lp[0]) data=naive_mul(a,b,n); //O(n^2)
            else data=karatsuba(a,b,n); // O(n^{1.58})
        }else{
            if(n<=Lp[2]) data=toom_cook(a,b,n); // O(n^{1.46})
            else data=ntt_cnvlt(a,b,n); // O(n\lg n)
        }
        shrink_to_fit();
    }
    public:
    bigInt& operator+(){return*this;}
    bigInt operator-(){
        bigInt r=*this;
        r.negate();
        return r;
    }
    template<class T>
    bigInt& operator+=(T&&rhs)requires is_constructible_v<bigInt,decay_t<T>> {
        // bigInt以外では構築
        if constexpr(!is_bigInt<T>) return*this+=bigInt(forward<T>(rhs));
        // 自分のほうが短いときに場所を確保する．
        if(rhs.data.size()>data.size())
            if constexpr (is_copy_needed_bigInt<T>) // rhsが左辺値なら拡張確定
                data.resize(rhs.data.size(),data.back()); // 符号拡張
            else swap(data,rhs.data); // movableなrhsが長いなら入れ替え
        uint64_t carry=0; // 桁溢れを保持
        // |lhs.data|>=|rhs.data|より，短い方の長さを参照する
        // 最後は符号なので飛ばす．
        for(uint64_t i=0;i<rhs.data.size()-1;++i){
            __uint128_t add=(__uint128_t)data[i]+rhs.data[i]+carry;
            data[i]=(uint64_t)add;
            carry=(uint64_t)(add>>64);
        }
        // 符号拡張
        uint64_t sign=rhs.data.back();
        for(uint64_t i=rhs.data.size()-1;i<data.size()-1;++i){
            __uint128_t add=(__uint128_t)data[i]+sign+carry;
            data[i]=(uint64_t)add;
            carry=(uint64_t)(add>>64);
        }
        if(data.back()!=sign){
            // 異符号の加算では元のbitより情報量が増えることはない．
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
        return*this;
    }
    template<class T>
    bigInt& operator-=(T&&rhs)requires is_constructible_v<bigInt,decay_t<T>> {
        if constexpr(!is_bigInt<T>)return*this-=bigInt(forward<T>(rhs));
        else *this+=forward<T>(-rhs);
        return*this;
    }
    template<class T>
    bigInt& operator*=(T&&rhs)requires is_constructible_v<bigInt,decay_t<T>> {
        if constexpr(!is_bigInt<T>)*this*=bigInt(forward<T>(rhs));
        else{
            if(data.back()==rhs.data.back()){ // 同符号
                data=multiply(data,rhs.data);
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

// bit operation
    bigInt operator~(){
        bigInt r=*this;
        r.bit_rev();
        return r;
    }


    template<class S,class T>
    friend bigInt operator+(S&&lhs,T&&rhs)
        requires is_constructible_v<bigInt,decay_t<T>> && is_constructible_v<bigInt,decay_t<T>> {
        if constexpr(is_bigInt<S>&&is_movable_bigInt<T>)
            return rhs+=forward<T>(lhs);
        else if constexpr(is_bigInt<T>&&is_movable_bigInt<S>)
            return lhs+=forward<S>(rhs);
        else return bigInt(forward<S>(lhs))+=forward<T>(rhs);
    }
    template<class S,class T>
    friend bigInt operator-(S&&lhs,T&&rhs)
        requires is_constructible_v<bigInt,decay_t<T>> && is_constructible_v<bigInt,decay_t<T>> {
        if constexpr(is_bigInt<S>&&is_movable_bigInt<T>)
            return rhs-=forward<T>(lhs);
        else if constexpr(is_bigInt<T>&&is_movable_bigInt<S>)
            return lhs+=forward<S>(rhs);
        else return bigInt(forward<S>(lhs))+=forward<T>(rhs);
    }
    template<class S,class T>
    friend bigInt operator*(S&&lhs,T&&rhs)
        requires is_constructible_v<bigInt,decay_t<T>> && is_constructible_v<bigInt,decay_t<T>> {
        return lhs; //dammy
    }
    template<class S,class T>
    friend bigInt operator/(S&&lhs,T&&rhs)
        requires is_constructible_v<bigInt,decay_t<T>> && is_constructible_v<bigInt,decay_t<T>> {
        return lhs; // dammy
    }

};
} // namespace elsie
#endif