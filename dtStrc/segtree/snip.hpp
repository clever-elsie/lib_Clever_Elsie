#ifndef ELSIE_SEG_SNIP
#define ELSIE_SEG_SNIP
#include <limits>
#include <cstddef>
#include <iostream>
#include <dtStrc/segtree/segtree.hpp>
#include <dtStrc/segtree/lazy_segtree.hpp>
namespace elsie{
    using namespace std;

    template<class S>S zeroE(){return S();}
    template<class S>S max4min(){return numeric_limits<S>::max();}
    template<class S>S min4max(){return numeric_limits<S>::lowest();}
    template<class S>S opAdd(S a,S b){return a+b;}
    template<class S>S opMax(S a,S b){return a>b?a:b;}
    template<class S>S opMin(S a,S b){return a<b?a:b;}
    template<class S>S cpAdd(S f,S g){return g+f;}
    template<class S,class F>S mpAdd(F f,S x){return x+f;}

    template<class S>using min_seg=segtree<S,opMin<S>,max4min<S>>;
    template<class S>using max_seg=segtree<S,opMax<S>,min4max<S>>;
    template<class S>using sum_seg=segtree<S,opAdd<S>,zeroE<S>>;

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
        friend sum_lst<S> operator+(const sum_lst<S>&lhs,const sum_lst<S>&rhs){
            return sum_lst<S>(lhs.val+rhs.val,lhs.size+rhs.size);
        }
    };
    template<class S> sum_lst<S> operator+(const sum_lst<S>&lhs,const sum_lst<S>&rhs){return sum_lst<S>(lhs)+=rhs;}
    template<class S> ostream&operator<<(ostream&os,const sum_lst<S>&v){return os<<v.val;}


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

/*
 * This code blocks are face to gcc-12 bug. don't use gcc-12 or older.
*/
//	template<class S>struct affine{
//		S a,b;
//		affine(S aa,S bb):a(aa),b(bb){}
//	};
//
//	template<class S>using range_affine_range_sum_lst=lazy_segtree<
//		/* S  */sum_lst<S>,
//		/* op */opAdd<sum_lst<S>>,
//		/* e  */zeroE<sum_lst<S>>,
//		/* F  */affine<S>,
//		/* map*/[](affine<S>f,sum_lst<S>x){return sum_lst<S>(f.a*x.val+f.b*x.size,x.size);},
//		/* cmp*/[](affine<S>f,affine<S>g){return affine<S>(f.a*g.a,f.a*g.b+f.b);},
//		/* id */[](){return affine<S>(1,0);}>;
//	template<class S>struct affine_sum_lst:public range_affine_range_sum_lst<S>{
//		private:
//			using super=range_affine_range_sum_lst<S>;
//		public:
//		using super::super;
//		void set(size_t idx,const S&val){ super::set(idx,sum_lst<S>(val,1)); }
//		S get(size_t idx){return super::get(idx).val;}
//		S prod(size_t l,size_t r){ return super::prod(l,r).val; }
//		S all_prod(){ return super::all_prod().val; }
//		void apply(size_t l,size_t r,S a,S b){ super::apply(l,r,affine(a,b)); }
//	};
//	
//	template<class S>using range_ch_range_composite_lst=lazy_segtree<
//		/* S  */sum_lst<affine<S>>,
//		/* op */[](sum_lst<affine<S>>a,sum_lst<affine<S>>b){return sum_lst<affine<S>>(affine<S>(a.val.a*b.val.a,a.val.b*b.val.a+b.val.b),a.size+b.size);},
//		/* e  */[](){return sum_lst(affine<S>(1,0),0);},
//		/* F  */sum_lst<affine<S>>,
//		/* map*/[](sum_lst<affine<S>>f,sum_lst<affine<S>>x){
//			if(f.size==1)return x;
//			affine<S>g(1,0),h=f.val;
//			int32_t k=x.size;
//			while(k){
//				if(k&1)g.a*=h.a,g.b=g.b*h.a+h.b;
//				h=affine<S>(h.a*h.a,h.a*h.b+h.b);
//				k>>=1;
//			}
//			return sum_lst(g,x.size);
//		},
//		/* cmp*/[](sum_lst<affine<S>>f,sum_lst<affine<S>>g){return(f.size==1?g:f);},
//		/* id */[](){return sum_lst(affine<S>(1,0),1);}
//	>;
//	template<class S>struct ch_composite_lst:public range_ch_range_composite_lst<S> {
//		private:using super=range_ch_range_composite_lst<S>;
//		public:
//		using super::super;
//		void set(size_t idx,S a,S b){ super::set(idx,sum_lst(affine<S>(a,b),1)); }
//		affine<S> get(size_t idx){return super::get(idx).val;}
//		affine<S> prod(size_t l,size_t r){return super::prod(l,r).val;}
//		affine<S> all_prod(){ return super::all_prod().val; }
//		void apply(size_t l,size_t r,S a,S b){ super::apply(l,r,sum_lst(affine(a,b),0)); }
//	};
}
#endif