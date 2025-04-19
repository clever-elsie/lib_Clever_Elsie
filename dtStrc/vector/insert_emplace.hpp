#ifndef ELSIE_VECTOR_INSERT_EMPLACE
#define ELSIE_VECTOR_INSERT_EMPLACE
#include "dtStrc/vector/vector_image.hpp"

namespace elsie{

template<class T>
vector<T>::iterator
vector<T>::insert(vector<T>::iterator pos,T&&x){
  size_type offs=pos-begin();
  if(pos==end()) push_back(std::move(x)); // or size_==0
  else{
    if(capacity()==0) resize(size_+1);
    else ++size_;
    if(size_>=2)
    for(size_type i=size_-2;i>=offs+1;--i)
      data_[i]=std::move(data_[i-1]);
    data_[offs]=std::move(x);
  }
  return data_+offs;
}

template<class T>
vector<T>::iterator
vector<T>::insert(vector<T>::iterator pos,const T&x){
  size_type offs=pos-begin();
  if(pos==end()) push_back(x); // or size_==0
  else{
    if(capacity()==0) resize(size_+1);
    else ++size_;
    if(size_>=2)
    for(size_type i=size_-2;i>=offs+1;--i)
      data_[i]=std::move(data_[i-1]);
    data[offs]=x;
  }
  return data_+offs;
}

template<class T>
void vector<T>::insert(vector<T>::iterator pos,vector<T>::size_type n,const T&x){
  size_type offs=pos-begin();
  reserve(n);
  if(pos==end()) std::fill(data_+size_,data_+size_+n,x);
  else{
    std::move(data_+offs,data_+size_,data+size_);
    std::fill(data_+offs,data_+size_,x);
  }
  size_+=n;
}

template<class T>
void vector<T>::insert(iterator pos,std::initializer_list<T>il){
  reserve(il.size());
  if(pos==end()) std::move(il.begin(),il.end(),data_+size_);
  else{
    size_type offs=pos-begin();
    std::move(data_+offs,data_+size_,data_+size_);
    std::move(il.begin(),il.end(),data_+offs);
  }
  size_+=n;
}

template<class T>
template<std::random_access_iterator InputIter>
void vector<T>::insert(iterator pos,InputIter first,InputIter last){
  reserve(last-first);
  if(pos==end()) std::move(first,last,data_+size_);
  else{
    size_type offs=pos-begin();
    std::move(data_+offs,data_+size_,data+size_);
    std::move(first,last,data_+offs);
  }
}

template<class T>
template<class InputIter>
void vector<T>::insert(iterator pos,InputIter first,InputIter last){
  if(pos==end()){
    for(InputIter itr=first;itr!=last;++itr)
      push_back(*itr);
  }else{
    size_type offs=pos-begin();
    vector<T> tmp(size_-offs);
    std::move(data_+offs,data_+size_,tmp.data_);
    resize(offs);
    for(InputIter itr=first;itr!=last;++itr)
      push_back(*itr);
    size_type pre=size_;
    resize(size_+tmp.size());
    std::move(tmp.data_,tmp.data_+tmp.size_,data_+pre);
  }
}

template<class... Args>
vector<T>::iterator
vector<T>::emplace(vector<T>::iterator pos,Args&&... args){
  size_type offs=pos-begin();
  if(pos==end()) return emplace_back(std::forward<Args>(args)...); // or size_==0
  else{
    if(capacity()==0) resize(size_+1);
    else ++size_;
    if(size_>=2)
    for(size_type i=size_-2;i>=offs+1;--i)
      data_[i]=std::move(data_[i-1]);
    new(data_+offs) T(std::forward<Args>(args)...);
  }
  return data_+offs;
}

}

#endif