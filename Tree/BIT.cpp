#include<vector>
#include<cstddef>
using namespace std;
template<class type>
class BIT{ // 0-indexed [l,r)
	vector<type>v;
	size_t _size;
	type sum(int64_t r){
		type ret=0;
		while(r){
			ret+=v[r-1];
			r-=r&-r;
		}
		return ret;
	}
public:
	BIT(int64_t size){
		assert(0<=size&&size<=1'000'000'000);
		int64_t n=1;
		while(n<size)n*=2;
		_size=n, v.resize(n);
	}
	void add(int64_t i,int val){
		i++;
		do{
			v[i-1]+=val;
			i+=i&-i;
		}while(i<=_size);
	}
	type sum(int64_t l,int64_t r){ return sum(r)-sum(l); }
	type get(int64_t i){ return sum(i,i+1); }
	void set(int64_t i,type t){
		type now=get(i);
		add(i,t-now);
	}
};