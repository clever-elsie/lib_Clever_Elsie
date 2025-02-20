## rolling hash
**遅い**  

`rollingHash<n>(const string&)` コンストラクタで検索元の文字列を設定。 nはロリハの本数

`dynamicRollingHash<n>(const string&)` セグ木版。文字変更機能あり。

`.find(const string&)` 対象の文字列を検索する。

`.same(int,int,int,int)` 区間の文字列一致判定 $[L1,R1), [L2,R2)$

`.set(int idx,char c)` idxの文字をcに変更する。セグ木版限定。

