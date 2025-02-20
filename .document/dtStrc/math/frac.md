### frac (分数)
分母分子が`unsigned long long`で管理される符号付き分数。  
比較と四則演算と代入演算子がオーバーロードされている。
整数値は分母省略可。
```C++
frac(); // 0/1
frac(bool,ull,ull=1ULL); // 符号、分子、分母
frac(ull,ull=1ULL); // 分子、分母
frac(ll,ll=1LL); // 分子、分母
frac(frac&), frac(frac*); // 参照、ポインタ

f.approx(); // 任意のタイミングで約分
//f+-*/=g // 四則演算では内部的に約分
```