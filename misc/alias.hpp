#ifndef ELSIE_ALIAS
#define ELSIE_ALIAS

template<class f>using vc=vector<f>;
template<class f>using vv=vc<vc<f>>;
template<class f>using v3=vv<vc<f>>;
template<class f>using v4=vv<vv<f>>;

template<class f>using gr=greater<f>;
template<class f>using pq=priority_queue<f>;
template<class f>using pqg=priority_queue<f, vc<f>, gr<f>>;
#define uset unordered_set
#define umap unordered_map
using it=int32_t;
using i8=int8_t;   using u8=uint8_t;
using i16=int16_t; using u16=uint16_t;
using i32=int32_t; using u32=uint32_t;
using i64=int64_t; using u64=uint64_t;
using i128=__int128_t; using u128=__uint128_t;
using intw=__int128_t; using uintw=__uint128_t;
using f32=float;
using f64=double;
using f128=__float128;
using vi=vc<i64>;
using vit=vc<it>;
using vb=vc<bool>;
using pi=pair<i64,i64>;
using pit=pair<it,it>;
using str=string;
using vs=vc<str>;
using pqgp=pqg<pi>;
#define int i64
#define itn i64
#define LL long long
#endif