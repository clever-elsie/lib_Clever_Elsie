# 接尾辞配列
## メンバ変数
- `sa`: suffix_array  
- `v` : 元の配列
## コンストラクタ
```C++
suffix_array<T>();
suffix_array<T>(const T&);
suffix_array<T>(T&&);
```
`T`はIterableな型．乱択必須．  
型`T`の配列をコピーかmoveして構築するか，空のクラスを作って後で`set()`して構築するか．

## set
```C++
void set(const T&);
void set(T&&);
```
配列を与えて構築． $O(n)$

## 検索
```C++
size_t lower_bound(const T&t);
size_t upper_bound(const T&t);
```
検索対象の文字列`t`がsuffix_array上で何番目にあるかを探索する．  
`lower_bound()`は辞書順で`t`以上，`upper_bound()`は`t`より真に大きい．  
したがって，`v`に含まれる`v`と一致する部分列の数は，`upper_bound()-lower_bound()`