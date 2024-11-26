#include <vector>
#include <type_traits>
#include <concepts>
#include <cstdint>
#include <cstring>
using namespace std;

template<class S,auto op=[](const S&a,const T&b){return a*b;},unsigned_integral M=998244353>
class convolution{
	using VS=vector<S>;
	using vtr=vector<S>::iterator;
	private:
	VS a,b,c;
	int64_t mpow(int64_t x,int64_t n){
		int64_t m=x%M,r=1;
		for(;n;n>>=1){
			if(n&1)r*=m;
			m=(m*m)%M;
		}
		return r;
	}
	void calc(){
	}
	public:
	convolution(const VS&s,const VS&t){
		size_t u=1,m=max(s.size(),t.size()),i;
		while(u<m)u<<=1;
		a.resize(u,0),b.resize(u,0),c.resize(u*2,0);
		for(i=0;i<s.size();i++)a[i]=s[i];
		for(i=0;i<t.size();i++)b[i]=t[i];
	}
}