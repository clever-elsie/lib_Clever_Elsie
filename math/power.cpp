#include<vector>
#include<cstdint>
#include<limits>
#ifndef ELSIE_POWER
#define ELSIE_POWER
namespace elsie{
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

template<class f>vector<vector<f>> Mpower(const vector<vector<f>>&A,uint64_t b){
	using vc=vector<f>;
	using vv=vector<vc>;
	using szt=size_t;
	auto mul=[&](vv&a,vv&b)->vv {
		szt r=a.size();
		vv res(r,vc(r,0));
		for(szt i=0;i<r;i++)for(szt j=0;j<r;j++)
			for(szt k=0;k<r;k++)res[i][j]+=a[i][k]*b[k][j];
		return res;
	};
	szt r=A.size();
	vv I=A,res(r,vc(r,0));
	for(szt i=0;i<r;i++) res[i][i]=1;
	while(b){
		if(b&1)res=mul(res,I);
		I=mul(I,I);
		b>>=1;
	}
	return res;
}
}
#endif