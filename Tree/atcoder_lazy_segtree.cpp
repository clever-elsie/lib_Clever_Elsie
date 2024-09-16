#include<atcoder/lazysegtree>
using namespace std;
using namespace atcoder;
#define int long long

/* add_RMaxQ */{
	using dat_t = int;
	using laz_t = int;
	dat_t dat_op(dat_t a, dat_t b){ return (a>b?a:b);}
	dat_t mapping(laz_t f,dat_t x){ return x+f; }
	laz_t composition(laz_t f,laz_t g){return g+f;}
	dat_t dat_e(){ return -(1ll<<62); }
	laz_t laz_id(){return 0;}
	lazy_segtree<dat_t,dat_op,dat_e,laz_t,mapping,composition,laz_id> lst(n);
}
/* add_RMinQ */{
	using dat_t = int;
	using laz_t = int;
	dat_t dat_op(dat_t a, dat_t b){ return (a<b?a:b);}
	dat_t mapping(laz_t f,dat_t x){ return x+f; }
	laz_t composition(laz_t f,laz_t g){return g+f;}
	dat_t dat_e(){ return (1ll<<62); }
	laz_t laz_id(){return 0;}
	lazy_segtree<dat_t,dat_op,dat_e,laz_t,mapping,composition,laz_id> lst(n);
};
/* add_RSQ */{
	struct dat_t { int val,size; };
	using laz_t = int;
	dat_t dat_op(dat_t a, dat_t b){ return {a.val+b.val,a.size+b.size};}
	dat_t mapping(laz_t f,dat_t x){ return {x.val+f*x.size,x.size}; }
	laz_t composition(laz_t f,laz_t g){return g+f;}
	dat_t dat_e(){ return {0,0}; }
	laz_t laz_id(){return 0;}
	lazy_segtree<dat_t,dat_op,dat_e,laz_t,mapping,composition,laz_id> lst(n);
};
/* ch_RMaxQ */{
	using dat_t = int;
	using laz_t = int;
	constexpr laz_t laz_ID=1ll<<62;
	dat_t dat_op(dat_t a, dat_t b){ return (a>b?a:b);}
	dat_t mapping(laz_t f,dat_t x){ return (f==laz_ID?x:f); }
	laz_t composition(laz_t f,laz_t g){return (f==laz_ID?g:f);}
	dat_t dat_e(){ return -(1ll<<62); }
	laz_t laz_id(){return laz_ID;}
	lazy_segtree<dat_t,dat_op,dat_e,laz_t,mapping,composition,laz_id> lst(n);
};
/* ch_RMinQ */{
	using dat_t = int;
	using laz_t = int;
	constexpr laz_t laz_ID=1ll<<62;
	dat_t dat_op(dat_t a, dat_t b){ return (a<b?a:b);}
	dat_t mapping(laz_t f,dat_t x){ return (f==laz_ID?x:f); }
	laz_t composition(laz_t f,laz_t g){return (f==laz_ID?g:f);}
	dat_t dat_e(){ return (1ll<<62); }
	laz_t laz_id(){return laz_ID;}
	lazy_segtree<dat_t,dat_op,dat_e,laz_t,mapping,composition,laz_id> lst(n);
};
/* ch_RSQ */{
	struct dat_t { int val,size; };
	using laz_t = int;
	constexpr laz_t laz_ID=1ll<<62;
	dat_t dat_op(dat_t a, dat_t b){ return {a.val+b.val,a.size+b.size};}
	dat_t mapping(laz_t f,dat_t x){
		if(f!=laz_ID) x.val=f*x.size;
		return x;
	}
	laz_t composition(laz_t f,laz_t g){ return (f==laz_ID?g:f);}
	dat_t dat_e(){ return {0,0}; }
	laz_t laz_id(){return laz_ID;}
	lazy_segtree<dat_t,dat_op,dat_e,laz_t,mapping,composition,laz_id> lst(n);
};