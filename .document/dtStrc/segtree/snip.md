# セグ木
```C++
min_seg<S>;
max_seg<S>;
sum_seg<S>;
```
型`S`に対する区間和，区間最小，区間最大．  

# 遅延セグ木
```C++
add_sum_lst<S,F>;
add_min_lst<S,F>;
add_max_lst<S,F>;
ch_sum_lst<S,F>;
ch_min_lst<S>;
ch_max_lst<S,F>;

affine_sum_lst<S,F>;
ch_composite_lst<S,F>;
```
型`S,F`に対する区間変更と区間加算に区間和，区間最小，区間最大．

区間和を得るときは`set`に`sum_lst<S>`を用いる必要がある．  
初期化では必ず`size`を $1$ にする．コンストラクタでは第二引数．

## affin_sum_lst
`affine_sum_lst`は区間 $[l,r)$ の要素に対する線形変換とその和を求めるもの．  
線形写像を何度も適用するため $998244353$ の法を答えとして要求されることが多いため`S=mint`として用いるとよい．  
```C++
void set(size_t idx,const S&val);
S get(size_t idx);
S prod(size_t l,size_t r);
S all_prod();
void apply(size_t l,size_t r,S a,S b);
```
継承クラスなので代入に要素数が必要なかったり，`prod`の戻り値が線形写像`a,b`の構造体だけだったり，`apply`が $ax+b$ の $a,b$だけでよかったりとちょっとだけ便利．  
それ以外の機能を使うときは `S=sum_lst<S>, F=affine<S>`であると考えればよい．右辺の`S`はテンプレート引数．左辺の`S`は本来の`S`.

## ch_composite_lst
`ch_composite_lst`は区間 $[l,r)$ の線形変換を変更し，その $r-l$ 重写像を求めるもの．  
`affine_sum_lst`と同様に `S=mint`とすることが多いだろう．  
内部的な型は`S=F=sum_lst<affine<S>>`.

```C++
void set(size_t idx,const S&val);
affine<S> get(size_t idx);
affine<S> prod(size_t l,size_t r);
affine<S> all_prod();
void apply(size_t l,size_t r,S a,S b);
```