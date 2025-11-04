# test_tools for lib_Clever_Elsie
テストはhppファイルと拡張子を除いて同名のディレクトリに配置する．  
`dtStrc/binary_tree/rbtree.hpp`のテストは`test/dtStrc/binary_tree/rbtree.hpp/`に配置する．  
テストは以下の要素からなる．
|要素|説明|
|-|-|
|`test.cpp`|コンパイルと動作の完全性のテスト|
|`spec.cpp`|性能テスト．`test.cpp`が通った後に行う．|
|`in/`|`spec.cpp`の入力|
|`out/`|`spec.cpp`の出力|
|`tin/`|`test.cpp`の入力|
|`tout/`|`test.cpp`の出力|
|`meta.txt`|最後に実行した日を記録する|

`test/test_tools`はディレクトリの状況と最後にテストしたときの情報を比較して更新があったもの，新たに追加されたテストのみを実行する．  
テストツール内に挿入する相対パスは，`test/test_tools/build/`からの相対アドレスを指定することに注意．