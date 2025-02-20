## detect_cycle (サイクル検出と辺集合構築)
### 有向グラフのサイクル検出
```C++
vector<int>find_cycle_directed(const vv<int>&e)
```
有向グラフのサイクルを一つ検出してその経路上の頂点を通る順に並べた`vector`を返す．  
ただし，この配列中の最初の要素と最後の要素は同じ．
すなわちサイクルに含まれる重複有りの頂点の数を $n$ ，配列を $v$ としたときに $v_0=v_{n-1}$が成り立つ．
### サイクルの辺構築
```C++
vector<int>
	reconstruct_edge_id_from_to(
		const vector<int>&v,
		const vector<map<int,int>>&e_id
	)
```
では，`find_cycle_directed()`で求めた頂点集合 $v$ と辺集合を渡すと，通る辺を順に辺の番号を構築して返す．  
ただし，多重辺があってはならないため，多重辺はどれか一つのみを入れること．  

`e_id`は`e_id[from][to]=id`となるようにする．

