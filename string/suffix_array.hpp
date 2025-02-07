#include <string>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <concepts>
#include <algorithm>
#ifndef ELSIE_SUFFIX_ARRAY
#define ELSIE_SUFFIX_ARRAY
namespace elsie{
	using namespace std;
	template<class T>concept Itrabl=requires(const T&x){x.begin();x.end();x.size();};
	template<Itrabl T>class suffix_array{
		template<class T>using vc=vector<T>;
		public:
		vc<size_t>sa;
		T v;
		protected:
		int32_t cmp(const T&s,size_t f){
			size_t len=v.size()-f;
			size_t sz=min(len,s.size());
			for(size_t i=0;i<sz;++i){
				if(v[f+i]==s[i])continue;
				return(s[i]<v[f+i]?-1:1);
			}
			if(len==s.size())return 0;
			return(s.size()<len?-1:1);
		}
		template<size_t lower_upper> // lower=0, upper=1
		size_t bisect(const T&s){
			int64_t l=-1,r=sa.size();
			while(l+1<r)
				if(int64_t m=(l+r)>>1;cmp(s,m)<lower_upper)l=m;
				else r=m;
			return r;
		}
		void construct(auto&&s)requires is_convertible_v<decltype(s),T>{
			v=forward(s);
			sa.resize(v.size());
			SA_IS();
		}
		void SA_IS(){

		}
		public:
		size_t lower_bound(const T&s){return bisect<0>(s);}
		size_t upper_bound(const T&s){return bisect<1>(s);}
		void set(const T&s){construct(s);}
		void set(T&&s){construct(move(s));}
		suffix_array():sa(0),v(0){}
		suffix_array(const T&s){construct(s);}
		suffix_array(T&&s){construct(move(s));}
		~suffix_array(){}
	};
}
#endif