#ifndef ELSIE_TREAP
#define ELSIE_TREAP
#include <mutex>
#include <random>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <concepts>
#include <functional>
#include <type_traits>
#include "dtStrc/holder.hpp"
namespace elsie{

template<class T>
class implicit_treap{
  struct node{
    enum way{L,R};
    node*p;
    node*ch[2]; // L=0, R=1;
    size_t size,prior;
    T val;
    ~node(){ delete ch[L]; delete ch[R]; }
    // default constructor
    node():p(nullptr),ch{nullptr,nullptr},size(1),prior(rng()),T(){}
    node(const T&val,node*par=nullptr,node*l=nullptr,node*r=nullptr)
    :p(par),ch{l,r},size(1+(l?l->size:0)+(r?r->size:0)),prior(rng()),val(val){}
    node(T&&val,node*par=nullptr,node*l=nullptr,node*r=nullptr)
    :p(par),ch{l,r},size(1+(l?l->size:0)+(r?r->size:0)),prior(rng()),val(std::move(val)){}

    // manipulator
    void update(){ size=1+(ch[L]?ch[L]->size:0)+(ch[R]?ch[R]->size:0); }
    void push(){
    }
    node* copy()const{
      node*ret=new node(val);
      if(ch[L]) ret->ch[L]=ch[L]->copy();
      if(ch[R]) ret->ch[R]=ch[R]->copy();
      ret->update();
      return ret;
    }
  };

  std::mt19937_64 rng;
  node* root;
  size_t sz;
  static std::mutex mtx;
  static std::random_device rd;


public:
  using value_type=T;
  using reference=T&;
  using const_reference=const T&;
  using pointer=T*;
  using const_pointer=const T*;
  using difference_type=std::ptrdiff_t;
  using size_type=size_t;
  using iterator_category=std::bidirectional_iterator_tag;
  class iterator{
    implicit_treap*self;
    node*x;
    public:
    iterator(implicit_treap*self,node*x):self(self),x(x){}
    friend bool operator==(const iterator&lhs,const iterator&rhs){ return lhs.x==rhs.x; }
    friend bool operator!=(const iterator&lhs,const iterator&rhs){ return lhs.x!=rhs.x; }
  };
  class const_iterator{
    public:
    const_iterator(const implicit_treap*self,node*x):iterator(self,x){}
    const_iterator(const iterator&it):iterator(it){}
  };
  class reverse_iterator{
  };
  class const_reverse_iterator{
  };
  ~implicit_treap(){ delete root; }
  implicit_treap():rng(),root(nullptr),sz(0){
    std::lock_guard<std::mutex>lk(mtx);
    rng.seed(rd());
  }
  implicit_treap(implicit_treap&&rhs):rng{std::move(rhs.rng)},root{rhs.root},sz{rhs.sz}{
    rhs.root=nullptr;
    rhs.sz=0;
  }
  implicit_treap(const implicit_treap&rhs){
    clear();
    if(rhs.root){
      root=rhs.root->copy();
      sz=rhs.sz;
    }
  }
  implicit_treap&operator=(implicit_treap&&rhs){
    if(this==&rhs) return *this;
    clear();
    rng=std::move(rhs.rng);
    root=rhs.root;
    sz=rhs.sz;
  }
  implicit_treap&operator=(const implicit_treap&rhs){
    if(this==&rhs) return *this;
    clear();
    if(rhs.root){
      root=rhs.root->copy();
      sz=rhs.sz;
    }
    return *this;
  }

  // accessors
  size_t size()const{ return sz; }
  bool empty()const{ return sz==0; }

  // manipulator
  void clear(){
    delete root;
    root=nullptr;
    sz=0;
  }
  
  private:
  std::pair<node*,node*> split(node*t,size_t pos){

  }
}; // implicit_treap

} // namespace elsie
#endif