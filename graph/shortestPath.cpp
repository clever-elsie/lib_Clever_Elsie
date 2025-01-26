#include <vector>
#include <queue>
#include <algorithm>
#include <utility>
#include <cstdint>
using namespace std;

#define int long long
template<class f>vc=vector<f>;
template<class f>vv=vc<vc<f>>;
template<class f>pqg=priority_queue<f,vc<f>,greater<f>>;
using vi = vc<int>;
using pi = pair<int,int>;

// weighted graph's dijkstra
vi dijkstra(const vv<pi>&edge,int start=0){
	constexpr size_t int_width=sizeof(int);
	vi dist(edge.size(),1ll<<int_width*8-int_width/2);//16->15, 32->30, 64->60
	pqg<pi>q;
	dist[start]=0;
	q.push(make_pair(dist[start], start));
	while (!q.empty()) {
		auto[now_dist,now]=q.top();q.pop();
		if(now_dist>dist[now])continue;
		for(auto[e_cost,to]:edge[now]){
			if(e_cost+now_dist<dist[to]){
				dist[to]=e_cost+now_dist;
				q.push(make_pair(dist[to],to));
			}
		}
	}
	return move(dist);
}
// [dist,path]
pair<vi,vi> dijkstra(const vv<pi>&edge,int start,int t){
	constexpr size_t int_width=sizeof(int);
	vi dist(edge.size(),1ll<<int_width*8-int_width/2);//16->15, 32->30, 64->60
	vi pre(edge.size(),-1);
	pqg<pi>q;
	dist[start]=0;
	q.push(make_pair(dist[start], start));
	while (!q.empty()) {
		auto[now_dist,now]=q.top();q.pop();
		if(now_dist>dist[now])continue;
		for(auto[e_cost,to]:edge[now]){
			if(e_cost+now_dist<dist[to]){
				dist[to]=e_cost+now_dist;
				pre[to]=now;
				q.push(make_pair(dist[to],to));
			}
		}
	}
	vi p(1,t);
	while(t!=start){
		t=pre[t];
		p.push_back(t);
	}
	reverse(p.begin(),p.end());
	return move(pair<vi,vi>(dist,p));
}

// weight matrix dijkstra
vi dijkstra(const vv<int>&edge,int start=0){
	constexpr size_t int_width=sizeof(int);
	vi dist(edge.size(),1ll<<int_width*8-int_width/2);
	pqg<pi>q;
	dist[start] = 0;
	q.push(make_pair(dist[start],start));
	while(q.size()){
		auto[cost,now]=q.top();
		q.pop();
		if(cost>dist[now])continue;
		for(int to=0;to<edge.size();to++){
			int next_cost=dist[now]+edge[now][to];
			if(next_cost<dist[to]){
				dist[to]=next_cost;
				q.push(make_pair(dist[to],to));
			}
		}
	}
	return dist;
}

vi BF(vv<pi>&e,int start=0) noexcept(false) {
	vi dist(e.size(),INT64_MAX);
	dist[start]=0;
	for(int _=1;_<e.size();_++)
	for(int i=0;i<e.size();i++)
	for(int j=0;j<e[i].size();j++)
		if(dist[i]<INT64_MAX)
		if(dist[i]+e[i][j].first<dist[e[i][j].second])
			dist[e[i][j].second]=dist[i]+e[i][j].first;
	for(int i=0;i<e.size();i++)
	for(int j=0;j<e[i].size();j++)
		if(dist[i]<INT64_MAX)
		if(dist[i]+e[i][j].first<dist[e[i][j].second])
			throw (int64_t)(-1);
	return move(dist);
}

void WF(vector<vector<int64_t>>&e)noexcept(false){
	size_t n=e.size();
	for(size_t k=0;k<n;++k)
	for(size_t i=0;i<n;++i)
	for(size_t j=0;j<n;++j)
		e[i][j]=min(e[i][j],e[i][k]+e[k][j]);
	for(size_t i=0;i<n;++i)
		if(e[i][i]<0)throw(int64_t)(-1);
}
