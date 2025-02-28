# lazy_segtree
`template<class S,S(*op),S(*e),class F,S(*mapping)(F,S),F(*composition)(F,F),F(*id)()>lst(n);`

|template|説明|
|-|-|
|型`S`|元の配列|
|型`F`|遅延の型|
|`op`|`S(S,S)`|
|`e`| $e\in S$ となる`op`の単位元|
|`mapping`|遅延要素を元の配列に対して適用するための関数`S(F,S)`|
|`composition`|遅延要素の計算 `F(F f,F g)`のとき，`f`が後から追加される遅延要素|
|`id`| $\text{id}\in F$となる`composition`の単位元|

## コンストラクタ $O(N)$
```C++
lazy_segtree(size_t N=0);
lazy_segtree(const vector<S>&v);
```

## 一点更新 $O(\lg N)$
```C++
void set(size_t idx, S val);
```
一点更新を $O(\lg N)$ で行う．  
型`S`のコピーコストとかデカいともちろん $O(|v|\lg N)$．  
move, copyのいずれのオーバーロードを使っても更新があればそんなに変わらない．

## 一点取得 $O(\lg N)$
```C++
S get(size_t idx);
```
$p\in[0,N)$ となる $p$ の値を得る．

## 区間更新 $O(\lg N)$
```C++
void apply(size_t l,size_t r,F f);
```
$i\in[l,r)$ に $composition(f, a_i)$ する．

## 区間積 $O(\lg N)$
```C++
S prod(size_t l,size_t r);
```
$[l,r)$ における $op$ の演算結果を $O(\lg(r-l))$ で得る．  

## 全区間積 $O(1)$
```C++
S all_prod()const;
```
全区間の演算結果を得る．  

## max_right $O(\lg N)$
```C++
size_t max_right<G>(size_t l,G g);
size_t max_right<bool(*g)(S)>(size_t l);
```
$[l,r)$ で $f$ を演算したときに`true`, $[l,r]$ で $f$ を演算したときに`false`となる $r$ を返す．  
ただし，すべての $l$ 以上の要素で $f$ が`true`のとき， $n$ を返す．  
ついでに，すべての $l$ 以上の要素で $f$ が`false`のとき， $l$ を返す．

## min_left $O(\lg N)$
```C++
size_t min_left<G>(size_t r,G g);
size_t min_left<bool(*g)(S)>(size_t r);
```
$[l,r)$ で $f$ を演算したときに`true`, $[l-1,r)$ で $f$ を演算したときに`false`となる $l$ を返す．  
ただし，すべての $r$ 以下の要素で $f$ が`true`のとき， $0$ を返す．  
ついでに，すべての $r$ 以下の要素で $f$ が`false`のとき， $r$ を返す．