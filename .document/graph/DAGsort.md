## DAGsort (トポロジカルソート)
`vector<size_t>DAGsort(const vv<T>&edge)`
辺集合`vector<vector<T>>`を渡すとトポロジカルソート順の配列を返す.  
ただし，`T`は整数型で`n=edge.size()`として $[0,n)$ を要素として持つものとする．  
有向サイクルを持つ辺集合を渡したときは未定義．  
戻り値は頂点 $i$ が $v_i$ 番目であることを表す配列．
トポロジカルソート順頂点集合を得るには，以下の変換が必要になる．

```C++
vector<size_t> v=DAGsort(edge), p(v.size());;
for(int i=0;i<v.size();++i)
	p[v[i]]=i;
```

