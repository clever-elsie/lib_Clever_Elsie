#include <algorithm>
#include <queue>
#include <vector>
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
		auto[now_dist,here]=q.top();q.pop();
		if(now_dist>dist[here])continue;
		for(auto[e_cost,to]:edge[here]){
			if(e_cost+now_dist<dist[to]){
				dist[to]=e_cost+now_dist;
				q.push(make_pair(dist[to],to));
			}
		}
	}
	return dist;
}

vi dijkstra(const vv<int>&edge,int start=0){
	constexpr size_t int_width=sizeof(int);
	vi dist(edge.size(),1ll<<int_width*8-int_width/2);
	pqg<pi>q;
	dist[start] = 0;
	q.push(make_pair(dist[start],start));
	while(q.size()){
		auto[cost,here]=q.top();
		q.pop();
		if(cost>dist[here])continue;
		for(int to=0;to<edge.size();to++){
			int next_cost=dist[here]+edge[here][to];
			if(next_cost<dist[to]){
				dist[to]=next_cost;
				q.push(make_pair(dist[to],to));
			}
		}
	}
	return dist;
}