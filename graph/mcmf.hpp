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
        using u32=uint32_t;
        public:
        struct edge{
            u32 to,rev;
            cap_t cap,flow;
            cost_t cost;
            edge(u32 to_,cap_t cap_,cost_t cost_,u32 rev_)
            :to(to_),cap(cap_),flow(0),cost(cost_),rev(rev_){}
        };
        private:
        uint32_t v;
        vc<edge>e;
        vc<cost_t>pot;
        vc<uint32_t>pv,pe;
        public:
        mcmf(uint32_t n):v(n),e(n),pot(n),pv(n),pe(n){}
        void add_edge(uint32_t s,uint32_t t,cap_t cap,cost_t cost){
            e[s].emplace_back(edge(t,cap,cost,e[t].size()));
            e[t].emplace_back(edge(s,0,-cost,g[s].size()-1));
        }
        pcc flow(uint32_t s,uint32_t t){}
        pcc flow(uint32_t s,uint32_t t,cap_t flow_limit){}
        vc<pcc>slope(uint32_t s,uint32_t t){}
        vc<pcc>slope(uint32_t s,uint32_t t,cap_t flow_limit){}
    };
}
#endif