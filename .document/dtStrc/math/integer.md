# 多倍長整数
- [mpInt](#mpint)
大量の素数を持つことによって四則演算と等価比較を行うことが可能．  
負の数を扱えないため引き算には注意．  
除算も割り切れない場合は要注意．  
大小比較も不可能なため要注意．  
文字列への変換はかなり低速なため要注意．  
実用性はかなり怪しい．

- [bigInt](#bigint)
未実装

## <a id="mpint">mpInt (many prime integer)</a>
$n\in [1,128]\in \mathbb{N}$ となる $n$ 個の素数の法で値 $x$ を持ち，必要に応じて $x$ をgarner法で復元する．  
文字列への変換は，garner法の計算量が桁数あたり $O(n^2)$ なので，桁数を $d$ としたときに $O(d n^2)$ となる．  
精度は， $d=9n$ 程度．内部的に $1,000,000,007$ から昇順に $128$ 個の素数を使うため．  
したがって， $O(n^3)$ になる．

### コンストラクタ
```C++
mpInt<mod_cnt>();
mpInt<mod_cnt>(int64_t x);
mpInt<mod_cnt>(uint64_t x);
mpInt<mod_cnt>(mpInt&&v);
mpInt<mod_cnt>(const mpInt&v);
```
整数からの構築とコピーコンストラクタとムーブコンストラクタを持つ．  
計算量は $O(mod_cnt)$

### raw
```C++
static mpInt raw(uint32_t v);
```
$v$ の剰余を取らず直接構築する定数倍高速化用関数．  
$0\le v< 10^9+7$ のときに使える．

### to_string
```C++
string to_string();
```
値を文字列に変換する．  
計算量は $O(\text{mod_cnt}^3)$

### 演算子
```C++
mpInt&operator=();

mpInt&operator+=();
mpInt&operator-=();
mpInt&operator*=();
mpInt&operator/=();

friend mpInt&operator+();
friend mpInt&operator-();
friend mpInt&operator*();
friend mpInt&operator/();

friend bool operator==();
friend bool operator!=();
```
すべての演算子について `mpInt` と `int64_t` のオーバーロードを提供する．  
注意点として，`mod_cnt`の異なる`mpInt`同士の演算は不可能．

## <a id="bigint">bigInt</a>