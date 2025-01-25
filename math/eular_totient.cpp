#include <cstdint>
using namespace std;
// O(\sqrt{n})
uint64_t eular_totient(uint64_t n){
	uint64_t r=n;
	for(uint64_t i=2;i*i<=n;++i){
		if(n%i==0){
			r-=r/i;
			while(n%i==0)n/=i;
		}
	}
	if(n>1)r-=r/n;
	return r;
}