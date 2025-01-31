#include <cstdint>
#include <vector>
#include <set>
#ifndef ELSIE_SCC
#define ELSIE_SCC
namespace elsie{
using namespace std;
class scc_graph{
	using vi=vector<int32_t>;
	using vvi=vector<vi>;
	size_t n,cnt;
	vi visited,cmp;
	vector<set<int32_t>> edge,redge;
	vector<bool>seen;
	void dfs(int32_t now){
		seen[now]=1;
		for(const auto&to:edge[now])
			if(!seen[to])dfs(to);
		visited.push_back(now);
	}
	void rdfs(int32_t now,int32_t k){
		seen[now]=1;
		cmp[now]=k;
		for(const auto&to:redge[now])
			if(!seen[to])rdfs(to,k);
	}
	public:
	scc_graph(int32_t N):n(N),edge(n),redge(n),seen(n),cmp(n,0),cnt(0){}
	void add_edge(int32_t u,int32_t v){
		edge[u].insert(v);
		redge[v].insert(u);
	}
	vvi scc(){
		seen.assign(seen.size(),false);
		for(int32_t i=0;i<n;i++)if(!seen[i])dfs(i);
		seen.assign(seen.size(),false);
		int32_t k=0;
		for(auto itr=visited.rbegin();itr!=visited.rend();itr++)
			if(!seen[*itr]){
				rdfs(*itr,k);
				k++;
			}
		vvi ans(k);
		for(int32_t i=0;i<n;i++)
			ans[cmp[i]].push_back(i);
		return move(ans);
	}
};
}
#endif