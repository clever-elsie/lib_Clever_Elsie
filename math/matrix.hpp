#include <vector>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>
#ifndef ELSIE_STRASSEN
#define ELSIE_STRASSEN
namespace elsie{
	using namespace std;
	using ps=pair<size_t,size_t>;
	template<class S>using vc=vector<S>;
	template<class S>using vv=vc<vc<S>>;
	template<class S>using v3=vv<vc<S>>;

	/**
	 * @brief a:NxN matrix. N is 2^i and N>1.
	 * @return {A_00, A_01, A_10, A_11}
	 */
	template<class S>v3<S>split_quad_matrix(const vv<S>&a){
		size_t n=a.size(),n2=n>>1;
		assert(n==a[0].size()&&popcount(n)==1);
		v3<S>r(4,vv<S>(n2,vc<S>(n2,S())));
		auto f=[&](size_t idx,size_t py,size_t px){
			for(size_t i=0,j;i<n2;++i)
			for(j=0;j<n2;++j)
				r[idx][i][j]=a[i+py][j+px];
		};
		f(0,0,0),f(1,0,n2),f(2,n2,0),f(3,n2,n2);
		return r;
	}

	template<class S>vv<S>merge_quad_matrix(const vv<S>&c11,const vv<S>&c12,const vv<S>&c21,const vv<S>&c22){
		size_t n=c11.size();
		vv<S>c(n*2,vc<S>(n*2,S()));
		for(size_t i=0,j;i<n;++i)
		for(j=0;j<n;++j)c[i][j]=c11[i][j];
		for(size_t i=0,j;i<n;++i)
		for(j=0;j<n;++j)c[i][j+n]=c12[i][j];
		for(size_t i=0,j;i<n;++i)
		for(j=0;j<n;++j)c[i+n][j]=c21[i][j];
		for(size_t i=0,j;i<n;++i)
		for(j=0;j<n;++j)c[i+n][j+n]=c22[i][j];
		return c;
	}

	template<bool sub=false,class S,class T,class U=common_type_t<S,T>>
	vv<U>matrix_add(const vv<S>&a,const vv<T>&b){
		assert(a.size()==b.size()&&a[0].size()==b[0].size());
		vv<U>c(a.size(),vc<U>(a[0].size(),U()));
		for(size_t i=0;i<a.size();++i)
		for(size_t j=0;j<a[i].size();++j)
			if constexpr(sub)c[i][j]=a[i][j]-b[i][j];
			else c[i][j]=a[i][j]+b[i][j];
		return c;
	}

	template<size_t threshold=256,bool q2=true,class S,class T,class U=common_type_t<S,T>>
	vv<U>matrix_mul(vv<S>&a,vv<T>&b){
		assert(a[0].size()==b.size());
		size_t N=a.size(),M=b.size(),L=b[0].size();
		if(max(N,max(M,L))<threshold){
			vv<U>c(N,vc<U>(L,U()));
			for(size_t i=0,j,k;i<N;++i)
			for(j=0;j<L;++j)for(k=0;k<M;++k)
				c[i][j]+=a[i][k]*b[k][j];
			return c;
		}else{
			size_t n=1;
			if constexpr(q2){
				for(size_t P=max(N,max(M,L));n<P;n<<=1);
				if(M<n)for(auto&x:a)x.resize(n,S());
				a.resize(n,vc<S>(n,S()));
				if(L<n)for(auto&x:b)x.resize(n,T());
				b.resize(n,vc<T>(n,T()));
			}else n=L;
			auto as=split_quad_matrix(a);
			auto bs=split_quad_matrix(b);
			if constexpr(q2){
				a.resize(N);
				if(M<n)for(auto&x:a)x.resize(M);
				b.resize(M);
				if(L<n)for(auto&x:b)x.resize(L);
			}
			auto s1=matrix_add<true>(bs[1],bs[3]);
			auto s2=matrix_add(as[0],as[2]);
			auto s3=matrix_add(as[2],as[3]);
			auto s4=matrix_add<true>(bs[2],bs[0]);
			auto s5=matrix_add(as[0],as[3]);
			auto s6=matrix_add(bs[0],bs[3]);
			auto s7=matrix_add<true>(as[1],as[3]);
			auto s8=matrix_add(bs[2],bs[3]);
			auto s9=matrix_add<true>(as[0],as[2]);
			auto sA=matrix_add(bs[0],bs[1]);

			auto p1=matrix_mul<threshold,false>(as[0],s1);
			auto p2=matrix_mul<threshold,false>(s2,bs[3]);
			auto p3=matrix_mul<threshold,false>(s3,bs[0]);
			auto p4=matrix_mul<threshold,false>(as[3],s4);
			auto p5=matrix_mul<threshold,false>(s5,s6);
			auto p6=matrix_mul<threshold,false>(s7,s8);
			auto p7=matrix_mul<threshold,false>(s9,sA);
			auto r=merge_quad_matrix(
				matrix_add(p5,matrix_add(p4,matrix_add<true>(p6,p2))),matrix_add(p1,p2),
				matrix_add(p3,p4),matrix_add<true>(matrix_add(p5,p1),matrix_add(p3,p7))
			);
			if constexpr(q2){
				r.resize(N);
				if(L<n)for(auto&x:r)x.resize(L);
			}
			return r;
		}
	}
};
#endif