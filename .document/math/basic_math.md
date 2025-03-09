# basic_math
## 数学関数
```C++
template<integral T> T ceil(T a,T b);       // 天井関数
template<integral T> T floor(T a,T b);      // 床関数
template<class S> S gcd(S a,S b);           // 最大公約数
template<class S> S lcm(S a,S b);           // 最小公倍数
// 再帰拡張ユークリッドの互除法
template<class S> S egcd(S a,S b,S&x,S&y);
// 非再帰拡張ユークリッドの互除法
template<class S> S exgcd(S a,S b,S&x,S&y);
```
```C++
template<class T> T mod_inv(S a,S m);       // 法mの逆元
template<class T> T mod_inv_prime(S a,S p); // 素数法pの逆元
// 繰り返し二乗法．オーバーフローでINT64_MAX
int64_t safepow(int64_t a,uint64_t b);
// 法modの繰り返し二乗法
constexpr int64_t modpow(int64_t a,uint64_t b,uint64_t mod);
```

## combination
```C++
combination<mint>(int64_t n);
void PreCalc(int64_t n);
```
$n$ に ${}_nP_k$ などで呼び出す最大の $n$ を渡しておくと前計算する．  
適宜計算するため，別に渡さなくても問題ない．  
$k$ までの階乗とその逆元が求まっているとき， $k+1$ のそれは $O(1)$ で計算できるため，計算量は全体で $O(n)$.  
前計算は`public`な関数なので，後から`PreCalc()`を呼び出しても良い．

`mint`はデフォルトで`atcoder::modint998244353`

```C++
mint factorial(size_t n);
mint invfact(size_t n);
mint P(size_t n,size_t k);
mint C(size_t n,size_t k);
mint H(size_t n,size_t k);
```
上から階乗，階乗の逆元，${}_n P_k,\ {}_n C_k,\ {}_n H_k$．  
$P,C,H$ はそれぞれ順列，組合せ，重複組合せ．
