#include <algorithm>
#include <numeric>
#include <vector>
#include <functional>
#include <cstdint>
using namespace std;

class unionFind{
	protected:
		using it=int32_t;
		it _order;
		mutable vector<it>pr;
	public:
		unionFind():_order(0),pr(0){}
		unionFind(it n):_order(n),pr(n,-1){}
		it ord()const{return _order;}
		it size(it u)const{return -pr[root(u)];}
		it root(it u)const{return(pr[u]<0?u:pr[u]=root(pr[u]));}
		bool same(it u,it v)const{return root(u)==root(v);}
		void unite(it u,it v)const{
			u=root(u),v=root(v);
			if(u==v)return;
			if(pr[u]<pr[v])swap(u,v);
			pr[v]+=pr[u];
			pr[u]=v;
		}
		unionFind&operator=(const unionFind&other){
			if(this!=&other){
				_order=other._order;
				pr=other.pr;
			} return*this;
		}
		unionFind&operator=(unionFind&&other){
			if(this!=&other){
				_order=other._order;
				pr=move(other.pr);
			} return*this;
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