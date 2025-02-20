## rbtree
`set,map,multiset,multimap`が可乱択で！！  
`.find_by_order(idx)`で順序参照．戻り値はイテレータ．  
`.order_of_key(key)`で`lower_bound(key)`のインデックスが手に入る．
それ以外は大体std::setなどと同じだが，`swap, merge`などはない．  
競プロライブラリによくある`split`などもない．  

そこになければ有りません．あるのは乱択機能だけ  
使うなら`__gnu_pbds::tree`のほうがいいですよ．