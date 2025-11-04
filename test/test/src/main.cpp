#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <format>
#include <string>
#include <cstdlib>
#include <map>
#include <unistd.h>
#include <sys/wait.h>

namespace fs = std::filesystem;

// lib_Clever_Elsie/test/test/src/main.cpp -> lib_Clever_Elsie/test/
inline const fs::path test_path = fs::path(__FILE__).parent_path().parent_path().parent_path();
// lib_Clever_Elsie/test/ -> lib_Clever_Elsie/
inline const fs::path lib_path = test_path.parent_path();
inline const std::string cc=std::string("g++ -std=c++23 -I")+lib_path.string()+" ";

void update_last_executed_time(const fs::path&path, const std::chrono::system_clock::time_point&time){
  if(!fs::exists(path / "meta.txt"))
    fs::create_directories(path/"meta.txt");
  std::ofstream ofs(path / "meta.txt", std::ios::trunc);
  ofs << time.time_since_epoch().count();
}

std::chrono::system_clock::time_point last_executed_time(const fs::path&path){
  using namespace std::chrono;
  if(!fs::exists(path / "meta.txt")) return system_clock::time_point::min();
  std::ifstream ifs(path / "meta.txt");
  std::string time_s;
  std::getline(ifs, time_s);
  return system_clock::time_point(system_clock::duration(std::stoll(time_s)));
}

void check_test(const fs::path&path){
  // test.cppが.hppか最終実行日より新しいとき，test.cppをコンパイル
  // spec.cppが.hppか最終実行日より新しいとき，spec.cppをコンパイル
  // .hppが最終実行日より新しいとき，test.cppとspec.cppをコンパイル
  // test.cppかspec.cppがリコンパイルされたとき，test.cppとspec.cppを実行．spec.cppはすべての入出力ケースに対して実行．
  // test.cppもspec.cppもリコンパイルされていないとき，テストの最終実行日より新しい入出力ファイルに対してのみ実行．
  const auto current_time = std::chrono::system_clock::now();
  const fs::path relative = fs::relative(path, test_path);
  const fs::path on_header = [](const fs::path&relative){ // testではディレクトリだが，ヘッダはレギュラーファイルなので末尾の/を削除
    std::string p =(lib_path/relative).string();
    while(p.size()&&p.back()=='/') p.pop_back();
    return fs::path(p);
  }(relative);
  const fs::path test_cpp = path / "test.cpp";
  const fs::path spec_cpp = path / "spec.cpp";
  const fs::path test_out = path / "test.out";
  const fs::path spec_out = path / "spec.out";
  using namespace std::chrono;
  const auto LET = last_executed_time(on_header);
  const auto TWT = clock_cast<system_clock>(fs::last_write_time(test_cpp));
  const auto SWT = clock_cast<system_clock>(fs::last_write_time(spec_cpp));
  const auto HWT = clock_cast<system_clock>(fs::last_write_time(on_header));
  const auto OLD = std::min(HWT,LET);
  using key_type = std::string;
  using value_type = std::pair<int, system_clock::time_point>;
  std::map<key_type, value_type> test_cases, spec_cases;
  // ファイル名{拡張子除く}
  // {bit{out, in}の存在判定, in/outの新しい方の時間} }
  static auto load_test_cases = [](std::map<key_type, value_type>&test_cases, const fs::path&in, const fs::path&out){
    for(const auto&entry:fs::directory_iterator(in))
      test_cases.emplace(entry.path().stem().string(),
      value_type{1, clock_cast<system_clock>(fs::last_write_time(entry.path()))});
    for(const auto&entry:fs::directory_iterator(out)){
      auto it = test_cases.find(entry.path().stem().string());
      if(it != test_cases.end()){
        it->second.first |= 2;
        it->second.second = std::max(it->second.second, clock_cast<system_clock>(fs::last_write_time(entry.path())));
      }
    }
    for(auto it = test_cases.begin(); it!=test_cases.end();)
      if(it->second.first != 3) it=test_cases.erase(it);
      else ++it;
  };
  load_test_cases(spec_cases, path/"in", path/"out");
  load_test_cases(test_cases, path/"tin", path/"tout");
  for(auto it = test_cases.begin(); it!=test_cases.end();)
    if(it->second.first != 3) it=test_cases.erase(it);
    else ++it;
  static auto execute_test = [](
    const std::map<key_type, value_type>&test_cases,
    const fs::path&in, const fs::path&out,
    const std::string& exec,
    const system_clock::time_point&since
  )-> bool {
    for(const auto&[name, flag_time]:test_cases){
      auto[flag, time] = flag_time;
      if(time<since) continue;
      int fd[2];
      if(pipe(fd)==-1){
        std::cerr << "pipe failed" << std::endl;
        return true;
      }
      if(int pid=fork(); pid==0){
        dup2(fd[0], STDIN_FILENO);
        dup2(fd[1], STDOUT_FILENO);
        dup2(fd[1], STDERR_FILENO);
        close(fd[0]),close(fd[1]);
        execlp(exec.c_str(), exec.c_str(), nullptr);
      }else{
        std::stringstream ss;
        std::fstream os(in/(name+".in"));
        ss << os.rdbuf();
        ss << std::endl;
        write(fd[1], ss.str().c_str(), ss.str().size());
        close(fd[1]);
        std::string output;
        char buf[4096];
        ssize_t len;
        for(;(len=read(fd[0], buf, sizeof(buf)))>0;)
          output.append(buf, len);
        close(fd[0]);
        int status;
        waitpid(pid, &status, 0);
        std::fstream expected(out/(name+".out"));
        std::stringstream expss;
        expss << expected.rdbuf();
        if(output!=expss.str()){
          std::cerr << name << " failed" << std::endl;
          return true;
        }
        return false;
      }
    }
  };
  if(HWT>LET || TWT>OLD || SWT>OLD){
    std::system((cc+test_cpp.string()+" -g -O0 -Wall -Wextra -Werror -o "+test_out.string()).c_str());
    std::system((cc+spec_cpp.string()+" -O2 -flto -march=native -o "+spec_out.string()).c_str());
    if(test_cases.size()){
      execute_test(test_cases, path/"tin", path/"tout", test_out.string(), system_clock::time_point::min());
    }else if(!fs::exists(path/"tin")||fs::hard_link_count(path/"tin")<=2){
      // testケースが無い場合はtest.cppのみでコンパイルテストができるとみなす
      int code = std::system(test_out.string().c_str());
      if(code!=0){
        std::cerr << test_out.string() << " failed to compile" << std::endl;
        return;
      }
    }
    if(spec_cases.size())
      execute_test(spec_cases, path/"in", path/"out", spec_out.string(), system_clock::time_point::min());
  }else{ // 新規追加の入出力ファイルに対してのみ実行
    if(test_cases.size())
      if(execute_test(test_cases, path/"tin", path/"tout", test_out.string(), LET))
        return;
    if(spec_cases.size())
      if(execute_test(spec_cases, path/"in", path/"out", spec_out.string(), LET))
        return;
  }
}

void recursively_scan_test_files(const fs::path&path){
  for(const auto&entry:fs::directory_iterator(path)){
    if(!entry.is_directory()) continue;
    if(entry.path().string().find(".hpp")!=std::string::npos)
      check_test(entry.path());
    else recursively_scan_test_files(entry.path());
  }
}

int main(){
  recursively_scan_test_files(test_path);
}