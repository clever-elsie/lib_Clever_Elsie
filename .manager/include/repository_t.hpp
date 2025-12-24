#pragma once

#include <filesystem>
#include <string>

constexpr const char* root_path = "../"; // CWDは/.manager．makeコマンドで実行されることを想定

struct repository_t {
  std::filesystem::path rel_path_from_root;
  std::filesystem::path abs_path_of_cloned_repository;
  std::string url;
  bool has_empty() const {
    return rel_path_from_root.empty() ||
           abs_path_of_cloned_repository.empty() || url.empty();
  }
  std::string to_string() const {
    std::string r,a,u;
    r=std::format("rel_path_from_root: {}", rel_path_from_root.string());
    a=std::format("abs_path_of_cloned_repository: {}", abs_path_of_cloned_repository.string());
    u=std::format("url: {}", url);
    return r + "\n" + a + "\n" + u;
  }
};