#ifndef ELSIE_MISC_FUNC
#define ELSIE_MISC_FUNC

namespace vies=std::views;
#define DR(i) views::drop(i)
#define TK(i) views::take(i)
#define RV views::reverse
#define IOTA vies::iota

#define rev(a) reverse(A(a))
#define minel(a) min_element(A(a))
#define maxel(a) max_element(A(a))
#define acm(a) accumulate(A(a),0ll)
#define nxpm(a) next_permutation(A(a))
#define Sort(a) sort(A(a))
#define uni(a) Sort(a);a.erase(unique(A(a)),a.end())
#define swapcase(a) a=(isalpha(a)?a^32:a)

template<class T,class U>inline void chmax(T&a,const U&b){if(a<b)a=b;}
template<class T,class U>inline void chmin(T&a,const U&b){if(a>b)a=b;}


#define _BISECT4(_1,_2,_3,_4,name,...) name
#define _LB_BEX(b,e,x) lower_bound(b,e,x)
#define _LB_BEXG(b,e,x,g) lower_bound(b,e,x,g)
#define _UB_BEX(b,e,x) upper_bound(b,e,x)
#define _UB_BEXG(b,e,x,g) upper_bound(b,e,x,g)
#define lb(...) _BISECT4(__VA_ARGS__,_LB_BEXG,_LB_BEX)(__VA_ARGS__)
#define ub(...) _BISECT4(__VA_ARGS__,_UB_BEXG,_UB_BEX)(__VA_ARGS__)

#define TP template<class T,class U,typename cp=less<U>>
template<class T,class U>concept LUBI= same_as<T,vector<U>>||same_as<T,deque<U>>||is_array_v<T>;
#define RL requires LUBI<T,U>

TP size_t lbi(const T&v,const U&x,cp cmp=cp())RL{RET lb(A(v),x,cmp)-begin(v);}
TP size_t ubi(const T&v,const U&x,cp cmp=cp())RL{RET ub(A(v),x,cmp)-begin(v);}
TP size_t lbi(size_t i,const T&v,const U&x,cp cmp=cp())RL{RET lb(i+A(v),x,cmp)-begin(v);}
TP size_t ubi(size_t i,const T&v,const U&x,cp cmp=cp())RL{RET ub(i+A(v),x,cmp)-begin(v);}
TP size_t lbi(const T&v,size_t i,const U&x,cp cmp=cp())RL{RET lb(I(v,i),x,cmp)-begin(v);}
TP size_t ubi(const T&v,size_t i,const U&x,cp cmp=cp())RL{RET ub(I(v,i),x,cmp)-begin(v);}
TP size_t lbi(size_t i,const T&v,size_t e,const U&x,cp cmp=cp())RL{RET lb(i+I(v,e),x,cmp)-begin(v);}
TP size_t ubi(size_t i,const T&v,size_t e,const U&x,cp cmp=cp())RL{RET ub(i+I(v,e),x,cmp)-begin(v);}

#undef TP
#undef RL
#define TP template<integral T>
#define MT make_unsigned_t<T>

TP constexpr int32_t pcnt(T p){return popcount(MT(p));}
TP constexpr int32_t lsb(T p){return countr_zero(MT(p));}
TP constexpr int32_t msb(T p){return sizeof(T)*8-1-countl_zero(MT(p));}

template<int32_t N,integral T>
void putbit(T s){
  char buf[N+1]={0};
  for(char*itr=buf+N-1;itr>=buf;itr--,s>>=1)
    *itr='0'+(s&1);
  cout<<buf<<sep;
}
#undef TP
#undef MT
#endif