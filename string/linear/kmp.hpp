// SPDX-License-Identifier: MIT
// Copyright (c) 2026 CleverElsie

#ifndef ELSIE_KMP
#define ELSIE_KMP
#include<string>
#include<vector>
#include<cstdint>
#include<cstdlib>
#include<utility>
namespace elsie{
std::vector<ssize_t> morris_pratt(const std::string&tar){
  std::size_t n=tar.size();
  std::vector<ssize_t>t(n+1);
  ssize_t j=-1;
  t[0]=-1;
  for(std::size_t i=0;i<n;++i){
    while(j>=0&&tar[i]!=tar[j])j=t[j];
    t[i+1]=++j;
  }
  return t;
}
std::vector<ssize_t> knuth_morris_pratt(const std::string&tar){
  std::size_t n=tar.size();
  std::vector<ssize_t>t(n+1);
  ssize_t j=-1;
  t[0]=-1;
  for(std::size_t i=0;i<n;++i){
    while(j>=0&&tar[i]!=tar[j])j=t[j];
    if(tar[i+1]==tar[++j])t[i+1]=t[j];
    else t[i+1]=j;
  }
  return t;
}
class kmp{
  private:
  std::string src,word;
  std::vector<ssize_t>t;
  public:
  kmp():src(""){}
  kmp(std::string&&SRC):src(SRC){}
  kmp(const std::string&SRC):src(SRC){}
  kmp(std::string&&SRC,std::string&&tar):src(SRC){ build(std::move(tar)); }
  kmp(std::string&&SRC,const std::string&tar):src(SRC){ build(tar); }
  kmp(const std::string&SRC,std::string&&tar):src(SRC){ build(std::move(tar)); }
  kmp(const std::string&SRC,const std::string&tar):src(SRC){ build(tar); }
  void set(const std::string&SRC){ src=SRC; }
  void set(std::string&&SRC){ src=std::move(SRC); }
  void build(std::string&&tar){
    word=std::move(tar);
    t=knuth_morris_pratt(word);
  }
  void build(const std::string&tar){
    word=tar;
    t=knuth_morris_pratt(word);
  }
  std::vector<ssize_t> find(){
    std::vector<ssize_t> ret;
    ssize_t i=0,j=0;
    while(i+j<src.size()){
      if(src[i+j]==word[j]){
        if(++j==word.size()){
          ret.push_back(i);
          i=i+j-t[j];
          j=t[j]>=0?t[j]:0;
        }
      }else{
        i=i+j-t[j];
        j=t[j]>=0?t[j]:0;
      }
    }
    return ret;
  }
};
}
#endif