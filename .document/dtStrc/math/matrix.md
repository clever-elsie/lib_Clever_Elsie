## matrix
```C++
vv<U> matrix_add<bool sub=false>(const vv<S>&,const vv<T>&);
vv<U> matrix_mul<threshold,q2>(vv<S>&,vv<T>&);
vv<S> Mpower(vv<S>&A,int b);
v3<S> split_quad_matrix(const vv<S>&);
vv<S> merge_quad_matrix(const vv<S>&,x4);
```

`matrix_add` : 計算量は $O(n^2)$.
第一テンプレート引数で引き算を指定．2つの行列は同じサイズである必要がある．
戻り値の型は`S,T`の両方からキャスト可能な`U`．

`matrix_mul` : `threshold`は $O(n^3)$ と $O(n^{\lg 7})$ を切り替える $n$.
`q2`は入力される行列が2つとも2冪のときに`false`．
行列のサイズは $n\times m,\ m\times l$ となるような任意の $n,m,l>0$ となる整数．
戻り値の型は`S,T`の両方からキャスト可能な`U`．

`Mpower` : 行列累乗．  
	`S`は乗算が定義されている必要がある。  
	計算量は $n\times n$ 行列について $O(n^3\lg b)$  
	通常オーバーフローするため，型には `modint` を入れる．

`split_quad_matrix` : 行列を4分割する． $O(n^2)$

`merge_quad_matrix` : 4つの行列を1つの行列にまとめる． $O(n^2)$