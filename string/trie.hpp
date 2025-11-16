#ifndef ELSIE_TRIE
#define ELSIE_TRIE
#include <algorithm>
#include <new>
#include <string>
#include <vector>
#include <deque>

namespace elsie{
class trie {
  struct node {
    int *cnt;
    node **next;
    node(int &size) {
      cnt = new int[size];
      next = new node *[size];
      for (int i = 0; i < size; i++) {
        cnt[i] = 0;
        next[i] = nullptr;
      }
    }
  } *tree;
  int count_prefix_internal(node *here) {
    int res = 0;
    for (int i = 0; i < char_size; i++)
      if (here->cnt[i] > 1)
        res += (here->cnt[i]) * (here->cnt[i] - 1) / 2;
    for (int i = 0; i < char_size; i++)
      if (here->next[i] != nullptr && here->cnt[i] > 1)
        res += count_prefix_internal(here->next[i]);
    return res;
  }
  int char_size;
  ::std::vector<char> char_set_press;
  int char_set(char x) {
    return ::std::lower_bound(char_set_press.begin(), char_set_press.end(), x) -
           char_set_press.begin();
  }

public:
  trie(char first_char, int size) {
    char_size = size;
    tree = new node(size);
    for (int i = 0; i < size; i++)
      char_set_press.push_back((char)(first_char + i));
  }
  trie(const ::std::vector<char>&char_list) {
    char_set_press = char_list;
    ::std::sort(char_set_press.begin(), char_set_press.end());
    char_set_press.erase(
      ::std::unique(char_set_press.begin(),char_set_press.end()),
      char_set_press.end());
    char_size = char_list.size();
    tree = new node(char_size);
  }
  ~trie(){
    ::std::deque<node*>st(1,tree);
    while(st.size()){
      node*cur=st.front();
      st.pop_front();
      for(::std::size_t i=0;i<char_size;++i){
        if(cur->next[i]!=nullptr)
          st.push_back(cur->next[i]);
      }
      delete[] cur->cnt;
      delete[] cur->next;
      delete cur;
    }
  }
  void insert(::std::string &s) {
    node *here = tree;
    for (auto x : s) {
      int c = char_set(x);
      here->cnt[c]++;
      if (here->next[c] == nullptr)
        here->next[c] = new node(char_size);
      here = here->next[c];
    }
  } 
  // ABC 353 E https://atcoder.jp/contests/abc353/tasks/abc353_e
  int count_prefix_sum() { return count_prefix_internal(tree); }
  // ABC 287 E https://atcoder.jp/contests/abc287/tasks/abc287_e
  int count_longest_prefix(::std::string&s){
    int l=0;
    node*here=tree;
    for(auto x:s){
      int c=char_set(x);
      if(here->next[c]==nullptr||here->cnt[c]<2)
        break;
      here=here->next[c];
      l++;
    }
    return l;
  }
};



#include <cstddef>
#include <cstdint>
#include <utility>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <concepts>
#include <iterator>
#include <type_traits>

namespace additional{

class trie{
  struct node{
    ::std::size_t cnt, is_terminal;
    node* parent;
    ::std::string key;
    ::std::unordered_map<char,node*> next;
    ~node(){
      for(auto&[c,n]:next)
        delete n;
    }
    /**
     * @param pos ここを後ろのノードの先頭にして分割
     */
    node* split(::std::size_t pos){
      if(pos>=key.size()||pos==0) return nullptr;
      ::std::string key_next=key.substr(pos);
      node*next_node=new node{cnt,is_terminal,this,::std::move(key_next),::std::unordered_map<char,node*>{}};
      is_terminal = false;
      key.erase(pos);
      next.clear();
      next[key_next[0]]=next_node;
      return next_node;
    }
    node*merge(){ // マージは一意に定まる
      return nullptr;
    }
    ::std::size_t LCP(const char*s){
      // Safe from out-of-bounds access under not undefined behavior:
      //  - The last character of `s` and `key` is always '\0' and any of other characters must not be '\0'.
      //  - The condition `s[i] != key[i]` will fail before accessing out of bounds.
      //  - Since `i < key.size()`, `key[i]` is always valid.
      //  Note: writing to `s[i]` is undefined behavior, but reading is safe.
      ::std::size_t ret=0;
      for(::std::size_t i=0;i<key.size();++i){
        if(s[i]!=key[i]) break;
        ++ret;
      }
      return ret;
    }
  };
  ::std::unique_ptr<node> root;
  public:
  struct iterator{
    using iterator_category = ::std::random_access_iterator_tag;
    using value_type = ::std::string;
    using difference_type = ::std::ptrdiff_t;
    using pointer = ::std::string*;
    using reference = ::std::string&;
    private:
    node*cur;
    public:
    iterator():cur(nullptr){}
    explicit iterator(node* n):cur(n){}
    iterator(const iterator&)=default;
    iterator& operator=(const iterator&)=default;
    bool operator==(const iterator& other)const{ return cur==other.cur; }
    bool operator!=(const iterator& other)const{ return cur!=other.cur; }
    node* operator->()const{ return cur; }
    node& operator*()const{ return *cur; }
  };
  using reverse_iterator = ::std::reverse_iterator<iterator>;
  using value_type = ::std::string;
  using size_type = ::std::size_t;

  // constructor/destructor/operator=
  trie():root(::std::make_unique<node>()){}
  ~trie()=default;
  trie(const trie&rhs){ root=::std::make_unique<node>(*rhs.root); }
  trie(trie&&rhs){ root=::std::move(rhs.root); }
  trie& operator=(const trie&rhs){ root=::std::make_unique<node>(*rhs.root); return *this; }
  trie& operator=(trie&&rhs){ root=::std::move(rhs.root); return *this; }

  // accessor
  iterator begin(){ return iterator(root.get()); }
  iterator end(){ return iterator(nullptr); }

  // modifier
  iterator insert(const ::std::string&s){ return insert(s.data()); }
  iterator insert(const ::std::string_view&s){ return insert(s.data()); }
  iterator insert(const char* s){
    node*cur=root.get();
    if(*s=='\0') {
      ++(cur->cnt);
      ++(cur->is_terminal);
      return iterator(cur);
    }
    while(true){
      ::std::size_t lcp=cur->LCP(s);
      if(s[lcp]=='\0'){ // ここが挿入処理の最後
        if(cur->key.size()==lcp) // 後ろの余計な文字列を別のノードとして分離
          cur->split(lcp);
        ++(cur->cnt); // 後ろの部分は増えないので，分割した後に加算する
        cur->is_terminal=true;
        break;
      }else{ // 挿入文字列に続きがある
        if(cur->key.size()!=lcp) cur->split(lcp);
        ++(cur->cnt);
        auto it=cur->next.find(s[lcp]);
        if(it!=cur->next.end()){ // 次のノードの処理へループ
          cur=it->second;
          s+=lcp;
        }else{ // 次が無いなら新しく作ればいいので，ループ終了
          cur->next[s[lcp]]=new node{1, 1, cur, ::std::string(s+lcp), ::std::unordered_map<char,node*>{}};
          cur=cur->next[s[lcp]];
          break;
        }
      }
    }
    return iterator(cur);
  }
  
  iterator find(const ::std::string&s)const{ return find(s.data()); }
  iterator find(const ::std::string_view&s)const{ return find(s.data()); }
  iterator find(const char* s)const{
    node*cur=root.get();
    while(true){
      ::std::size_t lcp=cur->LCP(s);
      if(s[lcp]=='\0'){
        if(lcp==cur->key.size() && cur->is_terminal)
          return iterator(cur);
        else return iterator(nullptr);
      }else{
        if(lcp==cur->key.size()){
          s+=lcp;
          auto it=cur->next.find(s[lcp]);
          if(it!=cur->next.end())
            cur=it->second;
          else return iterator(nullptr);
        }else return iterator(nullptr);
      }
    }
    return iterator(nullptr);// dead code
  }
  
  iterator erase(const ::std::string&s){ return erase(s.data()); }
  iterator erase(const ::std::string_view&s){ return erase(s.data()); }
  iterator erase(const char* s){
    iterator it=find(s);
    if(it==end()) return it;
    node*cur=it.operator->();
    --(cur->is_terminal), --(cur->cnt);
    if(cur->cnt==0 && cur->next.empty()){
      if(cur->parent) cur->parent->next.erase(cur->key[0]);
      delete cur;
    }
    return it;
  }
  
  iterator erase_all(const ::std::string&s){ return erase_all(s.data()); }
  iterator erase_all(const ::std::string_view&s){ return erase_all(s.data()); }
  iterator erase_all(const char* s){
    return end();
  }
  
  // counter
  size_type size()const noexcept{ return root->cnt; }
  size_type count(iterator it)const{ return it->is_terminal?it->cnt:0; }
  size_type count(const ::std::string&s)const{ return count(s.data()); }
  size_type count(const ::std::string_view&s)const{ return count(s.data()); }
  size_type count(const char* s)const{
    iterator it=find(s);
    return count(it);
  }

};

} // namespace additional
}
#endif