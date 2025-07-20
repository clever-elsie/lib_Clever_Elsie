# mex
MEXとは，Minimum EXcluded valueの略であり，全順序集合の部分集合に含まれない最小の値のことである．

このライブラリは全順序集合となる型`T`について，その部分集合を保持するクラス`mex`を定義する．  
最小値を求めるため，自明な事実として全順序集合は下に有界でなければならない．  
また値を求める必要があることから，有限集合または加算無限集合でなければならない．

ライブラリでサポートしている型は`std::signed_integral`と`std::string`のみである．  
`std::string`は英小文字のみを扱う．
`std::less<std::string>`を使うと無限に後ろに`a`を追加するだけの自明なものになるので，同じ長さのときに次に早い辞書順比較で，短い順に並べる．

## mex_traits
```C++
template<class T>
struct mex_traits{
  using value_type=T;
  constexpr static T LOWER_BOUND;
  constexpr static T LOWER_LIMIT;
  constexpr static struct{
    constexpr bool operator()(const T&lhs,const T&rhs)const;
  } pred{}; // std::less<T>など
  constexpr static T prev(const T&); // const T& または T
  constexpr static T next(const T&);
};
```
`LOWER_BOUND`はMEXに含まれない`T`の下界．  
`LOWER_LIMIT`はMEXに含むことができる`T`の下限．  
`pred`は`invokable`は比較述語で，`operator<`と同じように`pred(a,a)`が`false`になるべき．`pred`によって全順序になる必要がある．  
`prev`は整数におけるデクリメントの役割．  
`next`は整数におけるインクリメントの役割．  
`prev(LOWER_BOUND,LOWER_LIMIT)`である必要がある．  
`LOWER_BOUND==prev(LOWER_LIMIT)`である必要がある．  
`next(LOWER_BOUND)==LOWER_LIMIT`である必要がある．

## コンストラクタ/デストラクタ/operator=
```C++
mex<T,traits=mex_traits<T>>();
mex(std::initializer_list<T>);
mex(mex&&)=default;
mex(const mex&)=default;
~mex()=default;
mex& operator=(mex&&)=default;
mex& operator=(const mex&)=default;
```
`mex_traits`は上記の性質を満たす構造体を渡すことができる．

## 追加
```C++
void add(const T&value);
```
計算量 $O(\lg N)$ . $N$ はそのとき集合に含まれる相異なる要素数．  
集合に要素を加える．

## 削除
```C++
void erase(const T&value);
```
計算量 $O(\lg N)$ . $N$ はそのとき集合に含まれる相異なる要素数．  
集合から`value`を削除する．重複している場合は $1$ 減らす．

## mex
```C++
const T& find()const;
```
計算量 $O(1)$ .  
そのときのMEXを返す．

## 要素数
```C++
size_t count(const T&value)const;
```
計算量 $O(1)$ .  
指示された要素の要素数を求める．