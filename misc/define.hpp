#ifndef ELSIE_MISC_DEFINE
#define ELSIE_MISC_DEFINE
#define RET return
#define fi first
#define se second
#define endl '\n'
#define sn(i,c) " \n"[i==c]
#define rsv(n) reserve(n)
#define pf(a) push_front(a)
#define pb(a) push_back(a)
#define eb(...) emplace_back(__VA_ARGS__)
#define ppf() pop_front()
#define ppb() pop_back()
#define pp() pop()
#define ins(a) insert(a)
#define emp(...) emplace(__VA_ARGS__)
#define ers(a) erase(a)
#define cont(a) contains(a)
#define mp(f,s) make_pair(f,s)
#define A(a) begin(a),end(a)
#define I(a,i) begin(a),begin(a)+(i)
#define _SEL4(_1,_2,_3,_4,name,...) name
#define _SEL3(_1,_2,_3,name,...) name
#define _REP4(i,s,n,st) for(int i=(s);i<(n);i+=(st))
#define _REP3(i,s,n) _REP4(i,s,n,1)
#define _REP2(i,n) _REP3(i,0,n)
#define _REP1(n) _REP2(_,n)
#define _RREP4(i,n,t,s) for(int i=(n);i>=(t);i-=(s))
#define _RREP3(i,n,t) _RREP4(i,n,t,1)
#define _RREP2(i,n) _RREP3(i,n,0)
#define _ITER2(x,a) for(auto&x:a)
#define _ITER3(x,y,a) for(auto&[x,y]:a)
#define _CTER2(x,a) for(const auto&x:a)
#define _CTER3(x,y,a) for(const auto&[x,y]:a)
#define rep(...) _SEL4(__VA_ARGS__,_REP4,_REP3,_REP2,_REP1)(__VA_ARGS__)
#define rrep(...) _SEL4(__VA_ARGS__,_RREP4,_RREP3,_RREP2,_REP1)(__VA_ARGS__)
#define iter(...) _SEL3(__VA_ARGS__,_ITER3,_ITER2)(__VA_ARGS__)
#define cter(...) _SEL3(__VA_ARGS__,_CTER3,_CTER2)(__VA_ARGS__)
#endif