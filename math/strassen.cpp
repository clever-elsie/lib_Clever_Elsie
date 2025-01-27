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
	vv<U>matrix_mul(const vv<S>&a1,const vv<T>&a2){
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
		vv<U>a,b;
		size_t n=1;{
			size_t N=max(a.size(),max(b.size(),b[0].size()));
			while(n<N)n<<=1;
			a=b=vv<U>(n,vc<U>(n,U()));
		}
		return mul(ps(0,0),ps(0,0),n);
	}
};