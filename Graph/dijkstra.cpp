#include <algorithm>
#include <queue>
#include <vector>

// weighted graph's dijkstra
using namespace std;
template<class t>
using vc = vector<t>;
template<class t>
using vv = vc<vc<t>>;
using vi = vector<int>;
using vb = vector<bool>;
using pi = pair<int,int>;
vi dijkstra(vv<pi> &edge,int start=0) {
	vi dist(edge.size(), 1ll << 60);
	priority_queue<pi,vc<pi>,greater<pi>>q;
	dist[start] = 0;
	q.push(make_pair(dist[start], start));
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

// no weight graph dijkstra
std::vector<int> dijkstra(std::vector<std::vector<int>> &edge,int start=0) {
	using PAIR = std::pair<int, int>;
	std::vector<int> dist(edge.size(), 1ll << 60);
	std::vector<bool> confirm(edge.size(),false);
	std::priority_queue<PAIR, std::vector<PAIR>, std::greater<PAIR>> q;
	dist[start] = 0;
	q.push(std::make_pair(dist[start], start));
	while (!q.empty()) {
		auto [now_dist, here] = q.top();
		q.pop();
		if(confirm[here])continue;
		confirm[here]=true;
		for (auto to : edge[here]) {
			if (1 + now_dist < dist[to]) {
				dist[to] = 1 + now_dist;
				q.push(std::make_pair(dist[to], to));
			}
		}
	}
	return dist;
}