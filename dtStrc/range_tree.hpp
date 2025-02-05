#include <vector>
#include <cassert>
#include <cstdint>
#include <algorithm>
#include <unordered_map>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#ifndef ELSIE_RANGE_TREE
#define ELSIE_RANGE_TREE
namespace elsie{
	using namespace std;
	using namespace __gnu_pbds;

	template<class s,class t>using __gnu_map=tree<s,t,std::less<s>,rb_tree_tag,tree_order_statistics_node_update>;

	template<class s,class t>struct gnu_map:public __gnu_map<s,t>{
		using iterator=typename __gnu_map<s,t>::iterator;
		iterator get(int64_t idx){return this->find_by_order(idx<0?this->size()-idx:idx);}
		size_t ord(const s&key){return this->order_of_key(key);}
	};

	template<class s>struct gnu_set:public gnu_map<s,null_type>{gnu_map<s,null_type>::iterator operator[](int64_t i){return this->get(i);}};

	template<class S>class range_frequency{
		using it=int32_t;
		unordered_map<S,gnu_set<it>>v_idx;
		vector<S>va;
		public:
		range_frequency():va(0){}
		range_frequency(it n):va(n,S()){ for(it i=0;i<n;i++)v_idx[S()].insert(i); }
		range_frequency(const vector<S>&v):va(v){ for(size_t i=0;i<v.size();i++) v_idx[v[i]].insert(i); }
		void set(it idx,const S&v){
			v_idx[va[idx]].erase(idx);
			if(v_idx[va[idx]].size()==0)v_idx.erase(va[idx]);
			v_idx[v].insert(idx);
			va[idx]=v;
		}
		it cnt(it r,const S&x){
			if(auto itr=v_idx.find(x);itr!=v_idx.end()){
				auto jtr=itr->second.lower_bound(r);
				return(jtr==itr->second.end()?itr->second.size():itr->second.ord(*jtr));
			}return 0;
		}
		it cnt(it l,it r,const S&x){return cnt(r,x)-cnt(l,x); }
	};

	template<class S>class range_frequency_static{
		using it=int32_t;
		unordered_map<S,vector<it>>v_idx;
		public:
		range_frequency_static(){}
		range_frequency_static(const vector<S> &v){
			for(it i=0;i<v.size();i++)
				v_idx[v[i]].push_back(i);
			for(auto&[key,val]:v_idx)
				sort(begin(val),end(val));
		}
		it cnt(it r,const S&x) {
			if(auto itr=v_idx.find(x);itr!=v_idx.end())
				return lower_bound(begin(itr->second),end(itr->second),r)-begin(itr->second);
			return 0;
		}
		it cnt(it l,it r,const S&x){return cnt(r,x)-cnt(l,x); }
	};

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
#endif