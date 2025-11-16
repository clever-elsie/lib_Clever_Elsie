#ifndef ELSIE_UNIONFIND
#define ELSIE_UNIONFIND
#include <algorithm>
#include <numeric>
#include <vector>
#include <cstdint>
#include <utility>
#include <unordered_map>
namespace elsie{
class unionFind{
  protected:
    using it=int32_t;
    it _order;
    mutable std::vector<it>pr;
  public:
    unionFind():_order(0),pr(0){}
    unionFind(it n):_order(n),pr(n,-1){}
    it ord()const{return _order;}
    it size(it u)const{return -pr[root(u)];}
    it root(it u)const{return(pr[u]<0?u:pr[u]=root(pr[u]));}
    bool same(it u,it v)const{return root(u)==root(v);}
    void unite(it u,it v)const{
			if((u=root(u))==(v=root(v))) return;
      if(pr[u]<pr[v])std::swap(u,v);
      pr[v]+=pr[u];
      pr[u]=v;
    }
    std::vector<std::vector<it>> groups()const{
      std::vector<std::vector<it>>ret;
      std::unordered_map<it,it>idx;
      for(it i=0;i<_order;++i){
        it r=root(i);
        auto itr=idx.find(r);
        if(itr==idx.end()){
          idx[r]=ret.size();
          ret.emplace_back(1,i);
        }else ret[itr->second].push_back(i);
      }
      return ret; //NRVO
    }
    unionFind&operator=(auto&&other){
      if(this!=&other){
        _order=other._order;
        pr=std::forward(other.pr);
      }return*this;
    }
};

template<class S,S(*op)(S,S),S(*e)()>
class dsu:public unionFind{
  private:std::vector<S>_mset;
  public:
    dsu():unionFind(),_mset(0){}
    dsu(int32_t n):unionFind(n),_mset(n,e()){}
    void set(int32_t u,const S&v){_mset[root(u)]=v;}
    S prod(int32_t u){return _mset[root(u)];}
    void unite(int32_t u,int32_t v){
      int ru=root(u),rv=root(v);
      unionFind::unite(u,v);
      _mset[(ru==root(u)?ru:rv)]=op(_mset[ru],_mset[rv]);
    }
};

template<class S>class unionFindP{
  using it=int32_t;
  it _order;
  mutable std::vector<it>pr;
  mutable std::vector<S>pot;
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
    if(pr[u]<pr[v])std::swap(u,v),w=-w;
    pr[v]+=pr[u];pr[u]=v;pot[u]=w;return 1;
  }
  unionFindP&operator=(auto&&other){
    if(this!=&other){
      _order=other._order;
      pr=std::forward(other.pr);
      pot=std::forward(other.pot);
    }return*this;
  }
};
}
#endif