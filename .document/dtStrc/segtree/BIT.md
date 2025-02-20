### Binary Indexed Tree
`BIT<S>` : 1点更新と区間 $[0,N)$ の加算結果を $O(\lg N)$ で取得することができる。  

`BIT(size_t)` : 要素数を渡すコンストラクタ。 $O(N)$  
`void add(size_t,S)` : 一点差分更新。 $O(\lg N)$  
`void set(size_t,S)` : 一点即値更新。 $O(\lg N)$  
`void get(size_t)` : 一点取得。 $O(\lg N)$  
`S sum(size_t,size_t)` : $[l,r)$ の和を求める。 $O(\lg N)$  
