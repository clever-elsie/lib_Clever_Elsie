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
#include<atcoder/fenwicktree>
namespace elsie{
using namespace std;
using namespace atcoder;
template<class T>
concept Itrabl=requires(const T&x){x.begin();x.end();typename T::value_type;};
template<class T>using vc=vector<T>;
template<class T>using vv=vc<vc<T>>;

template<Itrabl T>
size_t LIS(const T&a){
  vc<typename T::value_type>dp(a.size()+1,numeric_limits<typename T::value_type>::max());
  dp[0]=numeric_limits<typename T::value_type>::lowest();
  size_t ans=0;
  for(size_t i=0;i<a.size();++i){
    size_t k=lower_bound(begin(dp),end(dp),a[i])-begin(dp);
    if(dp[k-1]<a[i]){
      dp[k]=min(dp[k],a[i]);
      ans=max(k,ans);
    }
  }
  return ans;
}

template<bool return_vector=false,Itrabl S>
pair<size_t,S>LCS(const S&s,const S&t){
  size_t ss=s.size(),ts=t.size();
  vv dp(ss+1,vc<uint32_t>(ts+1,0));
  for(size_t i=1;i<=ss;++i)
    for(size_t j=1;j<=ts;++j)
      if(s[i-1]!=t[j-1])dp[i][j]=max(dp[i-1][j],dp[i][j-1]);
      else dp[i][j]=max(dp[i-1][j-1]+1,max(dp[i][j-1],dp[i-1][j]));

  if constexpr(return_vector==false)
    return{dp[ss][ts],S()};

  vc<pair<uint32_t,uint32_t>>index,Stack;
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
  reverse(begin(index),end(index));
  S res;
  for(const auto&[i,j]:index)
    if(0<=i&&i<(uint32_t)ss)
      res.push_back(s[i]);
  return{dp[ss][ts],res};
}

template<Itrabl S>
size_t edit_dist(const S&s,const S&t){
  vc dp(s.size()+1,vc<size_t>(t.size()+1,UINT64_MAX));
  dp[0][0]=0;
  for(size_t i=0,j;i<dp.size();++i){
    for(j=0;j<dp[i].size();++j){
      if(i)dp[i][j]=min(dp[i-1][j]+1,dp[i][j]);
      if(j)dp[i][j]=min(dp[i][j-1]+1,dp[i][j]);
      if(i&&j)dp[i][j]=min(dp[i-1][j-1]+(s[i-1]!=t[j-1]),dp[i][j]);
    }
  }
  return dp.back().back();
}

template<Itrabl S>
size_t inv_number(const S&tar){
  const size_t n=tar.size();
  size_t res=0;
  S arr=tar,press=tar;
  fenwick_tree<typename S::value_type>bit(n);
  auto prb=press.begin(),pre=press.end();
  sort(prb,pre);
  press.erase(unique(prb,pre),pre);
  for(auto&x:arr)x=lower_bound(prb,pre,x)-prb;
  for(int i=0;i<n;++i){
    res+=i-bit.sum(0,arr[i]+1);
    bit.add(arr[i],1);
  }
  return res;
}
}
#endif