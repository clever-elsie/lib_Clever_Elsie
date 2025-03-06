#ifndef ELSIE_MIN_COST_MAX_FLOW
#define ELSIE_MIN_COST_MAX_FLOW
#include <vector>
#include <cstdint>
#include <utility>
namespace elsie{
	using namespace std;
	template<class cap_t,class cost_t>
	class mcmf{
		private:
		template<class S>using vc=vector<S>;
		template<class S>using vv=vc<vc<S>>;
		using pcc=pair<cap_t,cost_t>;
		public:
		struct edge{
			uint32_t from,to;
			cap_t cap,flow;
			cost_t cost;
		};
		private:
		uint32_t v;
		vc<edge>edges;
		public:
		mcmf(uint32_t n):v(n){}
		uint32_t add_edge(uint32_t s,uint32_t t){}
		pcc flow(uint32_t s,uint32_t t){}
		pcc flow(uint32_t s,uint32_t t,cap_t flow_limit){}
		vc<pcc>slope(uint32_t s,uint32_t t){}
		vc<pcc>slope(uint32_t s,uint32_t t,cap_t flow_limit){}
	};
}
#endif