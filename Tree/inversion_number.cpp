#include <algorithm>
#include <atcoder/fenwicktree>
#include <vector>

template <class f> int inversion_number(std::vector<f> &target) {
	std::vector<f> arr = target;
	std::vector<f> press = target;
	std::sort(press.begin(), press.end());
	press.erase(unique(press.begin(), press.end()), press.end());
	for (auto &x : arr)
		x = std::lower_bound(press.begin(), press.end(), x) - press.begin();
	atcoder::fenwick_tree<f> bit(press.size());
	int res = 0;
	for (int i = 0; i < arr.size(); i++) {
		res += i - bit.sum(0, arr[i]);
		bit.add(arr[i], 1);
	}
	return res;
}