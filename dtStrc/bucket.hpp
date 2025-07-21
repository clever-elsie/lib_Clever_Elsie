#ifndef ELSIE_BUCKET
#define ELSIE_BUCKET

#include <cstdint>
#include <cstddef>
#include <iterator>
#include <concepts>
#include <type_traits>
#include <initializer_list>

namespace elsie{
// コンパイル時に最大容量が決まっている必要がある．
// boolの特殊化は宿題として，現時点ではstd::vector<bool>かstd::bitset<N>を使うべき．
template<class T,size_t N>
class bucket{
  static_assert(N>0);
  char arr[N*sizeof(T)];
  size_t size_;
public:
  using value_type=T;
  using reference=T&;
  using const_reference=const T&;
  using pointer=T*;
  using const_pointer=const T*;
  using size_type=std::size_t;
  using difference_type=std::ptrdiff_t;
  struct iterator;
  struct const_iterator;
  using reverse_iterator=std::reverse_iterator<iterator>;
  using const_reverse_iterator=std::reverse_iterator<const_iterator>;
  ~bucket()=default;
  bucket();
  bucket(bucket&&x);
  bucket(const bucket&x);
  explicit bucket(size_t size,const_reference x);
  explicit bucket(std::initializer_list<value_type>init);
  template<class InputIterator>
  bucket(InputIterator first,InputIterator last);
  bucket& operator=(bucket&&x);
  bucket& operator=(const bucket&&x);
  bucket& operator=(std::initializer_list<value_type>x);

  size_type size()const noexcept;
  size_type max_size()const noexcept;
  size_type capacity()const noexcept;
  bool empty()const noexcept;
  pointer data()noexcept;
  const_pointer data()const noexcept;

  void resize(size_type size,const_reference x)const noexcept(false);  
  reference operator[](size_type idx)noexcept(false);
  const_reference operator[](size_type idx)const noexcept(false);
  reference at(size_type idx)noexcept(false);
  const_reference at(size_type idx)const noexcept(false);
  reference front()noexcept(false);
  const_reference front()const noexcept(false);
  reference back()noexcept(false);
  const_reference back()const noexcept(false);

  void fill(const_reference x);
  void swap(bucket&x);
  void clear();
  void push_back(const_reference x);
  void push_back(value_type&&x);
  template<class... Args>
  reference emplace(Args&&... args);
  void pop_back();

  iterator begin();
  const_iterator begin()const;
  const_iterator cbegin()const;
  iterator end();
  const_iterator end()const;
  const_iterator cend()const;
  reverse_iterator rbegin();
  const_reverse_iterator rbegin()const;
  const_reverse_iterator crbegin()const;
  reverse_iterator rend();
  const_reverse_iterator rend()const;
  const_reverse_iterator crend()const;
};

template<class T,size_t N>
struct bucket<T,N>::iterator:public std::contiguous_iterator_tag{
  iterator()=defualt;
  iterator(const iterator&);
  iterator(iterator&&);
  iterator& operator=(const iterator&);
  iterator& operator=(iterator&&);
  friend bool operator==(const iterator&lhs,const iterator&rhs);
  friend bool operator<(const iterator&lhs,const iterator&rhs);
  reference operator*()const;
  pointer operator->()const;
  iterator& operator++();
  iterator operator++(int32_t);
  iterator& operator--();
  iterator operator--(int32_t);
  iterator& operator+=(difference_type n);
  iterator& operator-=(difference_type n);
  reference operator[](difference_type n);
  friend iterator operator+(const iterator&lhs,const iterator&rhs);
  friend iterator operator+(const iterator&lhs,difference_type rhs);
  friend iterator operator+(difference_type lhs,const iterator&rhs);
  friend iterator operator-(const iterator&lhs,const iterator&rhs);
  friend iterator operator-(const iterator&lhs,difference_type rhs);
  private:
  friend class bucket<T,N>;
  pointer ptr;
};

}//namespace elsie
#endif