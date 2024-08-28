#include <algorithm>
#include <queue>
#include <vector>

// weighted graph's dijkstra
std::vector<int>
dijkstra(
	std::vector<
		std::vector<
			std::pair<int,int>>> &edge,
	int start=0
) {
	std::vector<int>
		dist(edge.size(),
			(sizeof(int)==4?1<<30:1ll<<60));
	std::priority_queue<
		std::pair<int, int>,
		std::vector<std::pair<int, int>>,
		std::greater<std::pair<int, int>>> q;
	dist[start] = 0;
	q.push(std::make_pair(dist[start], start));
	while (!q.empty()) {
		auto [now_dist, here] = q.top();
		q.pop();
		if(now_dist>dist[here])continue;
		for (auto [e_cost, to] : edge[here]) {
			if (e_cost + now_dist < dist[to]) {
				dist[to] = e_cost + now_dist;
				q.push(std::make_pair(dist[to], to));
			}
		}
	}
	return dist;
}

std::vector<int>
dijkstra(
	std::vector<
		std::vector<int>>&edge,
	int start=0;
){
	std::priority_queue<
		std::pair<int,int>,
		std::vector<std::pair<int,int>>,
		std::greater<std::pair<int,int>>>q;
	std::vector<int>dist(edge.size(),(sizeof(int)==4?1<<30:1ll<<60));
	dist[start]=0;
	q.push(std::make_pair(dist[start],start));
	while(q.size()){
		auto [cost,here]=q.top();
		q.pop();
		if(cost>dist[here])continue;
		for(int to=0;to<edge.size();to++){
			int next_cost=dist[here]+edge[here][to];
			if(next_cost<dist[to]){
				dist[to]=next_cost;
				q.push(std::make_pair(dist[to],to));
			}
		}
	}
	return dist;
}