# convolution
`convolution_fast`が性質の良い法を指定するもの．  
`convolution`は性質の良い法のときはそのまま畳み込みを行い，そうでないときにGarner法で復元する3回の畳み込みを行う．
```C++
convolution_fast<T,M>();
vector<T>prod(vector<T>,vector<T>);
```
```C++
convolution<T,M>();
vector<T>prod(const vector<T>&,const vector<T>&);
```
畳み込む配列の型`T`と法とする $M$ を与える．  
$M>3,037,000,499$ のとき，内部的に`__int128_t`を使うからちょっと遅くなる．  
`T`は整数．  
この定数の理由は，これが`int64_t`で最大のオーバーフローしない数だから．

原始根は存在するなら勝手に見つけるので必要ない．  
計算量は配列のサイズ $l$ が $l=\min\{2^i\ |\ 2^i\ge n+m+1,\ i\in\mathbb{N}\}$ となり， $O(l\lg l)$.  
