## convolution
畳み込み演算ができる．
```C++
convolution<size_t M,auto op,auto e>();
vector<int32_t>.prod(vector<int32_t>,vector<int32_t>);
```
具体的に積以外で何ができるかは知らないが，演算`op`と単位元`e()`が定義できれば大丈夫でしょ知らんけど．  
`e()`は余った部分を埋めるための値を返す関数．  
$M$ は法となる素数．
デフォルト引数は以下．
```C++
M=998244353
op=[](int64_t a,int64_t b){return a*b;}
e=[](){return 0;}
```
原始根は存在するなら勝手に見つけるので必要ない．  
任意の素数を入れてもいいが，Garnerで復元できるとは言ってない．  
再帰なので速くないが，計算量は配列のサイズ $l$ が $l=\min\{2^i\ |\ 2^i\ge n+m+1,\ i\in\mathbb{N}\}$ となり， $O(l\lg l)$.  
空間計算量は多分大きい．
