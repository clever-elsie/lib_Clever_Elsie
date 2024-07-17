# お手抜きライブラリ
## DP
### LCS(最長共通部分列)
最長共通部分文字列を求めて、その長さと文字列のペアを返す。  
文字列s,tについて、S=|s|,T=|t|としたときに計算量O(ST)で動作する。  
第3引数に1を与えた場合はLCSの長さを求めた後BFSでLCSとなる文字列の1つを探索して返す。  
数値だけが欲しいときは省略可。
```
string s,t;
pair<int,string> P = LCS(s,t,1); // 長さと文字列を求める
int L = LCS(s,t).fi; // 数値のみを求める
```
### LIS
最長部分増加列を求め、その長さを返す。  
最長部分増加列とは、連続とは限らない配列の要素i,jについて$A_i< A_j$が成り立つ部分列の事である。
```
vector<int> a;
int A=LIS(a);
```
## Graph
### diameter
無向木の直径を求める  
重み付は辺集合の配列はpair<int,int>でcost,dstの順である。  
重み無しはvector<vector<int>>でdstだけ持っていれば、{1,dst}に変えて重み付きに渡す。  
頂点数がnのとき頂点は0からn-1までとする。  
連結な木を想定しているが、非連結でも最初の頂点と連結な木の直径が戻り値になるはず。  
連結であるか、頂点0の連結成分の直径を求めたい場合は初期値の引数は省略できる。
```
vector<vector<pair<int,int>>>edge(n);
for(int i=0;i<n-1;i++){
	int a,b,c;
	cin>>a>>b>>c;
	a--,b--;
	edge[a].push_back(make_pair(c,b));
	edge[b].push_back(make_pair(c,a));
}
int D=diameter(e,0);
```
```
vector<vector<int>>edge(n);
for(int i=0;i<n-1;i++){
	int a,b;
	cin>>a>>b;
	a--,b--;
	edge[a].push_back(b);
	edge[b].push_back(a);
}
int D=diameter(e);
```
### dijkstar
ダイクストラ法により、負の重みをもたないグラフの最短距離を求め、その配列を返す。  
重み付きを想定しているが、重み無しにもオーバーロードしているので何らかの事情でBFSを使いたくない奇特な人間は使うといいでしょう。  
使い方はdiameterと同じで、cost,dstの順番で、重み無しはdstだけの辺集合を引数に渡し、第二引数に最初の頂点を渡せばいい。  
この時も、0からスタートするなら省略可能。  
戻り値はvector<int>で最初の頂点からの距離を持っている。

### unionfind
コンストラクタで頂点数を渡し、[0,n)で管理する。  
same(i,j):bool,unite(i,j):voidを利用してください。  
root(i)は提供してません。
## Tree
### inversion_number
atcoder::fenwicktreeが必要です.  
配列を渡すとその転倒数を求めます。
### trie
trie木のオブジェクトを生成し、その時に、文字種の一覧を配列で渡してください。  
重複があると正しく動作しません。今後改善します。  
あるいはtrie obj('a',26)のように連続することが保証される場合はそのように渡してください。  
obj.insert(string)で挿入できますが、コンストラクタで渡された文字以外が含まれる場合の動作は未定義です。  
obj.count()で、接頭辞が共通する文字列のその長さの総和を返します。
## 分数
分母分子unsigned long longで管理する。オーバーフローしない気がするときか、勇気ある者だけが使いなさい。  
frac(分子,分母)をullかllで使ってどうぞ。整数の時は単体で渡すと分母1ullとします。  
分母分子がullの範囲の負の分数を渡したいときはfrac(true,ull,ull)としてください。  
そんなんすぐオーバーフローすると思うから使うべきじゃないですけどねぇ。
## 行列累乗
MP(vv<typename>)の行列累乗。  
テンプレは乗算が定義されていれば動くんじゃないですか？  
整数なら何とかなるよ。知らんけど。
