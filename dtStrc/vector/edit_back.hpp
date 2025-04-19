#ifndef ELSIE_VECTOR_EDIT_BACK
#define ELSIE_VECTOR_EDIT_BACK
#include "dtStrc/vector/vector_image.hpp"

namespace elsie{

template<class T>
void vector<T>::push_back(T&&x){
  if(capacity()==0) resize(size_+1);
  data_[size_-1]=std::move(x);
}

template<class T>
void vector<T>::push_back(const T&x){
  if(capacity()==0) resize(size_+1);
  data_[size_-1]=x;
}

template<class T>
template<class... Args>
reference vector<T>::emplace_back(Args&&... args){
  if(capacity()==0) resize(size_+1);
  new(data+size_-1) T(std::forward<Args>(args)...);
  return data_[size_-1];
}

template<class T>
void vector<T>::pop_back(){ if(size_) data_[--size_]=T(); }

} // namespace elsie

#endif