# convolution
`convolution_fast`が性質の良い法を指定するもの．  
`convolution`は性質の良い法のときはそのまま畳み込みを行い，そうでないときにGarner法で復元する指定回数の畳み込みを行う．  
`convolution`では $1$ 以上 $11$ 以下の畳み込み回数を指定できる．[精度はこちら](#precision)．
```C++
convolution_fast<T,M>();
vector<T>prod<is_Zmod=true>(vector<T>,vector<T>);
vector<T>prod_amove<is_Zmod=true>(vector<T>&&,vector<T>);
vector<T>prod_bmove<is_Zmod=true>(vector<T>,vector<T>&&);
vector<T>prod_move<is_Zmod=true>(vector<T>&&,vector<T>&&);
```
`move`のオーバーロードは同名だと`ambiguous`なのですべて別名．  
`prod*()`では，配列のすべての要素が $[0,M)$ であるときに最初の剰余演算を省く．  
`is_Zmod`はデフォルトで`true`で剰余演算を省略する．

```C++
convolution<T,M,psz=2>();
vector<T>prod<is_Zmod=false>(const vector<T>&,const vector<T>&);
```
畳み込む配列の整数型`T`と法とする $M$ を与える．  
`psz`は法の数．法の数と精度・`is_Zmod`の基準は下記の表を参照．  
`is_Zmod`を`true`としてよい基準は`psz`の増加に従って厳しくなる．  
デフォルトで`false`なので普段は注意する必要はない．

原始根は存在するなら勝手に見つけるので必要ない．  
計算量は配列のサイズ $l$ が $l=\min\{2^i\ |\ 2^i\ge n+m+1,\ i\in\mathbb{N}\}$ となり， $O(l\lg l)$.  
もちろん`convolution`では`psz`にも比例する．

## <a id="precision">精度</a>
理論上以下の精度で復元できるが，もちろん`garner()`はそんな精度ではない．
以下の精度-1
|psz|精度|`is_Zmod`の基準|
|-|-|-|
|$1$|$1.67\times 10^{8}||
|$2$|$7.78\times 10^{16}||
|$3$|$8.72\times 10^{25}$||
|$4$|$1.49\times 10^{35}$||
|$5$|$2.70\times 10^{44}$||
|$6$|$5.44\times 10^{53}$||
|$7$|$1.15\times 10^{63}$||
|$8$|$2.62\times 10^{72}$||
|$9$|$6.52\times 10^{81}$||
|$10$|$1.77\times 10^{91}$||
|$11$|$5.11\times 10^{100}$||