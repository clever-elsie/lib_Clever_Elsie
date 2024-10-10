#include<string>
#include<vector>
#include<cstdint>
#include<cstdlib>
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
		size_t i=2,j=0;
		t.resize(tar.size());
		t[0]=-1,t[1]=0;
		while(i<tar.size()){
			if(tar[i-1]==tar[j]) t[i++]=++j;
			else if(j) j=t[j];
			else t[i++]=0;
		}
	}
	vector<size_t> find(){
		vector<size_t> ret;
		size_t i=0,j=0;
		while(i+j<src.size()){
			if(src[i+j]==word[j]){
				j++;
				if(j==word.size()){
					ret.push_back(i);
					i=i+j;
					j=0;
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