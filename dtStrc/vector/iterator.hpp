#ifndef ELSIE_VECTOR_ITERATOR
#define ELSIE_VECTOR_ITERATOR
#include "dtStrc/vector/vector_image.hpp"

namespace elsie{

template<class T> struct vector<T>::iterator{
  using value_type=vector<T>::value_type;
  using pointer=vector<T>::pointer;
  using reference=vector<T>::reference;
  using const_reference=vector<T>::const_reference;
  using size_type=vector<T>::size_type;
  using difference_type=vector<T>::difference_type;
  using iterator_category=vector<T>::iterator_category;
  iterator()=default;
  iterator(iterator&&)=default;
  iterator(const iterator&)=default;
  iterator(T*itr):ptr{itr}{}
  ~iterator()=default;
  iterator& operator=(iterator&&)=default;
  iterator& operator=(const iterator&)=default;
  iterator& operator++()noexcept{
    ++ptr;
    return*this;
  }
  iterator& operator--()noexcept{
    --ptr;
    return*this;
  }
  iterator operator++(int)noexcept{
    iterator ret=*this;
    ++ptr;
    return ret;
  }
  iterator operator--(int)noexcept{
    iterator ret=*this;
    --ptr;
    return ret;
  }

  T& operator*()noexcept{ return *ptr; }
  const T& operator*()const noexcept{ return *ptr; }

  T* operator->()noexcept{ return ptr; }
  const T* operator->()const noexcept{ return ptr; }

  explicit operator bool() const noexcept { return ptr!=nullptr; }
  bool operator!() const noexcept{ return !static_cast<bool>(*this); }

  iterator& operator+=(size_type n){
    ptr+=n;
    return*this;
  }
  iterator& operator-=(size_type n){
    ptr-=n;
    return*this;
  }

  friend iterator operator+(const iterator&lhs,size_type rhs){
    return iterator(lhs)+=rhs;
  }  
  friend iterator operator+(size_type lhs,const iterator&rhs){
    return iterator(rhs)+=lhs;
  }

  friend iterator operator-(const iterator&lhs,size_type rhs){
    return iterator(lhs)-=rhs;
  }  
  friend difference_type operator-(const iterator&lhs,const iterator&rhs){
    return lhs.ptr-rhs.ptr;
  }

  friend bool operator<=>(const iterator&,const iterator&)=default;
  private:
  T*ptr;
};

template<class T> struct vector<T>::const_iterator{
  using value_type=vector<T>::value_type;
  using pointer=vector<T>::pointer;
  using reference=vector<T>::reference;
  using const_reference=vector<T>::const_reference;
  using size_type=vector<T>::size_type;
  using difference_type=vector<T>::difference_type;
  using iterator_category=vector<T>::iterator_category;
  const_iterator()=default;
  const_iterator(const_iterator&&)=default;
  const_iterator(const const_iterator&)=default;
  const_iterator(T*itr):ptr{itr}{}
  const_iterator(const T*itr):ptr{itr}{}
  ~const_iterator()=default;
  const_iterator& operator=(const_iterator&&)=default;
  const_iterator& operator=(const const_iterator&)=default;
  const_iterator& operator++()noexcept{
    ++ptr;
    return*this;
  }
  const_iterator& operator--()noexcept{
    --ptr;
    return*this;
  }
  const_iterator operator++(int)noexcept{
    const_iterator ret=*this;
    ++ptr;
    return ret;
  }
  const_iterator operator--(int)noexcept{
    const_iterator ret=*this;
    --ptr;
    return ret;
  }

  const T& operator*()const noexcept{ return *ptr; }
  const T* operator->()const noexcept{ return ptr; }

  explicit operator bool() const noexcept { return ptr!=nullptr; }
  bool operator!() const noexcept{ return !static_cast<bool>(*this); }

  const_iterator& operator+=(size_type n){
    ptr+=n;
    return*this;
  }
  const_iterator& operator-=(size_type n){
    ptr-=n;
    return*this;
  }

  friend const_iterator operator+(const const_iterator&lhs,size_type rhs){
    return const_iterator(lhs)+=rhs;
  }  
  friend const_iterator operator+(size_type lhs,const const_iterator&rhs){
    return const_iterator(rhs)+=lhs;
  }

  friend const_iterator operator-(const const_iterator&lhs,size_type rhs){
    return const_iterator(lhs)-=rhs;
  }
  friend difference_type operator-(const const_iterator&lhs,const const_iterator&rhs){
    return lhs.ptr-rhs.ptr;
  }

  friend bool operator<=>(const const_iterator&,const const_iterator&)=default;

  private:
  const T*ptr;
};

template<class T> struct vector<T>::reverse_iterator{
  using value_type=vector<T>::value_type;
  using pointer=vector<T>::pointer;
  using reference=vector<T>::reference;
  using const_reference=vector<T>::const_reference;
  using size_type=vector<T>::size_type;
  using difference_type=vector<T>::difference_type;
  using iterator_category=vector<T>::iterator_category;
  reverse_iterator()=default;
  reverse_iterator(reverse_iterator&&)=default;
  reverse_iterator(const reverse_iterator&)=default;
  reverse_iterator(T*itr):ptr{itr+1}{}
  ~reverse_iterator()=default;
  reverse_iterator& operator=(reverse_iterator&&)=default;
  reverse_iterator& operator=(const reverse_iterator&)=default;
  reverse_iterator& operator++()noexcept{
    --ptr;
    return*this;
  }
  reverse_iterator& operator--()noexcept{
    ++ptr;
    return*this;
  }
  reverse_iterator operator++(int)noexcept{
    reverse_iterator ret=*this;
    --ptr;
    return ret;
  }
  reverse_iterator operator--(int)noexcept{
    reverse_iterator ret=*this;
    ++ptr;
    return ret;
  }

  T& operator*()noexcept{ return *(ptr-1); }
  const T& operator*()const noexcept{ return *(ptr-1); }

  T* operator->()noexcept{ return ptr-1; }
  const T* operator->()const noexcept{ return ptr-1; }

  explicit operator bool() const noexcept { return ptr!=nullptr; }
  bool operator!() const noexcept{ return !static_cast<bool>(*this); }

  reverse_iterator& operator+=(size_type n){
    ptr-=n;
    return*this;
  }
  reverse_iterator& operator-=(size_type n){
    ptr+=n;
    return*this;
  }

  friend reverse_iterator operator+(const reverse_iterator&lhs,size_type rhs){
    return reverse_iterator(lhs)-=rhs;
  }  
  friend reverse_iterator operator+(size_type lhs,const reverse_iterator&rhs){
    return reverse_iterator(rhs)-=lhs;
  }

  friend reverse_iterator operator-(const reverse_iterator&lhs,size_type rhs){
    return reverse_iterator(lhs)+=rhs;
  }  
  friend difference_type operator-(const reverse_iterator&lhs,const reverse_iterator&rhs){
    return rhs.ptr-lhs.ptr;
  }

  friend bool operator==(const reverse_iterator&lhs,const reverse_iterator&rhs){
    return lhs.ptr==rhs.ptr;
  }
  friend bool operator!=(const reverse_iterator&lhs,const reverse_iterator&rhs){
    return lhs.ptr!=rhs.ptr;
  }
  friend bool operator<(const reverse_iterator&lhs,const reverse_iterator&rhs){
    return lhs.ptr>rhs.ptr;
  }
  friend bool operator>(const reverse_iterator&lhs,const reverse_iterator&rhs){
    return lhs.ptr<rhs.ptr;
  }
  friend bool operator<=(const reverse_iterator&lhs,const reverse_iterator&rhs){
    return lhs.ptr>=rhs.ptr;
  }
  friend bool operator>=(const reverse_iterator&lhs,const reverse_iterator&rhs){
    return lhs.ptr<=rhs.ptr;
  }
  private:
  T*ptr;
};

template<class T> struct vector<T>::const_reverse_iterator{
  using value_type=vector<T>::value_type;
  using pointer=vector<T>::pointer;
  using reference=vector<T>::reference;
  using const_reference=vector<T>::const_reference;
  using size_type=vector<T>::size_type;
  using difference_type=vector<T>::difference_type;
  using iterator_category=vector<T>::iterator_category;
  const_reverse_iterator()=default;
  const_reverse_iterator(const_reverse_iterator&&)=default;
  const_reverse_iterator(const const_reverse_iterator&)=default;
  const_reverse_iterator(T*itr):ptr{itr+1}{}
  ~const_reverse_iterator()=default;
  const_reverse_iterator& operator=(const_reverse_iterator&&)=default;
  const_reverse_iterator& operator=(const const_reverse_iterator&)=default;
  const_reverse_iterator& operator++()noexcept{
    --ptr;
    return*this;
  }
  const_reverse_iterator& operator--()noexcept{
    ++ptr;
    return*this;
  }
  const_reverse_iterator operator++(int)noexcept{
    const_reverse_iterator ret=*this;
    --ptr;
    return ret;
  }
  const_reverse_iterator operator--(int)noexcept{
    const_reverse_iterator ret=*this;
    ++ptr;
    return ret;
  }

  T& operator*()noexcept{ return *(ptr-1); }
  const T& operator*()const noexcept{ return *(ptr-1); }

  T* operator->()noexcept{ return ptr-1; }
  const T* operator->()const noexcept{ return ptr-1; }

  explicit operator bool() const noexcept { return ptr!=nullptr; }
  bool operator!() const noexcept{ return !static_cast<bool>(*this); }

  const_reverse_iterator& operator+=(size_type n){
    ptr-=n;
    return*this;
  }
  const_reverse_iterator& operator-=(size_type n){
    ptr+=n;
    return*this;
  }

  friend const_reverse_iterator operator+(const const_reverse_iterator&lhs,size_type rhs){
    return const_reverse_iterator(lhs)-=rhs;
  }  
  friend const_reverse_iterator operator+(size_type lhs,const const_reverse_iterator&rhs){
    return const_reverse_iterator(rhs)-=lhs;
  }

  friend const_reverse_iterator operator-(const const_reverse_iterator&lhs,size_type rhs){
    return const_reverse_iterator(lhs)+=rhs;
  }  
  friend difference_type operator-(const const_reverse_iterator&lhs,const const_reverse_iterator&rhs){
    return rhs.ptr-lhs.ptr;
  }

  friend bool operator==(const const_reverse_iterator&lhs,const const_reverse_iterator&rhs){
    return lhs.ptr==rhs.ptr;
  }
  friend bool operator!=(const const_reverse_iterator&lhs,const const_reverse_iterator&rhs){
    return lhs.ptr!=rhs.ptr;
  }
  friend bool operator<(const const_reverse_iterator&lhs,const const_reverse_iterator&rhs){
    return lhs.ptr>rhs.ptr;
  }
  friend bool operator>(const const_reverse_iterator&lhs,const const_reverse_iterator&rhs){
    return lhs.ptr<rhs.ptr;
  }
  friend bool operator<=(const const_reverse_iterator&lhs,const const_reverse_iterator&rhs){
    return lhs.ptr>=rhs.ptr;
  }
  friend bool operator>=(const const_reverse_iterator&lhs,const const_reverse_iterator&rhs){
    return lhs.ptr<=rhs.ptr;
  }
  private:
  const T*ptr;
};

template<class T>
vector<T>::iterator
vector<T>::begin()noexcept{ return data_; }

template<class T>
vector<T>::iterator
vector<T>::end()noexcept{ return data_+size_; }
template<class T>
vector<T>::const_iterator
vector<T>::begin()const noexcept{ return data_; }

template<class T>
vector<T>::const_iterator
vector<T>::end()const noexcept{ return data_+size_; }

template<class T>
vector<T>::const_iterator
vector<T>::cbegin()const noexcept{ return data_; }

template<class T>
vector<T>::const_iterator
vector<T>::cend()const noexcept{ return data_+size_; }

template<class T>
vector<T>::reverse_iterator
vector<T>::rbegin()noexcept{ return data_+size_-1; }

template<class T>
vector<T>::reverse_iterator
vector<T>::rend()noexcept{ return data_-1; }

template<class T>
vector<T>::const_reverse_iterator
vector<T>::rbegin()const noexcept{ return data_+size_-1; }

template<class T>
vector<T>::const_reverse_iterator
vector<T>::rend()const noexcept{ return data_-1; }

template<class T>
vector<T>::const_reverse_iterator
vector<T>::crbegin()const noexcept{ return data_+size_-1; }

template<class T>
vector<T>::const_reverse_iterator
vector<T>::crend()const noexcept{ return data_-1; }

} // namespace elsie

#endif