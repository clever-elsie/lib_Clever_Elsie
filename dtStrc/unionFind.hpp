#include <algorithm>
#include <numeric>
#include <vector>
#include <cstdint>
#ifndef ELSIE_UNIONFIND
#define ELSIE_UNIONFIND
namespace elsie{
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
		unionFind&operator=(auto&&other){
			if(this!=&other){
				_order=other._order;
				pr=forward(other.pr);
			}return*this;
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

template<class S>class unionFindP{
	using it=int32_t;
	it _order;
	mutable vector<it>pr;
	mutable vector<S>pot;
	public:
	unionFindP():_order(0),pr(0),pot(0){}
	unionFindP(it n):_order(n),pr(n,-1),pot(n,S()){}
	it ord()const{return _order;}
	it size(it u)const{return -pr[root(u)];}
	bool same(it u,it v)const{return root(u)==root(v);}
	S potential(it u)const{root(u);return pot[u];}
	S diff(it u,it v)const{return potential(u)-potential(v);}
	it root(it u)const{
		if(pr[u]<0)return u;
		it r=root(pr[u]);
		pot[u]+=pot[pr[u]];
		return pr[u]=r;
	}
	bool unite(it u,it v,S w)const{
		w+=potential(v)-potential(u);
		u=root(u),v=root(v);
		if(u==v)return w==S();
		if(pr[u]<pr[v])swap(u,v),w=-w;
		pr[v]+=pr[u];pr[u]=v;pot[u]=w;return 1;
	}
	unionFindP&operator=(auto&&other){
		if(this!=&other){
			_order=other._order;
			pr=forward(other.pr);
			pot=forward(other.pot);
		}return*this;
	}
};
}
#endif