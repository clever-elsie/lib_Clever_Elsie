### segtreeとlazy_segtreeのスニペット
```C++
(sum|min|max)_seg<S>
(add|ch)_(sum|min|max)_lst<S,F>
range_affine_range_sum_lst<S,F>
```
セグ木は区間和，区間最小，区間最大．  
遅延セグ木は区間変更と区間加算に区間和，区間最小，区間最大．
さらに，区間への線形変換とその和を得る`range_affine_range_sum`を一応．