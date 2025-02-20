### segtree
`template<class S, S(*op)(S,S), S(*e)()> segtree`  
単位元`e`,演算`op`が定義された型`S`を渡して使う。 $[0,N)$

`segtree<>(const vc<S>&)` : 配列からセグ木を構築する。 $O(N\lg N)$  
`segtree<>(int)` : 要素数を指定するコンストラクタ。 $O(N)$  
`void set(size_t,S)` : 1点更新。 $O(\lg N)$  
`S get(size_t)` : 1点取得。 $O(1)$  
`S prod(size_t,size_t)` : 区間取得 $[l,r)$ 。 $O(\lg N)$
