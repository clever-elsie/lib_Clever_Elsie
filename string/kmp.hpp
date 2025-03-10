#ifndef ELSIE_KMP
#define ELSIE_KMP
#include<string>
#include<vector>
#include<cstdint>
#include<cstdlib>
namespace elsie{
using namespace std;
class kmp{
    private:
    string src,word;
    vector<ssize_t>t;
    public:
    kmp(const string&SRC):src(SRC){}
    kmp(const string&SRC,const string&tar):src(SRC){ make_match(tar); }
    void make_match(const string&tar){
        word=tar;
        size_t n=tar.size();
        ssize_t j=-1;
        t.resize(n+1);
        t[0]=-1;
        for(size_t i=0;i<n;i++){
            while(j>=0&&tar[i]!=tar[j])j=t[j];
            t[i+1]=++j;
        }
    }
    vector<ssize_t> find(){
        vector<ssize_t> ret;
        size_t i=0,j=0;
        while(i+j<src.size()){
            if(src[i+j]==word[j]){
                if(++j==word.size()){
                    ret.push_back(i);
                    i=i+j-t[j];
                    j=t[j];
                }
            }else{
                i=i+j-t[j];
                if(j)j=t[j];
                else j=0;
            }
        }
        return move(ret);
    }
};
}
#endif