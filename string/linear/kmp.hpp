#ifndef ELSIE_KMP
#define ELSIE_KMP
#include<string>
#include<vector>
#include<cstdint>
#include<cstdlib>
#include<utility>
namespace elsie{
using namespace std;
vector<ssize_t> morris_pratt(const string&tar){
    size_t n=tar.size();
    vector<ssize_t>t(n+1);
    ssize_t j=-1;
    t[0]=-1;
    for(size_t i=0;i<n;++i){
        while(j>=0&&tar[i]!=tar[j])j=t[j];
        t[i+1]=++j;
    }
    return t;
}
vector<ssize_t> knuth_morris_pratt(const string&tar){
    size_t n=tar.size();
    vector<ssize_t>t(n+1);
    ssize_t j=-1;
    t[0]=-1;
    for(size_t i=0;i<n;++i){
        while(j>=0&&tar[i]!=tar[j])j=t[j];
        if(tar[i+1]==tar[++j])t[i+1]=t[j];
        else t[i+1]=j;
    }
    return t;
}
class kmp{
    private:
    string src,word;
    vector<ssize_t>t;
    public:
    kmp():src(""){}
    kmp(string&&SRC):src(SRC){}
    kmp(const string&SRC):src(SRC){}
    kmp(string&&SRC,string&&tar):src(SRC){ build(move(tar)); }
    kmp(string&&SRC,const string&tar):src(SRC){ build(tar); }
    kmp(const string&SRC,string&&tar):src(SRC){ build(move(tar)); }
    kmp(const string&SRC,const string&tar):src(SRC){ build(tar); }
    void set(const string&SRC){ src=SRC; }
    void set(string&&SRC){ src=move(SRC); }
    void build(string&&tar){
        word=move(tar);
        t=knuth_morris_pratt(word);
    }
    void build(const string&tar){
        word=tar;
        t=knuth_morris_pratt(word);
    }
    vector<ssize_t> find(){
        vector<ssize_t> ret;
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