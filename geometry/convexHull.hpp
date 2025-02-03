#include <cstdint>
#include <algorithm>
#include <vector>
#include <utility>
#ifndef ELSIE_CONVEXHULL
#define ELSIE_CONVEXHULL
namespace elsie{
using namespace std;
class convex_hull{
	using it=int32_t;
	using ll=int64_t;
	using pi=pair<it,it>;
	template<class T>using vc=vector<T>;
	public:
	convex_hull(){};
	/**
	 * @brief counter clockwise
	 * @param p p[i]={x,y}
	 */
	vc<pi>monotone_chain(vc<pi>&p){
		auto cross=[](const pi&o,const pi&a,const pi&b){return ll(a.first-o.first)*(b.second-o.second)-ll(a.second-o.second)*(b.first-o.first);};
		sort(begin(p),end(p));
		p.erase(unique(begin(p),end(p)),end(p));
		it n=p.size(),k=0;
		if(n<=2)return p;
		vc<pi>h(2*n);
		for(it i=0;i<n;h[k++]=p[i++])
			while(k>=2&&cross(h[k-2],h[k-1],p[i])<=0)--k;
		for(it i=n-2,t=k+1;i>=0;h[k++]=p[i--])
			while(k>=t&&cross(h[k-2],h[k-1],p[i])<=0)--k;
		h.resize(k-1);
		return h;
	}
};
}
#endif