# お手抜きライブラリ
## 現在作成(放置)中
modint  
畳み込み
シンプレックス法
wavelet行列

## dp
### edit_distance (編集距離)
`string S,T`の編集距離を求める。
$O(|S||T|)$
```
size_t edit_distance(const string&,const string&);
```
### LCS (最長共通部分列)
最長共通部分文字列を求めて、その長さと文字列のペアを返す。
$O(|S||T|)$  
第3引数に0以外を与えた場合はLCSの長さを求めた後BFSでLCSとなる文字列の1つを探索して返す。  
数値だけが欲しいときは省略可。
```
pair<size_t,string>LCS(const string&,const string&,int=0);

```
### LIS (最長部分増加列)
最長部分増加列を求め、その長さを返す。  
最長部分増加列とは、連続とは限らない配列の要素 $i,j$ について`A_i< A_j`が成り立つ部分列の事である。
```
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
```
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
	```
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

### combination
* `combination<modint>(max)`のコンストラクタで，`modint`型と`N`の最大値を渡す．両方省略可．デフォルトでは`atcoder/modint998244353`.
* `P`,`C`,`H`,`factorial`,`invfact`で順列，組合せ，重複組合せ，階乗，逆数の階乗(?)を求めることができる．
* 前計算で最大値を渡していれば計算量は`O(1)`．
* そうでない場合は，`N`が更新される度に追加される要素数分だけ計算が必要になり，逆数の階乗(?)を計算は拡張ユークリッドの互除法の計算量を掛ける必要がある．
* 前計算は`O(N max\{f(i)\})`．`f`は拡張ユークリッドの互除法の計算量．
* ユークリッドの互除法の計算量はラメの定理で知られており，`a<b`のとき`a`の桁数を`m`として`5m`以下となる(らしい)．

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
```
int inversion_number(vector<class f>&v);
```