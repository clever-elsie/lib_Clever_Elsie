# rolling hash

## コンストラクタ
```C++
rollingHash<n,T>();
rollingHash<n,T(const T&SRC);
```
$n={1,2,3}$ はロリハの本数  
`T`は乱択可能な型`vector`とか`string`とか．  
`SRC`で初期化・構築するか，後から`set()`で構築する．
## set
```C++
void set(const T&SRC);
```
構築する. $O(n|\text{SRC}|)$
## hash_value
```C++
int64_t hash_value(int64_t l,int64_t r,int64_t i);
```
$i$ 本目のロリハの $[l,r)$ のハッシュ値を得る．  
$O(\lg (r-l))$

## find
```C++
vector<size_t>find(const T&t);
vector<size_t>find(const array<int64_t,n>&h,const size_t len);
vector<size_t>find(size_t len,const array<int64_t,n>&h);
vector<size_t>find(const pair<array<int64_t,n>,size_t>&hashed);
```
検索対象の文字列が現れる先頭位置を全列挙する． $O(|S|\lg|T|)$  
1番上以外は内部的に使うが，`hassing()`と組合せて使ってもいい．

## same
```C++
bool same(int64_t l,int64_t r,int64_t L,int64_t R);
```
SRCの区間 $[l,r)$ と 区間 $[L,R)$ が同じ文字列かどうかを調べる． $O(\lg(R-L))$

## hashing
```C++
pair<array<int64_t,n>,size_t>hashing(const T&tar);
```
`tar:T`をハッシュ化した値を得る．
`.find()`を何度も呼ぶときにはこれを使うと良いかも．

## get_b
```C++
int64_t get_b(size_t idx)const;
```
$idx<n$ となる `idx` のハッシュ化基数を返します．
$[l,r)$ を除去した文字列のハッシュ値などを求める際に必要になります．

## 動的ロリハ
```C++
dynamicRollingHash<n>(const string&);
vector<size_t>find(const string&);
bool same(int64_t l,int64_t r,int64_t L,int64_t R);
void set(size_t idx,char c);
```
`.set(int idx,char c)` idxの文字をc変更する。
