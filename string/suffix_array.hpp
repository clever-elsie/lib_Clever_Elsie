/**
 * @file suffix_array.hpp
 * @brief compute suffix_array in O(N). N is size of text.
 * @author CleverElsie
 * @details use default SA_IS. but O(N log N) can be used.
 */
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
	/**
	 * @class suffix_array
	 * @brief compute suffix array of iterable object.
	 * @attention Itrabl concept requires method {.begin(), .end(), .size()}. but may be needed operator++ and operator-- at T::iterator for sequential access.
	 */
	template<Itrabl T>class suffix_array{
		template<class T>using vc=vector<T>;
		public:
		vc<size_t>sa;
		T v;
		protected:
		/**
		 * @fn int32_t cmp(const T&,size_t)
		 * @param[in] s:T compared with v[f:]
		 * @param[in] f:size_t first index of substring v[f:]
		 * @return `s<=>v[f:]` \f$\{-1,0,1\} \f$
		 */
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
		/**
		 * @fn size_t bisect(const T&)
		 * @param[in] s target string
		 * @param lower_upper 0:lower, 1:upper
		 * @brief if lower_upper is 0, it returns the index of the first occurrence of a substring greater than or equal to s, or if it is 1, it returns the index of the first occurrence of a substring strictly greater than s.
		 */
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
		void SA_log(){ // log^2
			using ps2=pair<size_t,size_t>;
			using ps3=pair<ps2,size_t>;
			size_t n=v.size();
			vector<ps3>Rsubstr(n);
			vector<size_t>rank(n);
			for(size_t i=0;i<n;++i)
				Rsubstr[i]={{v[i],(i+1<n?v[i+1]:0)},i};
			sort(begin(Rsubstr),end(Rsubstr));
			for(size_t l=2;l<n;l<<=1){
				[&](){
					size_t r=0;
					rank[Rsubstr[0].second]=r;
					for(size_t i=1;i<n;++i){
						r+=Rsubstr[i].first.first!=Rsubstr[i-1].first.first
							||Rsubstr[i].first.second!=Rsubstr[i-1].first.second;
						rank[Rsubstr[i].second]=r;
					}
				}();
				for(size_t i=0;i<n;++i){
					Rsubstr[i].first=rank[i];
					Rsubstr[i].first.second=i+l<n?rank[i+l]:0;
					Rsubstr[i].second=i;
				}
				sort(begin(Rsubstr),end(Rsubstr));
			}
			for(size_t i=0;i<n;++i)
				sa[i]=Rsubstr[i].second;
		}
		void LCP_call_after_SA_(){}
		public:
		size_t lower_bound(const T&s){return bisect<0>(s);}
		size_t upper_bound(const T&s){return bisect<1>(s);}
		void set(const T&s){construct(s);}
		void set(T&&s){construct(move(s));}
		suffix_array():sa(0),v(0){}
		suffix_array(const T&s){construct(s);}
		suffix_array(T&&s){construct(move(s));}
		~suffix_array()=default;
	};
}
#endif