#include <functional>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <cstddef>
using namespace std;
template<class f> using vc=vector<f>;
template<class f> using vv=vc<vc<f>>;
using vi = vc<int>;
using vb = vc<bool>;
vi find_cycle_directed(const vv<int>&e){
	size_t n=e.size();
	vb seen(n,false);
	set<int>now_path;
	vi cycle(0);cycle.reserve(n);
	function<int(int)> dfs=[&](int now)->int {
		for(const auto&to:e[now]){
			if(now_path.contains(to)){
				cycle.push_back(to);
				cycle.push_back(now);
				return to;
			}else if(!seen[to]){
				now_path.ins(to);
				seen[to]=true;
				int v=dfs(to);
				if(v!=-1){
					if(v<-1)return v;
					cycle.push_back(now);
					return (v==now?-2:v);
				}
			}
		}
		now_path.erase(now);
		return -1;
	};
	for(size_t i=0;i<n;i++)if(!seen[i]){
		now_path.clear();
		now_path.insert(i);
		seen[i]=true;
		dfs(i);
		if(cycle.size())break;
	}
	if(cycle.size())reverse(cycle.begin(),cycle.end());
	return move(cycle);
}

vi reconstruct_edge_id_from_to(const vi&v,const vc<map<int,int>>&e_id){
	vi ans(v.size()-1);
	for(size_t i=0;i+1<v.size();i++){
		auto itr=e_id[v[i]].find(v[i+1]);
		ans[i]=itr->second;
	}
	return move(ans);
}