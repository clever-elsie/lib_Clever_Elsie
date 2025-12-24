#pragma once
#include "./repository_t.hpp"
#include <filesystem>

bool ready_repository(const repository_t &repo);
bool clone_repository(const repository_t &repo);
bool update_repository(const repository_t &repo);
std::filesystem::path relpath_to_symlink(const std::filesystem::path &rel_path);
std::filesystem::path abs_path_of_refferd_dir(const std::filesystem::path &abs_path);