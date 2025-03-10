# 静的 bit vector
## コンストラクタ
```C++
static_bit_vector();
static_bit_vector(size_t N);
```
サイズ $N$ のbit vectorを作るコンストラクタと，グローバルに置いてもコンパイルエラーを出さないためのデフォルトコンストラクタ．  

## 要素アクセス $O(1)$
```C++
bool get(size_t idx)const;
void set(size_t idx);
void reset(size_t idx);
```
`get()`は要素アクセス．  
`set()`は`idx`を $1$ にする．  
`reset()`は`idx`を $0$ にする．  

## 構築 $O(N)$
```C++
void fix();
```
これを行わないと`rank`や`select`を使うことができない．  
逆に，これを行った後に`set(), reset()`を呼び出してはいけない．  
`assert()`で落とされる．  
`get()`はどちらでも呼び出せる．

## rank $O(1)$
```C++
size_t rank<0,1>(size_t idx)const;
size_t rank<0,1>(size_t l,size_t r)const;
```
上は $[0,idx)$ の $0,1$ のbit数をそれぞれ数える．  
下は $[l,r)$ の $0,1$ のbit数をそれぞれ数える．  
それぞれデフォルトは $1$.  
範囲外はassert落ち．

## select $O(\lg N)$
```C++
size_t select<0,1>(size_t k)const;
```
$k\ge 1$ 番目に現れる $0,1$ のインデックスを返す．  
デフォルトは $1$.  
存在しない場合は，配列のサイズが返る．

## operator=
```C++
const static_bit_vector&operator=(const static_bit_vector&v);
const static_bit_vector&operator=(static_bit_vector&&v);
```
グローバルに定義したものに代入する時に使える．