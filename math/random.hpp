#ifndef ELSIE_RANDOM_HPP
#define ELSIE_RANDOM_HPP

#include <cstddef>
#include <cstdint>
#include <bit>
#include <random>
#include <concepts>
#include <type_traits>

namespace elsie{

template<size_t N=64>
class xor_shift{
  static_assert(N==32||N==64||N==128,"N must be 32, 64 or 128");
  using uint_t=std::conditional_t<N==32,uint32_t,
    std::conditional_t<N==64,uint64_t,__uint128_t>>;
  uint_t x;
public:
  xor_shift()=delete;
  xor_shift(uint_t x):x(x){}
  xor_shift(const xor_shift&x)=default;
  xor_shift&operator=(const xor_shift&x)=default;
  xor_shift(xor_shift&&x)=default;
  xor_shift&operator=(xor_shift&&x)=default;
  
  uint_t operator()(){
    if constexpr(N==128){
      x^=x<<11;
      x^=x>>8;
      x^=x<<19;
    }else{
      x^=x<<13;
      x^=x>>17;
      if constexpr(N==32) x^=x<<5;
      else x^=x<<17;
    }
    return x;
  }
};

} // namespace elsie

#endif