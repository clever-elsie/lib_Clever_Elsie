#ifndef ELSIE_STATIC_WAVELET
#define ELSIE_STATIC_WAVELET
#include <array>
#include <limits>
#include <vector>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <concepts>
#include <algorithm>
#include <type_traits>
#include <dtStrc/succinct/static_bit_vector.hpp>
namespace elsie{

template<std::unsigned_integral type>
class wavelet{
  private:
  template<class T> using vc=std::vector<T>;
  bool fixed;
  std::size_t lg,sz;
  vc<type>data;
  vc<static_bit_vector>bv;
  public:
  wavelet():sz(0),fixed(0),data(0),bv(1){}
  wavelet(std::size_t N):sz(N),fixed(0),data(N),bv(0){}
  wavelet(std::vector<type>&&v):sz(v.size()),fixed(0),data(std::move(v)),bv(0){}
  wavelet(const std::vector<type>&v):sz(v.size()),fixed(0),data(v),bv(0){}

  void set(std::size_t idx,type v){
    assert(!fixed&&idx<sz);
    data[idx]=v;
  }

  void fix(){
    assert(!fixed);
    if(sz==0){
      sz=1;
      data.resize(1,0);
    }
    fixed=1;
    lg=sizeof(type)*8-countl_zero(*std::max_element(data.begin(),data.end()));
    bv.resize(lg,static_bit_vector(sz));
    std::size_t s[2];
    std::array<vc<type>,2>b{vc<type>(sz),vc<type>(sz)};
    for(int32_t i=lg-1;i>=0;--i){
      s[0]=s[1]=0;
      type filter=type(1)<<i;
      for(std::size_t j=0;j<sz;++j){
        bool p=data[j]&filter;
        b[p][s[p]++]=data[j];
        if(p)bv[i].set(j);
      }
      for(std::size_t p=0,q=0;p<2;q+=s[p++])
        for(std::size_t j=0;j<s[p];++j)
          data[j+q]=b[p][j];
      bv[i].fix();
    }
  }

  type get(std::size_t idx)const{
    if(!fixed) return data[idx];
    type r=0;
    for(int32_t i=lg-1;i>=0;--i){
      if(bv[i].get(idx)){
        r|=type(1)<<i;
        idx=bv[i].rank<0>(sz)+bv[i].rank<1>(idx);
      }else idx=bv[i].rank<0>(idx);
    }
    return r;
  }
  std::size_t range_freq(std::size_t l,std::size_t r,type upper)const{
    if(upper>=(type(1)<<lg))return r-l;
    std::size_t ret=0;
    for(int32_t h=lg-1;h>=0;--h){
      uint32_t L0=bv[h].rank<0>(l),R0=bv[h].rank<0>(r);
      if(upper>>h&1){
        std::size_t zeros=bv[h].rank<0>(sz);
        ret+=R0-L0;
        l+=zeros-L0;
        r+=zeros-R0;
      }else l=L0,r=R0;
    }
    return ret;
  }
  std::size_t range_freq(std::size_t l,std::size_t r,type lower,type upper)const{
    return range_freq(l,r,upper)-range_freq(l,r,lower);
  }
  // [0,idx)に現れるvの数
  std::size_t rank(std::size_t l,std::size_t r,type v)const{
    if(v==std::numeric_limits<type>::max())
      return sz-range_freq(l,r,v);
    else return range_freq(l,r,v,v+1);
  }
  // ord is 0-based index
  std::size_t select(std::size_t idx,type v)const{
    std::size_t mx=rank(0,sz,v);
    if(idx>=mx) return sz;
    std::size_t l=0,r=sz;
    for(int32_t h=lg-1;h>=0;--h){
      std::size_t L0=bv[h].rank<0>(l),R0=bv[h].rank<0>(r);
      if(v>>h&1){
        std::size_t zeros=bv[h].rank<0>(sz);
        l+=zeros-L0,r+=zeros-R0;
      }else l=L0,r=R0;
    }
    std::size_t pos=l+idx;
    for(int32_t h=0;h<lg;++h)
      if(v>>h&1) pos=bv[h].select<1>(1+pos-bv[h].rank<0>(sz));
      else pos=bv[h].select<0>(1+pos);
    return pos;
  }
  std::size_t select(std::size_t l,std::size_t idx,type v)const{ return select(idx+rank(0,l,v),v); }
  type less(std::size_t l,std::size_t r,std::size_t ord)const{
    type ret=0;
    for(int32_t h=lg-1;h>=0;--h){
      uint32_t L0=bv[h].rank<0>(l),R0=bv[h].rank<0>(r);
      if(ord<R0-L0) l=L0,r=R0;
      else{
        uint32_t zeros=bv[h].rank<0>(sz);
        ord-=R0-L0;
        ret|=type(1)<<h;
        l+=zeros-L0,r+=zeros-R0;
      }
    }
    return ret;
  }
  type greater(std::size_t l,std::size_t r,std::size_t ord)const{ return less(l,r,r-l-ord-1); }
  // i \in [l,r), x \in [0,upper), x=data[i]
  type prev(std::size_t l,std::size_t r,type upper)const{
    std::size_t cnt=range_freq(l,r,upper);
    return cnt==0?type(-1):less(l,r,cnt-1);
  }
  // i \in [l,r), x \in [lower,max), x=data[i]
  type next(std::size_t l,std::size_t r,type lower)const{
    std::size_t cnt=range_freq(l,r,lower);
    return cnt==r-l?type(-1):less(l,r,cnt);
  }
};

template<std::signed_integral type>
class wavelet_s{
  private:
  using utype=std::make_unsigned_t<type>;
  utype pot;
  bool fixed;
  std::size_t sz;
  public:
  wavelet<utype>wv;
  std::vector<type>data;
  inline utype raise(type x)const{ return((x<0&&utype(-x)>=pot)?0:x+pot); }
  public:
  wavelet_s():pot(0),sz(0),fixed(0),data(0),wv(){}
  wavelet_s(std::size_t N,utype potential):pot(potential+1),sz(N),fixed(0),data(N,0),wv(N){}
  wavelet_s(std::vector<type>&&v,utype potential):pot(potential),sz(v.size()),fixed(0),data(std::move(v)),wv(sz){}
  wavelet_s(const std::vector<type>&v,utype potential):pot(potential),sz(v.size()),fixed(0),data(v),wv(sz){}
  void set(std::size_t idx,type v){
    assert(!fixed&&idx<data.size());
    data[idx]=v;
  }
  void fix(){
    for(std::size_t i=0;i<data.size();++i)
      wv.set(i,data[i]+pot);
    wv.fix();
  }
  type get(std::size_t idx)const{ return (type)wv.get(idx)-(type)pot; }
  std::size_t range_freq(std::size_t l,std::size_t r,type upper)const{ return wv.range_freq(l,r,raise(upper)); }
  std::size_t range_freq(std::size_t l,std::size_t r,type lower,type upper)const{return wv.range_freq(l,r,raise(lower),raise(upper));}
  std::size_t rank(std::size_t l,std::size_t r,type v)const{ return wv.rank(l,r,raise(v)); }
  std::size_t select(std::size_t idx,type v)const{ return wv.select(idx,raise(v)); }
  std::size_t select(std::size_t l,std::size_t idx,type v)const{ return wv.select(l,idx,raise(v)); }
  type less(std::size_t l,std::size_t r,std::size_t ord)const{ return wv.less(l,r,ord)-pot; }
  type greater(std::size_t l,std::size_t r,std::size_t ord)const{ return wv.greater(l,r,ord)-pot; }
  type prev(std::size_t l,std::size_t r,type upper)const{ return wv.prev(l,r,raise(upper))-pot; }
  type next(std::size_t l,std::size_t r,type lower)const{ return wv.next(l,r,raise(lower))-pot; }
};
} // namespace elsie
#endif // include guard
