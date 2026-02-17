// SPDX-License-Identifier: MIT
// Copyright (c) 2026 CleverElsie

#ifndef ELSIE_SVECTOR
#define ELSIE_SVECTOR
#include <cstddef>
#include <cstdint>
#include <memory>
#include <array>
#include <iterator>
#include <initializer_list>
#include <algorithm>
#include <concepts>
#include <type_traits>
#include <ranges>
#include <new>
#include <compare>

namespace elsie{

// vector with currious strategy
template<class T>
class svector final{
  public:
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using pointer = allocator_traits::pointer;
  using const_pointer = allocator_traits::const_pointer;
  using allocator_type = Allocator;
  private:
  template<bool is_const, bool is_reverse>
  struct iter:public std::random_access_iterator_tag{
    using iterator_category = std::random_access_iterator_tag;
    using value_type = svector::value_type;
    using pointer = svector::pointer;
    using reference = svector::reference;
    using difference_type = svector::difference_type;
    iter()=delete; // これは禁止
    iter(svector* vec, size_t idx):vec(vec), idx(idx){}
    iter(iter&&)=default;
    iter(const iter&)=default;
    iter& operator=(iter&&)=default;
    iter& operator=(const iter&)=default;
    iter& operator++(){ ++idx; return *this; }
    iter& operator--(){ --idx; return *this; }
    iter operator++(int32_t){ iter cur=*this; ++*this; return cur; }
    iter operator--(int32_t){ iter cur=*this; --*this; return cur; }
    auto& operator*(){
      if constexpr(is_reverse) return (*vec)[-(ssize_t)idx-1];
      else return (*vec)[idx];
    }
    auto operator->(){
      if constexpr(is_reverse) return &(*vec)[-(ssize_t)idx-1];
      else return &(*vec)[idx];
    }
    auto& operator*()const{
      if constexpr(is_reverse) return (*vec)[-(ssize_t)idx-1];
      else return (*vec)[idx];
    }
    auto operator->()const{
      if constexpr(is_reverse) return &(*vec)[-(ssize_t)idx-1];
      else return &(*vec)[idx];
    }
    template<bool is_const1>
    bool operator==(const iter<is_const1, is_reverse>& other)const{ return vec==other.vec&&idx==other.idx; }
    template<bool is_const1>
    auto operator<=>(const iter<is_const1, is_reverse>&other)const{
      if(vec!=other.vec) throw std::domain_error("iter is not belonging to the same vector");
      return idx<=>other.idx;
    }

    private:
    svector* vec;
    size_t idx;
  };
  public:
  using iterator = iter<false, false>;
  using const_iterator = iter<true, false>;
  using reverse_iterator = iter<false, true>;
  using const_reverse_iterator = iter<true, true>;
  private:
  template<size_t minExp, size_t elemsSizeExp>
  struct segment_info_t{
    static_assert(minExp<=elemsSizeExp);
    using segments_type=std::array<std::unique_ptr<T[]>, sizeof_segments()>;
    using each_segment_size_type=std::array<size_t, sizeof_segments()>;
    using segments_prefix_sum_type=std::array<size_t, sizeof_segments()+1>;
    constexpr static size_t min_exp=minExp, max_elem_size_exp=elemsSizeExp;
    constexpr static size_t segments_size=sizeof_segments();
    constexpr static each_segment_size_type each_size=each_segment_size_of_segments();
    constexpr static segments_prefix_sum_type prefix_sum=prefix_sum_of_segments();
    static constexpr size_t which_segment(size_t idx){
      size_t ok=0, ng=segments_size+1;
      while(ng-ok>1)
        if(size_t mid=(ok+ng)>>1;prefix_sum[mid]<=idx) ok=mid;
        else ng=mid;
      return ok;
    }
    private:
    static consteval size_t sizeof_segments(){
      // 2^minimum_exp, 2^minimum_exp, 2^{minimum_exp+1}, 2^{minimum_exp+1}, ... , 2^{max_elem_size_exp-1}
      return 1+(elemsSizeExp-minExp);
    }
    static consteval auto each_segment_size_of_segments()->each_segment_size_type{
      each_segment_size_type ret;
      ret[0]=1ull<<minExp;
      for(size_t i=1;i<ret.size();++i)
        ret[i]=1ull<<(minExp+i-1);
      return ret;
    }
    static consteval auto prefix_sum_of_segments()->segments_prefix_sum_type{
      segments_prefix_sum_type ret;
      ret[0]=0, ret[1]=1ull<<minExp;
      for(size_t i=2;i<=ret.size();++i)
        ret[i]=ret[i-1]+(1ull<<(minExp+i-2));
      return ret;
    }
  };
  // fields
  using segment_info_type=segment_info_t<5, 47>;
  size_t cur_size, max_size;
  // 2^5, 2^5, 2^6, 2^7, ... , 2^47 x86-64(のPC向けCPU)では48bit仮想アドレス空間2^48要素で過剰
  constexpr static segment_info_type seginfo=segment_info_type();
  using segments_type=segment_info_type::segments_type;
  segment_info_type::segments_type segments;
  private:
  void minimum_construct(){
    max_size=seginfo.each_size[0];
    segments[0]=std::make_unique_for_overwrite<T[]>(max_size);
    return;
  }
  public:
  // constructor/destructor/operator=
  svector():cur_size(0), max_size(seginfo.each_size[0]), segments(){
    segments[0]=std::make_unique_for_overwrite<T[]>(seginfo.each_size[0]);
  }
  svector(const svector&x):svector(x.begin(),x.end()){} // random_access_iteratorなので，そっちで処理してもらう
  svector(svector&&x):cur_size(x.cur_size), max_size(x.max_size), segments(){
    for(auto&[seg,xseg]:std::views::zip(segments,x.segments))
      if(xseg) seg=std::move(xseg);
      else break;
    x.cur_size=0;
    x.segments.fill(nullptr);
    x.max_size=seginfo.each_size[0];
    x.segments[0]=std::make_unique_for_overwrite<T[]>(x.max_size);
  }
  svector(size_type n):cur_size(n), max_size(0), segments(){
    if(n==0) minimum_construct();
    else
    for(size_type i=0;max_size<n;max_size+=seginfo.each_size[i++])
      segments[i]=std::make_unique_for_overwrite<T[]>(seginfo.each_size[i]);
  }
  svector(size_type n, const T& x):cur_size(n), max_size(), segments(){
    if(n==0) minimum_construct();
    else
    for(size_type i=0;max_size<n;max_size+=seginfo.each_size[i++]){
      segments[i]=std::make_unique_for_overwrite<T[]>(seginfo.each_size[i]);
      std::fill_n(segments[i].get(), std::min(seginfo.each_size[i], n-max_size), x);
    }
  }
  private:
  struct copy_op{
    template<class II, class OI>
    void operator()(II first, II last, OI result)const{
      std::copy(first, last, result);
    }
  };
  struct move_op{
    template<class II, class OI>
    void operator()(II first, II last, OI result)const{
      std::move(first, last, result);
    }
  };
  template<std::random_access_iterator InputIterator, class Op>
  svector(InputIterator first, InputIterator last, Op op):cur_size(std::distance(first, last)), max_size(initial_size), segments(){
    if(cur_size==0) minimum_construct();
    else
    for(size_type i=0;max_size<cur_size;max_size+=seginfo.each_size[i++]){
      segments[i]=std::make_unique_for_overwrite<T[]>(seginfo.each_size[i]);
      op(first, std::min(last, first+seginfo.each_size[i]), segments[i].get());
    }
  }
  public:
  template<std::random_access_iterator InputIterator>
  svector(InputIterator first, InputIterator last):svector(first, last, copy_op()){}
  template<std::move_iterator InputIterator>
    requires std::random_access_iterator<InputIterator::iterator_type>
  svector(InputIterator first, InputIterator last):svector(first, last, move_op()){}
  template<class InputIterator>
  svector(InputIterator first, InputIterator last):svector(){ insert(end(), first, last); }
  template<std::move_iterator InputIterator>
  svector(InputIterator first, InputIterator last):svector(){ insert(end(), first, last); }
  svector(std::initializer_list<T>init):svector(init.begin(), init.end()){}
  ~svector(){
    for(auto&seg:segments)
      if(seg) seg.release();
      else break;
  }
  svector& operator=(const svector&x){
    if(this!=&x){
      this->~svector();
      new(this) svector(x);
    }
    return *this;
  }
  svector& operator=(svector&&x){
    if(this!=&x){
      this->~svector();
      new(this) svector(std::move(x));
    }
    return *this;
  }
  // iterator
  iterator begin()noexcept{return iterator(this,0);}
  const_iterator begin()const noexcept{return const_iterator(this,0);}
  iterator end()noexcept{return iterator(this,cur_size);}
  const_iterator end()const noexcept{return const_iterator(this,cur_size);}
  reverse_iterator rbegin()noexcept{return reverse_iterator(this,0);}
  const_reverse_iterator rbegin()const noexcept{return const_reverse_iterator(this,0);}
  reverse_iterator rend()noexcept{return reverse_iterator(this,cur_size);}
  const_reverse_iterator rend()const noexcept{return const_reverse_iterator(this,cur_size);}
  const_iterator cbegin()const noexcept{return const_iterator(this,0);}
  const_iterator cend()const noexcept{return const_iterator(this,cur_size);}
  const_reverse_iterator crbegin()const noexcept{return const_reverse_iterator(this,0);}
  const_reverse_iterator crend()const noexcept{return const_reverse_iterator(this,cur_size);}
  // domain
  size_type size()const noexcept{return cur_size;}
  ptrdiff_t ssize()const noexcept{return static_cast<ptrdiff_t>(cur_size);}
  size_type max_size()const noexcept{return max_size;}
  bool empty()const noexcept{ return cur_size==0; }
  size_type capacity()const noexcept{ return max_size-cur_size; }
  void reserve(size_type n){
    if(max_size>=n+=cur_size) return;
    for(size_type i=0;i<seginfo.segments_size;++i)
      if(segments[i]) continue;
      else{
        max_size+=seginfo.each_size[i];
        segments[i]=std::make_unique_for_overwrite<T[]>(seginfo.each_size[i]);
        if(max_size>=n) break;
      }
    return;
  }
  private:
  void resize_shrink(size_type n)
  // pre(n<cur_size)
  {
    // TODO:最適化(セグメント毎に処理できる部分はまとめてやるほうがよい)
    for(size_type i=n;i<cur_size;++i)
      (*this)[i].~T();
    cur_size=n;
  }
  public:
  void resize(size_type n){
    if(n<cur_size){
      resize_shrink(n);
      return;
    }
    reserve(n-cur_size);
    cur_size=n;
  }
  void resize(size_type n, const T& x){
    if(n<cur_size){
      resize_shrink(n);
      return;
    }
    reserve(n-cur_size);
    for(size_type i=cur_size;i<n;++i)
      (*this)[i]=x;
    cur_size=n;
  }
  void shrink_to_fit(){
    size_type last_valid_segment=seginfo.which_segment(cur_size-1);
    for(size_type i=last_valid_segment+1;i<seginfo.segments_size;++i)
      if(segments[i]) segments[i].release();
      else break;
    max_size=seginfo.prefix_sum[last_valid_segment+1];
  }
  // accessor
  reference operator[](size_type idx){
    const size_type where=seginfo.which_segment(idx);
    return segments[where][idx-seginfo.prefix_sum[where]];
  }
  reference operator[](size_type idx)const{
    const size_type where=seginfo.which_segment(idx);
    return segments[where][idx-seginfo.prefix_sum[where]];
  }
  reference at(size_type idx)noexcept(false){
    if(idx>=cur_size) throw std::out_of_range("svector at : out of range");
    return (*this)[idx];
  }
  reference at(size_type idx)const noexcept(false){
    if(idx>=cur_size) throw std::out_of_range("svector at : out of range");
    return (*this)[idx];
  }
  reference front()noexcept(false){ return at(0); }
  reference front()const noexcept(false){ return at(0); }
  reference back()noexcept(false){ return at(cur_size-1); }
  reference back()const noexcept(false){ return at(cur_size-1); }
  // modifier
  void push_back(const T& x){
    reserve(1);
    new(&(*this)[cur_size++]) T(x);
  }
  void push_back(T&& x){
    reserve(1);
    new(&(*this)[cur_size++]) T(std::move(x));
  }
  template<class...Args>
  reference emplace_back(Args&&...args){
    reserve(1);
    new(&(*this)[cur_size++]) T(std::forward<Args>(args)...);
  }
  void pop_back()noexcept(false){
    if(cur_size==0) throw std::out_of_range("svector pop_back : container is empty");
    (*this)[cur_size-1].~T();
    --cur_size;
  }
  // insert/emplace/eraseは後で追加できたらいいね
  void swap(svector&x){ std::swap(cur_size,x.cur_size); }
  void clear(){
    for(size_type i=0;i<std::min(cur_size,initial_size);++i)
      segments[0][i].~T();
    for(auto&seg:segments|std::views::drop(1))
      if(seg) seg.release();
      else break;
    cur_size=0;
    max_size=initial_size;
  }
};

} // namespace elsie
#endif