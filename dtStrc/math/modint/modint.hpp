#include <dtStrc/math/modint/static_modint.hpp>
#include <dtStrc/math/modint/dynamic_modint.hpp>

namespace elsie{
template<class T>
struct is_modint{
  constexpr static bool value=requires(const T&x){
    x.inv();
    T::mod();
  };
};

template<class T>
inline constexpr bool is_modint_v=is_modint<T>::value;
}//namespace elsie