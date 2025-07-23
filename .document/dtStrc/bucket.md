# bucket
```C++
namespace elsie{
  template<class T, size_t N>
  class bucket;
}
```
`bucket`は上限`N`の可変長データ型．  
`N==0`はエラー．
`sizeof(bucket<T,N>)` $=N\times$ `sizeof(T)` $+$ `sizeof(size_t)` となる配列のラッパー型である．  
`std::vector`と`std::array`の中間の，固定長のメモリ領域を可変長で管理するために使う．  

最低限C++20である必要がある．
## メンバ関数
### 構築・破棄
|名前|説明|
|-|-|
|[`(constructor)`](#constructor)|コンストラクタ|
|[`(destructor)`](#destructor)|デストラクタ|
|[`operator=`](#operator=)|代入演算子|

### イテレータ
|名前|説明|
|-|-|
|[`begin`](#begin)|先頭の要素を指すイテレータを取得する|
|[`end`](#end)|末尾の次を指すイテレータを取得する|
|[`cbegin`](#cbegin)|先頭の要素を指すRead-Onlyテレータを取得する|
|[`cend`](#cend)|末尾の次を指すRead-Onlyイテレータを取得する|
|[`rbegin`](#rbegin)|末尾の要素を指す逆イテレータを取得する|
|[`rend`](#rend)|先頭の前を指す逆イテレータを取得する|
|[`crbegin`](#crbegin)|末尾の要素を指すRead-Only逆イテレータを取得する|
|[`crend`](#crend)|先頭の前を指すRead-Only逆イテレータを取得する|

### 領域
|名前|説明|
|-|-|
|[`size`](#size)|要素数を取得する|
|[`resize`](#resize)|要素数を変更する|
|[`capacity`](#capacity)|各納できる残りの要素数を取得する|
|[`empty`](#empty)|コンテナが空かどうか判定する|
|[`data`](#data)|データを保持する配列の先頭ポインタを返す|

### 要素アクセス
|名前|説明|
|-|-|
|[`operator[]`](#operator[])|要素アクセス|
|[`at`](#at)|要素アクセス|
|[`data`](#data)|配列の先頭へのポインタを取得する|
|[`front`](#front)|先頭要素への参照を取得する|
|[`back`](#back)|末尾要素への参照を取得する|

### コンテナの変更
|名前|説明|
|-|-|
|[`fill`](#fill)|現在使用されている領域をすべて与えられた値に変更する|
|[`push_back`](#push_back)|末尾に要素を追加する|
|[`emplace_back`](#emplace_back)|末尾に要素を直接構築で追加する|
|[`pop_back`](#pop_back)|末尾の要素を削除する|
|[`swap`](#swap)|コンテナの交換|
|[`clear`](#clear)|全要素削除|

## メンバ型
|名前|説明|
|-|-|
|`value_type`|`T`|
|`reference`|`T&`|
|`const_reference`|`const T&`|
|`pointer`|`T*`|
|`const_pointer`|`const T*`|
|`size_type`|`size_t`|
|`difference_type`|`ptrdiff_t`|
|`iterator`|ランダムアクセスイテレータという名の`T*`|
|`const_iterator`|`const T*`|
|`reverse_iterator`|`std::reverse_iterator<iterator>`|
|`const_reverse_iterator`|`const reverse_iterator`|

## 非メンバ関数
|名前|説明|
|-|-|
|[`max_size`](#max_size)|格納可能な最大の要素数を取得する|
|[`operator==`](#operator==)|等値比較|
|[`operator<=>`](#operator<=>)|三方比較|

## 構築・破棄
### <a id="constructor">コンストラクタ</a>
```C++
constexpr bucket()noexcept;       // (1)

constexpr bucket(bucket&&x);      // (2)

constexpr bucket(const bucket&x); // (3)

constexpr
bucket(size_t size,const T& x=T())noexcept(false);        // (4)

explicit constexpr
bucket(std::initializer_list<T>init)noexcept(false);  // (5)

template<class InputIterator>
constexpr
bucket(InputIterator first, InputIterator last)noexcept(false); // (6)
```
1. サイズ $0$ . デフォルト構築しないので $O(1)$ ．未使用領域へのアクセスは未定義動作なので使うときは配置`new`して使って，壊れても諦めるように．
1. `x.size()` 回の `move` を行うため，`T` の `move` の計算量を $O(\sigma)$ ， $n=$ `x.size()` として $O(\sigma n)$ .
1. `x.size()` 回のコピーを行うため，以下同文．
1. `size` 個の $x$ の `bucket` を構築． $\mathrm{size}>N$ のときエラー．
1. 初期化子リストから構築．コピー．
1. イテレータから構築．`std::random_access_iterator`のときサイズオーバーを短絡評価．そうでないときは入れれるだけ入れてみて余ってたらエラー．

### <a id="destructor">デストラクタ</a>
```C++
constexpr ~bucket();
```
すべての要素を破棄する．未使用領域に対してデストラクタは呼び出さない．

### <a id="operator=">代入演算子</a>
```C++
constexpr bucket& operator=(bucket&&x);
constexpr bucket& operator=(const bucket&&x);
constexpr bucket& operator=(std::initializer_list<value_type>x)noexcept(false);
```
コピー，ムーブ，初期化子リスト．

## イテレータ
### <a id="begin">begin</a>
```C++
constexpr iterator begin()noexcept;
constexpr const_iterator begin()const noexcept;
```
`const`のとき`const`になる．以下のメンバ関数でも同文．
### <a id="end">end</a>
```C++
constexpr iterator end()noexcept;
constexpr const_iterator end()const noexcept;
```
### <a id="cbegin">cbegin</a>
```C++
constexpr const_iterator cbegin()const noexcept;
```
### <a id="cend">cend</a>
```C++
constexpr const_iterator cend()const noexcept;
```
### <a id="rbegin">rbegin</a>
```C++
constexpr reverse_iterator rbegin()noexcept;
constexpr const_reverse_iterator rbegin()const noexcept;
```
逆イテレータ．
### <a id="rend">rend</a>
```C++
constexpr const_reverse_iterator crbegin()const noexcept;
```
### <a id="crbegin">crbegin</a>
```C++
constexpr reverse_iterator rend()noexcept;
constexpr const_reverse_iterator rend()const noexcept;
```
### <a id="crend">crend</a>
```C++
constexpr const_reverse_iterator crend()const noexcept;
```

## 領域
### <a id="size">size</a>
```C++
constexpr size_type size()const noexcept;
```
コンテナの要素数を定数時間で取得する．

### <a id="resize">resize</a>
```C++
constexpr void resize(size_type size,const_reference x=T())noexcept(false);  
```
コンテナの要素数を変更する．  
`size>N`のとき`std::out_of_range`を投げる．  
第二引数はサイズが拡張されるときに代入される値．

### <a id="capacity">capacity</a>
```C++
constexpr size_type capacity()const noexcept;
```
`N`から現在のコンテナのサイズを引いた数を返す．  
`bucket<T,N>::max_size()-obj.size()`と同じ．  

### <a id="empty">empty</a>
```C++
constexpr bool empty()const noexcept;
```
コンテナが空かどうか調べる．

## 要素アクセス
### <a id="operator[]">operator[]</a>
```C++
constexpr reference operator[](size_type idx);
constexpr const_reference operator[](size_type idx)const;
```
境界値チェック無し．

### <a id="at">at</a>
```C++
constexpr reference at(size_type idx)noexcept(false);
constexpr const_reference at(size_type idx)const noexcept(false);
```
範囲外参照で`std::out_of_range`を投げる．

### <a id="data">data</a>
```C++
constexpr pointer data()noexcept;
constexpr const_pointer data()const noexcept;
```
`const`のときは`const`に以下略．

### <a id="front">front</a>
```C++
constexpr reference front()noexcept(false);
constexpr const_reference front()const noexcept(false);
```
`const`のときは`constに……  
コンテナが空のときは未定義動作．

### <a id="back">back</a>
```C++
constexpr reference back()noexcept(false);
constexpr const_reference back()const noexcept(false);
```
`const`のときは`constに……  
コンテナが空のときは未定義動作．

## コンテナの変更
### <a id="fill">fill</a>
```C++
constexpr void fill(const_reference x);
```
現在使用されている領域を $x$ に変更する．  

### <a id="push_back">push_back</a>
```C++
constexpr void push_back(const_reference x)noexcept(false);
constexpr void push_back(value_type&&x)noexcept(false);
```
`emplace_back`のラッパー

### <a id="emplace_back">emplace_back</a>
```C++
template<class... Args>
constexpr reference emplace_back(Args&&... args)noexcept(false);
```
サイズオーバーで`std::out_of_range`

### <a id="pop_back">pop_back</a>
```C++
constexpr void pop_back()noexcept(false);
```
空のときに呼び出すと`std::range_error`

### <a id="swap">swap</a>
```C++
constexpr void swap(bucket&x);
```
コンテナを交換する．

### <a id="clear">clear</a>
```C++
constexpr void clear();
```
コンテナのすべての要素のデストラクタを呼び出す．

## 非メンバ関数
### <a id="max_size">max_size</a>
```C++
constexpr static size_type max_size()const noexcept;
```
`bucket<T,N>`の`N`を返す．
`bucket<T,N>::max_size()`か，`obj.max_size()`で呼び出す．

### <a id="operator==">operator==</a>
```C++
constexpr friend bool operator==(const bucket&lhs,const bucket&rhs);
```
サイズ違いで即座に`false`を返し，それ以外は要素が異なると短絡評価

### <a id="operator<=>">operator<=></a>
```C++
constexpr friend auto operator<=>(const bucket&lhs,const bucket&rhs);
```
これによって`operator<`, `operator<=`, `operator>`, `operator>=`が自動定義される．
