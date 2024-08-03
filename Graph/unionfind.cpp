#include <algorithm>
#include <numeric>
#include <vector>

class union_find {
	std::vector<int> root_array, size_array;
	int root(int i) { return (i==root_array[i]?i:root_array[i]=root(root_array[i])); }
public:
	union_find(int n) {
		root_array.resize(n), size_array.resize(n,1);
		std::iota(begin(root_array), end(root_array), 0);
	}
	bool same(int i, int j) { return root(i) == root(j); }
	void unite(int i, int j) {
		i=root(i),j=root(j);
		if(size_array[i]>size_array[j])std::swap(i,j);
		root_array[i]=j;
		size_array[j]+=size_array[i];
	}
	int size(int i) { return size_array[root(i)]; }
};