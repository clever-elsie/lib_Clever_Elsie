#include <algorithm>
#include <queue>
#include <vector>
using namespace std;

// weighted graph's dijkstra
vector<int> dijkstra(vector<vector<pair<int, int>>> &edge, int start = 0) {
	vector<int> dist(edge.size(), (sizeof(int) == 4 ? 1 << 30 : 1ll << 60));
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;
	dist[start] = 0;
	q.push(make_pair(dist[start], start));
	while (!q.empty()) {
		auto [now_dist, here] = q.top(); q.pop();
		if (now_dist > dist[here]) continue;
		for (auto [e_cost, to] : edge[here]) {
			if (e_cost + now_dist < dist[to]) {
				dist[to] = e_cost + now_dist;
				q.push(make_pair(dist[to], to));
			}
		}
	}
	return dist;
}

vector<int> dijkstra(vector<vector<int>> &edge, int start = 0) {
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;
	vector<int> dist(edge.size(), (sizeof(int) == 4 ? 1 << 30 : 1ll << 60));
	dist[start] = 0;
	q.push(make_pair(dist[start], start));
	while (q.size()) {
		auto [cost, here] = q.top();
		q.pop();
		if (cost > dist[here]) continue;
		for (int to = 0; to < edge.size(); to++) {
			int next_cost = dist[here] + edge[here][to];
			if (next_cost < dist[to]) {
				dist[to] = next_cost;
				q.push(make_pair(dist[to], to));
			}
		}
	}
	return dist;
}