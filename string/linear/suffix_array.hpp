#ifndef ELSIE_SUFFIX_ARRAY
#define ELSIE_SUFFIX_ARRAY
/**
 * @file suffix_array.hpp
 * @brief compute suffix_array in O(N). N is size of text.
 * @author CleverElsie
 * @details use default SA_IS. but O(N log^2 N) can be used.
 */
#include <array>
#include <string>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <concepts>
#include <algorithm>
#include <string/rollingHash.hpp>
namespace elsie{
using namespace std;
template<class T>concept Itrabl=requires(const T&x){x.begin();x.end();x.size();typename T::value_type;};
/**
 * @class suffix_array
 * @brief compute suffix array of iterable object.
 * @attention Itrabl concept requires method {.begin(), .end(), .size()}. but may be needed operator++ and operator-- at T::iterator for sequential access.
 */
template<size_t psz,Itrabl T>class suffix_array{
  public:
  template<class S>using vc=vector<S>;
  constexpr static size_t tsz=1024;
  T v;
  vc<int32_t>sa;
  rollingHash<psz,T>rolh;
  using arl=rollingHash<psz,T>::arl;
  public:
  suffix_array():sa(0),v(0),rolh(){}
  suffix_array(T&&s):v(s),rolh(){fix();}
  suffix_array(const T&s):v(s),rolh(){fix();}
  ~suffix_array()=default;
  size_t lower_bound(const T&t){
    if constexpr (psz>0){
      if(t.size()>tsz){
        vector<arl>h;
        rolh.make_hash(t,h);
        return lower_bound(h,t);
      }
    }
    int64_t ok=v.size(),ng=-1;
    while(ng+1<ok){
      int64_t m=(ok+ng)/2,nx=0;
      for(size_t i=0,u=min(t.size(),v.size()-sa[m]);i<u;++i){
        if(v[sa[m]+i]==t[i])continue;
        if(v[sa[m]+i]<t[i]) ng=m;
        else ok=m;
        nx=1;
        break;
      }
      if(nx)continue;
      if(t.size()<=v.size()-sa[m])ok=m;
      else ng=m;
    }
    return ok;
  }
  size_t upper_bound(const T&t){
    if constexpr(psz>0){
      if(t.size()>tsz){
        vector<arl>h;
        rolh.make_hash(t,h);
        return upper_bound(h,t);
      }
    }
    int64_t ok=v.size(),ng=-1;
    while(ng+1<ok){
      int64_t m=(ok+ng)/2,nx=0;
      for(size_t i=0,u=min(t.size(),v.size()-sa[m]);i<u;++i){
        if(v[sa[m]+i]==t[i])continue;
        if(v[sa[m]+i]<t[i]) ng=m;
        else ok=m;
        nx=1;
        break;
      }
      if(nx)continue;
      ng=m;
    }
    return ok;
  }
  size_t count(const T&t){
    if constexpr(psz>0){
      if(t.size()>tsz){
        vector<arl>h;
        rolh.make_hash(t,h);
        return upper_bound(h,t)-lower_bound(h,t);
      }
    }
    return upper_bound(t)-lower_bound(t);
  }
  void fix(){
    loop=((64-countl_zero(v.size()))+(popcount(v.size())!=1)+base_exp-1)/base_exp;
    T prs=v;
    sort(prs.begin(),prs.end());
    prs.erase(unique(prs.begin(),prs.end()),prs.end());
    vector<int32_t>p(v.size());
    for(size_t i=0;i<v.size();++i)
      p[i]=std::lower_bound(prs.begin(),prs.end(),v[i])-prs.begin();
    sa=SA_IS(p,prs.back());
    if constexpr(psz>0) rolh.set(v);
  }
  void set(const T&s){ v=s; fix(); }
  void set(T&&s){ v=move(s); fix(); }
  protected:
  using ps2=pair<int32_t,int32_t>;
  using ps3=pair<ps2,int32_t>;
  using vps3=vector<ps3>;
  constexpr static int32_t base_exp=4;
  constexpr static size_t base=1ull<<base_exp;
  constexpr static size_t bit_mask=base-1;
  //! loop size for radix_sort
  size_t loop;
  vector<int32_t> SA_log(const vector<int32_t>&p){
    size_t n=p.size();
    vps3 Rsubstr(n);
    vector<int32_t>rank(n);
    for(size_t i=0;i<n;++i) Rsubstr[i]={{p[i],(i+1<n?p[i+1]:-1)},i};
    sort(Rsubstr.begin(),Rsubstr.end());
    for(size_t l=2,r=1;l<n;l<<=1,r=1){
      rank[Rsubstr[0].second]=1;
      for(size_t i=1;i<n;++i){
        auto&&[a,b]=Rsubstr[i].first;
        auto&&[c,d]=Rsubstr[i-1].first;
        rank[Rsubstr[i].second]=(r+=a!=c||b!=d);
      }
      for(size_t i=0;i<n;++i) Rsubstr[i]={{rank[i],(i+l<n?rank[i+l]:-1)},i};
      //radix_sort(Rsubstr);
      sort(Rsubstr.begin(),Rsubstr.end());
    }
    vector<int32_t>sa_ret(n);
    for(size_t i=0;i<n;++i) sa_ret[i]=Rsubstr[i].second;
    return sa_ret;
  }
  vector<int32_t> SA_naive(const vector<int32_t>&s){
    size_t n=s.size();
    vector<int32_t>sap(n);
    iota(sap.begin(),sap.end(),0);
    sort(sap.begin(),sap.end(),[&](int32_t l,int32_t r){
      if(l==r)return false;
      while(l<n&&r<n){
        if(s[l]!=s[r])return s[l]<s[r];
        ++l,++r;
      }
      return l==n;
    });
    return sap;
  }
  vector<int32_t> SA_IS(const vector<int32_t>&p,int32_t upper){
    constexpr int32_t tnv=10,tlog=40;
    int32_t n=p.size();
    if(n<tlog){
      switch(n){
        case 0:return {};
        case 1:return {0};
        case 2:
          if(p[0]<p[1])return{0,1};
          else return{1,0};
        default:
        if(n<tnv)return SA_naive(p);
        else return SA_log(p);
      }
    }
    vector<int32_t>sap(n);
    vector<bool>ls(n);
    for(int32_t i=n-2;i>=0;--i) ls[i]=p[i]==p[i+1]?ls[i+1]:p[i]<p[i+1];
    vector<int32_t>sum_l(upper+1),sum_s(upper+1);
    for(int32_t i=0;i<n;++i)
      if(!ls[i])++sum_s[p[i]];
      else ++sum_l[p[i]+1];
    for(int32_t i=0;i<upper;++i){
      sum_s[i]+=sum_l[i];
      sum_l[i+1]+=sum_s[i];
    }sum_s[upper]+=sum_l[upper];
    auto induce=[&](const vector<int32_t>&lms){
      fill(sap.begin(),sap.end(),-1);
      vector<int32_t>buf(upper+1);
      copy(sum_s.begin(),sum_s.end(),buf.begin());
      for(const auto&d:lms){
        if(d==n)continue;
        sap[buf[p[d]]++]=d;
      }
      copy(sum_l.begin(),sum_l.end(),buf.begin());
      sap[buf[p[n-1]]++]=n-1;
      for(int32_t i=0;i<n;++i)
        if(int32_t v=sap[i];v>=1&&!ls[v-1])
          sap[buf[p[v-1]]++]=v-1;
      copy(sum_l.begin(),sum_l.end(),buf.begin());
      for(int32_t i=n-1;i>=0;--i)
        if(int32_t v=sap[i];v>=1&&ls[v-1])
          sap[--buf[p[v-1]+1]]=v-1;
    };
    vector<int32_t>lms_map(n+1,-1);
    int32_t m=0;
    for(int32_t i=1;i<n;++i) if(!ls[i-1]&&ls[i]) lms_map[i]=m++;
    vector<int32_t>lms;
    lms.reserve(m);
    for(int32_t i=1;i<n;++i) if(!ls[i-1]&&ls[i]) lms.push_back(i);
    induce(lms);
    if(m==0)return sap;
    vector<int32_t>sorted_lms;
    sorted_lms.reserve(m);
    for(const auto&v:sap) if(lms_map[v]!=-1) sorted_lms.push_back(v);
    vector<int32_t>rec_s(m);
    int32_t rec_upper=0;
    rec_s[lms_map[sorted_lms[0]]]=0;
    for(int32_t i=1;i<m;++i){
      int32_t l=sorted_lms[i-1],r=sorted_lms[i];
      int32_t end_l=lms_map[l]+1<m?lms[lms_map[l]+1]:n;
      int32_t end_r=lms_map[r]+1<m?lms[lms_map[r]+1]:n;
      bool same=true;
      if(end_l-l!=end_r-r) same=false;
      else{
        while(l<end_l){
          if(p[l]!=p[r])break;
          ++l,++r;
        }
        if(l==n||p[l]!=p[r])same=false;
      }
      rec_s[lms_map[sorted_lms[i]]]=(rec_upper+=!same);
    }
    auto rec_sa=SA_IS(rec_s,rec_upper);
    for(int32_t i=0;i<m;++i) sorted_lms[i]=lms[rec_sa[i]];
    induce(sorted_lms);
    return sap;
  }
  void radix_sort(vps3&a){
    const size_t n=a.size();
    auto rsort=[&](vps3::iterator l,vps3::iterator r,bool key_first){
      array<vps3,1<<base_exp>rd;
      for(size_t i=0,shift=0;i<loop;++i,shift+=base_exp){
        for(auto&x:rd)x.clear();
        if(key_first)
          for(auto itr=l;itr!=r;++itr)
            rd[(itr->first.first>>shift)&bit_mask].push_back(*itr);
        else
          for(auto itr=l;itr!=r;++itr)
            rd[(itr->first.second>>shift)&bit_mask].push_back(*itr);
        auto itr=l;
        for(const auto&x:rd)
        for(const auto&y:x){
          *itr=y;++itr;
        }
      }
    };
    rsort(a.begin(),a.end(),true);
    auto ltr=a.begin(),rtr=a.begin();
    while(ltr<a.end()){
      while(rtr<a.end()&&ltr->first.first==rtr->first.first)++rtr;
      if(rtr-ltr>1) rsort(ltr,rtr,false);
      ltr=rtr++;
    }
  }
  void LCP(){} // 未実装
  size_t lower_bound(const vector<arl>&h,const T&t){
    int64_t ok=v.size(),ng=-1;
    while(ng+1<ok){
      int64_t m=(ok+ng)>>1;
      size_t lcp=LCP_hash(sa[m],h),len=v.size()-sa[m];
      if(lcp<t.size()&&lcp<len){
        if(v[sa[m]+lcp]<t[lcp]) ng=m;
        else ok=m;
      }else{
        if(t.size()<=len)ok=m;
        else ng=m;
      }
    }
    return ok;
  }
  size_t upper_bound(const vector<arl>&h,const T&t){
    int64_t ok=v.size(),ng=-1;
    while(ng+1<ok){
      int64_t m=(ok+ng)>>1;
      size_t lcp=LCP_hash(sa[m],h),len=v.size()-sa[m];
      if(lcp<t.size()&&lcp<len){
        if(v[sa[m]+lcp]<t[lcp])ng=m;
        else ok=m;
      }else ng=m;
    }
    return ok;
  }
  inline size_t LCP_hash(size_t idx,const vector<arl>&h){
    size_t len=min(v.size()-idx,h.size()-1);
    int64_t ok=0,ng=len+1;
    while(ok+1<ng){
      int64_t m=(ok+ng)>>1;
      bool all_same=true;
      for(int32_t i=0;i<psz;++i)
        if(h[m][i]!=rolh.hash_value(idx,idx+m,i)){
          all_same=false;
          break;
        }
      if(all_same)ok=m;
      else ng=m;
    }
    return ok;
  }
};
}

#endif