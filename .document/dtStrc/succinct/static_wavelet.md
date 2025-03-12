# 静的 wavelet matrix
`wavelet`は`unsigned_integral`のみを許容し，`signed_integral`を使いたい場合は，与えられるすべての値より真に小さい値の絶対値をポテンシャルとして加算する`wavelet_s`を用いる必要がある．

|名前|説明|
|-|-|
|[(constructor)](#constructor)|コンストラクタ $O(N)$ |
|[set](#set)|構築前の値の変更 $O(1)$ |
|[fix](#fix)|wavelet matrixを構築 $O(N\lg N)$ |
|[get](#get)|値の取得 構築前 $O(1)$ 構築後 $O(\lg N)$ |
|[range_freq](#range_freq)|区間 $[L,R)$ の $x\in[lower,upper)$ の数を返す． $O(\lg N)$ |
|[rank](#rank)|区間 $[L,R)$ に含まれる $x$ の数を返す． $O(\lg N)$ |
|[select](#select)| $x$ が $idx$ 番目に現れるインデックスを返す． $O(\lg^2 N)$ |
|[less](#less)|小さい方から $ord$ 番目の値を返す． $0$-indexed $O(\lg N)$ |
|[greater](#greater)|大きい方から $ord$ 番目の値を返す． $0$-indexed $O(\lg N)$ |
|[prev](#prev)|区間 $[L,R)$ に含まれる $x$ 未満の最大の値を返す． $O(\lg N)$ |
|[next](#next)|区間 $[L,R)$ に含まれる $x$ 以上の最小の値を返す． $O(\lg N)$ |
|[operator=](#operator_eq)|代入演算子|



## <a id="constructor">constructor</a>
```C++
wavelet<UT>();
wavelet<UT>(size_t N);
wavelet<UT>(vector<type>&&v);
wavelet<UT>(const vector<type>&v);

wavelet_s<ST>();
wavelet_s<ST>(size_t N,UT potential);
wavelet_s<ST>(vector<type>&&v,UT potential);
wavelet_s<ST>(const vector<type>&v,UT potential);
```
`UT`は`unsigned_integral`, `ST`は`signed_integral`.  
`wavelet_s`の`UT`は`make_unsigned_t<ST>`の型．

空のコンストラクタはグローバルに置く場合に使う．  
サイズを与える場合は，すべての値が $0$ に初期化される．  
`vector`のcopyは $O(N)$, moveは $O(1)$.

`wavelet_s`はすべての要素に`potential`を加算して`wavelet`を`fix()`する．  
すべての`wavelet`のメソッドを`potential`で補正した状態で使えるようにラッパーする．

## <a id="set">set</a>
```C++
void set(size_t idx,type v);
```
`data[idx]=v;`を行うが，`fix()`以前にしか行ってはならない．

## <a id="fix">fix</a>
```C++
void fix();
```
wavelet matrix を構築する．

## <a id="get">get</a>
```C++
type get(size_t idx)const;
```
値を取得する．  
計算量は，`fix()`以前は`data`の値を直接得るため $O(1)$, `fix()`後はwavelet matrixから復元するため $O(\max\{data\})$. 
## <a id="range_freq">range_freq</a>
```C++
size_t range_freq(size_t l,size_t r,type upper)const;  // 1
size_t range_freq(size_t l,size_t r,type lower,type upper)const;  // 2
```
1. $[l,r)$ において， $[D,upper)$ の値の数を数える．  
1. $[l,r)$ において， $[lower,upper)$ の値の数を数える．  
1において，`wavelet_s`では $D=-potential$, `wavelet`では $D=0$.

## <a id="rank">rank</a>
```C++
size_t rank(size_t l,size_t r,type v)const;
```
$[l,r)$ に含まれる $v$ の数を数える．

## <a id="select">select</a>
```C++
size_t select(size_t idx,type v)const;  // 1
size_t select(size_t l,size_t idx,type v)const; // 2
```
1. 配列における $idx$ 番目の $v$ の位置を返す．
1. 配列の $[l,N)$ で $idx$ 番目の $v$ の位置を返す．
与えられた $idx$ が存在する数より多い場合は，配列のサイズ $N$ を返す．

## <a id="less">less</a>
```C++
type less(size_t l,size_t r,size_t ord)const;
```
$[l,r)$ で昇順 $ord$ 番目の値を得る． 0-indexed.

## <a id="greater">greater</a>
```C++
type greater(size_t l,size_t r,size_t ord)const;
```
$[l,r)$ で降順 $ord$ 番目の値を得る． 0-indexed.

## <a id="prev">prev</a>
```C++
type prev(size_t l,size_t r,type upper)const;
```
$[l,r)$ で $upper$ 未満の最大の値を返す．

## <a id="next">next</a>
```C++
type next(size_t l,size_t r,type lower)const;
```
$[l,r)$ で $lower$ 以上の最小の値を返す．