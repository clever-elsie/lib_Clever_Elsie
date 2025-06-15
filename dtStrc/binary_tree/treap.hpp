#ifndef ELSIE_TREAP
#define ELSIE_TREAP
#include <random>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <type_traits>
namespace elsie{

template<class key_t,class val_t,class cmp=std::less<key_t>>
class implicit_treap{
public:
  class null_t{};
protected:
  template<bool is_null_t=false>
  struct holder{ val_t val; };
  template<> struct holder<true>{};
  struct node:public holder<std::same_as<std::decay_t<val_t>,null_t>>{
    key_t key;
    node*p,*l,*r;
    size_t size,prior;
    void update(){ size=1+(l?l->size)+(r?r->size); }
  };
  using np=node*;
  np root;
public:
  implicit_treap()=default;
  implicit_treap(implicit_treap&&)=default;
  implicit_treap(const implicit_treap&);
  
}; // implicit_treap

} // namespace elsie
#endif