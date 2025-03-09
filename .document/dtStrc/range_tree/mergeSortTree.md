# merge sort tree
配列の区間 $[l,r)$ に含まれる $(lower, upper]$ の要素の総和や要素数などを数えられる．  

すべての値を設定し終えたら，`fix()`によって構築する必要がある．  

## コンストラクタ
```C++
merge_sort_tree<T>(); // 1
merge_sort_tree<T>(int32_t n,const T&v); // 2
merge_sort_tree<T>(const vector<T>&v);   // 3
merge_sort_tree<T>(vector<T>&&v);        // 4
```
1. 空のコンテナを宣言
1. 要素数 $n$ の 初期値 $v$ で宣言
1. `vector`をコピーして宣言
1. `vector`をmoveして宣言
いずれの場合でも要素数は不変．  
空のコンテナを使う場合は後から`operator=`で再代入する必要がある．

## fix() $O(N\lg N)$
```C++
void fix();
```
`fix()`を呼ばずに`prod(), cnt(), sum()`を呼ぶと正しい答えを返す保証はない．  
`fix()`を呼び出した後に変更すれば`assert`落ちする．  

## 色々
```C++
pair<T,int32_t>prod(size_t l,size_t r,const T&upper)const;
pair<T,int32_t>prod(size_t l,size_t r,const T&lower,const T&upper)const;
int32_t cnt(size_t l,size_t r,const T&upper)const;
int32_t cnt(size_t l,size_t r,const T&lower,const T&upper)const;
T sum(size_t l,size_t r,const T&upper)const;
T sum(size_t l,size_t r,const T&lower,const T&upper)const;
```
$[l,r)$ の $(lower,upper]$ の総和や数を数える関数と，両方を数えるやつ．  
`cnt, sum`は内部的に`prod`を呼び出すので，実行時間はすべて同じ．

## operator=()
```C++
merge_sort_tree<T>&operator=(const merge_sort_tree&);
merge_sort_tree<T>&operator=(merge_sort_tree&&);
```
グローバルに置いたときに`main()`とかで初期化したいときにどうぞ．