## basic_math
`gcd,lcm,egcd,exgcd,modinv`の計算量はラメの定理より $O(5\min(a,b))$.  
- `gcd(a,b)` : 最大公約数  
- `lcm(a,b)` : 最小公倍数  
- `egcd(a,b,x,y)` : 再帰拡張ユークリッドの互除法 $ax+by=\text{GCD}(a,b)$  
- `exgcd(a,b,x,y)` : 非再帰拡張ユークリッドの互除法  
- `mod_inv(a,m)` : $m$ を法とする逆元を求める．
- `mod_inv_prime(a,p)` : 素数 $p$ を法とする逆元を求める． $O(\lg p)$
- `safepow(a,b)` オーバーフロー防止。オーバーフロー時INT64_MAX $O(\lg b)$
- `modpow(a,b,m)` : $x\equiv a^b \pmod m$となる $x$ を求める． $O(\lg b)$

### combination
- `combination<modint>(max)`のコンストラクタで，`modint`型と $N$ の最大値を渡す．両方省略可．デフォルトでは`atcoder/modint998244353`.
- $P,C,H$, `factorial`, `invfact`で順列，組合せ，重複組合せ，階乗，逆数の階乗(?)を求めることができる．
- 前計算で最大値を渡していれば計算量は $O(1)$．
- そうでない場合は， $N$ が更新される度に追加される要素数分だけ計算が必要になり，逆数の階乗(?)の計算は線形時間
- 前計算は $O(N)$．
