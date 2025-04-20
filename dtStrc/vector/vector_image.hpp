#ifndef ELSIE_VECTOR_IMAGE
#define ELSIE_VECTOR_IMAGE

#include <new>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <compare>
#include <iterator>
#include <stdexcept>
#include <exception>
#include <algorithm>
#include <type_traits>
#include <initializer_list>
#include <immintrin.h>

namespace elsie{
template<class T> class vector{
  public: // types
  using value_type=T;
  using pointer=T*;
  using reference=T&;
  using const_reference=const T&;
  using size_type=std::size_t;
  using difference_type=std::ptrdiff_t;
  using iterator_category=std::random_access_iterator_tag;
  struct iterator;
  struct const_iterator;
  struct reverse_iterator;
  struct const_reverse_iterator;
  public:
  static size_type aligned64_size(size_type n){
    constexpr static const size_type filter=0b11'1111ull-1;
    return ((sizeof(T)*n+filter)&~filter)/sizeof(T);
  }
  public: // function
  // construction, destruction
  vector();
  vector(vector&&);
  vector(const vector&);
  explicit vector(size_t n);
  explicit vector(size_t n,const T&value);
  template<class InputIter> vector(InputIter first,InputIter last);

  ~vector();

  vector& operator=(vector&&x);
  vector& operator=(const vector&x);
  vector& operator=(std::initializer_list<T> init);

  // iterator
  iterator begin()noexcept;
  iterator end()noexcept;
  const_iterator begin()const noexcept;
  const_iterator end()const noexcept;
  const_iterator cbegin()const noexcept;
  const_iterator cend()const noexcept;
  reverse_iterator rbegin()noexcept;
  reverse_iterator rend()noexcept;
  const_reverse_iterator rbegin()const noexcept;
  const_reverse_iterator rend()const noexcept;
  const_reverse_iterator crbegin()const noexcept;
  const_reverse_iterator crend()const noexcept;

  // memory manage
  size_type size()const noexcept;
  size_type max_size()const noexcept;
  void resize(size_type n);
  void resize(size_type n,const T&value);
  size_type capacity()const noexcept;
  bool empty()const noexcept;
  void reserve(size_type n);
  void shrink_to_fit();

  // memory access
  reference operator[](size_type n);
  const_reference operator[](size_type n)const;
  reference at(size_type n);
  reference at(size_type n)const;
  T*data()noexcept;
  const T*data()const noexcept;
  reference front();
  const_reference front()const;
  reference back();
  const_reference back()const;

  // container change
  void push_back(T&&x);
  void push_back(const T&x);
  template<class... Args> reference emplace_back(Args&&... args);
  void pop_back();
  iterator insert(iterator pos,T&&x);
  iterator insert(iterator pos,const T&x);
  void insert(iterator pos,size_type n,const T&x);
  void insert(iterator pos,std::initializer_list<T>il);
  template<class InputIter> void insert(iterator pos,InputIter first,InputIter last);
  template<std::random_access_iterator InputIter> void insert(iterator pos,InputIter first,InputIter last);
  template<class... Args> iterator emplace(iterator pos,Args&&... args);
  iterator erase(iterator pos);
  iterator erase(iterator first,iterator last);
  void swap(vector&x);
  void clear();

  // comparison opeartor
  template<class U,class V>
  friend bool operator==(const vector<U>&lhs,const vector<V>&rhs);
  template<class U,class V>
  friend bool operator<(const vector<U>&lhs,const vector<V>&rhs);
  template<class U,class V>
  friend bool operator>(const vector<U>&lhs,const vector<V>&rhs);
  template<class U,class V>
  friend bool operator<=(const vector<U>&lhs,const vector<V>&rhs);
  template<class U,class V>
  friend bool operator>=(const vector<U>&lhs,const vector<V>&rhs);
  protected: // field
  size_t size_,cap_;
  T*data_;
};
} // namespace elsie
#endif