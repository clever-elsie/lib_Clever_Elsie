## unionFind
コンストラクタで頂点数を渡し、 $[0,n)$ で管理する。  
計算量は`same, unite, root`に関してアッカーマン関数の逆関数を $\alpha$ として、
本来償却計算時間で $O(\alpha(N))$ になる.  
ところで競プロではクエリの数が少ないため $O(\lg N)$ 程度であると考えてもよい(はず)。

`unionFind(int)`: 頂点数 $N$ を渡す。 $O(N)$  
`int ord()`: グラフの位数(頂点数)を求める。 $O(1)$  
`int root(int)`: 頂点の所属する素集合の代表元を求める。 $O(\lg N)$  
`int size(int)`: 頂点の所属する素集合に含まれる頂点数を求める。 $O(1)$  
`bool same(int,int)`: 2頂点が同じ頂点にある時`true` $O(\lg N)$  
`void unite(int,int)`: 2頂点を同じ素集合にまとめる。 $O(\lg N)$  
### ポテンシャル付き unionFind
`unionFindP(int)`: `unionFind`と同じ．  
`ord(), root(), size(), same()`: `unionFind`と同じ．  
`bool unite(int u,int v,int w)`:`p[u]=p[v]+w`を与える．既知の情報と整合するなら`true`を返し，そうでないとき`false`を返す．`false`のときは情報を捨てる．  
`S diff(int u,int v)`: `v`から見たときの`u`のポテンシャルを返す． $O(\lg N)$  
`S potential(int)`: 頂点のrootからのポテンシャルを返す． $O(\lg N)$