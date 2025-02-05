#include<limits>
#include<vector>
#include<cstdint>
#include<cstddef>
#include<iostream>
#include<atcoder/lazysegtree>
#include<atcoder/segtree>
#ifndef ELSIE_SEGTREE
#define ELSIE_SEGTREE
namespace elsie{
	using namespace std;
	using namespace atcoder;

	template<class type>class BIT{// 0-indexed [l,r)
		vector<type>v;
		size_t _size;
		type sum(int64_t r){
			type ret=0;
			for(;r;r-=r&-r) ret+=v[r-1];
			return ret;
		}
	public:
		BIT(size_t size){
			size_t n=1;
			while(n<size)n*=2;
			_size=n, v.resize(n);
		}
		void add(size_t i,type val){ for(i++;i<=_size;i+=i&-i) v[i-1]+=val; }
		type sum(size_t l,size_t r){ return sum(r)-sum(l); }
		type get(size_t i){ return sum(i,i+1); }
		void set(size_t i,type t){ add(i,t-get(i)); }
	};

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

	template<class S,S(*op)(S,S),S(*e)(),class F,S(*mapping)(F,S),F(*composition)(F,F),F(*id)()>
	class lazy_segtree{
	protected:
		using sz_t=uint32_t;
		vector<S>data;
		vector<F>lazy;
		sz_t sz,h;
		void update(sz_t idx){data[idx]=op(data[2*idx],data[2*idx+1]);}
		void all_apply(sz_t idx,F f){
			data[idx]=mapping(f,data[idx]);
			if(idx<sz)lazy[idx]=composition(f,lazy[idx]);
		}
		void push(sz_t idx){
			all_apply(2*idx,lazy[idx]);
			all_apply(2*idx+1,lazy[idx]);
			lazy[idx]=id();
		}
	public:
		lazy_segtree(size_t N=0):lazy_segtree(vector<S>(N,e())){}
		lazy_segtree(const vector<S>&v){
			sz=1ll<<(63-countl_zero(v.size())+(popcount(v.size())>1?1:0));
			h=countr_zero(sz);
			data=vector<S>(2*sz,e());
			for(sz_t i=0;i<v.size();++i)data[i+sz]=v[i];
			for(sz_t i=sz-1;i>0;--i)update(i);
			lazy=vector<F>(sz,id());

		}
		void set(sz_t p,S val){
			p+=sz;
			for(sz_t i=h;i>=1;--i)push(p>>i);
			data[p]=val;
			for(sz_t i=1;i<=h;++i)update(p>>i);
		}
		S get(sz_t p){
			p+=sz;
			for(sz_t i=h;i>=1;--i)push(p>>i);
			return data[p];
		}
		S prod(sz_t l,sz_t r){
			if(l==r)return e();
			l+=sz,r+=sz;
			for(sz_t i=h;i>=1;--i){
				if(((l>>i)<<i)!=l)push(l>>i);
				if(((r>>i)<<i)!=r)push((r-1)>>i);
			}
			S sl=e(),sr=e();
			while(l<r){
				if(l&1)sl=op(sl,data[l++]);
				if(r&1)sr=op(data[--r],sr);
				l>>=1,r>>=1;
			}
			return op(sl,sr);
		}
		S all_prod(){return data[1];}
		void apply(sz_t l,sz_t r,F f){
			if(l==r)return;
			l+=sz,r+=sz;
			for(sz_t i=h;i>=1;--i){
				if(((l>>i)<<i)!=l)push(l>>i);
				if(((r>>i)<<i)!=r)push((r-1)>>i);
			}
			sz_t L=l,R=r;
			while(L<R){
				if(L&1)all_apply(L++,f);
				if(R&1)all_apply(--R,f);
				L>>=1,R>>=1;
			}
			for(sz_t i=1;i<=h;++i){
				if(((l>>i)<<i)!=l)update(l>>i);
				if(((r>>i)<<i)!=r)update((r-1)>>i);
			}
		}
	};

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