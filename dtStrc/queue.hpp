#ifndef ELSIE_QUEUE
#define ELSIE_QUEUE
// double_ended_priority_queue
#include <cassert>
#include <concepts>
#include <functional>
#include <type_traits>

namespace elsie{
template<class T,class less_t=std::less<T>>
class depq{
  static constexpr less_t Less{};
  std::vector<T> data_;
  public:
  depq()=default;
  depq(const depq&)=default;
  depq(depq&&)=default;
  depq(const std::vector<T>&dat):data_(dat){ init(); }
  depq(std::vector<T>&&dat):data_(std::move(dat)){ init(); }
  depq& operator=(const depq&)=default;
  depq& operator=(depq&&)=default;
  template<class Iterator>
  requires requires(Iterator x){ ++x; x<x; }
  depq(Iterator first,Iterator last){
    for(;first!=last;++first)
      data_.push_back(*first);
    init();
  }
  
  size_t size()const noexcept{ return data_.size(); }
  bool empty()const noexcept{ return data_.empty(); }
  void reserve(size_t n){ data_.reserve(n); }
  void clear(){ data_.clear(); }

  template<class... Args>
  void emplace(Args&&... args){
    data_.emplace_back(args...);
    ascend(data_.size()-1);
  }
  void push(T&&x){
    data_.push_back(std::move(x));
    ascend(data_.size()-1);
  }
  void push(const T&x){
    data_.push_back(x);
    ascend(data_.size()-1);
  }
  const T& min()const noexcept{
    assert(data_.size()>0);
    return data_[1-(data_.size()<2ul)];
  }
  const T& max()const noexcept{
    assert(data_.size()>0);
    return data_[0];
  }
  T pop_min()noexcept{
    assert(data_.size()>0);
    T ret(std::move(data_[1-(data_.size()<2ul)]));
    if(data_.size()<=2ul) data_.pop_back();
    else{
      std::swap(data_[1],data_.back());
      data_.pop_back();
      ascend(descend(1));
    }
    return ret;
  }
  T pop_max()noexcept{
    assert(data_.size()>0);
    T ret(std::move(data_[0]));
    if(data_.size()==1ul) data_.pop_back();
    else{
      std::swap(data_[0],data_.back());
      data_.pop_back();
      ascend(descend(0));
    }
    return ret;
  }
  private:
  void init(){
    for(size_t i=data_.size();i--;){
      if(i&1 && data_[i-1]<data_[i]) std::swap(data_[i-1],data_[i]);
      ascend(descend(i),i);
    }
  }
  size_t par(size_t k) const noexcept{
    // min heapが奇数，maxが偶数インデックスを占有する2本の半直線上のheapと考える．
    // 実際のindexを2で割り切り捨てると0-indexのヒープが両方に現れる．
    // このときidx2=index/2;として，idx2の親はpar=(idx2-1)/2となる．
    // ここでparは半分にされたインデックス上でのアドレスなので，2倍して後はminかmaxかを元のindexの最下位ビットとorを取って戻す．
    // このとき親を求めるために2で割り，元に戻すため2を掛ける操作は最下位ビットを0にすることと同じである．
    return ((k>>1)-1)&~(size_t)1|(k&1);
  }
  size_t par_base(size_t k) const noexcept{
    // parの最下位ビット補正をしないバージョン
    return ((k>>1)-1)&~(size_t)1;
  }
  size_t ascend(size_t k,size_t root=1){
    // 葉ノードでは+1(|1)でmax heap->min heap
    // したがって，最下位ビット反転で所属するheapを入れ替えられる
    if((k|1)<data_.size() && Less(data_[k&~(size_t)1],data_[k|1]))
      std::swap(data_[k],data_[k^1]),k^=1;
    size_t p;
    while(root<k && Less(data_[p=par_base(k)],data_[k])) // max heap
      std::swap(data_[p],data_[k]),k=p;
    while(root<k && Less(data_[k],data_[p=par_base(k)|1])) // min heap
      std::swap(data_[p],data_[k]),k=p;
    return k; // 最終的な挿入した要素の位置を返す．
  }
  size_t descend(size_t k){
    if(size_t n=data_.size(); k&1){ // min heap
      while((k<<1)+1 < n){
        size_t c=(k<<1)+3; // 右の子
        if(n<=c||Less(data_[c-2],data_[c])) c-=2; // 左<右ならスワップ
        if(c<n&& ! Less(data_[k],data_[c])) // 親>=子 === !(親<子)なら交換
          std::swap(data_[c],data_[k]),k=c;
        else break;
      }
    }else{ // max heap
      while((k<<1)+2 < n){
        size_t c=(k<<1)+4; // 右の子
        if(n<=c||Less(data_[c],data_[c-2])) c-=2; // 左>右ならスワップ
        if(c<n&& ! Less(data_[c],data_[k])) // 親<=子 === !(親>子)なら交換
          std::swap(data_[c],data_[k]),k=c;
        else break;
      }
    }
    return k;
  }
};

} // namespace elsie
#endif