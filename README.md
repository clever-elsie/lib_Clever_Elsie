# お手抜きライブラリ
## 現在作成(放置)中
畳み込み  
シンプレックス法  
wavelet行列  
遅延セグ木  

## dp
### edit_distance (編集距離)
`string S,T`の編集距離を求める。
$O(|S||T|)$
```C++
size_t edit_distance(const string&,const string&);
```
### LCS (最長共通部分列)
最長共通部分文字列を求めて、その長さと文字列のペアを返す。
$O(|S||T|)$  
第3引数に0以外を与えた場合はLCSの長さを求めた後BFSでLCSとなる文字列の1つを探索して返す。  
数値だけが欲しいときは省略可。
```C++
pair<size_t,string>LCS(const string&,const string&,int=0);
```
### LIS (最長部分増加列)
最長部分増加列を求め、その長さを返す。  
最長部分増加列とは、連続とは限らない配列の要素 $i,j$ について`A_i< A_j`が成り立つ部分列の事である。
```C++
int LIS(const vector<T>&);
```

## dtStrc (データ構造)
### unionFind
コンストラクタで頂点数を渡し、$[0,n)$で管理する。
計算量は`same, unite, root`に関してアッカーマン関数の逆関数を$\alpha$として、
本来償却計算時間で$O(\alpha(N))$になるが、
競プロではクエリの数が少ないため$O(\log_2 N)$程度であると考えてもよい(はず)。

`unionFind(int)`: 頂点数$N$を渡す。 $O(N)$  
`int ord()`: グラフの位数(頂点数)を求める。 $O(1)$  
`int root(int)`: 頂点の所属する素集合の代表元を求める。  
`int size(int)`: 頂点の所属する素集合に含まれる頂点数を求める。  
`bool same(int,int)`: 2頂点が同じ頂点にある時`true` $O(\log_2 N)$  
`void unite(int,int)`: 2頂点を同じ素集合にまとめる。 $O(\log_2 N)$  

### BIT (binary indexed tree)
`template<class S>BIT`  
1点更新と区間 $[0,N)$ の加算結果を$O(\log_2 N)$で取得することができる。  

`BIT(size_t)`: 要素数を渡すコンストラクタ。 $O(N)$  
`void add(size_t,S)`: 一点差分更新。  
`void set(size_t,S)`: 一点即値更新。  
`void get(size_t)`: 一点取得。  
`S sum(size_t,size_t)`: $[l,r)$の和を求める。  

### segtree
`template<class S, S(*op)(), S(*e)()> segtree`  
単位元`e`,演算`op`が定義された型`S`を渡して使う。 $[0,N)$

`segtree<>(const vector\<S\>&)`: 配列からセグ木を構築する。  
`segtree<>(int)`: 要素数を指定するコンストラクタ。  
`void set(size_t,S)`: 1点更新。 $O(\log_2 N)$  
`S get(size_t)`: 1点取得。 $O(1)$  
`S prod(size_t,size_t)`: 区間取得$[l,r)$。 $O(\log_2 N)$

### trie
trie木のオブジェクトを生成し、その時に、文字種の一覧を配列で渡してください。  
重複があると正しく動作しません。今後改善します。  
あるいはtrie obj('a',26)のように連続することが保証される場合はそのように渡してください。  
obj.insert(string)で挿入できますが、コンストラクタで渡された文字以外が含まれる場合の動作は未定義です。  
count_prefix_sum()で共通接頭辞の総和を返します。
count_longest_prefix()で最長共通接頭辞を返します。

### mex
数列に含まれる最小の非負整数を求めるためのデータ構造
addで追加し、eraseで削除、findでmexを返す。
重複を考慮した区間[l,r)の管理をしているだけ。

### atcoder_lazy_segtree
AtCoder Libraryのlazy_segtreeを区間変更、区間加算の遅延演算で区間和、区間最小、区間最大が取れるように用意しているテンプレ。

### frac (分数)
分母分子が`unsigned long long`で管理される符号付き分数。  
比較と四則演算と代入演算子がオーバーロードされている。
整数値は分母省略可。
```C++
frac(); // 0/1
frac(bool,ull,ull=1ULL); // 符号、分子、分母
frac(ull,ull=1ULL); // 分子、分母
frac(ll,ll=1LL); // 分子、分母
frac(frac&), frac(frac*); // 参照、ポインタ

f.approx(); // 任意のタイミングで約分
f+-*/=g // 四則演算では内部的に約分
```
## Graph
### 最短距離
* dijkstra  
	辺集合と辺行列のバージョンがある。  
	ダイクストラ法により、負の重みをもたないグラフの最短距離を求め、その配列を返す。  
	頂点は$[0,n)$  
	重み付き隣接リスト(`vector`)か隣接行列を使う。  
	使い方は`cost,dst`の順番で`pair`を持つ二次元配列と、第二引数に開始点を渡す。
	隣接行列の場合は`edge[from][to]=w`となる隣接行列と、第二引数に開始点を渡す。  
	0からスタートするなら省略可能。  
	戻り値は`vector<int>`で最初の頂点からの距離を持っている。
	```C++
	vector<int> dist=dijkstra(edge,n);
	```
* bellmanford
	負のサイクル持ちは`int64_t(-1)`で例外を投げる。
* warshallfloyd
	見ての通り

### max_flow 最大フロー/最小カット
`max_flow(int n)`  
`void add(u,v,cost)`  
`int64_t flow(s,t)`

### MST (Minimum Spanning Tree)
`kruskal(int,vv)`:値だけと辺id構築  
`prim(vv)`

### LCA
`LCA(vv<integral>)edge,root)`  
`int get(u,v)`

### diameter (直径)
無向木の直径を求める  
重み付は辺集合の配列は`pair<int,int>`で`cost,dst`の順である。  
重み無しは`vector<vector<int>>`で`dst`だけ持っていれば、`{1,dst}`に変えて重み付きに渡す。  
頂点数が`n`のとき頂点は`0`から`n-1`までとする。  
連結な木を想定しているが、非連結でも最初の頂点と連結な木の直径が戻り値になるはず。  
連結であるか、頂点`0`の連結成分の直径を求めたい場合は初期値の引数は省略できる。
```C++
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
```C++
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

### SCC (強連結成分分解)
頂点数 $n$, 頂点番号 $[0,n)$.  
トポロジカルソートされた強連結成分の集合を返す．
強連結成分中の順序は不定
```C++
scc_graph(n);
void .add_edge(u,v);
vector<vector<size_t>> .scc();
```

### DAGsort (トポロジカルソート)
辺集合`vector<vector<T>>`を渡すとトポロジカルソート順の配列を返す.  
ただし，`T`は整数型で`n=edge.size()`として $[0,n)$を要素として持つものとする．  
有向サイクルを持つ辺集合を渡したときは未定義．

`vector<size_t>DAGsort(const vv<T>&edge)`

### detect_cycle (サイクル検出と辺集合構築)
#### 有向グラフのサイクル検出
```C++
vector<int>find_cycle_directed(const vv<int>&e)
```
有向グラフのサイクルを一つ検出してその経路上の頂点を通る順に並べた`vector`を返す．  
ただし，この配列中の最初の要素と最後の要素は同じ．すなわちサイクルに含まれる重複有りの頂点の数を $n$ ，配列を $v$ としたときに $v[0]=v[n-1]$が成り立つ．
#### サイクルの辺構築
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

## misc
### rotate
vector or dequeの2次元配列  

`rotate(const vv&)`: 反時計回り
`rotate<!=0>(const vv&)`: 時計回り

## math
### gcd, lcm, egcd
`gcd_(a,b)`  
`lcm_(a,b)`  
`egcd(a,b,x,y)`

### power
* safepow オーバーフロー防止。オーバーフロー時INT64_MAX
* modpow(a,b,mod) modが法
* Mpower  
	`vv<S> Mpower(vv<S>&,int)`の行列累乗。  
	`S`は乗算が定義されている必要がある。

### modint
* `modint(N)`で使う．`N`は省略時 $0$
* 四則演算が`modint`と整数型でできる．
* `pow(B)`で繰り返し二乗法で $n^B$ を $O(\log_2 B)$で計算．
* `inv()`で逆元を計算．
* `val()`で整数値を取得．
* でも実は`operator size_t()`があるので整数値として扱える．(ただしprintfみたいな場合を除く)
* でも実は`cout,cin`では直接使える．

### combination
* `combination<modint>(max)`のコンストラクタで，`modint`型と`N`の最大値を渡す．両方省略可．デフォルトでは`atcoder/modint998244353`.
* `P`,`C`,`H`,`factorial`,`invfact`で順列，組合せ，重複組合せ，階乗，逆数の階乗(?)を求めることができる．
* 前計算で最大値を渡していれば計算量は`O(1)`．
* そうでない場合は，`N`が更新される度に追加される要素数分だけ計算が必要になり，逆数の階乗(?)の計算は線形時間
* 前計算は`O(N)`．

### convolution
畳み込み演算ができる．
```C++
convolution<size_t M,auto op,auto e>();
vector<int32_t>.prod(vector<int32_t>,vector<int32_t>);
```
具体的に積以外で何ができるかは知らないが，演算`op`と単位元`e()`が定義できれば大丈夫でしょ知らんけど．  
`e()`は余った部分を埋めるための値を返す関数．  
$M$は法となる素数．
デフォルト引数は以下．
```C++
M=998244353
op=[](int64_t a,int64_t b){return a*b;}
e=[](){return 0;}
```
原始根は存在するなら勝手に見つけるので必要ない．
任意の素数を入れてもいいが，Garnerで復元できるとは言ってない．


### floor_quotients (商の切り捨て列挙)
```C++
vector<uint64_t>quotients(uint64_t n);
```
$n$ を $\forall x\in [1,n]$で割ったときの商の切り捨てとして有り得るすべてを列挙する．
計算時間は多分 $O(\sqrt{N})$

### factorize (素因数分解)
```C++
factorize(upperlimit);
vector<int64_t>.factor_vi(int64_t x);
vector<pair<int64_t,uint32_t>>.factor_vp(int64_t x);
map<int64_t,uint32_t>factor(int64_t x);
```
計算量は $O(\sqrt{N})$．  
素因数を全列挙する`vi`，同じ物の数をまとめる`vp`，面倒なので`map`にした`factor`がある．
ミラーラビンではないので別に速くない．

## string
### kmp
kmp法により $O(|S|+|T|)$で`T`が現れる先頭位置を全列挙。

`kmp(const string&)`: Sを登録。  
`kmp(const string&,const string&)`: Sを登録し、Tの移動テーブル作成
`void make_match(const string&tar)`: Tの移動テーブルを作成
`vector<ssize_t>find()`: Tの位置を全列挙.

### rolling hash
`rollingHash<n>(const string&)` コンストラクタで検索元の文字列を設定。 nはロリハの本数

`dynamicRollingHash<n>(const string&)` セグ木版。文字変更機能あり。

`.find(const string&)` 対象の文字列を検索する。

`.same(int,int,int,int)` 区間の文字列一致判定 $[L1,R1), [L2,R2)$

`.set(int idx,char c)` idxの文字をcに変更する。セグ木版限定。


## tree
### inversion_number
atcoder::fenwicktreeが必要.  
配列を渡すとその転倒数を求める。
配列は比較が定義されている必要がある。
```C++
int inversion_number(vector<class f>&v);
```