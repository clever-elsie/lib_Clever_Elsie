# お手抜きライブラリ
## DP
### LCS (最長共通部分列)
最長共通部分文字列を求めて、その長さと文字列のペアを返す。  
文字列s,tについて、`S=|s|,T=|t|`としたときに計算量`O(ST)`で動作する。  
第3引数に1を与えた場合はLCSの長さを求めた後BFSでLCSとなる文字列の1つを探索して返す。  
数値だけが欲しいときは省略可。
```
string s,t;
pair<int,string> P = LCS(s,t,1); // 長さと文字列を求める
int L = LCS(s,t).fi; // 数値のみを求める
```
### LIS (最長部分増加列)
最長部分増加列を求め、その長さを返す。  
最長部分増加列とは、連続とは限らない配列の要素 $i,j$ について`A_i< A_j`が成り立つ部分列の事である。
```
vector<int> a;
int A=LIS(a);
```
## FRAC (分数)
分母分子が`unsigned long long`で管理される符号付き分数。  
比較と四則演算と代入演算子がオーバーロードされている。
整数値は分母省略可。
```
frac(); // 0/1
frac(bool,ull,ull=1ULL); // 符号、分子、分母
frac(ull,ull=1ULL); // 分子、分母
frac(ll,ll=1LL); // 分子、分母
frac(frac&), frac(frac*); // 参照、ポインタ

f.approx(); // 任意のタイミングで約分
f+g // 四則演算では内部的に約分
```
## Graph
### diameter (直径)
無向木の直径を求める  
重み付は辺集合の配列は`pair<int,int>`で`cost,dst`の順である。  
重み無しは`vector<vector<int>>`で`dst`だけ持っていれば、`{1,dst}`に変えて重み付きに渡す。  
頂点数が`n`のとき頂点は`0`から`n-1`までとする。  
連結な木を想定しているが、非連結でも最初の頂点と連結な木の直径が戻り値になるはず。  
連結であるか、頂点`0`の連結成分の直径を求めたい場合は初期値の引数は省略できる。
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
### dijkstra (ダイクストラ法)
ダイクストラ法により、負の重みをもたないグラフの最短距離を求め、その配列を返す。  
頂点は$[0,n)$  
重み付き隣接リスト(`vector`)か隣接行列を使う。  
使い方は`cost,dst`の順番で`pair`を持つ二次元配列と、第二引数に開始点を渡す。
隣接行列の場合は`edge[from][to]=w`となる隣接行列と、第二引数に開始点を渡す。  
0からスタートするなら省略可能。  
戻り値は`vector<int>`で最初の頂点からの距離を持っている。
```
vector<int> dist=dijkstra(edge,n);
```
### unionfind
コンストラクタで頂点数を渡し、$[0,n)$で管理する。  
```
union_find uf(n);
uf.unite(u,v);
if(uf.same(u,v)) ;
```
## Matrix
### MatrixPower
M=MP(vector\<vector\<S\>\>&M)の行列累乗。  
`S`は乗算が定義されている必要がある。

## Tree
### inversion_number
atcoder::fenwicktreeが必要.  
配列を渡すとその転倒数を求める。
配列は比較が定義されている必要がある。
```
int inversion_number(vector<class f>&v);
```
### segtree
`template<class S,S e>segment_tree`  
単位元`e`,演算`op`が定義された型`S`を渡して使う。
$[0,n)$
```
int op(int a,int b){return a+b;}
segtree<int,op,0> t(n);
t.set(i,val);
t.prod(l,r); // [l,r)
```
### trie
trie木のオブジェクトを生成し、その時に、文字種の一覧を配列で渡してください。  
重複があると正しく動作しません。今後改善します。  
あるいはtrie obj('a',26)のように連続することが保証される場合はそのように渡してください。  
obj.insert(string)で挿入できますが、コンストラクタで渡された文字以外が含まれる場合の動作は未定義です。  
count_prefix_sum()で共通接頭辞の総和を返します。
count_longest_prefix()で最長共通接頭辞を返します。