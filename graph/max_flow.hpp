#include<cstddef>
#include<cstdint>
#include<vector>
#ifndef ELSIE_MAXFLOW
#define ELSIE_MAXFLOW
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
}
#endif