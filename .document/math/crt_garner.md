# crt garner
中国剰余定理とGarner法のライブラリ．  
`decomposite_garner`では，法が素数でない場合にも頑張ってくれる．

## CRT(中国剰余定理) $O(n\lg \max(m))$
```C++
template<integral T>
pair<T,T> crt(const vector<T>&b, const vector<T>&m);
```
$|b|=|m|, n=|b|$ とする．  
$0\le i < n$ を満たす任意の $i$ について，
$x\equiv b_i \bmod m_i$ を満たす $x$ が存在するとき，その $x$ を返す．  
$m_i$ はすべて互いに素である必要がある．

戻り値:  
$x$ が存在するとき， $z=\text{LCM}(m)$ として $(x\bmod z,z)$ を返す．  
$x$ が存在しないとき， $(0,0)$ を返す．  
$n=0$ のとき， $(0,1)$ を返す．

## Garner法 $O(k^2+k\lg\max(m))$
```C++
template<integral T>
T garner(vector<T>&b,vector<T>&m,T mod);
```
条件と求めるものは`crt()`と同じである．  
戻り値は $x \bmod mod$ で， $z$ は存在しない．

```C++
template<integral T>
T decomposite_garner(vector<T>&b,vector<T>&m,T mod);
```
法が互いに素ではないときに，いい感じに情報を分解して`garner()`にぶち込む関数．  

戻り値:  
$b$ のすべての要素が $0$ のとき， $\text{LCM}(m)$ を返す．  
復元不能なとき， $-1$ を返す．  
復元可能なとき， $x\bmod mod$ を返す．  
