# segtree
`template<class S, S(*op)(S,S), S(*e)()> segtree`  
単位元`e`, 演算`op`が定義された型`S`を渡して使う。 $[0,N)$

## コンストラクタ $O(N)$
```C++
segtree();                 // (1)
segtree(size_t n);         // (2)
segtree(const vector<S>&); // (3)
segtree(segtree&&);        // (4)
segtree(const segtree&);   // (5)
```
(1) 空の配列  
(2) サイズ $n$ の配列のセグ木を構築する．セグ木は $2n$.  
(3) `vector`をcopyしてセグ木を構築．  
(4) セグ木をmoveするだけなので $O(1)$.  
(5) セグ木をcopyするので $O(n)$.  

4,5 は内部的に`operator=`を呼び出す．

## 代入演算子
```C++
segtree&operator=(segtree&&);      // (1)
segtree&operator=(const segtree&); // (2)
```
(1) $O(1)$ のmove  
(2) $O(n)$ でcopy  

## 一点更新 $O(\lg N)$
```C++
void set(size_t p,S&&v);
void set(size_t p,const S&v);
```
一点更新を $O(\lg N)$ で行う．  
型`S`のコピーコストとかデカいともちろん $O(|v|\lg N)$．  
move, copyのいずれのオーバーロードを使っても更新があればそんなに変わらない．

## 一点加算 $O(\lg N)$
```C++
void add(size_t p,const S&v);
```
動作としては`seg.set(p,seg.get(p)+v)`と同じだが，よく使うので．  
`fenwick_tree`とかの`.add()`だよ．

## 一点取得 $O(1)$
```C++
S get(size_t)const;
```
$p\in[0,N)$ となる $p$ の値を得る．

## 区間積 $O(\lg N)$
```C++
S prod(size_t l,size_t r)const;
```
$[l,r)$ における $op$ の演算結果を $O(\lg(r-l))$ で得る．  

## 全区間積 $O(1)$
```C++
S all_prod()const;
```
全区間の演算結果を得る．  

## max_right
```C++
size_t max_right<F>(size_t l, F f)const;
size_t max_right<bool(*f)(S)>(size_t l)const;
```
$[l,r)$ で $f$ を演算したときに`true`, $[l,r]$ で $f$ を演算したときに`false`となる $r$ を返す．  
ただし，すべての $l$ 以上の要素で $f$ が`true`のとき， $n$ を返す．  
ついでに，すべての $l$ 以上の要素で $f$ が`false`のとき， $l$ を返す．

## min_left
```C++
size_t min_left<F>(size_t r, F f)const;
size_t min_left<bool(*f)(S)>(size_t r)const;
```
$[l,r)$ で $f$ を演算したときに`true`, $[l-1,r)$ で $f$ を演算したときに`false`となる $l$ を返す．  
ただし，すべての $r$ 以下の要素で $f$ が`true`のとき， $0$ を返す．  
ついでに，すべての $r$ 以下の要素で $f$ が`false`のとき， $r$ を返す．