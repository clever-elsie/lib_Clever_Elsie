# rbtree
`set`, `map`, `multiset`, `multimap` が可乱択で！！  
赤黒木は2-3木の二分木への自然な変換らしいですよ．知らんけど．  

## 目次
ちょっと記述が多いのでメンバ関数とインナークラスを以下に表にする．

### 構築・破棄
|名前|説明|
|-|-|
|[constructor](#constructor)</a>|コンストラクタ|
|[destructor](#destructor)</a>|デストラクタ|
|[operator=](#operator=)</a>|代入演算子|
### イテレータ
|名前|説明|
|-|-|
|[iterator](#iterator)|イテレータクラス|
|[reverse_iterator](#reverse_iterator)|イテレータが逆向きに動くだけ|
|[begin](#begin)|最小要素を $O(\lg N)$ で取得|
|[end](#end)|nil番兵 $O(1)$|
|[rbegin](#rbegin)|最大要素|
|[rend](#rend)|nil番兵|
### 領域
|名前|説明|
|-|-|
|[empty](#empty)|コンテナが空であるかどうかを調べる|
|[size](#size)|要素数を取得する|
### 変更
|名前|説明|
|-|-|
|[clear](#clear)|すべての要素を削除する|
|[reset](#reset)|すべてのメモリを解放する|
|[insert](#insert)|要素を挿入する|
|[erase](#erase)|要素を削除する|
### アクセス
|名前|説明|
|-|-|
|[count](#count)|指定したキーと同じキーを持つ要素の数を数える|
|[find](#find)|指定したキーと同じ要素を探す|
|[contains](#contains)|指定したキーを持つ要素があるかどうか調べる|
|[lower_bound](#lower_bound)|指定したキー以上の最小のキーを持つ要素を返す|
|[upper_bound](#upper_bound)|指定したキーより大きい最小のキーを持つ要素を返す|
|[find_by_order](#find_by_order)|順序を指定して要素にアクセスする|
|[order_of_key](#order_of_key)|指定したキーの順序を返す|


## <a id="constructor">コンストラクタ</a>
```C++
rbtree();               // 空のクラスを作成
rbtree(rbtree&&);       // move
rbtree(const rbtree&);  // copy
rbtree(initializer_list<key_t>);       // keyだけで構築．setで使う．
rbtree(initializer_list<key_t,val_t>); // 構築
```
move や copy では [operator=](#operator=) を内部的に用いる．

## <a id="destructor">デストラクタ</a>
```C++
~rbtree();
```
内部的に [reset](#reset) を呼び出すだけ．  
計算量は $O(N)$.

## <a id="operator=">代入演算子</a>
```C++
rbtree&operator=(rbtree&&);      // move
rbtree&operator=(const rbtree&); // copy
```
move は元のクラスの中身を移し替えるが，元々持っていた要素はすべて解法するため計算量は $O(N)$.  
copy も当然 $O(N)$.

## <a id="iterator">iterator</a>
`iterator` は以下のメンバ関数を持つ `rbtree` のインナークラスである．
```C++
iterator(const rbtree*,node*); // nodeはprotectedなので使用不可
iterator&operator=(const iterator&);   // コピー
bool operator==(const iterator&)const; // 参照するノードが同じときtrue
bool operator!=(const iterator&)const; // 参照するノードが違うときtrue
iterator&operator++();         // ++itr
iterator&operator--();         // --itr
iterator operator++(int32_t);  // itr++
iterator operator--(int32_t);  // itr--
const key_t&operator*();       // set, multiset
value_type&operator*();        // map, multimap
const key_t&f()const;          // first
val_t&s();                     // second
```
内部的には `node` へのポインタと所属する木へのポインタを持っている．  
`operator++` や `operator--` で全部移動すると移動量は $2N$ 程度になるため $O(N)$.  
`operator++` や `operator--` の1回あたりの最大移動量は $2\lg N$ 程度だが，償却で $O(1)$.  

イテレータは循環するため，`++nil`なら最小値に移動し，`--nil`なら最大値に移動する．

## <a id="reverse_iterator">reverse_iterator</a>
`reverse_iterator` は [`iterator`](#iterator) を継承して，移動方向が逆になっているもの．

## <a id="begin">begin</a>
```C++
iterator begin()const;
```
$O(\lg N)$ で最小要素の[イテレータ](#iterator)を返す．

## <a id="end">end</a>
```C++
iterator end()const;
```
$O(1)$ でnil番兵の[イテレータ](#iterator)を返す．

## <a id="rbegin">rbegin</a>
```C++
reverse_iterator rbegin()const;
```
$O(\lg N)$ で最大要素の[逆イテレータ](#reverse_iterator)を返す．

## <a id="rend">rend</a>
```C++
reverse_iterator rend()const;
```
$O(1)$ でnil番兵の[逆イテレータ](#reverse_iterator)を返す．

## <a id="empty">empty</a>
```C++
bool empty()const;
```
木が要素を持たないときに `true` を返す．  
$O(1)$.

## <a id="size">size</a>
```C++
size_t size()const;
```
木が持つ要素数を返す．  
$O(1)$.

## <a id="clear">clear</a>
```C++
void clear();
```
木の要素をすべて `free` する．
$O(N)$.  
`std::move`などでデストラクタが呼ばれたとき，内部的に`nil`ノードが削除され使用時に`nullptr`を参照する可能性が高い．
このとき，再使用前に`clear()`を呼び出すことで`nil`ノードを再確保し，コンテナを再利用可能になる．

## <a id="insert">insert</a>
```C++
iterator insert(value_type&&);
iterator insert(const value_type&);
iterator insert(const key_t& ,const val_t& );
iterator insert(const key_t& ,      val_t&&);
iterator insert(      key_t&&,const val_t& );
iterator insert(      key_t&&,      val_t&&);
// set, multiset
iterator insert(const key_t& );
iterator insert(      key_t&&);
```
$O(\lg N)$ で要素を挿入する．  
move するために場合分けが沢山あるが，使う上では気にしなくていい．  
`std::map` の `insert` と異なり，`key_t`, `val_t` を分けて与えることができる．  
組込み型なら `emplace` に近い感じ．  
しかしこの `rbtree` に `emplace` は無い．

## <a id="erase">erase</a>
```C++
iterator erase(iterator&);
iterator erase(iterator&&);
iterator erase(key_t&&);
iterator erase(const key_t&);
```
イテレータを渡してピンポイントで消すか，キーを指定してその最も古い一つを消すかできる．
キーを渡すとき，`multiset`, `multimap` のときは最も古いものを消すが，`set`, `map` のときはただ一つの要素を消す．  
存在しないキーを与えたときは何もせず，nilへのイテレータを返す．  
要素を削除したときは，削除した要素の次の要素へのイテレータを返す．  
消した要素が最大のときはnilへのイテレータを返す．  
[`reverse_iterator`](#reverse_iterator)を与えたときは，削除したイテレータの1つ前のイテレータが返る．
`reverse_iterator`を与えても戻り値は`iterator`なので`auto`を使うべきではない．再代入なら気にしなくていいはず．

## <a id="count">count</a>
```C++
size_t count(key_t&&)const;
size_t count(const key_t&)const;
```
キーの数を数える．  
`set`, `map`では`contains`と同じ．
`multimap`, `multiset` では，内部的に [`order_of_key`](#order_of_key)を用いる．  
いずれも計算量は $O(\lg N)$.

## <a id="find">find</a>
```C++
iterator find(key_t&&)const;
iterator find(const key_t&)const;
```
キーを探索する．ただし，`multiset`, `multimap`では最も古い要素を返す．  
計算量は $O(\lg N)$.

## <a id="contains">contains</a>
```C++
bool contains(key_t&&)const;
bool contains(const key_t&)const;
```
木がキーを持っていれば `true` を返す．  
$O(\lg N)$.

## <a id="lower_bound">lower_bound</a>
```C++
iterator lower_bound(key_t&&)const;
iterator lower_bound(const key_t&)const;
```
キー以上の最小の要素を返す．  
$O(\lg N)$.

## <a id="upper_bound">upper_bound</a>
```C++
iterator upper_bound(key_t&&)const;
iterator upper_bound(const key_t&)const;
```
キーより大きいの最小の要素を返す．  
$O(\lg N)$.

## <a id="find_by_order">find_by_order</a>
```C++
iterator find_by_order(int64_t)const;
```
0-indexedな昇順乱択ができる．  
負の数を入れると後ろから1-indexedなアクセスができる．  
つまりｐｙてょｎ．
計算量はもちろん $O(\lg N)$.  
範囲外を参照したらnilが返る．

## <a id="order_of_key">order_of_key</a>
```C++
size_t order_of_key(key_t&&)const;
size_t order_of_key(const key_t&)const;
```
0-indexedな昇順の番号を返す．  
キーが存在しないときはキー以上の最も小さい要素の番号を返す．  
`lower_bound` が`nil`なら返ってくるのは`size()`と同じ．
