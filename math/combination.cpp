#include <vector>
#include <atcoder/modint>

using namespace std;
using namespace atcoder;

template<class mint=modint998244353>
class combination{
	private:
		vector<mint>invprod,prod;
	public:
	combination(int n=1):invprod(n,1),prod(n,1){}
	void PreCalc(int n){
		int presize=prod.size();
		prod.resize(n+1);
		invprod.resize(n+1);
		for(int i=presize+1;i<=n;i++){
			prod[i]=prod[i-1]*i;
			invprod[i]=invprod[i-1]*mint(i).inv();
		}
	}
	mint factorial(size_t n){
		if(prod.size()<n+1) PreCalc(n);
		return prod[n];
	}
	mint invfact(size_t n){
		if(prod.size()<n+1) PreCalc(n);
		return invprod[n];
	}
	mint P(size_t n,size_t k){
		if(n<k) return 0;
		if(prod.size()<n+1) PreCalc(n);
		return prod[n]*invprod[n-k];
	}
	mint C(size_t n,size_t k){
		if(n<k) return 0;
		if(prod.size()<n+1) PreCalc(n);
		return prod[n]*invprod[n-k]*invprod[k];
	}
	mint H(size_t n,size_t k){
		if(n==0) return mint((int)(k!=0));
		if(prod.size()<n+1) PreCalc(n);
		return C(n+k-1,k);
	}
};