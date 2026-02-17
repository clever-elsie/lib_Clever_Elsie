#include "../include/parser.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include <utility>
#include <string>
#include <string_view>
#include <exception>
#include <stdexcept>

namespace fs = std::filesystem;
using sv = std::string_view;

[[noreturn]]
void throw_error(const std::string &message){
  throw std::runtime_error("Invalid JSON: " + message);
}

inline bool is_eof(const std::string &s, size_t i){
  return i >= s.size();
}

inline void skip_whitespace_and_comments(const std::string &s, size_t &i){
  while(i<s.size()){
    if(std::isspace(s[i])){
      ++i;
      continue;
    }
    if(s[i]=='#'){
      while(i<s.size() && s[i]!='\n') ++i;
      continue;
    }
    if(i+1<s.size() && s[i]=='/' && s[i+1]=='/'){
      while(i<s.size() && s[i]!='\n') ++i;
      continue;
    }
    break;
  }
}

inline std::string parse_string(const std::string &s, size_t &i)
// pre(s[i]=='\"')
// post(i==0 || s[i-1]=='\"')
{
  std::string ret;
  ++i;
  while(!is_eof(s, i)){
    if(s[i]=='\"'){
      ++i;
      return ret;
    }
    if(s[i]=='\\'){
      ++i;
      if(is_eof(s, i)) throw_error("escaped string has not ended");
      switch(s[i]){
        case '"': ret.push_back('"'); break;
        case '/': ret.push_back('/'); break;
        case '\\': ret.push_back('\\'); break;
        case 'b': ret.push_back('\b'); break;
        case 'f': ret.push_back('\f'); break;
        case 'n': ret.push_back('\n'); break;
        case 'r': ret.push_back('\r'); break;
        case 't': ret.push_back('\t'); break;
        case 'u': throw_error("unicode escape is not supported");
        default: throw_error("invalid escaped character: " + std::string(1, s[i]));
      }
    }else ret+=s[i];
    ++i;
  }
  throw_error("string has not ended");
}

inline std::pair<std::string, std::string> parse_key_value(const std::string &s, size_t &i)
// pre(s[i]=='\"')
// post(i==0 || s[i-1]=='\"')
{
  std::pair<std::string, std::string> ret;
  ret.first=parse_string(s, i);
  skip_whitespace_and_comments(s, i);
  if(s[i]!=':') throw_error("':' has not found");
  ++i;
  skip_whitespace_and_comments(s, i);
  if(s[i]!='"') throw_error("value has not started");
  ret.second=parse_string(s, i);
  return ret;
}

repository_t parse_repository(const std::string &s, size_t &i)
// pre(s[i]=='{')
// post(i==0 || s[i-1]=='}')
{
  ++i;
  repository_t repo;
  while(!is_eof(s, i)){
    skip_whitespace_and_comments(s, i);
    if(s[i]=='}'){
      ++i;
      if(repo.has_empty()) throw_error("repository object has empty fields");
      return repo; // オブジェクトは正常に閉じた
    }
    if(s[i]==','){ // オブジェクト内のキーと値の任意の場所にカンマOK
      ++i;
      continue;
    }
    if(s[i]=='"'){ // キーの開始
      auto[key,value]=parse_key_value(s, i);
      if(key=="rel_path_from_root") repo.rel_path_from_root=std::move(value);
      else if(key=="abs_path_of_cloned_repository") repo.abs_path_of_cloned_repository=std::move(value);
      else if(key=="url") repo.url=std::move(value);
      else throw_error("invalid key in object: " + key);
    }else throw_error("invalid value in object");
  }
  throw_error("repository object has not ended");
}

std::vector<repository_t> parse_array(const std::string &s, size_t &i)
// pre(s[i]=='[')
// post(i==0 || s[i-1]==']')
{
  ++i;
  std::vector<repository_t> repos;
  while(!is_eof(s, i)){
    skip_whitespace_and_comments(s, i);
    if(s[i]==']'){
      ++i;
      return repos; // 配列は正常に閉じた
    }
    if(s[i]==','){ // 配列内のオブジェクト以外の任意の場所にカンマOK
      ++i;
      continue;
    }
    if(s[i]=='{'){// オブジェクトの開始
      size_t start = i;
      try{
        repos.emplace_back(parse_repository(s, i));
      }catch(const std::runtime_error& e){
        std::cerr<<e.what()<<std::endl;
        std::cerr<<sv(s).substr(start, i-start)<<std::endl;
        std::terminate();
      }
    }else throw_error("invalid value in array");
  }
  throw_error("array has not ended");
}

std::vector<repository_t> parse_repositories() {
  std::stringstream ss;
  ss << std::cin.rdbuf();
  std::string s(ss.str());
  size_t i=0;
  skip_whitespace_and_comments(s, i);
  if(s[i]!='[') throw_error("array has not started");
  return parse_array(s, i);
}