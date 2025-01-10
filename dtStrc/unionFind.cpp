#include <algorithm>
#include <numeric>
#include <vector>
#include <functional>
#include <cstdint>
using namespace std;

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