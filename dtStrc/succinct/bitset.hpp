#ifndef ELSIE_BITSET
#define ELSIE_BITSET
#include <bit>
#include <ranges>
#include <string>
#include <vector>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <iostream>

namespace elsie{

class bitset{
  size_t len,last_filter;
  std::vector<uint64_t> data;
  size_t ceil64(size_t x){ return (x>>6)+(x&0x3F?1:0); }
  public:
  struct reference{
    reference()=delete;
    constexpr reference(uint64_t&pos, uint8_t p):idx(pos),bit(p){}
    constexpr ~reference()=default;
    constexpr reference& operator=(bool x)noexcept{
      if(x)idx|=  (uint64_t)1<<bit;
      else idx&=~((uint64_t)1<<bit);
    }
    constexpr reference& operator=(const reference& x)noexcept{
      if(x)idx|=  (uint64_t)1<<bit;
      else idx&=~((uint64_t)1<<bit);
    }
    constexpr operator bool() const noexcept{ return (idx>>bit&1)!=0; }
    constexpr bool operator~()const noexcept{ return (idx>>bit&1)==0; }
    constexpr bool operator!()const noexcept{ return (idx>>bit&1)==0; }
    constexpr bool operator&(bool x)const noexcept{ return x&(idx>>bit&1); }
    constexpr bool operator|(bool x)const noexcept{ return x|(idx>>bit&1); }
    constexpr bool operator^(bool x)const noexcept{ return x^(idx>>bit&1); }
    constexpr reference& flip()noexcept{
      idx^=(uint64_t)1<<bit;
      return*this;
    }
    constexpr reference& operator&=(bool x)noexcept{
      if(!x)*this=0;
      return*this;
    }
    constexpr reference& operator|=(bool x)noexcept{
      if(x)*this=1;
      return*this;
    }
    constexpr reference& operator^=(bool x)noexcept{
      if(x) this->flip();
      return*this;
    }
    private:
    uint64_t& idx;
    const uint8_t bit;
  };
  bitset()=default;
  bitset(const bitset&rhs):len(rhs.len),last_filter(rhs.last_filter),data(rhs.data){}
  bitset(bitset&&rhs):len(rhs.len),last_filter(rhs.last_filter),data(std::move(rhs.data)){}
  bitset(size_t n, bool val=0):len(n),last_filter((uint64_t(1)<<(n&0x3F))-1),data(ceil64(n),val?uint64_t(-1):0){}
  bitset(const std::string&s):bitset(s.size(),0){
    for(size_t i=0;i<len;++i)
      if(s[i]=='1') set(i);
  }
  bitset&operator=(const bitset&rhs){
    len=rhs.len,last_filter=rhs.last_filter;
    data=rhs.data;
    return*this;
  }
  bitset&operator=(bitset&&rhs){
    len=rhs.len,last_filter=rhs.last_filter;
    data=std::move(rhs.data);
    return*this;
  }
  void resize(size_t n,bool x=false){
    if(data.size()==0){
      *this=bitset(n,x);
      return;
    }
    size_t L=n>>6, b=n&0x3f,LF=(uint64_t(1)<<b)-1;
    if(L<data.size()) data.resize(L);
    else{
      if(L>data.size() || LF>last_filter)
        if(x) data.back()|=!last_filter;
        else data.back()&=last_filter;
      if(L>data.size()) data.resize(L,x?uint64_t(-1):0);
    }
    len=n,last_filter=LF;
  }
  bitset&operator&=(const bitset&rhs){
    assert(len==rhs.len);
    for(auto&[L,R]:std::views::zip(data,rhs.data))
      L&=R;
  }
  bitset&operator|=(const bitset&rhs){
    assert(len==rhs.len);
    for(auto&[L,R]:std::views::zip(data,rhs.data))
      L|=R;
  }
  bitset&operator^=(const bitset&rhs){
    assert(len==rhs.len);
    for(auto&[L,R]:std::views::zip(data,rhs.data))
      L^=R;
  }
  bitset&operator<<=(const size_t rhs){
    if(rhs>=len){
      this->reset();
      return*this;
    }
    if(rhs==0) return*this; // size_t: 0-1=inf64
    for(size_t i=len-1;i>=rhs;--i)
      this->set(i,data[i-rhs>>6]&(uint64_t)1<<(i-rhs&0x3f));
    for(ssize_t i=rhs-1;i>=0;--i)
      this->reset(i);
    return*this;
  }
  bitset&operator>>=(const size_t rhs){
    if(rhs>=len){
      this->reset();
      return*this;
    }
    if(rhs==0)return*this;
    for(size_t i=0;i+rhs<len;++i)
      this->set(i,data[i+rhs>>6]&(uint64_t)1<<(i+rhs&0x3f));
    for(size_t i=len-rhs;i<len;++i)
      this->reset(i);
    return*this;
  }
  bitset operator~()const noexcept{
    bitset r(*this);
    for(auto&x:r.data)x=~x;
    return r;
  }
  reference operator[](size_t pos){
    assert(pos<=len);
    return reference(data[pos>>6],pos&0x3f);
  }
  friend bool operator==(const bitset&lhs,const bitset&rhs){
    assert(lhs.len==rhs.len);
    if(lhs.len==0) return true;
    for(auto&[L,R]:std::views::zip(lhs.data,rhs.data)|std::views::take(lhs.data.size()-1))
      if(L!=R) return false;
    return(lhs.data.back()&lhs.last_filter)==(rhs.data.back()&rhs.last_filter);
  }
  friend bitset operator<<(const bitset&lhs,size_t rhs){
    size_t len=lhs.len+rhs;
    bitset ndata(len,false);
    for(size_t i=rhs;i<len;++i)
      if(lhs.data[i-rhs>>6]&(uint64_t)1<<(i-rhs&0x3f))
        ndata.set(i,1);
    return ndata;
  }
  friend bitset operator>>(const bitset&lhs,size_t rhs){
    size_t len=std::max<ssize_t>(0, (ssize_t)(lhs.len-rhs));
    bitset ndata(len,false);
    for(size_t i=0;i+rhs<len;++i)
      if(lhs.data[i+rhs>>6]&(uint64_t)1<<(i+rhs&0x3f))
        ndata.set(i,1);
    return ndata;
  }
  friend bitset operator&(const bitset&lhs,const bitset&rhs){
    assert(lhs.len==rhs.len);
    bitset p(lhs.data.size());
    auto itr=p.data.begin();
    for(auto&[L,R]:std::views::zip(lhs.data,rhs.data))
      *(itr++)=L&R;
    return p;
  }
  friend bitset operator|(const bitset&lhs,const bitset&rhs){
    assert(lhs.len==rhs.len);
    bitset p(lhs.data.size());
    auto itr=p.data.begin();
    for(auto&[L,R]:std::views::zip(lhs.data,rhs.data))
      *(itr++)=L|R;
    return p;
  }
  friend bitset operator^(const bitset&lhs,const bitset&rhs){
    assert(lhs.len==rhs.len);
    bitset p(lhs.data.size());
    auto itr=p.data.begin();
    for(auto&[L,R]:std::views::zip(lhs.data,rhs.data))
      *(itr++)=L^R;
    return p;
  }
  template<class CharT,class Traits>
  friend std::basic_ostream<CharT,Traits>& operator<<(std::basic_ostream<CharT,Traits>&os,bitset&rhs){
    return os<<to_string();
  }
  bitset&set(){
    for(auto&x:data) x=uint64_t(-1);
    return*this;
  }
  bitset&set(size_t pos, bool val=true){
    (*this)[pos]=val;
    return*this;
  }
  bitset&reset(){
    for(auto&x:data) x=0;
    return*this;
  }
  bitset&reset(size_t pos){
    (*this)[pos]=0;
    return*this;
  }
  bitset&flip(){
    for(auto&x:data)x=~x;
    return*this;
  }
  bitset&flip(size_t pos){
    (*this)[pos].flip();
    return*this;
  }
  size_t count()const{
    if(len==0) return 0;
    size_t cnt=0;
    for(auto&x:data|std::views::take(data.size()-1))
      cnt+=std::popcount(x);
    cnt+=std::popcount(data.back()&last_filter);
    return cnt;
  }
  size_t size()const{ return len; }
  bool test(size_t pos)const{ return (*this)[pos]==1; }
  bool all()const{
    if(len==0) return true;
    for(auto&x:data|std::views::take(data.size()-1))
      if(x!=uint64_t(-1)) return false;
    return (data.back()&last_filter)==last_filter;
  }
  bool any()const{
    if(len==0) return false;
    for(auto&x:data|std::views::take(data.size()-1))
      if(x) return true;
    return data.back()&last_filter;
  }
  bool none()const{
    if(len==0) return true;
    for(auto&x:data|std::views::take(data.size()-1))  
      if(x) return false;
    return(data.back()&last_filter)==0;
  }
  std::string to_string()const{
    std::string s(len,'0');
    for(size_t i=0;i<len;++i)
      if((*this)[i]) ++s[i];
    return s;
  }
  uint64_t to_u64()const{
    assert(len<=64);
    uint64_t s=0;
    for(size_t i=0;i<len;++i)
      if((*this)[i]) s|=(uint64_t)1 << i;
    return s;
  }
  bitset(const bitset&rhs);
  bitset(bitset&&rhs);
  bitset&operator=(const bitset&rhs);
  bitset&operator=(bitset&&rhs);
  void resize(size_t n,bool x=false);
  bitset&operator&=(const bitset&rhs);
  bitset&operator|=(const bitset&rhs);
  bitset&operator^=(const bitset&rhs);
  bitset&operator<<=(const bitset&rhs);
  bitset&operator>>=(const bitset&rhs);
  bitset&operator~()const noexcept;
  reference operator[](size_t pos);
  const reference operator[](size_t pos)const;
  friend bool operator==(const bitset&lhs,const bitset&rhs)const;
  friend bitset operator<<(const bitset&lhs,size_t rhs);
  friend bitset operator>>(const bitset&lhs,size_t rhs);
  friend bitset operator&(const bitset&lhs,const bitset&rhs);
  friend bitset operator|(const bitset&lhs,const bitset&rhs);
  friend bitset operator^(const bitset&lhs,const bitset&rhs);
  template<class CharT,class Traits>
  friend std::basic_istream<CharT,Traits>& operator>>(std::basic_istream<CharT,Traits>&is,bitset&rhs);
  template<class CharT,class Traits>
  friend std::basic_ostream<CharT,Traits>& operator<<(std::basic_ostream<CharT,Traits>&os,bitset&rhs);
  bitset&set();
  bitset&set(size_t pos, bool val=true);
  bitset&reset();
  bitset&reset(size_t pos);
  bitset&flip();
  bitset&flip(size_t pos);
  size_t count()const;
  size_t size()const;
  bool test(size_t pos)const;
  bool all()const;
  bool any()const;
  bool none()const;
  std::string to_string()const;
  uint64_t to_u64()const;
};
} // namespace elsie
#endif