#include <map>
#include <vector>
#include <cstdint>
#ifndef ELSIE_FACTORIZE
#define ELSIE_FACTORIZE
namespace elsie{
using namespace std;
class factorize{
	const uint64_t upper_limit;
	vector<unsigned>p;
	public:
	factorize(uint64_t upperlimit):upper_limit(upperlimit){
		uint64_t ng=0,ok=min(upper_limit,3'000'000'000ull);
		while(ng+1<ok){
			uint64_t m = midpoint(ok,ng);
			if(m*m<upper_limit)ng=m;
			else ok=m;
		}
		unsigned rs=max(ok/10,10000);
		p.reserve(rs);
		vector<bool> erathosthenes(ok+1,1);
		for(unsigned i=2;i<=ok;i++)
		if(erathosthenes[i]){
			p.push_back(i);
			for(unsigned j=i+i;j<=ok;j+=i)
				erathosthenes[j]=0;
		}
	}
	vector<int64_t>factor_vi(int64_t x){
		auto u=factor_vp(x);
		vector<int64_t>ret;ret.reserve(u.size()*2);
		for(const auto&v:u)
			for(uint32_t c=0;c<v.second;c++)
				ret.push_back(v.first);
		return move(ret);
	}
	vector<pair<int64_t,uint32_t>>factor_vp(int64_t x){
		vector<pair<int64_t,uint32_t>>ret;
		for(const auto&v:p){
			if(x%v)continue;
			ret.emplace_back(v,0);
			while(x%v==0){
				ret.back().second++;
				x/=v;
			}
		}
		return move(ret);
	}
	map<int64_t,uint32_t>factor(int64_t x){
		map<int64_t,uint32_t>ret;
		for(const auto&v:p){
			if(x%v)continue;
			auto itr=ret.emplace(v,0).first;
			while(x%v==0){
				itr->se++;
				x/=v;
			}
		}
		return move(ret);
	}
};
}
#endif