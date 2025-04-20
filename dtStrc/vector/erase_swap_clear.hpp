#ifndef ELSIE_VECTOR_ERASE_SWAP_CLEAR
#define ELSIE_VECTOR_ERASE_SWAP_CLEAR
#include "dtStrc/vector/vector_image.hpp"

namespace elsie{

template<class T>
vector<T>::iterator
vector<T>::erase(vector<T>::iterator pos){
  pos->~T();
  for(auto itr=pos+1;itr<end();++itr)
    *(itr-1)=std::move(*itr);
  --size_;
  return pos;
}

template<class T>
vector<T>::iterator
vector<T>::erase(vector<T>::iterator first,vector<T>::iterator last){
  assert(begin()<=first&&first<=last&&last<=end());
  size_type rm_size=last-first; //[first, last)
  for(auto itr=last;itr<end();++itr){
    (itr-rm_size)->~T();
    *(itr-rm_size)=std::move(*itr);
  }
  size_-=rm_size;
  return first;
}

template<class T>
void vector<T>::swap(vector&x){
  vector<T> tmp(std::move(x));
  x=std::move(*this);
  *this=std::move(tmp);
}

template<class T>
void vector<T>::clear(){
  size_=cap_=0;
  delete data_;
}

} //namespace elsie

#endif