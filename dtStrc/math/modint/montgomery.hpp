// SPDX-License-Identifier: MIT
// Copyright (c) 2026 CleverElsie

#ifndef ELSIE_MONTGOMERY
#define ELSIE_MONTGOMERY
#include <cstddef>
#include <cstdint>
#include <utility>
#include <concepts>
#include <type_traits>
#include <limits>
#include <charconv>
#include <math/basic_math.hpp>
#include <math/prime_factor.hpp>
#include <string>
#include <iostream>

/*
 * モンゴメリ乗算では，モンゴメリ表現空間で演算し，逆変換で元に戻す．
 * モンゴメリ表現空間では[0, M)の値が[0, M*R)に写像される．
 * モンゴメリ表現空間では加減算はそのまま行い，オーバーやアンダーのフローのみを補正する．
 * モンゴメリ表現空間では乗算はx->tのモンゴメリ表現はt=xR^{-1}なので，a*bを a->s, b->tの下で s*tとすると a*b*R^{-2}となるためR^{-1}を一つ除去する必要がある
 * 以下のクラスでは内部表現をT, モンゴメリ表現空間の型をUとし，T==Uならば内部的にはモンゴメリ表現で保持し，sizeof(T)*2<=Uならば内部表現をMを法とするTの変数とする
 * 定数
 *   M    :法
 *   R    :R>M, gcd(R, M)=1
 *   Rinv : R*Rinv=1 (mod M) Rの逆元
 *   R2   : R*R (mod M)
 *   M'   : M*M'=-1(mod R)を満たすように選ぶ整数定数
 *          pR-qM=1 を満たす 0<q<R として拡張互除法で求める
 * 変数
 *   x    :[0, MR)
 *
 * リダクション
 *   F(x) = x * Rinv (mod M)
 *   計算
 *     tmp = (x + (x * M' mod R) * M) / R
 *     tmp = x/R + (x*M' mod R)*M/R
 *     x = tmp<M ? tmp : tmp-M
 *   このとき，Rがunsigned型の最大値ならば除算，剰余算は不要
 *   証明
 *     x+(x * M' mod R) * M
 *     ≡ x+x*M*M'
 *     ≡ x-x
 *     ≡ 0 (mod R)
 *     以上より， x+(x*M' mod R)*MはRで割り切れる
 *     tmp * R
 *     ≡ (x+(x*M' mod R)*M)/R * R
 *     ≡ x+(x*M' mod R)*M
 *     ≡ x (mod M)
 *     以上より x=tmp*R (mod M)
 *     xの定義より x<M*R -> x/R < M
 *     T*M' mod R < R より (T*M' mod R)*M < R*M -> (T*M' mod R)*M/R < M
 *     tmp = x/R + (T*M' mod R)*M/R < M+M
*/

namespace elsie{
using std::size_t;

// M:法，T
template<size_t M=998244353,class T=uint32_t>
class montgomery_modint{
  static_assert(M%2==1,"M must be odd");
  static_assert(std::same_as<T,uint32_t> || std::same_as<T,uint64_t>,"T must be uint32_t or uint64_t");
  template<size_t> friend class mmint;
  private:
  using UpperType=std::conditional_t<std::same_as<T,uint32_t>,uint64_t,__uint128_t>;
  constexpr static std::pair<T,T> RRinv(); // Rはbit数がちょうど足りない．RinvはMの下でのRの逆元なのでTで収まる
  constexpr static T reduction(UpperType x);
  constexpr static T get_m();
  constexpr static T get_R2(){
    if constexpr(M%2==1){
      UpperType x=((UpperType)1<<(sizeof(T)*8))%M;
      return (x*x)%M;
    }else{
      static_assert(false, "未実装");
    }
  }
  template<class S>
    requires std::integral<S> || std::same_as<S,__int128_t> || std::same_as<S,__uint128_t>
  constexpr static T init_value(S val){
    constexpr static UpperType R2=get_R2();
    if constexpr(std::signed_integral<S>||std::same_as<S,__int128_t>){
      val%=std::make_signed_t<size_t>(M);
      if(val<0) val+=M;
    }else val%=M;
    return reduction(val*R2);
  }
  montgomery_modint(std::nullptr_t, T val):value(reduction(static_cast<UpperType>(val)*R2)){}
  private:
  T value;
  public:
  using value_type=T;
  constexpr static bool is_modint=true, is_montgomery=true;
  constexpr static std::pair<T,T> rrinv=RRinv();
  // R, RinvはTで表現可能だが，キャストを後で書きたくないのでUpperTypeにする
  constexpr static UpperType R=rrinv.first, Rinv=rrinv.second;
  constexpr static UpperType R2=get_R2();
  constexpr static UpperType m=get_m();
  constexpr static size_t mod(){ return M; }
  constexpr static T Mdash(){ return get_m(); }
  
  montgomery_modint():value(0){}
  montgomery_modint(montgomery_modint&&)=default;
  montgomery_modint(const montgomery_modint&)=default;
  template<class S>
    requires std::integral<S> || std::same_as<S,__int128_t> || std::same_as<S,__uint128_t>
  montgomery_modint(S val):value(init_value(val)){}
  montgomery_modint(T val,std::nullptr_t):value(val){}
  static montgomery_modint raw(T val){
    return montgomery_modint(nullptr,val);
  }
  montgomery_modint& operator=(montgomery_modint&&)=default;
  montgomery_modint& operator=(const montgomery_modint&)=default;
  operator std::string()const{
    return std::to_string(val());
  }
  
  T val()const{
    return reduction(value);
  }
  T pow(uint64_t n)const{
  }
  template<size_t N>
  std::pair<char*,size_t> write(char(&buf)[N])const{
    if constexpr(std::same_as<T,uint32_t>) static_assert(N >= 11);
    else static_assert(N >= 21);
    const T x = val();
    const auto [ptr, ec] = std::to_chars(buf, buf + N, x, 10);
    (void)ec; // 正しい利用前提（バッファ不足等は起きない）
    return {buf, static_cast<size_t>(ptr - buf)};
  }
  
  // 演算について，montgomery_modintの法が同じ場合と整数型についてのみ定義する
  template<class T2>
  montgomery_modint& operator+=(const montgomery_modint<M,T2>&rhs){
    value+=rhs.value;
    if(value>=M) value-=M;
    return *this;
  }
  template<class T2>
  montgomery_modint& operator-=(const montgomery_modint<M,T2>&rhs){
    if(value<rhs.value) value=M+value-rhs.value;
    else value-=rhs.value;
    return *this;
  }
  template<class T2>
  montgomery_modint& operator*=(const montgomery_modint<M,T2>&rhs){
    value=reduction((UpperType)value*rhs.value);
    return *this;
  }
  // 除算->逆元の積は法が素数なら逆元を得られるためnoexcept
  // 法が素数でない場合は逆元が無い場合に例外を投げる
  template<class T2>
  montgomery_modint& operator/=(const montgomery_modint<M,T2>&rhs)
    noexcept
    requires (elsie::is_prime<UpperType>(M))
  {
    static_assert(false, "未実装");
  }
  montgomery_modint& operator/=(const montgomery_modint&rhs)
    noexcept(false)
    requires (!elsie::is_prime<UpperType>(M))
  {
    static_assert(false, "未実装");
  }
  
  montgomery_modint operator+(const montgomery_modint&rhs)const{
    return montgomery_modint(*this)+=rhs;
  }
  montgomery_modint operator-(const montgomery_modint&rhs)const{
    return montgomery_modint(*this)-=rhs;
  }
  montgomery_modint operator*(const montgomery_modint&rhs)const{
    return montgomery_modint(*this)*=rhs;
  }
};

template<size_t M,class T>
constexpr std::pair<T,T>
montgomery_modint<M,T>::RRinv(){
  // 奇数の法しか許さないので，R=0 (== max<T>())でよい．
  UpperType R=UpperType(1)<<(sizeof(T)*8);
  UpperType inv=mod_inv<std::make_signed_t<UpperType>>(R,M);
  return {0,inv};
}

template<size_t M,class T>
constexpr T montgomery_modint<M,T>::reduction(UpperType x){
  if constexpr(R==0){
    const T t=(T)x*(T)m;
    const UpperType u=(x+t*(UpperType)M)>>sizeof(T)*8;
    if(u>=M) return u-M;
    return u;
  }else{
    const UpperType t=x*m%R;
    const UpperType u=(x+t*M)/R;
    if(u>=M) return u-M;
    return u;
  }
}

template<size_t M,class T>
constexpr T montgomery_modint<M,T>::get_m(){
  // pR - qM = 1 (mod R)を満たす q を求める
  if constexpr (M%2==1){ //Mが奇数なので R=2^32, 2^64である
    // このとき， qM=-1 (mod R) -> qM=R-1 (mod R)で左からすべてのbitが1である．
    // Mにかけてすべてのbitが1になるようなresultをもとめる
    // qM=R-1となるqは繰り返し二乗法の加算版で求められる．
    // t=qMとなるtを構築することを考える
    // 教科書法の乗算では加算キャリーは下の桁から決定される
    // このとき，tを構成するqを下位ビットから構成できる
    // 下からiビット目を構成するとき，それ以前のビットはtについてもqについても無視できる
    // したがって，毎回Mと残りの(sizeof(T)*8-i)ビットについてのみ考える
    // 最下位ビットを決定するとき，q[i]&M[0]==1ならばよい．このとき奇数法なのでM[0]==1である．
    // 教科書乗法は当然上のビットにも影響を与えるので，q[i]==1とするとき，Mをtに加算する．
    // 最下位ビットについてのみ考えればよいので，毎回tを右シフトし，現在の構築状況を確認する
    UpperType result=0, t=0, r=(UpperType)1<<(sizeof(T)*8), i=1;
    for(;r>1;t>>=1,r>>=1,i<<=1){
      if(t&1) continue;
      t+=M;
      result|=i;
    }
    return result;
  }else{
    static_assert(false, "未実装");
  }
}

// モンゴメリmodintの型を決定するためのヘルパー型
// こっちを使え
// Mは法，shrinkがtrueなら乗算の度に変換する代わりに最小の型，falseなら内部表現はモンゴメリ空間
// モンゴメリ空間表現で加算が出来ないなら自動的にshrinkする
// つまり xRinv<MRなるx1,x2について，x1+x2が必ずTに収まらなければならない
// このとき，(MR-1)*2<=max<T>()を満たさねばならない
template<size_t M>
class mmint{
  using T=std::conditional_t<
    __uint128_t(M-1)*2<=std::numeric_limits<uint32_t>::max(),
      uint32_t,
      std::conditional_t<__uint128_t(M-1)*2<=std::numeric_limits<uint64_t>::max(),
        uint64_t, __uint128_t
      >
    >;
  static_assert(M>0,"M must be positive");
  static_assert(!(std::same_as<T,__uint128_t>),"M must be smaller than 2^63");
  public:
  using type = montgomery_modint<M,T>;
};

} // namespace elsie

#endif