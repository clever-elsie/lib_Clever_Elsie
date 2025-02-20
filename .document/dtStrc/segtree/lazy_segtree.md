### lazy_segtree
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

```C++
lazy_segtree(u32 n);
lazy_segtree(const vector<S>&v);
void set(u32 idx, S val);
S get(u32 idx);
void apply(u32 l,u32 r,F f);
S prod(u32 l,u32 r);
```