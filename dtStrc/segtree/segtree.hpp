// SPDX-License-Identifier: MIT
// Copyright (c) 2026 CleverElsie

#ifndef ELSIE_SEGTREE
#define ELSIE_SEGTREE
#include<limits>
#include<vector>
#include<cstdint>
#include<cstddef>
#include<concepts>
#include<functional>
#include<type_traits>
namespace elsie{

  template<class S,auto op,auto e>
  class segtree{
    static_assert(std::is_convertible_v<decltype(op),std::function<S(S,S)>>);
    static_assert(std::is_convertible_v<decltype(e),std::function<S()>>);
  protected: // 1-indexed [l,r)
    //! data vector
    std::vector<S> v;
    //! _n is native vector size.
    std::size_t n;
    //! n is segtree size.
    std::size_t _n;
    /**
     * @brief min{ i | 2^i >= n }
     * @param n n >= 1
     */
    std::size_t ceil2exp(std::size_t n)const{ return 1ull<<(63+(popcount(n)!=1)-countl_zero(n)); }
  public:
    segtree():_n(0),n(0),v(0){}
    segtree(std::size_t N):_n(N),n(ceil2exp(N)){ v.resize(n+n,e()); }
    segtree(const std::vector<S>&V):_n(V.size()),n(ceil2exp(V.size())){
      v.resize(n+n,e());
      for(std::size_t i=n;auto&x:V)v[i++]=x;
      for(std::size_t i=n-1;i;--i)v[i]=op(v[2*i],v[2*i+1]);
    }
    segtree(segtree&&seg){*this=std::move(seg);}
    segtree(const segtree&seg){*this=seg;}
    segtree&operator=(segtree&&seg){
      n=seg.n,_n=seg._n,v=std::move(seg.v);
      return*this;
    }
    segtree&operator=(const segtree&seg){
      n=seg.n,_n=seg._n,v=seg.v;
      return*this;
    }
    void set(std::size_t p,S&&val){
      v[p+=n]=val;
      for(std::size_t i=p>>1;i;i>>=1)
        v[i]=op(v[2*i],v[2*i+1]);
    }
    void set(std::size_t p,const S&val){
      v[p+=n]=val;
      for(std::size_t i=p>>1;i;i>>=1)
        v[i]=op(v[2*i],v[2*i+1]);
    }
    void add(std::size_t p,const S&val){ set(p,v[p+n]+val); }
    S get(std::size_t p)const{ return v[n+p];}

    S prod(std::size_t l,std::size_t r)const{
      S pl=e(),pr=e();
      for(l+=n,r+=n;l<r;l>>=1,r>>=1){
        if(l&1)pl=op(pl,v[l++]);
        if(r&1)pr=op(v[--r],pr);
      }
      return op(pl,pr);
    }
    S all_prod()const{ return v[1]; }

    template<class F>
    std::size_t max_right(std::size_t l,F f)const
    requires std::is_convertible_v<F,std::function<bool(S)>>
    { return max_right<f>(l); }

    template<bool(*f)(S)>
    std::size_t max_right(std::size_t l)const{
      assert(l<=_n&&f(e()));
      if(l==_n)return _n;
      l+=n;
      S p=e();
      do{
        while(!(l&1))l>>=1;
        if(!f(op(p,v[l]))){
          while(l<n){
            l<<=1;
            if(f(op(p,v[l])))
              p=op(p,v[l++]);
          }
          return l-n;
        }
        p=op(p,v[l++]);
      }while((l&-l)!=l);
      return _n;
    }

    template<class F>
    std::size_t min_left(std::size_t r,F f)const
    requires std::is_convertible_v<F,std::function<bool(S)>>
    { return min_left<f>(r); }

    template<bool(*f)(S)>
    std::size_t min_left(std::size_t r)const{
      assert(r<=_n&&f(e()));
      if(r==0)return 0;
      r+=n;
      S p=e();
      do{
        --r;
        while(r>1&&(r&1))r>>=1;
        if(!f(op(v[r],p))){
          while(r<n){
            r=1+(r<<1);
            if(f(op(v[r],p)))
              p=op(v[r--],p);
          }
          return r+1-n;
        }
        p=op(v[r],p);
      }while((r&-r)!=r);
      return 0;
    }
  };
}
#endif