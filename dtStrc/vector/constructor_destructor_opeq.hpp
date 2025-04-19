#ifndef ELSIE_VECTOR_CONSTRUCTOR_DESTRUCTOR_OPEQ
#define ELSIE_VECTOR_CONSTRUCTOR_DESTRUCTOR_OPEQ
#include "dtStrc/vector/vector_image.hpp"

namespace elsie{

template<class T>
vector<T>::vector()
:size_{},cap_{},data_{nullptr}{}

template<class T>
vector<T>::vector(vector<T>&&x)
:size_{x.size_},cap_{x.cap_},data_{x.data_}{ x.data_=nullptr; }

template<class T>
vector<T>::vector(const vector<T>&x):size_{x.size_}{
  if(cap_<x.cap_){
    std::free(data_);
    cap_=x.cap_;
    data_=static_cast<T*>(std::aligned_alloc(64,sizeof(T)*x.cap_));
  }
  if constexpr(__AVX512F__){
    auto*d=static_cast<__m512i*>(data);
    const auto*s=static_cast<__m512i*>(x.data_);
    size_type bcnt=size*sizeof(T);
    size_type vcnt=bcnt/64;
    size_type vrem=bcnt%64;
    for(size_type i=0;i<vcnt;++i){
      __m512i val=_mm512_load_si512(s+i);
      __m512_store_si512(d+i,val);
    }
    if(vrem)
      std::memcpy(reinterpret_cast<std::byte*>(d+vcnt),reinterpret_cast<const std::byte*>(s+vcnt),rem);
  }else std::copy(x.data_,x.data_+x.size_,data_);
}

template<class T>
vector<T>::vector(size_t n)
:size_(n),cap_(aligned64_size(n))
,data_(static_cast<T*>(std::aligned_alloc(64,cap_*sizeof(T)))){}

template<class T>
vector<T>::vector(size_t n,const T&value)
:size_(n),cap_(aligned64_size(n))
,data_(static_cast<T*>(std::aligned_alloc(64,cap_*sizeof(T)))){ std::fill_n(data_,n,value); }

template<class T>
template<class InputIter>
vector<T>::vector(InputIter first,InputIter last)
:size_(last-first),cap_(aligned64_size(size_))
,data_(static_cast<T*>(std::aligned_alloc(64,cap_*sizeof(T))))
{ std::copy(first,last,data_); }

template<class T>
vector<T>::~vector(){
  delete this->data_;
  this->data_=nullptr;
  this->size_=this->cap_=0;
}

template<class T>
vector<T>& vector<T>::operator=(vector&&x){
  size_=x.size_,cap_=x.cap_;
  std::free(data_);
  data_=x.data_;
  x.data_=nullptr;
  return*this;
}

template<class T>
vector<T>& vector<T>::operator=(const vector<T>&x){
  size_=x.size_;
  if(cap_<x.cap_){
    std::free(data_);
    cap_=x.cap_;
    data_=static_cast<T*>(std::aligned_alloc(64,sizeof(T)*x.cap_));
  }
  if constexpr(__AVX512F__){
    auto*d=static_cast<__m512i*>(data);
    const auto*s=static_cast<__m512i*>(x.data_);
    size_type bcnt=size*sizeof(T);
    size_type vcnt=bcnt/64;
    size_type vrem=bcnt%64;
    for(size_type i=0;i<vcnt;++i){
      __m512i val=_mm512_load_si512(s+i);
      __m512_store_si512(d+i,val);
    }
    if(vrem) std::memcpy(reinterpret_cast<std::byte*>(d+vcnt),reinterpret_cast<const std::byte*>(s+vcnt),rem);
  }else std::copy(x.data_,x.data_+x.size_,data_);
  return*this;
}

template<class T>
vector<T>& vector<T>::operator=(std::initializer_list<T> init){
  size_=init.size();
  cap_=aligned64_size(size_);
  size_type i=0;
  for(const auto itr=init.begin();itr!=init.end();++itr)
    data_[i++]=*itr;
  return*this;
}

} // namespace elsie

#endif