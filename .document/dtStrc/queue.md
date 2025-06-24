# Queue
QueueはFIFO(First in First out)のデータ構造であるが，全順序集合のキーに対して小さい(または大きい)順に取り出すような優先度付きキューも作ることができる．  
優先度付きキューは通常min/maxのいずれか一方を取り出すことができるが，インデックスの奇数をmaxに，偶数をminに割り当てると，実インデックスを2で割った値が0-indexedな優先度付きキューになっている．
それぞれのキューは葉ノードで実インデックスの最下位ビットをフリップすることで行き来でき，これにより1つの配列で両端優先度付きキューを実現できる．  
なお，2つの配列を使ったりポインタで木を作ったりすると，このような複雑なことを考えなくとも簡素にそこそこ遅い両端キューは作ることができる．

## double-ended priority queue
### 構築/代入
```C++
template<class T>
class depq<T>;

depq()=default;
depq(depq<T>&&)=default;
depq(const depq<T>&&)=default;
depq(std::vector<T>&&dat);
depq(const std::vector<T>&dat);
template<class Iterator>
  requires requires(Iterator x){ ++x; x!=x; }
  depq(Iterator first, Iterator last);
depq& operator=(depq<T>&&)=default;
depq& operator=(const depq<T>&&)=default;
```
コンストラクタと代入演算子は`default`をそのまま用いる．  
`vector`やイテレータから構築するとき， $O(N)$ なので，事前に入れる要素が決まっている場合はこちらを使うべき．

### 領域
```C++
size_t size()const noexcept;
bool empty()const noexcept;
void reserve(size_t n);
void clear();
```
内部の`vector`の同名関数を外部に公開する．

### 追加
```C++
template<class... Args>
void emplace(Args&&... args);
void push(T&&x);
void push(const T&x);
```
$O(\lg N)$

### 要素取得/削除
```C++
const T& min()const noexcept;
const T& max()const noexcept;
T pop_min()noexcept;
T pop_max()noexcept;
```
いずれも例外を投げないが，空のコンテナに対してこの操作を行うと`assert`落ちする．

$O(\lg N)$