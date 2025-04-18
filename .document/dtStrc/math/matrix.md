# 行列
内部的に平坦化配列を持ち，主にloop-unroolingで高速化している．テンプレート化するためにAVX命令を用いてないので限界高速化はされてない．

以下計算量は行数 $R$ , 列数 $C$ として表す．

## コンストラクタ・デストラクタ・代入
```C++
matrix<T>()=default;
matrix<T>(matrix<T>&&)=default;
matrix<T>(const matrix<T>&)=default;
matrix<T>&operator=(matrix<T>&&)=default;
matrix<T>&operator=(const matrix<T>&)=default;
~matrix<T>()=default;

matrix<T>(size_t row,size_t col); // 1
matrix<T>(size_t row,size_t col,const T&init); // 2

template<class S>
requires std::same_as<std::decay_t<S>,std::vector<T>>
matrix<T>(size_t row,size_t col,S&&data_); // 3

template<size_t row,size_t col>
matrix<T>(const array<array<T,col_>,row_>&data_); // 4

matrix<T>(const std::vector<std::vector<T>>&data_); // 5
```
基本的に`default`生成コードでcopy, moveに対応．

1. row行col列の行列を`T()`で初期化．
1. 初期値`init:T`のrow行col列行列．
1. row行col列の平坦化配列をcopyまたはmove
1. row行col列の2次元`std::array`をcopy
1. row行col列の2次元`std::vector`をcopy．列数が異なる行があるときassert落ち．

## 要素アクセス $O(1)$
```C++
T& operator[](size_t i,size_t j);
const T& operator[](size_t i,size_t j)const;
```
`A[i][j]`ではなく`A[i,j]`としてアクセスする．
`.begin(),.cbegin(),end(),cend(),rbegin(),crbegin(),rend(),crend()`と`iterator, const_iterator, reverse_iterator, const_reverse_iterator`が存在するので範囲forが使える．

## 次元 $O(1)$
```C++
pair<size_t,size_t> size()const;
```
pair<行,列>;

## 転置 $O(RC)$
```C++
void transpose();
```
平坦化された配列をブロック化して転置．

## 四則演算
### スカラ
```C++
template<class U>
matrix<T>& operator*=(const U&rhs);
template<class U,class V,class W=std::common_type_t<U,V>>
matrix<W>& operator*(const U&lhs,const matrix<V>&rhs);
template<class U,class V,class W=std::common_type_t<U,V>>
matrix<W>& operator*(const matrix<U>&lhs,const V&rhs);
template<class U>
matrix<T>& operator/=(const U&rhs);
template<class U,class V,class W=std::common_type_t<U,V>>
matrix<W>& operator/(const matrix<U>&lhs,const V&rhs);
```
スカラ倍の複合代入演算子と演算子を定義．
行列でスカラを割ることはできない．
### 行列和
```C++
template<class U>
matrix<T>& operator+=(const matrix<U>&rhs);
template<class U>
matrix<T>& operator-=(const matrix<U>&rhs);

template<class U,class V,class W=std::common_type_t<U,V>>
matrix<W>& operator+(const matrix<U>&lhs,const matrix<V>&rhs);
template<class U,class V,class W=std::common_type_t<U,V>>
matrix<W>& operator-(const matrix<U>&lhs,const matrix<V>&rhs);
```
次元が一致しなければならない．

処理は内部的にloop-unroolされている．

### 行列積
```C++
template<class U>
matrix<T>&operator*=(const matrix<U>&rhs);
template<class U,class V,class W=std::common_type_t<U,V>>
matrix<W>& operator*(const matrix<U>&lhs,const matrix<V>&rhs);
```
$N\times M,\ M\times L$ の行列積で， $\min{N,M,L}\ge 256$ で strassen algorithm を用いる．
そうでないとき， $\max{N,M,L}\ge 64$ でブロック化，そうでないときナイーブ実装．

### 行列累乗
```C++
matrix pow(uint64_t b)const;
```
行列累乗 $R=C$ のとき， $N:=R$ とし $O(N^{\lg 7}\lg b)$
