#include <vector>
#include <atcoder/modint>
#ifndef ELSIE_COMBINATION
#define ELSIE_COMBINATION
namespace elsie{
using namespace std;
using namespace atcoder;
template<class mint=modint998244353>
class combination{
	private:
		using tint = long long;
		vector<mint>invprod,prod;
		vector<tint>inv;
		const unsigned long long M;
	public:
	combination(int n=1):invprod(2,1),prod(2,1),inv(2,0),M(mint(-1).val()+1){
		inv[1]=1;
		if(n>1)PreCalc(n);
	}
	void PreCalc(int n){
		int presize=inv.size();
		if(presize>=n+1)return;
		inv.resize(n+1);
		prod.resize(n+1);
		invprod.resize(n+1);
		for(int i=presize;i<=n;i++){
			prod[i]=prod[i-1]*i;
			inv[i]=(((M/i)*(-inv[M%i]))+M)%M;
			invprod[i]=invprod[i-1]*inv[i];
		}
	}
	mint factorial(size_t n){
		PreCalc(n);
		return prod[n];
	}
	mint invfact(size_t n){
		PreCalc(n);
		return invprod[n];
	}
	mint P(size_t n,size_t k){
		if(n<k) return 0;
		PreCalc(n);
		return prod[n]*invprod[n-k];
	}
	mint C(size_t n,size_t k){
		if(n<k) return 0;
		PreCalc(n);
		return prod[n]*invprod[n-k]*invprod[k];
	}
	mint H(size_t n,size_t k){
		if(n==0) return mint(k!=0);
		PreCalc(n);
		return C(n+k-1,k);
	}
};
}
#endif