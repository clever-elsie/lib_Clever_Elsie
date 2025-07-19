#ifndef ELSIE_HOLDER
#define ELSIE_HOLDER
#include <type_traits>
namespace elsie{
struct null_t{};
bool operator==(const null_t&,const null_t&){ return true; }
bool operator<(const null_t&,const null_t&){ return false; }

// 第三テンプレート引数を使用者が与えてはならない．
// もしそのbool値が正しければ問題ないが，そうでない場合はstatic_assertで落とす．
template<class key_t,class val_t,bool null_val=std::same_as<std::decay_t<val_t>,null_t>>
struct holder{
  static_assert(null_val==std::same_as<std::decay_t<val_t>,null_t>);
  key_t key;
  val_t val;
  ~holder()=default;
  holder()=default;
  holder(const holder&rhs)=default;
  holder(holder&&rhs)=default;
  holder&operator=(const holder&rhs)=default;
  holder&operator=(holder&&rhs)=default;
  holder(const key_t&key,const val_t&val):key(key),val(val){}
  holder(const key_t&key,val_t&&val):key(key),val(std::move(val)){}
  holder(key_t&&key,const val_t&val):key(std::move(key)),val(val){}
  holder(key_t&&key,val_t&&val):key(std::move(key)),val(std::move(val)){}
  holder(const key_t&key):key(key),val(val_t()){}
  holder(key_t&&key):key(std::move(key)),val(val_t()){}
  friend bool operator<(const holder&lhs,const holder&rhs){ return lhs.key<rhs.key; }
  friend bool operator==(const holder&lhs,const holder&rhs){ return lhs.key==rhs.key; }
};

// 特殊化側は正しく動作している場合に選ばれるので，static_assertは不要．
template<class key_t>
struct holder<key_t,null_t,true>{
  key_t key;
  ~holder()=default;
  holder()=default;
  holder(const holder&rhs)=default;
  holder(holder&&rhs)=default;
  holder&operator=(const holder&rhs)=default;
  holder&operator=(holder&&rhs)=default;
  holder(const key_t&key):key(key){}
  holder(key_t&&key):key(std::move(key)){}
  friend bool operator<(const holder&lhs,const holder&rhs){ return lhs.key<rhs.key; }
  friend bool operator==(const holder&lhs,const holder&rhs){ return lhs.key==rhs.key; }
};

}
#endif