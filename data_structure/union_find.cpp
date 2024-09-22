#include <algorithm>
#include <numeric>
#include <vector>

class union_find {
	std::vector<int> par, rank;
public:
	union_find(int n) { par.resize(n), rank.resize(n,0), std::iota(begin(par), end(par), 0); }
	int root(int i) { return (i==par[i]?i:par[i]=root(par[i])); }
	bool same(int i, int j) { return root(i) == root(j); }
	void unite(int i, int j) {
		i=root(i),j=root(j);
		if(i==j)return;
		rank[i]+=rank[i]==rank[j];
		if(rank[i]<rank[j])par[i]=j;else par[j]=i;
	}
};