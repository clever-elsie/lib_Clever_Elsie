# bitset
サイズをコンストラクタで実行時に指定できる． `std::bitset<N>`は`N`がコンパイル時定数である必要があるため，適宜使い分けるべき．

サイズが異なる`bitset`同士の演算は`assert`落ち．
サイズは`.resize()`で変更可能．

`elsie::bitset`なので，`using namespace std;`と`using namespace elsie;`は同時にしてはいけない．

## コンストラクタ/operator=
```C++
bitset()=default;
bitset(const bitset&rhs);
bitset(bitset&&rhs);
bitset(size_t n, bool val=0);
bitset(const std::string&s);
bitset& operator=(const bitset&rhs);
bitset& operator=(bitset&&rhs);
```
サイズ0の`bitset`は内部的に特別な扱いをするが，利用する上では考慮する必要はない．  
`const std::string&`を受け取るとき，その文字列の要素は半角の0,1のいずれかでなければならない．

## データ領域
```C++
void resize(size_t n, bool x=false);
size_t size()const;
```
`resize()`の第2引数は要素数が増えるときに，その増える部分の初期値であり，元々ある要素の中身が書き変わることはない．

`size()`は`bitset`のアクセス可能な要素数を返す．

## 要素アクセス
```C++
size_t count()const;
bool test(size_t pos)const;
bool all()const;
bool any()const;
bool none()const;
reference operator[](size_t pos);
```
`count()`は`bitset`に含まれる $1$ の数を返す．

`test()`は`pos`が $1$ のとき`true`を返す．

`all()`はすべての要素が $1$ のときに`true`を返す．
`size()==0`のとき`true`を返す．

`any()`は $1$ が一つでもあれば`true`を返す．
`size()==0`のとき`false`を返す．

`none()`はすべての要素が $0$ のときに`true`を返す．
`size()==0`のとき`true`を返す．

`operator[]`は要素の変更が行えるようにラッパーする`bitset::reference`型を返す．  
`reference`はビット演算，代入演算，これらの複合代入演算，`operator bool()`，`operator!()`，`flip()`が使える．  
`reference`を取った後に`resize()`を行い，再度その`reference`にアクセスしたときの動作はUB.


## 変換
```C++
std::string to_string()const;
uint64_t to_u64()const;
```

## 要素の変更
```C++
bitset&set();
bitset&set(size_t pos, bool val=true);
bitset&reset();
bitset&reset(size_t pos);
bitset&flip();
bitset&flip(size_t pos);
```
引数無しはすべての要素に対して作用する．

`set()`は基本的に`pos`を $1$ にするものだが，一応 $0$ にもできる．

`reset()`は`pos`を $0$ にする．

`flip()`は`pos`をビット反転する．

## 演算
```C++
bitset&operator&=(const bitset&rhs);
bitset&operator|=(const bitset&rhs);
bitset&operator^=(const bitset&rhs);
bitset&operator<<=(const bitset&rhs);
bitset&operator>>=(const bitset&rhs);

friend bitset&operator&(const bitset&rhs);
friend bitset&operator|(const bitset&rhs);
friend bitset&operator^(const bitset&rhs);
friend bitset&operator<<(const bitset&rhs);
friend bitset&operator>>(const bitset&rhs);
```
演算は，長さが同じ`bitset`についてビット演算を行う．

シフト演算はまだ高速化の余地があるため，あまり利用しないほうがよい．
