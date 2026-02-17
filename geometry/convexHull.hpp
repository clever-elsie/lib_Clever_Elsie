// SPDX-License-Identifier: MIT
// Copyright (c) 2026 CleverElsie

#ifndef ELSIE_CONVEXHULL
#define ELSIE_CONVEXHULL
#include <cstdint>
#include <algorithm>
#include <vector>
#include <utility>
namespace elsie{

std::vector<std::pair<int32_t,int32_t>>
convex_hull(std::vector<std::pair<int32_t,int32_t>>&p){
  using i32=int32_t;
  using i64=int64_t;
  using pi=std::pair<i32,i32>;
  auto potential=[](const pi&a, const pi&o)-> pi {
    return {a.first-o.first, a.second-o.second};
  };
  auto cross=[&potential](const pi&o, const pi&a, const pi&b){
    auto [ax,ay]=potential(a,o);
    auto [bx,by]=potential(b,o);
    return (i64)ax*by-(i64)ay*bx;
  };
  std::sort(p.begin(),p.end());
  p.erase(std::unique(p.begin(),p.end()),p.end());
  i32 n=p.size(),k=0;
  if(n<=2)return p;
  std::vector<pi>h(2*n);
  for(i32 i=0;i<n;h[k++]=p[i++])
    while(k>=2&&cross(h[k-2],h[k-1],p[i])<=0)--k;
  for(i32 i=n-2,t=k+1;i>=0;h[k++]=p[i--])
    while(k>=t&&cross(h[k-2],h[k-1],p[i])<=0)--k;
  h.resize(k-1);
  return h;
}

} // namespace elsie
#endif