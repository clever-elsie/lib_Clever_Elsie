// SPDX-License-Identifier: MIT
// Copyright (c) 2026 CleverElsie

#ifndef ELSIE_NULL_T
#define ELSIE_NULL_T

#include <compare>

namespace elsie{

struct null_t{ // null_tは{\empty}の集合
  null_t()=default;
  null_t(null_t&&)=default;
  null_t(const null_t&)=default;
  null_t&operator=(null_t&&)=default;
  null_t&operator=(const null_t&)=default;
};

constexpr auto operator<=>(const null_t&,const null_t&){
  return std::strong_ordering::equal;
}

}
#endif