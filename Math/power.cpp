#include<vector>
#include<cstdint>
#include<limits>

int64_t safepow(int64_t a,uint64_t b){
	int64_t ret=1;
	int64_t k=a;
	double check=1,kcheck=a;
	while(b){
		if(b&1){
			check*=k;
			if(check>INT64_MAX)return INT64_MAX;
			if(check<INT64_MIN)return INT64_MIN;
			ret*=k;
		}
		kcheck*=k;
		if(kcheck>INT64_MAX)return INT64_MAX;
		if(kcheck<INT64_MIN)return INT64_MIN;
		k*=k;
		b>>=1;
	}
	return ret;
}

int64_t modpow(int64_t a,uint64_t b,uint64_t mod){
	__uint128_t k=a%mod,ret=1;
	if(k<0)k+=mod;
	while(b){
		if(b&1)ret=(ret*k)%mod;
		k=(k*k)%mod;
		b>>=1;
	}
	return int64_t(ret);
}

template<class f>
vector<vector<f>>Mpower(vector<vector<f>>&A,uint64_t b) {
	auto mul=[&](vector<vector<f>>&a,vector<vector<f>>&b)->vector<vector<f>> {
		size_t rank=a.size();
		vector<vector<f>>res(rank,vector<f>(rank,0));
		for(size_t i=0;i<rank;i++)
		for(size_t j=0;j<rank;j++)
		for(size_t k=0;k<rank;k++)
			res[i][j]+=a[i][k]*b[k][j];
		return res;
	};
	size_t rank=A.size();
	vector<vector<f>>I=A;
	vector<vector<f>>res(rank,vector<f>(rank,0));
	for(int i=0;i<rank;i++)
		res[i][i]=1;
	while(b){
		if(b&1)res=mul(res,I);
		I=mul(I,I);
		b>>=1;
	}
	return res;
}