#ifndef ELSIE_VECTOR_MEMORY_MANAGE
#define ELSIE_VECTOR_MEMORY_MANAGE
#include "dtStrc/vector/vector_image.hpp"

namespace elsie{
template<class T>
size_type vector<T>::size()const noexcept{ return size_; }

template<class T>
size_type vector<T>::max_size()const noexcept{ return cap_; }

template<class T>
void vector<T>::resize(size_type n){
  if(n>cap_){
    size_type n_=aligned64_size(size_type(1)<<(sizeof(size_type)*8-std::countl_zero(n)-(std::popcount(n)==1)));
    vector<T> tmp(n_);
    tmp=*this;
    *this=std::move(tmp);
  }
  size_=n;
}

template<class T>
void vector<T>::resize(size_type n,const T&value){
  if(n>=size_){
    if(n<=cap_) std::fill(data_+size_,data_+cap_,value);
    else{
      size_type n_=aligned64_size(size_type(1)<<(sizeof(size_type)*8-std::countl_zero(n)-(std::popcount(n)==1)));
      vector<T> tmp(n_);
      tmp=*this;
      std::fill(tmp.data_+size_,tmp.data_+n,value);
      *this=std::move(tmp);
    }
  }
  size_=n;
}

template<class T>
size_type vector<T>::capacity()const noexcept{ return cap_-size_; }

template<class T>
bool vector<T>::empty()const noexcept{ return 0==size_; }

template<class T>
void vector<T>::reserve(size_type n){
  size_type rem=cap_-size_;
  if(rem>=n)return;
  rem=n-rem;
  resize(size_+rem);
  size_-=rem;
}

template<class T>
void vector<T>::shrink_to_fit(){
  size_type n_=aligned64_size(size_type(1)<<(sizeof(size_type*8-std::countl_zero(size_)-(std::popcount(size_)==1))));
  if(n_==cap_)return;
  vector<T> tmp(n_);
  tmp=*this;
  *this=std::move(tmp);
}

} // namespace elsie
#endif