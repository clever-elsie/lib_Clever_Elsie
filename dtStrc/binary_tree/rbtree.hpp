#ifndef ELSIE_RBTREE
#define ELSIE_RBTREE
#include<any> // swap()
#include<new>
#include<cstdint>
#include<cstddef> // size_t
#include<utility> // forward(), move()
#include<compare>
#include<concepts>
#include<iterator>
#include<functional> // less<>
#include<type_traits>
#include<initializer_list>
#include "dtStrc/holder.hpp"

namespace elsie{

template<class key_t,class val_t,class Cmp=std::less<key_t>,bool allow_duplicate_keys=false>
class rbtree{
  constexpr static bool is_set=std::same_as<val_t, null_t>;
  public:
  using value_type = std::conditional_t<is_set, const key_t, std::pair<const key_t, val_t>>;
  using pointer = value_type*;
  using reference = value_type&;
  protected:
  using mutable_value_type = std::conditional_t<is_set, key_t, std::pair<key_t, val_t>>;
  struct node{
    using ip=node*;
    ip p,ch[2]; // ch0=L, ch1=R
    uint64_t time,size; // time: odd red, even black
    value_type val;
    node()=default;
    node(ip nil):p{nil},ch{nil,nil},time{0},size{1},val(){}

    template<class KEY_T> requires (std::same_as<std::decay_t<KEY_T>,key_t>) && (is_set)
    node(KEY_T&&k,uint64_t tm,ip P,ip L,ip R):p{P},ch{L,R},time{tm},size{1},val(std::forward<KEY_T>(k)){}

    template<class KEY_T,class VAL_T>
    requires (std::same_as<std::decay_t<KEY_T>,key_t>) && (std::same_as<std::decay_t<VAL_T>,val_t>)
    node(KEY_T&&k,VAL_T&&v,uint64_t tm,ip P,ip L,ip R)
    :p{P},ch{L,R},time{tm},size{1},val(std::forward<KEY_T>(k),std::forward<VAL_T>(v)){}

    ~node()=default;
    
    const key_t& key()const{
      if constexpr(is_set) return val;
      else return val.first;
    }
    
    val_t& value(){
      if constexpr(!is_set) return val.second;
      else throw std::invalid_argument("elsie::rbtree node is set");
    }

    const val_t& value()const{
      if constexpr(!is_set) return val.second;
      else throw std::invalid_argument("elsie::rbtree node is set");
    }

    void clear(const node*nil){
      for(size_t i=0;i<2;++i)
        if(nil!=ch[i]){
          ch[i]->clear(nil);
          delete ch[i];
        }
    }
    
    size_t update(){
      return size=1+ch[0]->size+ch[1]->size;
    }
  };
  using np=node*;
  template<bool is_const, bool is_reverse>
  struct iter{
    private:
    public:
    using difference_type = std::ptrdiff_t;
    using value_type = rbtree::value_type;
    using pointer = rbtree::pointer;
    using reference = rbtree::reference;
    using iterator_category = std::bidirectional_iterator_tag;
    iter()=default;
    iter(iter&&)=default;
    iter(const iter&)=default;
    iter(rbtree const* tree, node* ptr)requires (!is_const):tree(tree), ptr(ptr) {}
    iter(rbtree const* tree, node const* ptr)requires (is_const):tree(tree), ptr(ptr) {}
    iter& operator=(iter&&)=default;
    iter& operator=(const iter&)=default;
    
    iter& operator++(){
      ptr=tree->template prev_next<!is_reverse>(ptr);
      return *this;
    }
    iter& operator--(){
      ptr=tree->template prev_next<is_reverse>(ptr);
      return *this;
    }
    iter operator++(int32_t){
      iter cur=*this;
      ++*this;
      return cur;
    }
    iter operator--(int32_t){
      iter cur=*this;
      --*this;
      return cur;
    }
    decltype(auto) operator*(){ return ptr->val; }
    decltype(auto) operator->(){ return &ptr->val; }
    decltype(auto) operator*()const{ return ptr->val; }
    decltype(auto) operator->()const{ return &ptr->val; }
    template<bool is_const1, bool is_reverse1>
    bool operator==(const iter<is_const1,is_reverse1>&itr)const{ return ptr==itr.ptr; }
    template<bool is_const1>
    auto operator<=>(const iter<is_const1, is_reverse>&itr)const{
      if(tree!=itr.tree) throw std::domain_error("iter is not belonging to the same tree");
      return tree->order_of(*this)<=>itr.tree->order_of(itr);
    }
    iter& operator+=(difference_type n){
      auto x=tree->find_by_order(tree->order_of(*this)+n);
      ptr=x.ptr;
      return*this;
    }
    iter& operator-=(difference_type n){
      auto x=tree->find_by_order(tree->order_of(*this)-n);
      ptr=x.ptr;
      return*this;
    }
    iter operator+(difference_type n)const{
      return iter(tree,ptr)+=n;
    }
    iter operator-(difference_type n)const{
      return iter(tree,ptr)-=n;
    }
    template<bool is_const1>
    difference_type operator-(const iter<is_const1,is_reverse>&itr)const{
      if(tree!=itr.tree) throw std::domain_error("iter is not belonging to the same tree");
      return tree->order_of(*this)-tree->order_of(itr);
    }
    template<bool is_const1,bool is_reverse1>
    explicit operator iter<is_const1, is_reverse1>()const{
      return iter<is_const1, is_reverse1>(tree,ptr);
    }
    private:
    rbtree const* tree;
    std::conditional_t<is_const,node const*, node*> ptr;
    friend class rbtree;
    void is_belonging_to(const rbtree*tree)const{
      #ifdef DEBUG
      if(tree!=this->tree)
        throw std::invalid_argument("iter is not belonging to the same tree");
      #endif
    }
  };
  template<bool is_const, bool is_reverse>
  friend struct iter;

  public:
  using iterator=iter<false,false>;
  using reverse_iterator=iter<false,true>;
  using const_iterator=iter<true,false>;
  using const_reverse_iterator=iter<true,true>;

  protected:
  constexpr static uint64_t unit_time=2;
  constexpr static uint64_t filter_red=1;
  constexpr static uint64_t filter_black=0xFFFF'FFFF'FFFF'FFFEul;
  size_t cur_size,time;
  np root,nil;
  [[no_unique_address]] Cmp cmp;
  public:
  rbtree():cur_size(0),time(0){
    root=nil=new node(nullptr);
    nil->p=nil->ch[0]=nil->ch[1]=nil;
    nil->size=0;
  }
  rbtree(rbtree&&v):nil(nullptr){ *this=std::move(v); }
  rbtree(const rbtree&v):nil(nullptr){ *this=v; }
  template<class T>
  rbtree(std::initializer_list<T>init)
    requires (std::same_as<T,key_t>)||(std::same_as<T,std::pair<key_t,val_t>>)
    :rbtree(){
    for(auto&&x:init)
      if constexpr (std::same_as<T,key_t>)
        this->insert(std::move(x),val_t());
      else this->insert(std::move(x));
  }
  ~rbtree(){
    clear();
    delete nil;
  }
  rbtree&operator=(rbtree&&rhs){
    if(nil) this->~rbtree();
    cur_size=rhs.cur_size, time=rhs.time;
    root=rhs.root, nil=rhs.nil;
    cmp=std::move(rhs.cmp);
    rhs.root=rhs.nil=nullptr;
    rhs.cur_size=rhs.time=0;
    rhs.clear();
    return*this;
  }
  rbtree&operator=(const rbtree&rhs){
    if(nil) this->~rbtree();
    for(auto itr=rhs.begin();itr!=rhs.end();++itr)
      this->insert(*itr);
    return*this;
  }
  protected:
  //! @param to_max: 0 min, 1 max
  template<bool to_max, class NP>
    requires (std::same_as<NP,np>||std::same_as<NP,node const*>)
  NP min_max(NP p)const{
    if(!p||p==nil)return nil;
    for(;p&&p->ch[to_max]!=nil;p=p->ch[to_max]);
    return p;
  }
  //!@param to_next: 0 prev, 1 next
  template<bool to_next, class NP>
  requires (std::same_as<NP,np>||std::same_as<NP,node const*>)
  NP prev_next(NP x)const{
    if(x==nil)return min_max<!to_next>(root);
    if(x->ch[to_next]!=nil)return min_max<!to_next>(x->ch[to_next]);
    while(1){
      if(x==root)return nil;
      node const* y=x;
      x=x->p;
      if(x->ch[!to_next]==y)break;
    }
    return x;
  }
  np find_impl(const key_t&key)const{
    np x=root;
    while(x!=nil){
      if(bool L=cmp(key,x->key());L==cmp(x->key(),key)){
        if constexpr(allow_duplicate_keys){
          uint64_t xt=x->time&filter_black; // p->time == 0
          if(0==xt)break;
          else x=x->ch[1];
        }else break;
      }else x=x->ch[!L];
    }
    return x;
  }
  public:
  template<std::convertible_to<key_t> KEY_T>
  iterator find(KEY_T&&key){
    return iterator(this,find_impl(std::forward<KEY_T>(key)));
  }
  template<std::convertible_to<key_t> KEY_T>
  const_iterator find(KEY_T&&key)const{
    return const_iterator(this,find_impl(std::forward<KEY_T>(key)));
  }

  template<std::convertible_to<key_t> KEY_T>
  requires (!is_set)
  val_t& operator[](KEY_T&&key){
    auto it=find(std::forward<KEY_T>(key));
    if(it==end()){
      if constexpr(!std::is_default_constructible_v<val_t>)
        throw std::invalid_argument("elsie::rbtree out of map. val_t is not default constructible");
      it=insert(std::forward<KEY_T>(key),val_t());
    }
    return it.ptr->val;
  }
  
  const key_t& operator[](size_t idx)const requires (is_set){
    return find_by_order(idx).key;
  }
  
  private:
  np lower_bound_impl(const key_t&key)const{
    np cur=root,res=nil;
    while(cur!=nil){
      bool L=cmp(cur->key(),key);
      if(L==cmp(key,cur->key())){
        if constexpr(allow_duplicate_keys){
          uint64_t ct=cur->time&filter_black;
          if(ct==0)return cur;
          else res=cur,cur=cur->ch[0];
        }else return cur;
      }else if(L)cur=cur->ch[1];
      else res=cur,cur=cur->ch[0];
    }
    return res;
  }
  public:
  template<std::convertible_to<key_t> KEY_T>
  iterator lower_bound(KEY_T&&key){
    return iterator(this,lower_bound_impl(std::forward<KEY_T>(key)));
  }
  template<std::convertible_to<key_t> KEY_T>
  const_iterator lower_bound(KEY_T&&key)const{
    return const_iterator(this,lower_bound_impl(std::forward<KEY_T>(key)));
  }
  private:
  np upper_bound_impl(const key_t&key)const{
    np cur=root,res=nil;
    while(cur!=nil){
      bool L=cmp(cur->key(),key);
      if(L||L==cmp(key,cur->key())){
        if constexpr(allow_duplicate_keys)
          if(uint64_t ct=cur->time&filter_black;ct>0){
            res=cur,cur=cur->ch[0];
            continue;
          }
        cur=cur->ch[1];
      }else res=cur,cur=cur->ch[0];
    }
    return res;
  }
  public:
  template<std::convertible_to<key_t> KEY_T>
  iterator upper_bound(KEY_T&&key){
    return iterator(this,upper_bound_impl(std::forward<KEY_T>(key)));
  }
  template<std::convertible_to<key_t> KEY_T>
  const_iterator upper_bound(KEY_T&&key)const{
    return const_iterator(this,upper_bound_impl(std::forward<KEY_T>(key)));
  }
  private:
  np find_by_order_impl(int64_t idx)const{
    if(idx>=(int64_t)cur_size||-idx>(int64_t)cur_size) return nil;
    if(idx<0) idx=cur_size+idx;
    np cur=root;
    while(cur!=nil){
      size_t L=cur->ch[0]->size;
      if(L>idx)cur=cur->ch[0];
      else if(L<idx){
        cur=cur->ch[1];
        idx-=L+1;
      }else return cur;
    }
    return nil;
  }
  public:
  iterator find_by_order(int64_t idx){
    return iterator(this,find_by_order_impl(idx));
  }
  const_iterator find_by_order(int64_t idx)const{
    return const_iterator(this,find_by_order_impl(idx));
  }
  template<std::convertible_to<key_t> KEY_T>
  size_t order_of(KEY_T&&key)const{
    return order_of(lower_bound(std::forward<KEY_T>(key)));
  }
  template<bool is_const, bool is_reverse>
  size_t order_of(const iter<is_const, is_reverse>&itr)const{
    node const* p = itr.ptr;
    if(p==nil) return cur_size;
    size_t R=p->ch[0]->size;
    while(p!=root){
      if(p==p->p->ch[1])
        R+=1+p->p->ch[0]->size;
      p=p->p;
    }
    if constexpr(is_reverse)
      return cur_size-R-1;
    else return R;
  }
  template<std::convertible_to<key_t> KEY_T>
  size_t count(KEY_T&&key)const{
    if constexpr(allow_duplicate_keys){
      key_t tkey(std::forward<KEY_T>(key));
      return order_of(upper_bound(tkey))-order_of(tkey);
    }
    return contains(std::forward<KEY_T>(key));
  }
  template<std::convertible_to<key_t> KEY_T>
  bool contains(KEY_T&&key)const{ return end()!=find(std::forward<KEY_T>(key)); }
  protected:
  //! @param left_right: 0 left, 1 right
  void rotation(np x,bool left_right){
    np y=x->ch[!left_right];
    x->ch[!left_right]=y->ch[left_right];
    y->p=x->p;
    if(y->ch[left_right]!=nil)y->ch[left_right]->p=x;
    if(x->p==nil)root=y;
    else x->p->ch[x==x->p->ch[1]]=y;
    y->ch[left_right]=x;
    x->p=y;
    y->size=x->size;
    x->update();
  }
  void rb_insert_fixup(np z){
    while(z->p->time&filter_red){
      bool LR=z->p==z->p->p->ch[0];
      if(np y=z->p->p->ch[LR];y->time&filter_red){
        z->p->time&=filter_black,y->time&=filter_black;
        z->p->p->time|=filter_red,z=z->p->p;
      }else{
        if(z==z->p->ch[LR])rotation(z=z->p,!LR);
        z->p->time&=filter_black,z->p->p->time|=filter_red;
        rotation(z->p->p,LR);
      }
    }
    root->time&=filter_black;
  }
  public:
  template<class... Args>
  requires (std::is_constructible_v<value_type, Args...>)
  iterator insert(Args&&... args){
    mutable_value_type t(std::forward<Args>(args)...);
    const key_t* key;
    if constexpr(is_set) key=&t;
    else key=&t.first;
    np x=root,y=nil;
    while(x!=nil){
      y=x;
      bool L=cmp(*key,x->key());
      if(L==cmp(x->key(),*key))
        if constexpr(!allow_duplicate_keys){
          if constexpr(!is_set)
            x->value()=std::move(t.second);
          return iterator(this,x);
        }else x=x->ch[1];
      else x=x->ch[!L];
    }
    cur_size+=1;
    time+=unit_time;
    np z;
    if constexpr(is_set) z=new node(std::move(t), time|filter_red,y,nil,nil);
    else z=new node(std::move(t.first),std::move(t.second),time|filter_red,y,nil,nil);
    if(y==nil) root=z;
    else y->ch[!cmp(z->key(),y->key())]=z;
    for(np s=y;s!=nil;s=s->p)
      s->size+=1;
    rb_insert_fixup(z);
    return iterator(this,z);
  }
  protected:
  void rb_delete_fixup(np x){
    while(x!=root&&!(x->time&filter_red)){
      bool LR=x==x->p->ch[0];
      np w=x->p->ch[LR];
      if(w->time&filter_red){
        w->time&=filter_black,x->p->time|=filter_red;
        rotation(x->p,!LR);
        w=x->p->ch[LR];
      }
      if(!((w->ch[0]->time&filter_red)||(w->ch[1]->time&filter_red)))
        w->time|=filter_red,x=x->p;
      else{
        if(!(w->ch[LR]->time&filter_red)){
          w->ch[!LR]->time&=filter_black,w->time|=filter_red;
          rotation(w,LR);
          w=x->p->ch[LR];
        }
        if(x->p->time&filter_red)w->time|=filter_red;
        else w->time&=filter_black;
        x->p->time&=filter_black,w->ch[LR]->time&=filter_black;
        rotation(x->p,!LR);
        x=root;
      }
    }
    x->time&=filter_black;
  }
  void erase(np z){
    auto transplant=[&](np u,np v){
      if(u->p==nil)root=v;
      else u->p->ch[u==u->p->ch[1]]=v;
      v->p=u->p;
    };
    np x,y=z,w=z->p;
    bool y_was_red=y->time&filter_red;
    if(z->ch[0]==nil) transplant(z,x=z->ch[1]);
    else if(z->ch[1]==nil) transplant(z,x=z->ch[0]);
    else{
      for(y=z->ch[1];1;y=y->ch[0]){
        y->size-=1;
        if(y->ch[0]==nil)break;
      }
      y_was_red=y->time&filter_red;
      x=y->ch[1];
      if(y!=z->ch[1]){
        transplant(y,y->ch[1]);
        y->ch[1]=z->ch[1],y->ch[1]->p=y;
      }else x->p=y;
      transplant(z,y);
      y->ch[0]=z->ch[0];
      y->ch[0]->p=y;
      if(z->time&filter_red)y->time|=filter_red;
      else y->time&=filter_black;
      y->update();
    }
    if(w!=nil)for(;w!=nil;w=w->p)
      w->size-=1;
    if(!y_was_red)rb_delete_fixup(x);
    delete z;
  }
  public:
  // is_constはfalseじゃないと書き換えできないので，false
  template<bool is_reverse>
  iter<false, is_reverse> erase(iter<false, is_reverse>&itr){
    if(itr.ptr!=nil){
      cur_size-=1;
      np nx=prev_next<!is_reverse>(itr.ptr);
      erase(itr.ptr);
      itr.ptr=nx;
      return itr;
    }
    return iterator(this,nil);
  }
  template<bool is_reverse>
  iter<false, is_reverse> erase(iter<false, is_reverse>&&itr){ return erase(itr); }
  template<std::convertible_to<key_t> KEY_T>
  iterator erase(KEY_T&& key){
    return erase(find(std::forward<KEY_T>(key)));
  }
  // memory
  public:
  void clear(){
    if(nil==nullptr){
      root=nil=new node(nullptr);
      return;
    }
    if(root!=nil){
      root->clear(nil);
      delete root;
    }
    time=cur_size=0;
    root=nil;
  }
  bool empty()const{return cur_size==0;}
  size_t size()const{return cur_size;}
  // iterator
  iterator begin(){ return iterator(this,cur_size?min_max<false>(root):nil); }
  iterator end(){ return iterator(this,nil); }
  const_iterator begin()const{ return cbegin();}
  const_iterator end()const{ return cend(); }
  const_iterator cbegin()const{ return const_iterator(this,cur_size?min_max<false>(root):nil); }
  const_iterator cend()const{ return const_iterator(this,nil); }
  reverse_iterator rbegin(){ return reverse_iterator(this,cur_size?min_max<true>(root):nil); }
  reverse_iterator rend(){ return reverse_iterator(this,nil); }
  const_reverse_iterator rbegin()const{ return crbegin(); }
  const_reverse_iterator rend()const{ return crend(); }
  const_reverse_iterator crbegin()const{ return const_reverse_iterator(this,cur_size?min_max<true>(root):nil); }
  const_reverse_iterator crend()const{ return const_reverse_iterator(this,nil); }
};

template<class key_t, class cmp=std::less<key_t>>
using set=rbtree<key_t,null_t,cmp,false>;

template<class key_t, class val_t, class cmp=std::less<key_t>>
using map=rbtree<key_t,val_t,cmp,false>;

template<class key_t, class cmp=std::less<key_t>>
using multiset=rbtree<key_t,null_t,cmp,true>;

template<class key_t,class val_t,class cmp=std::less<key_t>>
using multimap=rbtree<key_t,val_t,cmp,true>;


template<class val_t>
class varray:public rbtree<null_t,val_t,std::less<null_t>,true>{
  protected:
  using super=rbtree<null_t,val_t,std::less<null_t>,true>;
  typename super::iterator insert_key_null(size_t idx,val_t&&t){
    if(super::cur_size==0)return super::insert(null_t(),std::move(t));
    else{
      super::cur_size+=1;
      super::time+=super::unit_time;
      typename super::np y,z;
      if(super::unused==nullptr)z=new super::node(super::nil);
      else{
        z=super::unused;
        super::unused=z->p;
      }
      z->val=std::move(t),z->ch[0]=z->ch[1]=super::nil;
      z->size=1,z->time=super::time|super::filter_red;
      if(super::cur_size-1<=idx){
        y=super::template min_max<true>(super::root);
        z->p=y,y->ch[1]=z;
      }else{
        y=super::find_by_order_impl(idx);
        if(y->ch[0]!=super::nil){
          y->ch[0]->p=z;
          z->ch[0]=y->ch[0];
        }
        z->p=y,y->ch[0]=z;
        z->update();
      }
      for(typename super::np s=z->p;s!=super::nil;s=s->p)
        s->size+=1;
      super::rb_insert_fixup(z);
      return typename super::iterator(this,z);
    }
  }
  public:
  using iterator=super::iterator;
  iterator insert(int64_t idx,const val_t&v){ return insert_key_null(idx<0?super::cur_size+idx:idx,v); }
  iterator insert(int64_t idx,val_t&&v){ return insert_key_null(idx<0?super::cur_size+idx:idx,move(v)); }
  iterator erase(int64_t idx){ return super::erase(super::find_by_order(idx)); }
  val_t& get(int64_t idx){ return super::find_by_order(idx).s(); }
};

}

#endif // ELSIE_RBTREE