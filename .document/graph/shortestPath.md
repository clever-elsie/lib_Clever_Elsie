## 最短距離
* dijkstra  
	辺集合と辺行列のバージョンがある。  
	辺集合のとき $O((V+E)\lg V)$, 辺行列のとき $O(V^2\lg V)$  
	ダイクストラ法により、負の重みをもたないグラフの最短距離を求め、その配列を返す。  
	頂点は $\left[0,n\right)$  
	重み付き隣接リスト(`vector`)か隣接行列を使う。  
	使い方は`cost,dst`の順番で`pair`を持つ二次元配列と、第二引数に開始点を渡す。
	隣接行列の場合は`edge[from][to]=w`となる隣接行列と、第二引数に開始点を渡す。  
	0からスタートするなら省略可能。  
	戻り値は`vector<int>`で最初の頂点からの距離を持っている。
	```C++
	vector<int> dist=dijkstra(edge,n);
	```
* bellmanford  
	計算量は $O(VE)$.  
	負サイクルはsecondがfalse
* warshallfloyd  
	計算量は $O(V^3)$.  
	負のサイクルは`int64_t -1`の例外を投げる．
	