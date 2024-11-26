#include <algorithm>
#include <cstdint>
#include <list>
#include <vector>
using namespace std;

int diameter(vector<vector<pair<int, int>>> &edge, int start = 0) {
	auto farthest = [&](vector<vector<pair<int, int>>> &Edge, int Start) -> pair<int, int> {
		int inf = INT32_MAX;
		vector<int> dist(Edge.size(), -1);
		list<int> q;
		q.push_back(Start);
		dist[Start] = 0;
		while (!q.empty()) {
			int here = q.front();
			q.pop_front();
			for (auto [to_cost, to_id] : Edge[here]) {
				if (dist[to_id] == -1) {
					dist[to_id] = dist[here] + to_cost;
					q.push_back(to_id);
				}
			}
		}
		vector<int>::iterator res = max_element(begin(dist), end(dist));
		return pair<int, int>(*res, res - dist.begin());
	};
	return farthest(edge, farthest(edge, start).second).first;
}

int diameter(vector<vector<int>> &edge, int start = 0) {
	vector<vector<pair<int, int>>> weighted_edge(edge.size());
	for (size_t i = 0; i < edge.size(); i++) {
		weighted_edge[i].resize(edge[i].size());
		for (size_t j = 0; j < edge[i].size(); j++)
			weighted_edge[i][j] = make_pair(1, edge[i][j]);
	}
	return diameter(weighted_edge, start);
}