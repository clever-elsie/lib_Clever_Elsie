#ifndef ELSIE_MERGE_SORT_TREE
#define ELSIE_MERGE_SORT_TREE
#include <vector>
#include <cassert>
#include <cstdint>
#include <algorithm>
namespace elsie{
	using namespace std;
	template<class T>
	class merge_sort_tree{
		using vc=vector<T>;
		using vv=vector<vc>;
		using it=int32_t;
		public:
		vv b,s;
		size_t n,max_log;
		bool fixed;
		size_t calc_depth(size_t N)const{return 65-countl_zero(N)+(popcount(N)!=1); }
		void make_tree(it id,it l,it r){
			for(it i=l;i<r;++i) b[id+1][i]=b[id][i];
			if(r-l>1) sort(l+b[id].begin(),b[id].begin()+r);
			if(l+1<r){
				it m=(l+r)/2;
				make_tree(id+1,l,m);
				make_tree(id+1,m,r);
			}
			if(r-l>0){
				s[id][l]=b[id][l];
				for(it i=l+1;i<r;i++)
					s[id][i]+=s[id][i-1]+b[id][i];
			}
		}
		pair<T,int32_t>prod_tree(it id,it l,it r,const T&x,it sl,it sr)const{
			if(r<=sl||sr<=l)return {0,0};
			if(l<=sl&&sr<=r){
				auto bbid=begin(b[id]);
				it p=upper_bound(sl+bbid,bbid+sr,x)-bbid-1;
				if(p>=sl){
					return{s[id][p],1+p-sl};
				}
				else return {0,0};
			}
			it m=(sl+sr)/2;
			auto[t1,c1]=prod_tree(id+1,l,r,x,sl,m);
			auto[t2,c2]=prod_tree(id+1,l,r,x,m,sr);
			return{t1+t2,c1+c2};
		}
		public:
		merge_sort_tree(){}
		merge_sort_tree(it N,const T&v):n(N),fixed(0),max_log(calc_depth(N)),b(0),s(0){
			b.resize(max_log,vc(n,v));
			s.resize(max_log,vc(n,v));
		}
		merge_sort_tree(const vc&v):merge_sort_tree(v.size()){ b[0]=v; }
		merge_sort_tree(vc&&v):merge_sort_tree(v.size()){ b[0]=move(v); }
		void fix(){ make_tree(0,0,n); fixed=1; }
		void set(size_t idx,const T&val){assert(!fixed);b[0][idx]=val; }
		pair<T,it>prod(size_t l,size_t r,const T&upper)const{
			auto[tr,cr]=prod_tree(0,0,r,upper,0,n);
			auto[tl,cl]=prod_tree(0,0,l,upper,0,n);
			return{tr-tl,cr-cl};
		}
		pair<T,it>prod(size_t l,size_t r,const T&lower,const T&upper)const{
			auto[tu,cu]=prod(l,r,upper);
			auto[tl,cl]=prod(l,r,lower);
			return {tu-tl,cu-cl};
		}
		T sum(size_t l,size_t r,const T&upper)const{ return prod(l,r,upper).first; }
		T sum(size_t l,size_t r,const T&lower,const T&upper)const{return sum(l,r,upper)-sum(l,r,lower);}
		it cnt(size_t l,size_t r,const T&upper)const{ return prod(l,r,upper).second; }
		it cnt(size_t l,size_t r,const T&lower,const T&upper)const{return cnt(l,r,upper)-cnt(l,r,lower);}

		merge_sort_tree&operator=(const merge_sort_tree&o){ n=o.n,b=o.b,s=o.s; return*this;}
		merge_sort_tree&operator=(merge_sort_tree&&o){
			if(this!=&o){
				n=o.n;
				b=move(o.b);
				s=move(o.s);
			}
			return*this;
		}
	};
}
#endif