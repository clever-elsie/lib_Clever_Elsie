# 1次元化多次元配列
```C++
template<class T, size_t dimension>
class fvec;
```
次元数 `dimension` の固定長多次元配列を作る．  
`array<array<array<....`と同じはず．

## コンストラクタ
```C++
template<class T, size_t dimension>
fvec(fvec&&)=default;
fvec(const fvec&)=default;
fvec(const array<size_t,dimension>&dim_); // 1
fvec(const array<size_t,dimension>&dim_,const auto&v); // 2
```
1. 各次元での要素数を指定する．
1. 初期値を`vector`や`array`で与える．要素数などはすべて`dim_`を参照するので，与える`v`については呼出し側が責任を持つ．

次元というのはあくまでも平面が二次元，立体が三次元のような話であり，行列の $2\times 2$ 次元みたいな話ではない．それはむしろ`dim_`のことである．

## 要素アクセス
```C++
T& operator[](Args... args);
T& at(Args... args);
```
可変長引数，ただしサイズは`dimension`と等しくなければならない．  
`at()`は範囲外で`std::out_of_range`を`throw`する．


## fill
```C++
void fill(const T&val);
```
全要素を`val`で埋める．  
内部的には16個ずつのloop unroolを行っているためちょっと速い．

## size
```C++
size_t size()const; // 1
size_t size(size_t d)const; // 2
```
1. 全要素数
1. $d$ 次元目の配列数．含まれる全要素ではなく，それが持っている一個下の次元の数．

## iterator
```C++
iterator begin()const;
iterator end()const;
const_iterator cbegin()const;
const_iterator cend()const;
iterator rbegin()const;
iterator rend()const;
const_iterator crbegin()const;
const_iterator crend()const;
```
見ての通り．

以下の型もサポートしている．
```
value_type
reference
const_reference
iterator
const_iterator
reverse_iterator
const_reverse_iterator
```

## operator=
```C++
template<class RHS>
fvec&operator=(RHS&&rhs);
```
完全転送するためにテンプレートになっているが，`RHS`は`fvec`の右辺値か左辺値しか受け付けない．  


