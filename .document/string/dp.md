## dp/edit_distance (編集距離)
計算量 $O(ST)$.  
`.begin(),.end()`のあるコンテナクラスには使える．
```C++
size_t edit_distance(const Itrabl&,const Itrabl&);
```
## dp/LCS (最長共通部分列)
計算量 $O(ST)$.  
LCSを求めて、その長さと列のペアを返す。  
列はテンプレート引数が非 $0$ のときのみ得る．  
省略時及び $0$ のときは空列を得るため無視してよい．
```C++
pair<size_t,Itrabl>LCS<return_Itrabl=0>(const Itrabl&,const Itrabl&);
```
## dp/LIS (最長部分増加列)
最長部分増加列を求め、その長さを返す。  
最長部分増加列とは、連続とは限らない配列の要素 $i,j$ について $A_i < A_j$ が成り立つ部分列の事である。
```C++
size_t LIS(const Itrabl&);
```
## dp/inv_num (転倒数)
計算量 $O(N\lg N)$.  
`atcoder::fenwicktree`が必要.  
```C++
size_t inv_num(Itrabl&v);
```

