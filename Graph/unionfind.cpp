#include <algorithm>
#include <numeric>
#include <vector>

class union_find {
	std::vector<int> root_array;
	int root(int i) {
		if (i >= root_array.size())
			return -1;
		else if (i == root_array[i])
			return i;
		else
			return root_array[i] = root(root_array[i]);
	}
public:
	union_find(int n) {
		root_array.resize(n);
		std::iota(begin(root_array), end(root_array), 0);
	}
	bool same(int i, int j) { return root(i) == root(j); }
	void unite(int i, int j) { root_array[root(i)] = root(j); }
};