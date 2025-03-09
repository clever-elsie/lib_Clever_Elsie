## Eular's $\phi$ function
```C++
uint64_t eular_totient(uint64_t n);
```
オイラーの $\phi$ 関数．
$[1,n]$ で $n$ と互いに素なものの数．  
計算量は $O(\sqrt{N})$

## 商の切り捨て列挙
```C++
vector<uint64_t>quotients(uint64_t n);
```
$n$ を $\forall x\in [1,n]$で割ったときの商の切り捨てとして有り得るすべてを列挙する．
計算時間は多分 $O(\sqrt{N})$

## 素数判定
```C++
bool is_prime(uint64_t);
```
Miller Rabinです．
$O(\sqrt{N})$ より真に速い．

## 素因数分解
```C++
vector<uint64_t>factorize(uint64_t n);
```
pollard's rhoです．
乱択なのでそこそこ速い．  
計算量は $O(\sqrt{N})$ より真に速い．

## 原始根
```C++
int64_t primitive_root(uint64_t);
```
素数の原始根を求める．
乱択なのでそこそこ速い．