# convolution
```C++
convolution<T,M>();
vector<T>prod(vector<T>,vector<T>);
```
畳み込む配列の型`T`と法とする $M$ を与える．  
$M>2e9$ のとき，内部的に`__int128_t`を使うからちょっと遅くなる．  
`T`は整数．

原始根は存在するなら勝手に見つけるので必要ない．  
任意の素数を入れてもいいが，Garnerで復元できるとは言ってない．  
計算量は配列のサイズ $l$ が $l=\min\{2^i\ |\ 2^i\ge n+m+1,\ i\in\mathbb{N}\}$ となり， $O(l\lg l)$.  
