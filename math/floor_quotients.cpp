#include <vector>
#include <cstdint>
using namespace std;

vector<uint64_t>quotients(uint64_t n){
	using u64 = uint64_t;
	auto ceil=[](u64 p,u64 x)->u64 { return (p+x-1)/x; };
	vector<u64> ret;
	for(int x=n;x>0;){
		u64 q=n/x;
		ret.push_back(q);
		x=n/(q+1);
	}
	return move(ret);
}
