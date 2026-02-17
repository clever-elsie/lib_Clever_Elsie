// SPDX-License-Identifier: MIT
// Copyright (c) 2026 CleverElsie

#ifndef ELSIE_CONCEPTS
#define ELSIE_CONCEPTS
#include <type_traits>
#include <concepts>
#include <iterator>

namespace elsie{

template<class T, class... Ts>
concept included_in=((std::same_as<T,Ts>||...));

template<class T>
concept Itrabl=requires(const T&x){
  typename T::value_type;
  typename T::iterator;
  typename T::const_iterator;
  requires std::random_access_iterator<typename T::iterator>;
  requires std::random_access_iterator<typename T::const_iterator>;
};

}
#endif