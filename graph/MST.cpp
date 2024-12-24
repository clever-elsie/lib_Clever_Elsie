#include <vector>
#include <queue>
#include <algorithm>
#include <utility>
using namespace std;
#define int long long
template <class f> using vc=vector<f>;
template <class f> using vv=vc<vc<f>>;
using vi = vc<int>;
using vb = vc<bool>;
using pi = pair<int,int>;
template<class f>
using pqg=priority_queue<f,vc<f>,greater<f>>;

class unionFind{
	private:const int _order;
	protected:vector<int>pr;
	public:
		unionFind(int n):_order(n),pr(n,-1){}
		int ord(){return _order;}
		int size(int u){return -pr[root(u)];}
		int root(int u){return(pr[u]<0?u:pr[u]=root(pr[u]));}
		bool same(int u,int v){return root(u)==root(v);}
		void unite(int u,int v) {
			u=root(u),v=root(v);
			if(u==v)return;
			if(pr[u]<pr[v])swap(u,v);
			pr[v]+=pr[u];
			pr[u]=v;
		}
};

// vector<cost,{from,to}> |E|!=n
size_t kruskal(const int n,vc<pair<int,pi>>&e){
	unionFind uf(n);
	sort(e.begin(),e.end());
	size_t ans=0;
	for(const auto&[cost,ft]:e){
		auto [u,v]=ft;
		if(!uf.same(u,v)){
			uf.unite(u,v);
			ans+=cost;
		}
	}
	return ans;
}
pair<size_t,vi> kruskal(const int n,const vc<pair<int,pi>>&e){
	unionFind uf(n);
	vc<pair<int,pair<int,pi>>>p(e.size());
	for(int i=0;i<e.size();i++)
		p[i]=make_pair(e[i].first,make_pair(i,e[i].second));
	sort(p.begin(),p.end());
	size_t ans=0;
	vi ansv;
	for(const auto&[cost,ift]:p){
		auto [id,ft]=ift;
		auto [u,v]=ft;
		if(!uf.same(u,v)){
			uf.unite(u,v);
			ans+=cost;
			ansv.push_back(id);
		}
	}
	return move(make_pair(ans,ansv));
}

// v[from]={cost,to}
size_t prim(const vv<pi>&e){
	size_t ans=0;
	vb seen(e.size(),0);
	seen[0]=1;
	pqg<pi>q;
	for(const auto&edge:e[0])q.push(edge);
	while(q.size()){
		auto [cost,to]=q.top();q.pop();
		if(seen[to])continue;
		seen[to]=1;
		ans+=cost;
		for(const auto&edge:e[to])q.push(edge);
	}
	return ans;
}
// v[from]={cost,to,id}
pair<size_t,vi> prim(const vv<pair<pi,int>>&e){
	size_t ans=0;
	vi ansv;
	vb seen(e.size(),0);
	seen[0]=1;
	pqg<pair<pi,int>>q;
	for(const auto&edge:e[0])q.push(edge);
	while(q.size()){
		auto [E,id]=q.top();q.pop();
		auto [cost,to]=E;
		if(seen[to])continue;
		seen[to]=1;
		ans+=cost;
		ansv.push_back(id);
		for(const auto&edge:e[to])q.push(edge);
	}
	return move(make_pair(ans,ansv));
}