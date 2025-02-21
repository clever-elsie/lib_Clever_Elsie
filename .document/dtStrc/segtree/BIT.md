# Binary Indexed Tree
## コンストラクタ $O(N)$
```C++
BIT<type>();                      // empty
BIT<type>(size_t size);           // 0 fill
BIT<type>(const vector<type>&v_); // copy
BIT<type>(vector<type>&&v_);      // move
BIT<type>(const BIT<type>&b);     // copy
BIT<type>(BIT<type>&&b);          // move
```

## 一点取得 $O(\lg N)$
```C++
type get(size_t i);
```
$i\in[0,N)$ となる整数 $i$ の示す場所の値を得る．

## 一点更新 $O(\lg N)$
```C++
void set(size_t i,type t);
```
$i$ の値を $t$ に書き換える．  

## 一点加算 $O(\lg N)$
```C++
void add(size_t i,type t);
```
$i$ に $t$ 加算する．

## 区間和 $O(\lg N)$
```C++
type sum(size_t l,size_t r);
```
$[l,r)$ の区間和を得る．

## operator=
```C++
BIT<type>&operator=(const BIT<type>&b);
BIT<type>&operator=(BIT<type>&&b);
```
代入できる．
# 内部の関数
## 先頭累積和 $O(\lg N)$
```C++
type sum(size_t r);
```
$[0,r)$ の合計を求める

## 2冪の切り上げ $O(1)$
```C++
size_t ceil2exp(size_t n);
```
$n$ 以上の最小の $2$ 冪の数を返す．  
厳密にいうと，
$$\begin{align*}
r,e,n\in \mathbb{N},\ \ 
r=\min\{e\mid 2^e\ge n\}
\end{align*}$$
となる $r$ を返します．
## セットアップ $O(N)$
```C++
void setup(auto&&v_);
```
`vector<type>&`または`vector<type>&&`を受け取り，適切にmoveまたはcopyします．  
その後，2冪のサイズに切り上げ拡張した部分を0埋めします．