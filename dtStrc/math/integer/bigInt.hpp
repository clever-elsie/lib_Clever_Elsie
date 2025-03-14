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
class bigInt{
    private:
    template<class T>
    static constexpr bool is_bigInt=is_same_v<remove_cvref_t<T>,bigInt>;
    template<class T>concept evalable_t=is_constructible_v<bigInt,decay_t<T>>;
    template<evalable_t T>
    static constexpr bool is_copy_needed_bigInt=is_bigInt<T>&&(is_lvalue_reference_v<T>||is_const_v<remove_reference_t<T>>);
    template<evalable_t T>
    static constexpr bool is_movable_bigInt=is_bigInt<T>&&is_rvalue_reference_v<T>&& ! is_const_v<remove_reference_t<T>>;
    bool sign;
    // dataの最後の要素は必ず0にする．
    vector<uint64_t>data;
    // a<b -1, a==b 0, a>b 1
    int32_t abs_compare(const vector<uint64_t>&a,const vector<uint64_t>&b)const{
        if(a.size()==b.size()){
            // a.back()==b.back()==0
            for(int64_t i=a.size()-2;i>=0;--i){
                if(a[i]==b[i])continue;
                return a[i]<b[i]?-1:1;
            }
            return 0;
        }else return a.size()<b.size()?-1:1;
    }
    public:
    bigInt():sign(0),data(1,0){}
    bigInt(bigInt&&b){}
    bigInt(const bigInt&b){}
    bigInt(const string&s){}
    template<signed_integral T>
    bigInt(T x){}
    template<unsigned_integral T>
    bigInt(T x){}

    // 適切にコンストラクタを呼ぶためこれでよい
    template<evalable_t T> bigInt& operator=(T&&rhs){ return*this=bigInt(forward<T>(rhs)); }

    template<evalable_t T>
    bigInt& operator+=(T&&rhs){
        if constexpr(is_bigInt<T>){
            // 同符号で加算，異符号で減算
            if(sign==rhs.sign){
                // 自分のほうが短いときに場所を確保する．
                if(rhs.data.size()>data.size())
                    if constexpr (is_copy_needed_bigInt<T>) // rhsが左辺値なら拡張確定
                        data.resize(rhs.data.size(),0);
                    else swap(data,rhs.data); // 長さが異なるなら入れ替え．
                uint64_t carry=0; // 桁溢れを保持
                // |lhs.data|>=|rhs.data|より，短い方の長さを参照する
                for(uint64_t i=0;i<rhs.data.size();++i){
                    __uint128_t add=(__uint128_t)data[i]+rhs.data[i]+carry;
                    data[i]=(uint64_t)add;
                    carry=(uint64_t)(add>>64);
                }
                // さらに桁溢れ対策
                for(uint64_t i=rhs.data.size();carry>0;++i){
                    if(i==data.size())data.push_back(0);
                    __uint128_t add=(__uint128_t)data[i]+carry;
                    data[i]=(uint64_t)add;
                    carry=(uint64_t)(add>>64);
                }
                if(data.back()) data.push_back(0);// 末尾は必ず0
            }else{
                rhs.sign=!rhs.sign;
                *this-=forward<T>(rhs);
            }
        }else*this+=bigInt(forward<T>(rhs)); // bigInt以外は構築
        return*this;
    }
    template<evalable_t T>
    bigInt& operator-=(T&&rhs){
        if constexpr(is_bigInt<T>){
        }else*this-=bigInt(forward<T>(rhs));
        return*this;
    }
    template<evalable_t T>
    bigInt& operator*=(T&&rhs){
    }
    template<evalable_t T>
    bigInt& operator/=(T&&rhs){
    }


    template<evalable_t S,evalable_t T>
    friend bigInt& operator+(S&&lhs,T&&rhs){
        if constexpr(is_bigInt<S>&&is_movable_bigInt<T>)
            return lhs+=forward<T>(rhs);
        else if constexpr(is_bigInt<T>&&is_movable_bigInt<S>)
            return rhs+=forward<S>(lhs);
        else return bigInt(forward<S>(lhs))+=forward<T>(rhs);
    }
    template<evalable_t S,evalable_t T>
    friend bigInt& operator-(S&&lhs,T&&rhs){}
    template<evalable_t S,evalable_t T>
    friend bigInt& operator*(S&&lhs,T&&rhs){}
    template<evalable_t S,evalable_t T>
    friend bigInt& operator/(S&&lhs,T&&rhs){}

};
} // namespace elsie
#endif