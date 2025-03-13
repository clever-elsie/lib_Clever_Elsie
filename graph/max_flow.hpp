#ifndef ELSIE_MAXFLOW
#define ELSIE_MAXFLOW
#include<vector>
#include<cstddef>
#include<cstdint>
#include<utility>
namespace elsie{
using namespace std;
template<class type>
class max_flow{
    class edge {
        public:
            edge(int64_t u, type c, int64_t r):to(u),cap(c),rev(r) {}
            int64_t to, rev;
            type cap;
    };
    vector<vector<edge>>e;
    vector<bool>seen;
    int64_t g,r;
    int64_t dfs(int64_t now,type ret){
        if(now==g)return ret;
        seen[now]=true;
        for(auto&x:e[now]){
            if(x.cap==0||seen[x.to])continue;
            int64_t p=dfs(x.to,min(x.cap,ret));
            if(p){
                x.cap-=p;
                e[x.to][x.rev].cap+=p;
                return p;
            }
        }
        return 0;
    }
    public:
        max_flow(int64_t n){ e.resize(n), seen.resize(n); }
        void add(int64_t u,int64_t v,type c){
            int64_t ru=e[u].size(),rv=e[v].size();
            e[u].push_back(edge(v,c,rv)), e[v].push_back(edge(u,static_cast<type>(0),ru));
        }
        int64_t flow(int64_t s,int64_t t){
            g=t,r=0;
            while(1){
                fill(begin(seen),end(seen),false);
                int64_t p=dfs(s,1ll<<62);
                if(p)r+=p;
                else break;
            }
            return r;
        }
};
#ifdef ELSIE_MCMF
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
#endif
}
#endif