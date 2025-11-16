#ifndef ELSIE_BUCKET
#define ELSIE_BUCKET

#include <new>
#include <cassert>
#include <cstddef>
#include <utility>
#include <iterator>
#include <concepts>
#include <algorithm>
#include <stdexcept>
#include <initializer_list>

namespace elsie{
// コンパイル時に最大容量が決まっている必要がある．
// boolの特殊化は宿題として，現時点ではstd::vector<bool>かstd::bitset<N>を使うべき．
template<class T,size_t N>
class bucket{
  static_assert(N>0);
  alignas(T) std::byte arr[sizeof(T)*N];
  size_t size_;
public:
  using value_type=T;
  using reference=T&;
  using const_reference=const T&;
  using pointer=T*;
  using const_pointer=const T*;
  using size_type=std::size_t;
  using difference_type=std::ptrdiff_t;
  using iterator=pointer;
  using const_iterator=const_pointer;
  using reverse_iterator=std::reverse_iterator<iterator>;
  using const_reverse_iterator=std::reverse_iterator<const_iterator>;
  constexpr ~bucket();
  constexpr bucket()noexcept;
  constexpr bucket(bucket&&x);
  constexpr bucket(const bucket&x);
  constexpr bucket(size_t size,const_reference x=T())noexcept(false);
  explicit constexpr bucket(std::initializer_list<value_type>init)noexcept(false);
  template<std::random_access_iterator InputIterator>
  constexpr bucket(InputIterator first,InputIterator last)noexcept(false);
  template<std::forward_iterator InputIterator>
  requires (!std::random_access_iterator<InputIterator>)
  constexpr bucket(InputIterator first,InputIterator last)noexcept(false);
  constexpr bucket& operator=(bucket&&x);
  constexpr bucket& operator=(const bucket&&x);
  constexpr bucket& operator=(std::initializer_list<value_type>x)noexcept(false);

  constexpr size_type size()const noexcept;
  constexpr size_type capacity()const noexcept;
  constexpr bool empty()const noexcept;
  constexpr pointer data()noexcept;
  constexpr const_pointer data()const noexcept;

  constexpr void resize(size_type size,const_reference x=T())noexcept(false);  
  constexpr reference operator[](size_type idx);
  constexpr const_reference operator[](size_type idx)const;
  constexpr reference at(size_type idx)noexcept(false);
  constexpr const_reference at(size_type idx)const noexcept(false);
  constexpr reference front()noexcept(false);
  constexpr const_reference front()const noexcept(false);
  constexpr reference back()noexcept(false);
  constexpr const_reference back()const noexcept(false);

  constexpr void fill(const_reference x);
  constexpr void swap(bucket&x);
  constexpr void clear();
  constexpr void push_back(const_reference x)noexcept(false);
  constexpr void push_back(value_type&&x)noexcept(false);
  template<class... Args>
  constexpr reference emplace_back(Args&&... args)noexcept(false);
  constexpr void pop_back()noexcept(false);

  constexpr iterator begin()noexcept;
  constexpr const_iterator begin()const noexcept;
  constexpr const_iterator cbegin()const noexcept;
  constexpr iterator end()noexcept;
  constexpr const_iterator end()const noexcept;
  constexpr const_iterator cend()const noexcept;
  constexpr reverse_iterator rbegin()noexcept;
  constexpr const_reverse_iterator rbegin()const noexcept;
  constexpr const_reverse_iterator crbegin()const noexcept;
  constexpr reverse_iterator rend()noexcept;
  constexpr const_reverse_iterator rend()const noexcept;
  constexpr const_reverse_iterator crend()const noexcept;

  constexpr static size_type max_size()noexcept;
  constexpr friend bool operator==(const bucket&lhs,const bucket&rhs);
  constexpr friend auto operator<=>(const bucket&lhs,const bucket&rhs);
};

template<class T,size_t N>
constexpr inline bucket<T,N>::~bucket(){ clear(); }

template<class T,size_t N>
constexpr inline bucket<T,N>::bucket()noexcept:size_{0}{}

template<class T,size_t N>
constexpr inline
bucket<T,N>::bucket(bucket&&x):size_{x.size()}{
  for(size_t i=0;i<size_;++i)
    new (data()+i) T(std::move(x[i]));
}

template<class T,size_t N>
constexpr inline
bucket<T,N>::bucket(const bucket&x):size_{x.size()}{
  for(size_t i=0;i<size_;++i)
    new (data()+i) T(x[i]);
}

template<class T,size_t N>
constexpr inline
bucket<T,N>::bucket(size_t size,const_reference x)noexcept(false):size_{size}{
  if(size>N) throw std::out_of_range("bucket constructor : too many elements");
  for(size_t i=0;i<size_;++i)
    new (data()+i) T(x);
}

template<class T,size_t N>
constexpr inline
bucket<T,N>::bucket(std::initializer_list<value_type>init)noexcept(false):size_{init.size()}{
  if(size_>N) throw std::out_of_range("bucket constructor : too many elements");
  T* const ptr=init.begin();
  for(size_t i=0;i<size_;++i)
    new (data()+i) T(init.begin()[i]);
}

template<class T,size_t N>
template<std::random_access_iterator InputIterator>
constexpr inline
bucket<T,N>::bucket(InputIterator first,InputIterator last)noexcept(false)
:size_(std::distance(first,last)){
  if(size_>N) throw std::out_of_range("bucket constructor: too many elements");
  for(size_t i=0;i<size_;++i)
    new (data()+i) T(*first++);
}

template<class T,size_t N>
template<std::forward_iterator InputIterator>
requires (!std::random_access_iterator<InputIterator>)
constexpr inline
bucket<T,N>::bucket(InputIterator first,InputIterator last)noexcept(false):size_{0}{
  for(;first!=last && size_!=N;++first)
    new (itr++) T(*first++);
  if(first!=last) throw std::out_of_range("bucket constructor: too many elements");
}

template<class T,size_t N>
constexpr inline
bucket<T,N>& bucket<T,N>::operator=(bucket&&x){
  for(auto itr=begin()+x.size();itr<end();++itr)
    itr->~T();
  const size_t sz=size_;
  if(x.size()>sz)
    for(;size_<x.size();++size_)
      new (data()+size_) T(std::move(x[size_]));
  std::move(x.begin(),x.begin()+sz,begin());
  x.size_=0;
}

template<class T,size_t N>
constexpr inline
bucket<T,N>& bucket<T,N>::operator=(const bucket&&x){
  for(auto itr=begin()+x.size();itr<end();++itr)
    itr->~T();
  const size_t sz=size_;
  if(x.size()>sz)
    for(;size_<x.size();++size_)
      new (data()+size_) T(std::copy(x[size_]));
  std::copy(x.begin(),x.begin()+sz,begin());
}

template<class T,size_t N>
constexpr inline
bucket<T,N>& bucket<T,N>::operator=(std::initializer_list<value_type>x)noexcept(false){
  *this=bucket(x);
}

template<class T,size_t N>
constexpr inline
bucket<T,N>::size_type 
bucket<T,N>::size()const noexcept{ return size_; }

template<class T,size_t N>
constexpr inline
bucket<T,N>::size_type
bucket<T,N>::capacity()const noexcept{ return N-size_; }

template<class T,size_t N>
constexpr inline
bool bucket<T,N>::empty()const noexcept{ return size_==0; }

template<class T,size_t N>
constexpr inline
bucket<T,N>::pointer
bucket<T,N>::data()noexcept{ return reinterpret_cast<T*>(arr); }

template<class T,size_t N>
constexpr inline
bucket<T,N>::const_pointer
bucket<T,N>::data()const noexcept{ return reinterpret_cast<const T*>(arr); }

// f
template<class T,size_t N>
constexpr inline
void bucket<T,N>::resize(size_type size,const_reference x)noexcept(false){
  if(size==size_) return;
  if(size<size_)
    while(size_>size)
      data()[--size_].~T();
  else{
    if(size>N) throw std::out_of_range("bucket resize : too long size");
    while(size_<size)
      new(arr+size++) T(x);
  }
}

template<class T,size_t N>
constexpr inline
bucket<T,N>::reference 
bucket<T,N>::operator[](size_type idx){ return data()[idx]; }

template<class T,size_t N>
constexpr inline
bucket<T,N>::const_reference
bucket<T,N>::operator[](size_type idx)const{ return data()[idx]; }

template<class T,size_t N>
constexpr inline
bucket<T,N>::reference
bucket<T,N>::at(size_type idx)noexcept(false){
  if(idx>=size_) std::out_of_range("bucket at : out of range");
  return data()[idx];
}

template<class T,size_t N>
constexpr inline
bucket<T,N>::const_reference
bucket<T,N>::at(size_type idx)const noexcept(false){
  if(idx>=size_) std::out_of_range("bucket at : out of range");
  return data()[idx];
}

template<class T,size_t N>
constexpr inline
bucket<T,N>::reference
bucket<T,N>::front()noexcept(false){ return*data(); }

template<class T,size_t N>
constexpr inline
bucket<T,N>::const_reference
bucket<T,N>::front()const noexcept(false){ return*data(); }

template<class T,size_t N>
constexpr inline
bucket<T,N>::reference
bucket<T,N>::back()noexcept(false){ return*(data()+size_-1); }

template<class T,size_t N>
constexpr inline
bucket<T,N>::const_reference
bucket<T,N>::back()const noexcept(false){ return*(data()+size_-1); }

template<class T,size_t N>
constexpr inline
void bucket<T,N>::fill(const_reference x){
  std::fill_n(begin(),size_,x);
}

template<class T,size_t N>
constexpr inline
void bucket<T,N>::swap(bucket&x){
  const size_t min=std::min(size_,x.size());
  for(size_t i=0;i<min;++i)
    std::swap(arr[i],x.arr[i]);
  if(size_>min)
    for(size_t i=min;i<size_;++i)
      new (x.data()+i) T(std::move((*this)[i]));
  else if(x.size()>min)
    for(size_t i=min;i<x.size();++i)
      new (data()+i) T(std::move(x.data()[i]));
}

template<class T,size_t N>
constexpr inline
void bucket<T,N>::clear(){
  const auto end_=cend();
  for(auto itr=begin();itr!=end_;++itr)
    itr->~T();
  size_=0;
}

template<class T,size_t N>
constexpr inline
void bucket<T,N>::push_back(const_reference x)noexcept(false){ emplace_back(x); }

template<class T,size_t N>
constexpr inline
void bucket<T,N>::push_back(value_type&&x)noexcept(false){ emplace_back(std::move(x)); }

template<class T,size_t N>
template<class... Args>
constexpr inline
bucket<T,N>::reference
bucket<T,N>::emplace_back(Args&&... args)noexcept(false){
  if(N==size_) throw std::out_of_range("bucket push_back : container is already full");
  return*new(begin()+size_++) T(std::forward<Args>(args)...);
}

template<class T,size_t N>
constexpr inline
void pop_back()noexcept(false){
  if(size_==0) throw std::range_error("bucket pop_back : container is already empty");
  begin()[--size_].~T();
}

template<class T,size_t N>
constexpr inline
bucket<T,N>::iterator
bucket<T,N>::begin()noexcept{ return data(); }

template<class T,size_t N>
constexpr inline
bucket<T,N>::const_iterator
bucket<T,N>::begin()const noexcept{ return data(); }

template<class T,size_t N>
constexpr inline
bucket<T,N>::const_iterator
bucket<T,N>::cbegin()const noexcept{ return data(); }

template<class T,size_t N>
constexpr inline
bucket<T,N>::iterator
bucket<T,N>::end()noexcept{ return data()+size_; }

template<class T,size_t N>
constexpr inline
bucket<T,N>::const_iterator
bucket<T,N>::end()const noexcept{ return cend(); }

template<class T,size_t N>
constexpr inline
bucket<T,N>::const_iterator
bucket<T,N>::cend()const noexcept{ return data()+size_; }

template<class T,size_t N>
constexpr inline
bucket<T,N>::reverse_iterator
bucket<T,N>::rbegin()noexcept{ return reverse_iterator(end()); }

template<class T,size_t N>
constexpr inline 
bucket<T,N>::const_reverse_iterator
bucket<T,N>::rbegin()const noexcept{ return crbegin(); }

template<class T,size_t N>
constexpr inline
bucket<T,N>::const_reverse_iterator
bucket<T,N>::crbegin()const noexcept{ return const_reverse_iterator(end()); }

template<class T,size_t N>
constexpr inline
bucket<T,N>::reverse_iterator
bucket<T,N>::rend()noexcept{ return reverse_iterator(begin()); }

template<class T,size_t N>
constexpr inline
bucket<T,N>::const_reverse_iterator
bucket<T,N>::rend()const noexcept{ return crend(); }

template<class T,size_t N>
constexpr inline
bucket<T,N>::const_reverse_iterator
bucket<T,N>::crend()const noexcept{ return const_reverse_iterator(begin()); }

template<class T,size_t N>
constexpr inline
bucket<T,N>::size_type
bucket<T,N>::max_size()noexcept{ return N; }

template<class T,size_t N>
constexpr inline
bool operator==(const bucket<T,N>&lhs,const bucket<T,N>&rhs){
  if(lhs.size()!=rhs.size()) return false;
  for(size_t i=0;i!=lhs.size();++i)
    if(lhs[i]!=rhs[i])
      return false;
  return true;
}

template<class T,size_t N>
constexpr inline
auto operator<=>(const bucket<T,N>&lhs,const bucket<T,N>&rhs){
  return std::lexicographical_compare_three_way(lhs.begin(),lhs.end(),rhs.begin(),rhs.end());
}

}//namespace elsie
#endif