## diameter (直径)
無向木の直径を求める  
重み付は辺集合の配列は`pair<int,int>`で`cost,dst`の順である。  
重み無しは`vector<vector<int>>`で`dst`だけ持っていれば、`{1,dst}`に変えて重み付きに渡す。  
頂点数が $n$ のとき頂点は $0$ から $n-1$ までとする。  
連結な木を想定しているが、非連結でも最初の頂点と連結な木の直径が戻り値になるはず。  
連結であるか、頂点 $0$ の連結成分の直径を求めたい場合は初期値の引数は省略できる。
```C++
vv<pair<int,int>>edge(n);
for(int i=0;i<n-1;i++){
	int a,b,c;
	cin>>a>>b>>c;
	a--,b--;
	edge[a].push_back(make_pair(c,b));
	edge[b].push_back(make_pair(c,a));
}
int D=diameter(e,0);
```
```C++
vv<int>edge(n);
for(int i=0;i<n-1;i++){
	int a,b;
	cin>>a>>b;
	a--,b--;
	edge[a].push_back(b);
	edge[b].push_back(a);
}
int D=diameter(e);
```

