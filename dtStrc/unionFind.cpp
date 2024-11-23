#include <algorithm>
#include <numeric>
#include <vector>
#include <functional>
#include <cstdint>
using namespace std;

class unionFind{
	private:const int _order;
	protected:vector<int>pr,rk,sz;
	public:
		unionFind(int n):_order(n),pr(n),rk(n,0),sz(n,1){iota(begin(pr),end(pr),0u);}
		int ord(){return _order;}
		int size(int u){return sz[root(u)];}
		int root(int u){return(u==pr[u]?u:pr[u]=root(pr[u]));}
		bool same(int u,int v){return root(u)==root(v);}
		void unite(int u,int v) {
			u=root(u),v=root(v);
			if(u==v)return;
			rk[u]+=rk[u]==rk[v];
			if(rk[u]>rk[v])swap(u,v);
			pr[u]=v;
			sz[v]+=sz[u];
		}
};

template<class S,S(*op)(S,S),S(*e)()>
class dsu:public unionFind{
	private:vector<S>_mset;
	public:
		dsu(int n):unionFind(n),_mset(n,e()){}
		void set(int u,const S&v){_mset[root(u)]=v;}
		S prod(int u){return _mset[root(u)];}
		void unite(int u,int v){
			int ru=root(u),rv=root(v);
			unionFind::unite(u,v);
			_mset[(ru==root(u)?ru:rv)]=op(_mset[ru],_mset[rv]);
		}
};