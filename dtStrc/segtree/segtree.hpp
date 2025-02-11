#include<limits>
#include<vector>
#include<cstdint>
#include<cstddef>
#ifndef ELSIE_SEGTREE
#define ELSIE_SEGTREE
namespace elsie{
	using namespace std;

	template<class S,S(*op)(S,S),S(*e)()>class segtree{
	protected: // 1-indexed [l,r)
		vector<S> v;
		size_t n,tl,tr;
		S prod(size_t l,size_t r,size_t idx){
			if(tl<=l&&r<=tr)return v[idx];
			size_t m=l+(r-l)/2;
			if((tr<=l||r<=tl)||!(tl<=m||m<=tr))return e();
			return op(prod(l,m,idx*2),prod(m,r,idx*2+1));
		}
	public:
		segtree(int N):n(1){
			while(n<N)n<<=1;
			v.resize(n+n,e());
		}
		segtree(const vector<S>&V):n(1){
			while(n<V.size())n<<=1;
			v.resize(n+n,e());
			for(size_t i=n;auto&x:V)v[i++]=x;
			for(size_t i=n-1;i;i--)v[i]=op(v[2*i],v[2*i+1]);
		}
		void set(size_t p,S val){
			v[n+p]=val;
			for(size_t i=(n+p)>>1;i;i>>=1)
				v[i]=op(v[2*i],v[2*i+1]);
		}
		S get(size_t p){ return v.at(n+p);}
		S prod(size_t l,size_t r){
			tl=l,tr=r;
			return prod(0,n,1);
		}
	};

	template<class S>S zeroE(){return S();}
	template<class S>S max4min(){return numeric_limits<S>::max();}
	template<class S>S min4max(){return numeric_limits<S>::lowest();}
	template<class S>S opAdd(S a,S b){return a+b;}
	template<class S>S opMax(S a,S b){return a>b?a:b;}
	template<class S>S opMin(S a,S b){return a<b?a:b;}

	// segtree
	template<class S>using min_seg=segtree<S,opMin<S>,max4min<S>>;
	template<class S>using max_seg=segtree<S,opMax<S>,min4max<S>>;
	template<class S>using sum_seg=segtree<S,opAdd<S>,zeroE<S>>;
}
#endif