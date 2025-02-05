# お手抜きライブラリ
## 注意事項
型エイリアスは`local.h`準拠で記述したり，`std::`準拠だったり．  
`vc=vector`, `vv=vector<vector>>`, `pq=priority_queue`, `pqg=priority_queue<,,greater>`とか．  
計算量で $\lg$ とあるとき，それは $\log_2$ と同じ．  
$V,E$ は頂点集合，辺集合と考える．計算量に入れるときは頂点数，辺数と考える．  
$S,T$ は文字列だが，計算量に入れるときはその長さとする．  
$N$ は配列の長さとする． $2$ つ以上の配列について論じる場合は $M, L,\ldots$ と続ける．


## 現在作成(放置)中
- 赤黒木  
- suffix array  
- wavelet matrix  
- meissel Lehmer  

# dtStrc (データ構造)
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


## segtree.hpp
### segtreeとlazy_segtreeのスニペット
```C++
(sum|min|max)_seg<S>
(add|ch)_(sum|min|max)_lst<S,F>
range_affine_range_sum_lst<S,F>
```
セグ木は区間和，区間最小，区間最大．  
遅延セグ木は区間変更と区間加算に区間和，区間最小，区間最大．
さらに，区間への線形変換とその和を得る`range_affine_range_sum`を一応．

### Binary Indexed Tree
`BIT<S>` : 1点更新と区間 $[0,N)$ の加算結果を $O(\lg N)$ で取得することができる。  

`BIT(size_t)` : 要素数を渡すコンストラクタ。 $O(N)$  
`void add(size_t,S)` : 一点差分更新。 $O(\lg N)$  
`void set(size_t,S)` : 一点即値更新。 $O(\lg N)$  
`void get(size_t)` : 一点取得。 $O(\lg N)$  
`S sum(size_t,size_t)` : $[l,r)$ の和を求める。 $O(\lg N)$  

### segtree
`template<class S, S(*op)(S,S), S(*e)()> segtree`  
単位元`e`,演算`op`が定義された型`S`を渡して使う。 $[0,N)$

`segtree<>(const vc<S>&)` : 配列からセグ木を構築する。 $O(N\lg N)$  
`segtree<>(int)` : 要素数を指定するコンストラクタ。 $O(N)$  
`void set(size_t,S)` : 1点更新。 $O(\lg N)$  
`S get(size_t)` : 1点取得。 $O(1)$  
`S prod(size_t,size_t)` : 区間取得 $[l,r)$ 。 $O(\lg N)$

### lazy_segtree
`template<class S,S(*op),S(*e),class F,S(*mapping)(F,S),F(*composition)(F,F),F(*id)()>lst(n);`

|template|説明|
|-|-|
|型`S`|元の配列|
|型`F`|遅延の型|
|`op`|`S(S,S)`|
|`e`| $e\in S$ となる`op`の単位元|
|`mapping`|遅延要素を元の配列に対して適用するための関数`S(F,S)`|
|`composition`|遅延要素の計算 `F(F f,F g)`のとき，`f`が後から追加される遅延要素|
|`id`| $\text{id}\in F$となる`composition`の単位元|

```C++
lazy_segtree(u32 n);
lazy_segtree(const vector<S>&v);
void set(u32 idx, S val);
S get(u32 idx);
void apply(u32 l,u32 r,F f);
S prod(u32 l,u32 r);
```
## math.hpp
### mex
数列に含まれる最小の非負整数を求めるためのデータ構造  
`add(int)`で追加し、`erase(int)`で削除、`find(int)`でmexを返す。  
重複を考慮した区間 $[l,r)$ の管理をしているだけ。  
計算量はすべての操作に対して $O(\lg N)$.  
同じ数字の重複を許すため`add`した回数と同じ回数`erase`しないと消えない．

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
//f+-*/=g // 四則演算では内部的に約分
```

## modint
- `modint<M>(N)` : $N$ は省略時 $0$. $M$ は省略時 $998244353$.
- 四則演算が`modint`と整数型でできる．
- `pow(B)`で繰り返し二乗法で $n^B$ を $O(\lg B)$ で計算．
- `inv()`で逆元を計算． ラメの定理より $O(5x)$
- `val()`で整数値を取得． $O(1)$
- でも実は`operator size_t()`があるので整数値として扱える．(ただしprintfみたいな場合を除く)
- でも実は`cout,cin`では直接使える．

# geometry
## convex_hull
計算量は点の数を $N$ として, $O(N\lg N)$.  
最も左の頂点から始めて，反時計回りに凸包の極小の頂点集合を返す．  
最も左の頂点が複数ある場合は，そのうち最も下の点から始める．  
凸包の線分上に両端以外の点があるとき，その頂点は答えに含まない．  
```C++
convex_hull c(); // デフォルトコンストラクタ 宣言するだけ
vc<pi> c.monotone_chain(vc<pi>&); // 引数の配列はsortしてuniqueされる．コピーは無し
```
関数内部の`while`文に対して，比較演算子`<=`を`<`とすれば線分上の余分な点を含むことができる．  
`x,y`を逆転させた入力に対して，演算子を`>=`に変えれば，最も下の最も左の点から反時計回りに点集合を構築できる．  
時計回りにしたいとき，単に演算子を`>=`にするだけでよい．

## rotate
vector or dequeの2次元配列  
`rotate(const vv&)`: 反時計回り90度  
`rotate<!=0>(const vv&)`: 時計回り90度  

# Graph
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
	負のサイクルは`int64_t -1`の例外を投げる．
* warshallfloyd  
	計算量は $O(V^3)$.  
	負のサイクルは`int64_t -1`の例外を投げる．
	

## max_flow 最大フロー/最小カット
計算量は $O(VF)$. ただし $F$ は最大容量．辺の容量はすべて整数とする．  
`max_flow(int n)`  
`void add(u,v,cost)`  
`int64_t flow(s,t)`

## MST (Minimum Spanning Tree)
`kruskal(int,vc<pair<int,pi>>)` : 値だけと辺id構築． $O(E\lg E)$.  
`prim(vv{cost,to})` : dijkstraとかで使う普通の辺集合を渡す． $O(E\lg E)$.

## LCA
`LCA(vv<integral>)edge,root)` : 構築は $O(V+E)$ のDFS.  
`int get(u,v)` : 計算量 $O(\lg (V+E))$.

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

## SCC (強連結成分分解)
頂点数 $n$, 頂点番号 $[0,n)$.  
トポロジカルソートされた強連結成分の集合を返す．
強連結成分中の順序は不定
```C++
scc_graph(n);
void .add_edge(u,v);
vector<vector<size_t>> .scc();
```

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

# math
## basic_math
`gcd,lcm,egcd,exgcd,modinv`の計算量はラメの定理より $O(5\min(a,b))$.  
- `gcd(a,b)` : 最大公約数  
- `lcm(a,b)` : 最小公倍数  
- `egcd(a,b,x,y)` : 再帰拡張ユークリッドの互除法 $ax+by=\text{GCD}(a,b)$  
- `exgcd(a,b,x,y)` : 非再帰拡張ユークリッドの互除法  
- `mod_inv(a,m)` : $m$ を法とする逆元を求める．
- `mod_inv_prime(a,p)` : 素数 $p$ を法とする逆元を求める． $O(\lg p)$
- `safepow(a,b)` オーバーフロー防止。オーバーフロー時INT64_MAX $O(\lg b)$
- `modpow(a,b,m)` : $x\equiv a^b \pmod m$となる $x$ を求める． $O(\lg b)$

### combination
- `combination<modint>(max)`のコンストラクタで，`modint`型と $N$ の最大値を渡す．両方省略可．デフォルトでは`atcoder/modint998244353`.
- $P,C,H$, `factorial`, `invfact`で順列，組合せ，重複組合せ，階乗，逆数の階乗(?)を求めることができる．
- 前計算で最大値を渡していれば計算量は $O(1)$．
- そうでない場合は， $N$ が更新される度に追加される要素数分だけ計算が必要になり，逆数の階乗(?)の計算は線形時間
- 前計算は $O(N)$．

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


## floor_quotients (商の切り捨て列挙)
```C++
vector<uint64_t>quotients(uint64_t n);
```
$n$ を $\forall x\in [1,n]$で割ったときの商の切り捨てとして有り得るすべてを列挙する．
計算時間は多分 $O(\sqrt{N})$

## factorize (素因数分解)
```C++
factorize(upperlimit);
vector<int64_t>.factor_vi(int64_t x);
vector<pair<int64_t,uint32_t>>.factor_vp(int64_t x);
map<int64_t,uint32_t>factor(int64_t x);
```
計算量は $O(\sqrt{N})$．  
素因数を全列挙する`vi`，同じ物の数をまとめる`vp`，面倒なので`map`にした`factor`がある．
ミラーラビンではないので別に速くない．

## matrix
```C++
vv<U> matrix_add<bool sub=false>(const vv<S>&,const vv<T>&);
vv<U> matrix_mul<threshold,q2>(vv<S>&,vv<T>&);
vv<S> Mpower(vv<S>&A,int b);
v3<S> split_quad_matrix(const vv<S>&);
vv<S> merge_quad_matrix(const vv<S>&,x4);
```

`matrix_add` : 計算量は $O(n^2)$.
第一テンプレート引数で引き算を指定．2つの行列は同じサイズである必要がある．
戻り値の型は`S,T`の両方からキャスト可能な`U`．

`matrix_mul` : `threshold`は $O(n^3)$ と $O(n^{\lg 7})$ を切り替える $n$.
`q2`は入力される行列が2つとも2冪のときに`false`．
行列のサイズは $n\times m,\ m\times l$ となるような任意の $n,m,l>0$ となる整数．
戻り値の型は`S,T`の両方からキャスト可能な`U`．

`Mpower` : 行列累乗．  
	`S`は乗算が定義されている必要がある。  
	計算量は $n\times n$ 行列について $O(n^3\lg b)$  
	通常オーバーフローするため，型には `modint` を入れる．

`split_quad_matrix` : 行列を4分割する． $O(n^2)$

`merge_quad_matrix` : 4つの行列を1つの行列にまとめる． $O(n^2)$

# string
## kmp
kmp法により $O(S+T)$で`T`が現れる先頭位置を全列挙。

`kmp(const string&)`: Sを登録。  
`kmp(const string&,const string&)`: Sを登録し、Tの移動テーブル作成  
`void make_match(const string&tar)`: Tの移動テーブルを作成  
`vector<ssize_t>find()`: Tの位置を全列挙.  

## rolling hash
**遅い**  

`rollingHash<n>(const string&)` コンストラクタで検索元の文字列を設定。 nはロリハの本数

`dynamicRollingHash<n>(const string&)` セグ木版。文字変更機能あり。

`.find(const string&)` 対象の文字列を検索する。

`.same(int,int,int,int)` 区間の文字列一致判定 $[L1,R1), [L2,R2)$

`.set(int idx,char c)` idxの文字をcに変更する。セグ木版限定。

## dp/edit_distance (編集距離)
計算量 $O(ST)$.  
`.begin(),.end()`のあるコンテナクラスには使える．
```C++
size_t edit_distance(const Itrabl&,const Itrabl&);
```
## dp/LCS (最長共通部分列)
計算量 $O(ST)$.  
LCSを求めて、その長さと列のペアを返す。  
列はテンプレート引数が非 $0$ のときのみ得る．  
省略時及び $0$ のときは空列を得るため無視してよい．
```C++
pair<size_t,Itrabl>LCS<return_Itrabl=0>(const Itrabl&,const Itrabl&);
```
## dp/LIS (最長部分増加列)
最長部分増加列を求め、その長さを返す。  
最長部分増加列とは、連続とは限らない配列の要素 $i,j$ について $A_i < A_j$ が成り立つ部分列の事である。
```C++
size_t LIS(const Itrabl&);
```
## dp/inv_num (転倒数)
計算量 $O(N\lg N)$.  
`atcoder::fenwicktree`が必要.  
```C++
size_t inv_num(Itrabl&v);
```

## trie
trie木のオブジェクトを生成し、その時に文字集合を決定する．重複は取り除く．  
あるいは`trie obj('a',26)`のように連続することが保証される場合はそのように渡す．
`obj.insert(string)`で挿入．文字集合に含まれない文字を含む文字列の動作は未定義．  
`count_prefix_sum()`で共通接頭辞の総和を返す．
`count_longest_prefix()`で最長共通接頭辞を返す．
