# kmp
kmp法により $O(S+T)$で`T`が現れる先頭位置を全列挙。
## コンストラクタ
```C++
kmp(const string&SRC); // 1
kmp(const string&SRC,const string&tar); // 2
```
1. sourceとなる文字列をコピーするだけ
1. ついでに検索対象の文字列の遷移テーブルを構築
## 遷移テーブル構築
```C++
void make_math(const string&tar);
```
$O(|tar|)$ で遷移テーブルを構築．

## 検索
```C++
vector<ssize_t>find()
```
sourceに現れるtarの先頭0-based indexを返す．

