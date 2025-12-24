#include <cctype>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <format>
#include <cstdlib>

#include "../include/repository_t.hpp"
#include "../include/parser.hpp"
#include "../include/ready.hpp"

// 標準入力で以下の形式が与えられる
/*
[
  {
    "rel_path_from_root": "dtStrc/binary_tree/rbtree", // this path is indicates directory
    "abs_path_of_cloned_repository": "/path/to/cloned/repository",
    "url": "https://github.com/clever-elsie/rbtree.git"
  },
  ...
]
ケツカンマOK，コメント付き，文字列，配列のJSONサブセットのみで記述．
コメントは#で始まるか//で始まる行末かEOFまでの文字列
*/

int main() {
  auto repositories = parse_repositories();
  for(const auto& repo : repositories)
    if(!ready_repository(repo)){
      std::cerr << std::format("Failed to ready repository: {}", repo.to_string()) << std::endl; }
}
