#ifndef ELSIE_HOLDER
#define ELSIE_HOLDER
#include <compare>
#include <utility>
#include <concepts>
#include <type_traits>
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

template<class key_t,class val_t=null_t>
struct holder{
  key_t key;
  val_t val;
  ~holder()=default;
  holder()=default;
  holder(const holder&rhs)=default;
  holder(holder&&rhs)=default;
  holder&operator=(const holder&rhs)=default;
  holder&operator=(holder&&rhs)=default;
  template<class KEY_T,class VAL_T>
  requires std::same_as<std::decay_t<KEY_T>, key_t>
        && std::same_as<std::decay_t<VAL_T>, val_t>
  holder(KEY_T&&key,VAL_T&&val)
    :key(std::forward<KEY_T>(key))
    ,val(std::forward<VAL_T>(val)){}
  template<class KEY_T>
  requires std::same_as<std::decay_t<KEY_T>, key_t>
  holder(KEY_T&&key):key(std::forward<KEY_T>(key)),val(){}
};

template<class key_t>
struct holder<key_t,null_t>{
  key_t key; // EBOではなく特殊化
  ~holder()=default;
  holder()=default;
  holder(const holder&rhs)=default;
  holder(holder&&rhs)=default;
  holder&operator=(const holder&rhs)=default;
  holder&operator=(holder&&rhs)=default;
  holder(const key_t&key):key(key){}
  holder(key_t&&key):key(std::move(key)){}
};

}
#endif