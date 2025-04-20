#ifndef ELSIE_VECTOR_MEMORY_ACCESS
#define ELSIE_VECTOR_MEMORY_ACCESS
#include "dtStrc/vector/vector_image.hpp"

namespace elsie{

template<class T>
vector<T>::reference
vector<T>::operator[](size_type n){ return data_[n]; }

template<class T>
vector<T>::const_reference
vector<T>::operator[](size_type n)const{ return data_[n]; }

template<class T>
vector<T>::reference
vector<T>::at(size_type n){
  if(n>=size_)throw std::out_of_range();
  return data_[n];
}

template<class T>
vector<T>::reference
vector<T>::at(size_type n)const{
  if(n>=size_)throw std::out_of_range();
  return data_[n];
}

template<class T>
T* vector<T>::data()noexcept{ return data_; }

template<class T>
const T* vector<T>::data()const noexcept{ return data_; }

template<class T>
vector<T>::reference
vector<T>::front(){ return data_[0]; }

template<class T>
vector<T>::const_reference
vector<T>::front()const{ return data_[0]; }

template<class T>
vector<T>::reference
vector<T>::back(){ return data_[size_-1]; }

template<class T>
vector<T>::const_reference
vector<T>::back()const{ return data_[size_-1]; }

} // namespace elsie
#endif