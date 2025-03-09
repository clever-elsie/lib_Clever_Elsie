#include <vector>
#include <array>
#include <cstddef>
#include <cstdint>
using namespace std;

template<integral type>
class wavelet{
	// val \in [0,type_max]
	array<vector<bool>,sizeof(type)*8>b;
	vector<type>t;
	public:
	wavelet(const vector<type>&v){
		t.resize(v.size());
		// bucket and index for sort
		size_t idx[4]={0,v.size(),0,0};
		bool bflip=0,pflip=1;
		array<vector<type>,4>bt; // 0:(0,1), 1:(2,3)
		for(size_t i=0;i<4;i++)bt[i].resize(v.size());
		bt[1]=v;
		for(ssize_t s=b.size()-1;s>=0;s--){
			size_t mask=1ll<<s,bidx=0;
			b[s].resize(t.size());
			for(size_t i=0;i<idx[pflip];i++){
				if(bt[pflip][i]&mask){
					b[s][bidx++]=true;
					bt[2+bflip][idx[2+bflip]++]=bt[pflip][i];
				}else{
					b[s][bidx++]=false;
					bt[bflip][idx[bflip]++]=bt[pflip][i];
				}
			}
			for(size_t i=0;i<idx[2+pflip];i++){
				if(bt[2+pflip][i]&mask){
					b[s][bidx++]=true;
					bt[2+bflip][idx[2+bflip]++]=bt[2+pflip][i];
				}else{
					b[s][bidx++]=false;
					bt[bflip][idx[bflip]++]=bt[2+pflip][i];
				}
			}
			idx[pflip]=idx[2+pflip]=0;
			swap(bflip,pflip);
		}
		size_t tidx=0;
		for(size_t i=0;i<idx[pflip];i++)t[tidx++]=bt[pflip][i];
		for(size_t i=0;i<idx[2+pflip];i++)t[tidx++]=bt[2+pflip][i];
	}
	void clear(){
		t.clear();
		for(size_t i=0;i<b.size();i++)b[i].clear();
	}
};