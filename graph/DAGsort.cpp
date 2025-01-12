#include <vector>
#include <deque>

using namespace std;

template<class T>vector<size_t>DAGsort(const vector<vector<T>>&edge){
	size_t n=edge.size();
	vector<set<T>>e(n);
	vector<size_t>deg(n,0);
	for(size_t i=0;i<n;i++)
		for(const auto&x:edge[i]){
			e[i].insert(x);
			deg[x]++;
		}
	int cnt=0;
	vector<size_t>ans(n,0);
	deque<size_t>q;
	for(size_t i=0;i<n;i++)
		if(!deg[i]){
			q.push_back(i);
			ans[i]=cnt++;
		}
	while(q.size()){
		size_t now=q.front();
		q.pop_front();
		for(const auto&to:e[now]){
			deg[to]--;
			if(deg[to]==0){
				q.push_back(to);
				ans[to]=cnt++;
			}
		}
	}
	return move(ans);
}