#include "../include/ready.hpp"
#include <iostream>
namespace fs = std::filesystem;


bool ready_repository(const repository_t &repo) {
  // 絶対パスがあるとき，update
  // ないときclone
  if(std::filesystem::exists(repo.abs_path_of_cloned_repository))
    return update_repository(repo);
  else
    return clone_repository(repo);
}

bool clone_repository(const repository_t &repo) {
  // 絶対パスにリポジトリがインストールされてないので，
  // cloneして，そのincludeにsymlinkを張る
  fs::path repo_root = repo.abs_path_of_cloned_repository;
  fs::path target_link = root_path / repo.rel_path_from_root;

  std::string clone_command = std::format("git clone {} {}", repo.url, repo_root.string().c_str());
  if(std::system(clone_command.c_str())!=0) return false;

  if(!fs::exists(target_link.parent_path()))
    fs::create_directories(target_link.parent_path());
  try{
    fs::create_directory_symlink(repo_root/"include", target_link);
  }catch(const fs::filesystem_error &e){
    std::cerr << e.what() << std::endl;
    return false;
  }
  return true;
}

bool update_repository(const repository_t &repo) {
  // リンクチェック
  // 次にgit pull
  fs::path target_link = root_path / repo.rel_path_from_root;
  fs::path repo_root = repo.abs_path_of_cloned_repository;
  if(!fs::exists(target_link)|| fs::read_symlink(target_link)!=repo_root/"include"){
    fs::remove(target_link);
    fs::create_directories(target_link.parent_path());
    fs::create_directory_symlink(repo_root/"include", target_link);
  }
  const std::string pull_format = std::format("cd \"{}\" && git pull", repo_root.string().c_str());
  return std::system(pull_format.c_str())==0;
}