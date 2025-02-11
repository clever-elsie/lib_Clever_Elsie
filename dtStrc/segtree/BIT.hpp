#include<vector>
#include<cstdint>
#include<cstddef>
#ifndef ELSIE_SEGTREE
#define ELSIE_SEGTREE
namespace elsie{
	using namespace std;

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
}
#endif
