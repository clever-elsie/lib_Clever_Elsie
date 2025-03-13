# kmp
kmp法により $O(S+T)$で`T`が現れる先頭位置を全列挙。
## コンストラクタ
```C++
kmp();                                  // 1
kmp(string&&SRC);                       // 2
kmp(const string&SRC);                  // 3
kmp(string&&SRC,string&&tar);           // 4
kmp(string&&SRC,const string&tar);      // 5
kmp(const string&SRC,string&&tar);      // 6
kmp(const string&SRC,const string&tar); // 7
```
1. 空文字列
1. sourceとなる文字列をmove
1. sourceとなる文字列をcopy
1. sourceをmove, tarをmove
1. sourceをmove, tarをcopy
1. sourceをcopy, tarをmove
1. sourceをcopy, tarをcopy

## set
```C++
void set(string&&SRC);
void set(const string&SRC);
```
sourceとなる文字列を設定.

## build
```C++
void build(string&&tar);
void build(const string&tar);
```
$O(|tar|)$ で遷移テーブルを構築．

## 検索
```C++
vector<ssize_t>find()
```
sourceに現れるtarの先頭0-based indexを返す．

# 遷移テーブル構築するための関数
```C++
vector<ssize_t>morris_pratt(const string&tar);
vector<ssize_t>knuth_morris_pratt(const string&tar);
```
`morris_pratt()`は最長ボーダーを求める．  
`knuth_morris_pratt()`は文字列検索するときの最長ボーダーよりも高速なスキップテーブルを求める．  
いずれも線形時間.
