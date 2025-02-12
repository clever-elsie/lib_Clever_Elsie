/**
 * @file suffix_array.hpp
 * @brief compute suffix_array in O(N). N is size of text.
 * @author CleverElsie
 * @details use default SA_IS. but O(N log^2 N) can be used.
 */
#include <array>
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
	template<class T>concept Itrabl=requires(const T&x){x.begin();x.end();x.size();typename T::value_type;};
	/**
	 * @class suffix_array
	 * @brief compute suffix array of iterable object.
	 * @attention Itrabl concept requires method {.begin(), .end(), .size()}. but may be needed operator++ and operator-- at T::iterator for sequential access.
	 */
	template<Itrabl T>class suffix_array{
		template<class S>using vc=vector<S>;
		public:
		vc<size_t>sa;
		T v;
		protected:
		void construct(){
			sa.resize(v.size());
			SA_log();
		}
		void SA_IS(){

		}
		using ps2=pair<size_t,size_t>;
		using ps3=pair<ps2,size_t>;
		using vps3=vector<ps3>;
		void SA_log(){ // log^2
			size_t n=v.size();
			vps3 Rsubstr(n);
			vector<size_t>rank(n);
			for(size_t i=0;i<n;++i)
				Rsubstr[i]={{v[i],(i+1<n?v[i+1]:0)},i};
			sort(begin(Rsubstr),end(Rsubstr));
			for(size_t l=2;l<n;l*=2){
				[&](){
					size_t r=1;
					rank[Rsubstr[0].second]=r;
					for(size_t i=1;i<n;++i){
						r+=Rsubstr[i].first.first!=Rsubstr[i-1].first.first
							||Rsubstr[i].first.second!=Rsubstr[i-1].first.second;
						rank[Rsubstr[i].second]=r;
					}
				}();
				for(size_t i=0;i<n;++i){
					Rsubstr[i].first.first=rank[i];
					Rsubstr[i].first.second=i+l<n?rank[i+l]:0;
					Rsubstr[i].second=i;
				}
				sort(Rsubstr.begin(),Rsubstr.end());
			}
			for(size_t i=0;i<n;++i)
				sa[i]=Rsubstr[i].second;
		}
		void LCP_call_after_SA_(){}
		public:
		size_t lower_bound(const T&t){
			int64_t ok=v.size(),ng=-1;
			while(ng+1<ok){
				int64_t m=(ok+ng)/2,nx=0;
				for(size_t i=0,u=min(t.size(),v.size()-sa[m]);i<u;++i){
					if(v[sa[m]+i]==t[i])continue;
					if(v[sa[m]+i]<t[i]) ng=m;
					else ok=m;
					nx=1;
					break;
				}
				if(nx)continue;
				if(t.size()<=v.size()-sa[m])ok=m;
				else ng=m;
			}
			return ok;
		}
		size_t upper_bound(const T&t){
			int64_t ok=v.size(),ng=-1;
			while(ng+1<ok){
				int64_t m=(ok+ng)/2,nx=0;
				for(size_t i=0,u=min(t.size(),v.size()-sa[m]);i<u;++i){
					if(v[sa[m]+i]==t[i])continue;
					if(v[sa[m]+i]<t[i]) ng=m;
					else ok=m;
					nx=1;
					break;
				}
				if(nx)continue;
				ng=m;
			}
			return ok;
		}
		void set(const T&s){
			v=s;
			construct();
		}
		void set(T&&s){
			v=move(s);
			construct();
		}
		suffix_array():sa(0),v(0){}
		suffix_array(const T&s):v(s){construct();}
		suffix_array(T&&s):v(move(s)){construct();}
		~suffix_array()=default;
	};
}
#endif