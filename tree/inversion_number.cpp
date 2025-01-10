#include <algorithm>
#include <atcoder/fenwicktree>
#include <vector>
using namespace std;
using namespace atcoder;

template<class f>
int inversion_number(const vector<f>&tar){
	int res=0;
	const size_t n=tar.size();
	vector<f>arr=tar, press=tar;
	fenwick_tree<f>bit(n);
	auto prb=press.begin(), pre=press.end();
	sort(prb,pre);
	press.erase(unique(prb,pre),pre);
	for(auto&x:arr)x=lower_bound(prb,pre,x)-prb;
	for (int i=0;i<n;i++){
		res+=i-bit.sum(0,arr[i]);
		bit.add(arr[i],1);
	}
	return res;
}