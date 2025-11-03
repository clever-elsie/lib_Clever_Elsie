#ifndef ELSIE_HUFFMAN_HPP
#define ELSIE_HUFFMAN_HPP
#include <cstddef>
#include <utility>
#include <vector>
#include <queue>
#include <unordered_map>
#include <variant>
#include <functional>
namespace elsie{

template<class T, class Hash=std::hash<T>, class Equal=std::equal_to<T>>
struct huffman{
  struct node{
    using pointer = node*;
    using const_pointer = const node*;
    using array_pointer = const_pointer[2];
    using value_type=std::variant<T,array_pointer>;
    const value_type value;
    size_t cnt;
    node()=delete;
    node(const T&v,size_t cnt_):value(v),cnt(cnt_){}
    node(T&&v,size_t cnt_):value(std::move(v)),cnt(cnt_){}
    node(std::pair<pointer,pointer>ptr)
      :value(std::in_place_index<1>,ptr.first,ptr.second)
    {
      const auto[zero,one]=get_pointers();
      cnt=zero->cnt+one->cnt;
    }
    bool is_leaf()const{
      return std::holds_alternative<T>(value);
    }
    const T& get_value()const{
      return std::get<T>(value);
    }
    std::pair<const_pointer,const_pointer>
    get_pointers()const{
      const auto ptrv=std::get<1>(value);
      return {ptrv[0],ptrv[1]};
    }
  };

  huffman(const vector<T>&v):code(){
    if(v.size()==0) return;
    if(v.size()==1){
      code[v[0]]=std::vector<bool>{false};
      return;
    }
    std::unordered_map<T,size_t,Hash,Equal> cnt;
    for(const auto&e:v) ++cnt[e];
    static auto f=[](const node*a,const node*b){
      return a->cnt>b->cnt; // cntが小さい方を根に．等しいときはどうでもいい
    };
    std::priority_queue<node*,std::vector<node*>,decltype(f)> tree(f); // 降順
    for(const auto&[v,c]:cnt)
      tree.emplace(new node(v,c));
    while(tree.size()>1){
      node*first=tree.top();tree.pop();
      node*second=tree.top();tree.pop();
      std::pair<node*,node*> init(second,first);
      tree.push(new node(init));
    }
    node* root=tree.top();tree.pop();
    std::vector<bool> ci;
    dfs(root,ci);
  }

  std::vector<bool> operator[](const T&val)const{
    if(!code.contains(val)) return empty_v;
    return code.at(val);
  }
  private:
  void dfs(const node*ptr,std::vector<bool>&ci){
    if(ptr->is_leaf()) // 終端ノード
      code[ptr->get_value()]=ci;
    else{
      const auto[zero,one]=ptr->get_pointers();
      ci.push_back(false);
      dfs(zero,ci);
      ci.back()=true;
      dfs(one,ci);
      ci.pop_back();
    }
    return;
  }
  std::unordered_map<T,std::vector<bool>,Hash,Equal> code;
  static constexpr std::vector<bool> empty_v{};
};
} //namespace elsie
#endif