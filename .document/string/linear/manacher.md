# 奇数長の回文半径
```C++
template<class T>
vector<uint32_t>manacher(const T&v);
```
`T`は`operator[]`が $O(1)$ でないといけない．  
この関数によって，v[i]を中心とする奇数長の回文の最長の長さが求まる．  
線形時間

# 回文の長さ
```C++
template<class T>
vector<pair<uint32_t,uint32_t>>
LongestPalindrome(const vector<T>&v);
```
戻り値は(奇数長の回文の長さ，偶数長の回文の長さ)の`vector`を返す．  
偶数長の回文の長さが前後のどちらかと一致するとき，iはそちら側ではないほうの中心になる．つまりi-1の偶数長と一致するならば，iは右側の中心で，そうでないときは左側の中心である．