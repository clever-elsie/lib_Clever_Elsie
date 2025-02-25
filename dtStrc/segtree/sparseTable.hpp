#ifndef ELSIE_SPARSE_TABLE
#define ELSIE_SPARSE_TABLE
#include <vector>
#include <cassert>

namespace elsie{
	using namespace std;
	template<class S> using vc=vector<S>;
	template<class S> using vv=vc<vc<S>>;

	/**
	 * @brief 更新を禁止せず，更新があると最初のprodでO(NlogN)
	 * @brief 配列サイズNも可変．これも更新に含まれる
	 * @brief 配列サイズ超過では自動リサイズ．拡張範囲にe()
	 */
	template<class S,S(*op)(S,S),S(*e)()>class sparseTable{
		private:
		vv<S>b;
		bool fixed;
		void preprocess(){
			for(size_t i=1,p=1;1;++i,p<<=1){
				vc<S>nx;
				size_t pre_size=b[i-1].size();
				for(size_t j=0;j+p<pre_size;++j)
					nx.push_back(op(b[i-1][j],b[i-1][j+p]));
				if(nx.size())b.push_back(move(nx));
				else break;
			}
		}
		public:
		sparseTable():b(1,vc<S>(0)),fixed(false){}
		sparseTable(const vc<S>&v):b(1,v),fixed(false){}
		sparseTable(vc<S>&&v):b(1,v),fixed(false){}
		void resize(size_t n){
			fixed=false;
			b[0].resize(n,e());
		}
		void set(size_t idx,const S&x){
			fixed=false;
			if(idx>=b[0].size())resize(idx+1);
			b[0][idx]=x;
		}
		/**
		 * @fn prod
		 * @brief [L,R)
		 */
		S prod(size_t l,size_t r){
			if(!fixed){
				fixed=true;
				preprocess();
			}
			if(r>b[0].size())r=b[0].size();
			if(l==r)return e();
			assert(l<r);
			size_t p=63-countl_zero(r-l);
			return op(b[p][l],b[p][r-(1ull<<p)]);
		}
	};
} // namespace elsie
#endif