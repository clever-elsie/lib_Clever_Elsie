# データ構造
##  <a href="./unionFind.md">unionFind</a>
素集合データ構造
* unionFind
* unionFindP (ポテンシャル付き)
* dsu (可換モノイド)
## <a href="./segtree/segtree.md">BIT・セグ木・遅延セグ木</a>
セグ木上の二分探索が無いので，`atcoder::segtree`などを使うと良いとされている．
* BIT (Binary Indexed Tree)
* segtree
* lazy_segtree
## <a href="./range_tree/range_tree.md">区間計数木</a>
ある区間 $[l,r)$ に含まれる要素の数を数えたりなどができる．
* merge_sort_tree
* range_frequency (可乱択二分木)
* static_range_frequency (二分探索)
## <a href="./binary_tree/binary_tree.md">二分木</a>
任意位置に挿入可能なsplay木とただの二分木．  
rbtreeは`set,map,multiset,multimap`に対応．
* rbtree_tree
* splay_tree