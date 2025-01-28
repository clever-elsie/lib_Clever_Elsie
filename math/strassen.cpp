#include <vector>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>

namespace elsie{
	using namespace std;
	using ps=pair<size_t,size_t>;
	template<class S>using vc=vector<S>;
	template<class S>using vv=vc<vc<S>>;
	template<class S,class T,class U=common_type_t<S,T>>
	vv<U>matrix_mul(vv<S>&a,vv<T>&b){
		assert(a.size()>0&&b.size()&&a[0].size()==b.size());
		vv<U>c;
		auto resize=[&](size_t na,size_t ma,size_t nb,size_t mb,size_t nc,size_t mc){
			for(auto&x:a)x.resize(ma,S());
			for(auto&x:b)x.resize(mb,T());
			for(auto&x:c)x.resize(mc,U());
			a.resize(na,vc<S>(ma,S()));
			b.resize(nb,vc<S>(mb,T()));
			c.resize(nc,vc<U>(mc,U()));
		};
		size_t N=a.size(),M=b.size(),L=b[0].size();
		size_t n=[&]()->size_t {
			size_t p=1,P=max(N,max(M,L));
			while(p<N)p<<=1;
			return p;
		}();
		resize(n,n,n,n,n,n);
		auto add=[&](ps alu,ps blu,size_t sz){
			for(size_t i=0;i<sz;++i)
			for(size_t j=0;j<sz;++j);

		};
		auto sub=[&](ps alu,ps blu,size_t sz){
		};
		auto mul=[&](ps alu,ps blu,size_t sz){
			if(sz<256){ // O(n^3)
			}else{ // O(n^lg 7)
			}
		};
	}
};