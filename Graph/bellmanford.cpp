#include<vector>
#include<utility>
#include<cstdint>
using namespace std;

vector<int64_t> BF(vector<vector<pair<int64_t,pair<int,int>>>>&e,int64_t start=0) noexcept(false) {
	vector<int64_t> dist(e.size(),INT64_MAX);
	e[start]=0;
	for(int _=1;_<e.size();_++){
		for(int i=0;i<e.size();i++){
			for(int j=0;j<e[i].size();j++){
				if(dist[e[i][j].second.first]<INT64_MAX){
					if(dist[e[i][j].second.first]+e[i][j].first<dist[e[i][j].second.second])
						dist[e[i][j].second.second]=dist[e[i][j].second.first]+e[i][j].first;
				}
			}
		}
	}
	for(int i=0;i<e.size();i++){
		for(int j=0;j<e[i].size();j++){
			if(dist[e[i][j].second.first]<INT64_MAX){
				if(dist[e[i][j].second.first]+e[i][j].first<dist[e[i][j].second.second])
					throw (int64_t)(-1);
			}
		}
	}
	return move(dist);
}