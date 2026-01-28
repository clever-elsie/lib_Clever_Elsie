#ifndef ELSIE_DP
#define ELSIE_DP
#include<algorithm>
#include<string>
#include<vector>
#include<limits>
#include<cstddef>
#include<cstdint>
#include<numeric>
#include<utility>
#include<concepts>
#include<iterator>
#include<ranges>

#include<dtStrc/segtree/BIT.hpp>
#include<misc/concepts.hpp>

namespace elsie{

// longest increasing subsequence
template<Itrabl T>
size_t _LIS(const T&a){
  typedef typename T::value_type V;
  constexpr static auto maxofT = std::numeric_limits<V>::max();
  std::vector<V>dp(a.size()+1,maxofT);
  dp[0]=std::numeric_limits<V>::lowest();
  size_t ans=0;
  for(size_t i=0;i<a.size();++i){
    size_t k=std::lower_bound(dp.begin(),dp.end(),a[i])-dp.begin();
    if(dp[k-1]<a[i]){
      dp[k]=std::min(dp[k],a[i]);
      ans=std::max(k,ans);
    }
  }
  return ans;
}

template<std::ranges::range T>
std::pair<size_t,std::vector<size_t>> LIS(const T&a){
  typedef typename T::value_type V;
  constexpr static auto maxofT = std::numeric_limits<V>::max();
  static auto lbidx=[]<std::ranges::range X, class W>(const X&b, const W&x) -> size_t {
    return std::ranges::lower_bound(b,x)-b.begin();
  };
  std::vector<V>dp(a.size()+1, maxofT);
  std::vector<int64_t>seq(a.size(),-1);
  dp[0]=std::numeric_limits<V>::lowest();
  size_t ans=0;
  for(auto[i,x]:a|std::views::enumerate)
    if(const size_t k=lbidx(dp,x);dp[k-1]<x){
      dp[k]=std::min(dp[k],x);
      ans=std::max(k,ans);
      seq[i]=k;
    }
  std::vector<size_t> lis(ans);
  V pre;
  size_t cur=ans;
  for(auto[i,x]:a|std::views::enumerate|std::views::reverse){
    if(cur==seq[i]){
      if(cur==ans) lis[--cur]=i, pre=x;
      else if(x<pre) lis[--cur]=i, pre=x;
    }
  }
  return std::make_pair(ans,std::move(lis));
}

template<bool return_vector=false,Itrabl S>
std::pair<size_t,S>LCS(const S&s,const S&t){
  size_t ss=s.size(),ts=t.size();
  std::vector dp(ss+1, std::vector<uint32_t>(ts+1,0u));
  for(size_t i=1;i<=ss;++i)
    for(size_t j=1;j<=ts;++j)
      if(s[i-1]!=t[j-1])dp[i][j]=std::max(dp[i-1][j],dp[i][j-1]);
      else dp[i][j]=std::max(dp[i-1][j-1]+1,std::max(dp[i][j-1],dp[i-1][j]));

  if constexpr(return_vector==false)
    return{dp[ss][ts],S()};

  using p32=std::pair<uint32_t,uint32_t>;
  std::vector<p32>index,Stack;
  Stack.emplace_back(ss, ts);
  while(Stack.size()){
    auto[y,x]=Stack.back();
    uint32_t now=dp[y][x];
    Stack.pop_back();
    index.emplace_back(y,x);
    if(now==0) break;
    while(dp[y-1][x]==now)--y;
    while(dp[y][x-1]==now)--x;
    Stack.emplace_back(y-1,x-1);
  }
  std::reverse(index.begin(),index.end());
  S res;
  for(const auto&[i,j]:index)
    if(0<=i&&i<(uint32_t)ss)
      res.push_back(s[i]);
  return{dp[ss][ts],res};
}

template<Itrabl S>
size_t edit_dist(const S&s,const S&t){
  constexpr static auto maxofu64 = std::numeric_limits<size_t>::max();
  std::vector dp(s.size()+1,std::vector<size_t>(t.size()+1,maxofu64));
  dp[0][0]=0;
  for(size_t i=0,j;i<dp.size();++i){
    for(j=0;j<dp[i].size();++j){
      if(i)dp[i][j]=std::min(dp[i-1][j]+1,dp[i][j]);
      if(j)dp[i][j]=std::min(dp[i][j-1]+1,dp[i][j]);
      if(i&&j)dp[i][j]=std::min(dp[i-1][j-1]+(s[i-1]!=t[j-1]),dp[i][j]);
    }
  }
  return dp.back().back();
}

template<Itrabl S>
size_t inv_number(const S&tar){
  const size_t n=tar.size();
  size_t res=0;
  S arr=tar,press=tar;
  elsie::BIT<typename S::value_type>bit(n);
  auto prb=press.begin(),pre=press.end();
  std::sort(prb,pre);
  press.erase(std::unique(prb,pre),pre);
  for(auto&x:arr)x=std::lower_bound(prb,pre,x)-prb;
  for(size_t i=0;i<n;++i){
    res+=i-bit.sum(0,arr[i]+1);
    bit.add(arr[i],1);
  }
  return res;
}
}
#endif