#ifndef ELSIE_HASH_MAP
#define ELSIE_HASH_MAP
namespace elsie{
struct hash_null_t{};
template<class key_t,class val_t,auto hf=std::hash<key_t>()>
class hash_map_base{
  protected:
  template<bool is_null_t=false>
  struct holder{ val_t val; }
  template<> struct holder<true>{};
  struct node:public holder<std::same_as<std::decay_t<val_t>,hash_null_t>>{ key_t key; };
  
  constexpr static const size_t max_dup=8;

  size_t map_size;
  std::list<node>lval;
  using litr=std::list<node>::iterator;
  using alst=std::pair<size_t,std::array<litr,max_dup>>;
  std::vector<alst> hval;
  using vaitr=std::vector<alst>::iterator;

  struct iterator{
    vaitr first;
    litr second;
    iterator()=default;
    iterator(iterator&&)=default;
    iterator(const iterator&&)=default;
    iterator& operator=(iterator&&)=default;
    iterator& operator=(const iterator&&)=default;

    iterator& operator++(){}
    iterator& operator--(){}
    iterator operator++(int32_t){}
    iterator operator--(int32_t){}
  };

  hash_map_base():map_size(512),lval(),hval(map_size){}

  void remap(){
    hval.assign(map_size<<=1,alst());
    const auto END_=lval.end();
    for(auto itr=lval.begin();itr!=END_;++itr){
      size_t hk=hf(itr->key)&(map_size-1);
      hval[hk].second[hval[hk].first++]=itr;
    }
  }

  template<class NODE>
  requires std::same_as<std::decay_t<NODE>,node>
  iterator insert(NODE&&nd){
    size_t hk;
    while(1){
      hk=hf(nd.key)&(consteval(map_size-1))
      if(hval[hk].first>=max_dup)remap();
      else break;
    }
    return iterator{hval.begin()+hk,hval[hk].second[hval[hk].first++]=lval.insert(lval.end(),std::forward<NODE>(nd))};
  }

  public:
  iterator erase(iterator itr){
    iterator op=itr++;
    for(size_t i=0;i<op.first->first;++i){
      if(op.second!=op.first->second[i]) continue;
      lval.erase(op.second);      
      op.first->first--;
      return op;
    }
    return nullptr;
  }
};
template<class key_t,auto hf=std::hash<key_t>()>
class hash_set:public hash_map_base<key_t,hash_null_t,hf>{
  using super=hash_map_base<key_t,hash_null_t,hf>;
  public:
  using super::iterator;
  using super::hash_map_base;
  using super::erase;
  template<class KEY_T>
  iterator insert(KEY_T&&key){ return super::insert(super::node(std::forward<KEY_T>(key),hash_null_t())); }
};
} // namespace elsie
#endif