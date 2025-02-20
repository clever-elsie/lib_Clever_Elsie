## kmp
kmp法により $O(S+T)$で`T`が現れる先頭位置を全列挙。

`kmp(const string&)`: Sを登録。  
`kmp(const string&,const string&)`: Sを登録し、Tの移動テーブル作成  
`void make_match(const string&tar)`: Tの移動テーブルを作成  
`vector<ssize_t>find()`: Tの位置を全列挙.  

