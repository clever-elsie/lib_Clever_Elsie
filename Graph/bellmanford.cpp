#include<vector>
#include<utility>
#include<cstdint>
using namespace std;

vector<int64_t> BF(vector<vector<pair<int64_t,int>>>&e,int64_t start=0) noexcept(false) {
	vector<int64_t> dist(e.size(),INT64_MAX);
	dist[start]=0;
	for(int _=1;_<e.size();_++){
		for(int i=0;i<e.size();i++){
			for(int j=0;j<e[i].size();j++){
				if(dist[i]<INT64_MAX){
					if(dist[i]+e[i][j].first<dist[e[i][j].second])
						dist[e[i][j].second]=dist[i]+e[i][j].first;
				}
			}
		}
	}
	for(int i=0;i<e.size();i++){
		for(int j=0;j<e[i].size();j++){
			if(dist[i]<INT64_MAX){
				if(dist[i]+e[i][j].first<dist[e[i][j].second])
					throw (int64_t)(-1);
			}
		}
	}
	return move(dist);
}