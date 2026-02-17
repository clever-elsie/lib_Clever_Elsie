// SPDX-License-Identifier: MIT
// Copyright (c) 2026 CleverElsie

#ifndef ELSIE_HASH_MAP
#define ELSIE_HASH_MAP
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <concepts>
#include <type_traits>
#include "dtStrc/null_t.hpp"
namespace elsie{

/**
 * @param key_t キーの型
 * @param val_t 値の型
 * @param Hash ハッシュ関数の型
 * @param Equal 等値比較関数の型
 */
template<class key_t, class val_t, bool is_multi,
  class Hash=std::hash<key_t>,
  class Equal=std::equal_to<key_t>,
  class Allocator=std::allocator<
    std::conditional_t<(std::same_as<val_t, null_t>),
      key_t,
      std::pair<const key_t, val_t>
    >
  >
>
class hash_map_SoA{
  constexpr static bool is_set = std::same_as<val_t, null_t>;
  constexpr static size_t penalty_threshold = 10;
  size_t penalty;
  public:
  using key_type = key_t;
  using value_type = std::conditional_t<is_set, const key_t, std::pair<const key_t, val_t>>;
  using mapped_type = val_t;
  using hasher = Hash;
  using key_equal = Equal;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
};

} // namespace elsie
#endif