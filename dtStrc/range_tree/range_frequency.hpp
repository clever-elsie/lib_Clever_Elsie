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
}
#endif