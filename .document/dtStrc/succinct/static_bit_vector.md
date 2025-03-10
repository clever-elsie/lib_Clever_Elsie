# 静的 bit vector
## コンストラクタ
```C++
static_bit_vector();
static_bit_vector(size_t N);
```
サイズ $N$ のbit vectorを作るコンストラクタと，グローバルに置いてもコンパイルエラーを出さないためのデフォルトコンストラクタ．  

## 要素アクセス
```C++
bool get(size_t idx)const;
void set(size_t idx);
void reset(size_t idx);
```
`get()`は要素アクセス．  
`set()`は`idx`を $1$ にする．  
`reset()`は`idx`を $0$ にする．  

## 構築
```C++
void fix();
```
これを行わないと`rank`から始まる関数を使うことはできない．  
逆に，これを行った後に`set(), reset()`を呼び出してはいけない．  
`assert()`で落とされる．  
`get()`はどちらでも呼び出せる．

## rank
```C++
size_t rank1(size_t idx)const;
size_t rank0(size_t idx)const;
size_t rank1(size_t l,size_t r)const;
size_t rank0(size_t l,size_t r)const;
```
上 $2$ つは $[0,idx)$ の $0,1$ のbit数をそれぞれ数える．  
下 $2$ つは $[l,r)$ の $0,1$ のbit数をそれぞれ数える．

## operator=
```C++
const static_bit_vector&operator=(const static_bit_vector&v);
const static_bit_vector&operator=(static_bit_vector&&v);
```
グローバルに定義したものに代入する時に使える．