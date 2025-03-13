# Z-algorithm
```C++
template<class T>
vector<uint32_t>z_algorithm(const T&s);
```
`T`は乱択可能なコンテナ．  
`s[i:s.size()]`と`s`のLCPの長さを持つ配列を返す．