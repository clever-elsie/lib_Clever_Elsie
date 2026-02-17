// SPDX-License-Identifier: MIT
// Copyright (c) 2026 CleverElsie

#pragma once
#include <memory>
#include <cstddef>
#include <concepts>

namespace elsie{

template<class T>
class observer_ptr{
  T* ptr_;
  public:
  using pointer=T*;
  using const_pointer=const T*;
  using element_type=T;
  using value_type=T;
  // constructor
  constexpr observer_ptr()noexcept:ptr_{nullptr}{}
  constexpr observer_ptr(const observer_ptr&)=default;
  constexpr observer_ptr(observer_ptr&&)=default;
  constexpr observer_ptr(std::nullptr_t)noexcept:ptr_{nullptr}{}
  template<class T2>
  requires std::convertible_to<T2*,pointer>
  constexpr explicit observer_ptr(T2* ptr)noexcept:ptr_{ptr}{}
  template<class T2>
  requires std::convertible_to<T2*,pointer>
  constexpr observer_ptr(observer_ptr<T2> other)noexcept:ptr_{other.ptr_}{}
  template<class T2>
  requires std::convertible_to<T2*,pointer>
  constexpr observer_ptr(const std::unique_ptr<T2>&other):ptr_{other.get()}{}
  template<class T2>
  requires std::convertible_to<T2*,pointer>
  constexpr observer_ptr(std::unique_ptr<T2>&&other)=delete;
  // destructor
  constexpr ~observer_ptr()=default;
  // operator=
  constexpr observer_ptr& operator=(const observer_ptr&)=default;
  constexpr observer_ptr& operator=(observer_ptr&&)=default;
  // modifiers
  constexpr pointer release()noexcept{
    pointer ret=ptr_;
    ptr_=nullptr;
    return ret;
  }
  constexpr void reset(pointer ptr=nullptr)noexcept{
    ptr_=ptr;
  }
  constexpr void swap(observer_ptr&other)noexcept{
    pointer tmp=ptr_;
    ptr_=other.ptr_;
    other.ptr_=tmp;
  }
  // observer
  constexpr pointer get()const noexcept{
    return ptr_;
  }
  constexpr explicit operator bool()const noexcept{
    return ptr_!=nullptr;
  }
  constexpr std::add_lvalue_reference_t<element_type> operator*()const noexcept{
    return *ptr_;
  }
  constexpr pointer operator->()const noexcept{
    return ptr_;
  }
  constexpr operator pointer()const noexcept{
    return ptr_;
  }
};

} // namespace elsie