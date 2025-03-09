# range_frequency
## range_frequency(動的)
### コンストラクタ $O(N\lg N)$
```C++
range_frequency<S>();                  // 1
range_frequency<S>(int32_t n,const S&v);         // 2
range_frequency<S>(const vector<S>&v); // 3
```
内部的には可乱択二分木を使っているため配列のサイズを超過した $[l,r)$を使える．  
1. 空のクラスを作成
1. 要素数 $n$ の $v$ 埋めされた配列で構築
1. 配列 $v$ で構築．

### 一点更新 $O(\lg N)$
```C++
void set(int32_t idx,const S&v);
```
配列`va[idx]=v`で更新．

### 区間カウント $O(\lg N)$
```C++
int32_t cnt(int32_t r,const S&x)const;
int32_t cnt(int32_t l,int32_t r,const S&x)const;
```
区間 $[0,r)$ で $x$ の数を数えるものと，区間 $[l,r)$ で数えるインタフェースを提供する．  
上の関数を使うことで下の関数は簡単に作れる．

## range_frequency_static
### コンストラクタ
```C++
range_frequency_static<S>();
range_frequency_static<S>(const vector<S>&v);
```
### 区間カウント
```C++
int32_t cnt(int32_t r,const S&x)const;
int32_t cnt(int32_t l,int32_t r,const S&x)const;
```