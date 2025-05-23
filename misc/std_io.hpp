#ifndef ELSIE_STD_IO
#define ELSIE_STD_IO

#define INT(...) int __VA_ARGS__;in(__VA_ARGS__)
#define CHR(...) char __VA_ARGS__;in(__VA_ARGS__)
#define STR(...) str __VA_ARGS__;in(__VA_ARGS__)
#define VI(a,n) vi a(n);in(a)
#define VIT(a,n) vit a(n);in(a)
#define VS(a,n) vs a(n);in(a)
#define UV(u,v) INT(u,v);--u,--v
#define UVW(u,v,w) INT(u,v,w);--u,--v
#ifdef LOCAL
#define dput(...) dos=&cerr;out(__VA_ARGS__);dos=&cout
#else
#define dput(...)
#endif

template<class T>concept Lint=same_as<T,__int128_t>||same_as<T,__uint128_t>;
template<class T>concept Itrabl=requires(const T&x){x.begin();x.end();}&&!std::is_same_v<T,string>;
template<class T>concept IItrabl=Itrabl<T>&&Itrabl<typename T::value_type>;
template<class T>concept ModInt=requires(const T&x){x.val();};
template<class T>concept NLobj=Itrabl<T>||std::is_same_v<T,string>;

istream&operator<<(istream&is,__float128&x){double y;is>>y;x=y;return is;}
ostream&operator<<(ostream&os,const __float128&x){return os<<static_cast<double>(x);}

template<Lint T>ostream&operator<<(ostream&dst,T val){
  ostream::sentry s(dst);
  if(!s)return dst;
  char _O128[64];
  char*d=end(_O128);
  bool vsign=val<0;
  __uint128_t v=val;
  if(vsign&&val!=numeric_limits<T>::min())v=1+~(__uint128_t)val;
  do{
    *(--d)="0123456789"[v%10];
    v/=10;
  }while(v!=0);
  if(vsign)*(--d)='-';
  ptrdiff_t len=end(_O128)-d;
  if(dst.rdbuf()->sputn(d,len)!=len)dst.setstate(ios_base::badbit);
  return dst;
}

template<Lint T>istream&operator>>(istream&src,T&val) {
  string s;src>>s;
  bool is_neg=numeric_limits<T>::is_signed&&s.size()>0&&s[0]=='-';
  for(val=0;const auto&x:s|views::drop(is_neg))val=10*val+x-'0';
  if(is_neg)val*=-1;
  return src;
}

template<ModInt T>istream&operator>>(istream&is,T&v){int64_t x;is>>x;v=x;return is;}
template<class T,class U>istream&operator>>(istream&is,pair<T,U>&v){return is>>v.first>>v.second;}
template<Itrabl T>istream&operator>>(istream&is,T&v){for(auto&&x:v)is>>x;return is;}

template<class T>void in(T&a){cin>>a;}
template<class T,class... Ts>void in(T&a,Ts&... b){in(a);in(b...);}

template<class T,class U>vector<pair<T,U>>zip(size_t n,size_t m){
  vector<pair<T,U>>r(min(n,m));
  iter(x,y,r)in(x);
  iter(x,y,r)in(y);
  return move(r);
}
template<class T,class U>vector<pair<T,U>>zip(size_t n){return move(zip<T,U>(n,n));}

template<ModInt T>ostream&operator<<(ostream&os,const T&v){return os<<v.val(); }
template<class T,class U>ostream&operator<<(ostream&os,const pair<T,U>&v){return os<<'('<<v.first<<','<<v.second<<')';}
template<Itrabl T>ostream&operator<<(ostream&os,const T&v){size_t cnt=0;cter(x,v)os<<x<<(++cnt<v.size()?" ":"");return os;}
template<IItrabl T>ostream&operator<<(ostream&os,const T&v){size_t cnt=0;cter(x,v)os<<x<<(++cnt<v.size()?"\n":"");return os;}
inline ostream*dos=&cout;
inline int32_t OFLG; // 0:first, 1:notNLobj, 2:NLobj
template<class T>void _out(const T&a){if(OFLG)(*dos)<<"0 \n"[OFLG]<<a;else(*dos)<<a;OFLG=1;}
template<NLobj T>void _out(const T&a){(*dos)<<(OFLG?"\n":"")<<a;OFLG=2;}
template<class T,class...Ts>void _out(const T&a,const Ts&... b){_out(a);_out(b...);}
template<class... Ts>void out(const Ts&... v){OFLG=0;_out(v...);(*dos)<<sep;}
#endif