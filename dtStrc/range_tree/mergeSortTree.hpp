#include <vector>
#include <cassert>
#include <cstdint>
#include <algorithm>
namespace elsie{
	using namespace std;
	template<class T,size_t max_log=21>
	class merge_sort_tree{
		using vc=vector<T>;
		using vv=vector<vc>;
		using it=int32_t;
		vv b,s;
		size_t n;
		bool fixed;
		void make_tree(it id,it l,it r){
			for(it i=l;i<r;i++) b[id+1][i]=b[id][i];
			if(abs(r-l)>1){
				it m=(l+r)/2;
				make_tree(id+1,l,m);
				make_tree(id+1,m,r);
				sort(l+I(b[id],r));
			}
			s[id][l]=b[id][l];
			for(it i=l+1;i<r;i++) s[id][i]+=s[id][i-1]+b[id][i];
		}
		T get_tree(it id,it l,it r,const T&x,it sl,it sr){
			if(r<=sl||sr<=l)return 0;
			if(l<=sl&&sr<=r){
				auto bbid=begin(b[id]);
				it p=upper_bound(sl+bbid,bbid+sr,x)-bbid-1;
				if(p>=sl) return s[id][p];
				else return 0;
			}
			it m=(sl+sr)/2;
			return get_tree(id+1,l,r,x,sl,m)+get_tree(id+1,l,r,x,m,sr);
		}
		public:
		merge_sort_tree():fixed(0){}
		merge_sort_tree(it N):n(N),fixed(0),b(max_log,vc(N)),s(max_log,vc(N)){}
		merge_sort_tree(const vc&v):merge_sort_tree(v.size()){ b[0]=v; fix(); }
		void fix(){ make_tree(0,0,n); fixed=1; }
		void set(size_t idx,const T&val){assert(!fixed);b[0][idx]=val; }
		T get(size_t l,size_t r,const T&val){ return get_tree(0,0,r,val,0,n)-get_tree(0,0,l,val,0,n); }
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