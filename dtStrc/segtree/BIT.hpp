#ifndef ELSIE_SEGTREE
#define ELSIE_SEGTREE
#include<vector>
#include<cstdint>
#include<cstddef>
#include<utility>
namespace elsie{
  using namespace std;
  template<class type>class BIT{// 0-indexed [l,r)
    vector<type>v;
    type sum(int64_t r){
      type ret=0;
      for(;r;r-=r&-r) ret+=v[r-1];
      return ret;
    }
    size_t ceil2exp(size_t n)const{ return 1ull<<(63+(popcount(n)!=1)-countl_zero(n)); }
    void setup(auto&&v_){
      v=forward<vector<type>>(v_);
      v.resize(ceil2exp(v.size()),type());
    }
  public:
    BIT():v(0){}
    BIT(size_t size){ setup(vector<type>(size,type())); }
    BIT(const vector<type>&v_){ setup(v_); }
    BIT(vector<type>&&v_){ setup(move(v_)); }
    BIT(const BIT&b){ setup(vector<type>(b.v));}
    BIT(BIT&&b){ setup(move(b.v));}
    void add(size_t i,type val){ for(i++;i<=v.size();i+=i&-i) v[i-1]+=val; }
    type sum(size_t l,size_t r){ return sum(r)-sum(l); }
    type get(size_t i){ return sum(i,i+1); }
    void set(size_t i,type t){ add(i,t-get(i)); }
    BIT&operator=(const BIT&b){
      v=b.v;
      return*this;
    }
    BIT&operator=(BIT&&b){
      v=move(b.v);
      return*this;
    }
  };
}
#endif
