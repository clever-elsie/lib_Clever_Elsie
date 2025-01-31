#include <iostream>
#include <cstdint>
#include <cstddef>
#include <limits>
#include <atcoder/lazysegtree>
#include <atcoder/segtree>
#ifndef ELSIE_LAZY_SEG_SNIP
#define ELSIE_LAZY_SEG_SNIP
namespace elsie{
using namespace std;
using namespace atcoder;
template<class S>struct sum_lst{
	S val;
	size_t size;
	sum_lst():val(),size(0){}
	sum_lst(S a,size_t b):val(a),size(b){}
	sum_lst(const sum_lst<S>&a):sum_lst<S>(a.val,a.size){}
	sum_lst<S>&operator+=(const sum_lst<S>&a){
		val+=a.val,size+=a.size;
		return*this;
	}
};
template<class S> sum_lst<S> operator+(const sum_lst<S>&lhs,const sum_lst<S>&rhs){return sum_lst<S>(lhs)+=rhs;}
template<class S> ostream&operator<<(ostream&os,const sum_lst<S>&v){return os<<v.val;}

template<class S>S zeroE(){return S();}
template<class S>S max4min(){return numeric_limits<S>::max();}
template<class S>S min4max(){return numeric_limits<S>::lowest();}
template<class S>S opAdd(S a,S b){return a+b;}
template<class S>S cpAdd(S f,S g){return g+f;}
template<class S,class F>S mpAdd(F f,S x){return x+f;}
template<class S>S opMax(S a,S b){return a>b?a:b;}
template<class S>S opMin(S a,S b){return a<b?a:b;}

// segtree
template<class S>using min_seg=segtree<S,opMin<S>,max4min<S>>;
template<class S>using max_seg=segtree<S,opMax<S>,min4max<S>>;
template<class S>using sum_seg=segtree<S,opAdd<S>,zeroE<S>>;

// lazy_segtree
template<class S,class F>using add_sum_lst=lazy_segtree<
	sum_lst<S>,opAdd<sum_lst<S>>,zeroE<sum_lst<S>>,
	F,[](F f, sum_lst<S> x){return x+sum_lst<S>(f*x.size,0);},cpAdd<F>,zeroE<F>>;

template<class S,class F>using add_max_lst=lazy_segtree<S,opMax<S>,min4max<S>,F,mpAdd<S,F>,cpAdd<F>,zeroE<F>>;
template<class S,class F>using add_min_lst=lazy_segtree<S,opMin<S>,max4min<S>,F,mpAdd<S,F>,cpAdd<F>,zeroE<F>>;

template<class S,class F>using ch_sum_lst=lazy_segtree<
	sum_lst<S>,opAdd<sum_lst<S>>,zeroE<sum_lst<S>>,
	F,[](F f,sum_lst<S>x){if(f!=max4min<F>())x.val=f*x.size;return x;},
	[](F f,F g){return(f==max4min<F>()?g:f);},max4min<F>>;

template<class S,class F>using ch_max_lst=lazy_segtree<S,opMax<S>,min4max<S>,F,
	[](F f,S x){return(f==min4max<F>()?x:f);},
	[](F f,F g){return(f==min4max<F>()?g:f);},min4max<F>>;
template<class S,class F>using ch_min_lst=lazy_segtree<S,opMin<S>,max4min<S>,F,
	[](F f,S x){return(f==max4min<F>()?x:f);},
	[](F f,F g){return(f==max4min<F>()?g:f);},max4min<F>>;

template<class S,class F>using range_affine_range_sum_lst=lazy_segtree<
	sum_lst<S>,[](sum_lst<S>a,sum_lst<S>b){return a+b;},zeroE<sum_lst<S>>,
	pair<F,F>,[](pair<F,F>a,pair<F,F>b){return pair<F,F>(a.first*b.first,a.first*b.second+a.second);},
	[](pair<F,F>f,sum_lst<S>x){x.val*=f.first;x.val+=x.size*f.second;return x;},[](){pair<F,F>(1,0);}>;

}
#endif