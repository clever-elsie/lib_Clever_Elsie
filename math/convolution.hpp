#ifndef ELSIE_CONVOLUTION
#define ELSIE_CONVOLUTION
#include <array>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <type_traits>
#include <math/basic_math.hpp>
#include <math/crt_garner.hpp>
#include <math/prime_factor.hpp>
namespace elsie{
using namespace std;
template<class T,T M=998244353>
class convolution_fast{
    private:
    using ll = conditional_t<(M>3'037'000'499),__int128_t,int64_t>;
    using ull= conditional_t<(M>3'037'000'499),__uint128_t,uint64_t>;
    vector<ll> root,invroot;
    vector<uint32_t>rev_swp;
    vector<uint32_t>bit_reverse_permutation(uint32_t n){
        uint32_t lgN=countr_zero(n);
        vector<uint32_t>ret(n);
        for(uint32_t i=0;i<n;++i){
            ret[i]=0;
            for(uint32_t j=0;j<lgN;++j)
                if(i&(1ul<<j))
                    ret[i]|=(1ul<<(lgN-1-j));
        }
        return ret;
    }
    void bit_reverse_sort(vector<T>&s){
        uint32_t n=s.size();
        for(uint32_t i=0;i<n;++i)
            if(i<rev_swp[i])swap(s[i],s[rev_swp[i]]);
    }
    template<bool sign>
    void ntt(vector<T>&s,uint32_t term){
        bit_reverse_sort(s);
        uint64_t N=1;
        for(uint32_t cnt=1;cnt<=term;++cnt,N<<=1){
            for(size_t st=0;st+N<s.size();st+=N<<1){
                ll z,p=1;
                if constexpr(sign)z=invroot[cnt];
                else z=root[cnt];
                for(size_t j=0;j<N;++j){
                    size_t pt=st+j,qt=pt+N;
                    ll f=s[pt], t=p*s[qt]%M;
                    s[pt]=f+t; if(s[pt]>=M)s[pt]-=M;
                    s[qt]=f-t; if(s[qt]<0)s[qt]+=M;
                    p=mul64to128(p,z)%M;
                }
            }
        }
    }
    public:
    convolution_fast(){}
    vector<T>prod(vector<T>a,vector<T>b){
        if(a.empty()||b.empty()) return vector<T>(0);
        ull m=a.size()+b.size()-1;
        ull u=1ull<<(sizeof(ull)*8-1-countl_zero(m)+(popcount(m)!=1));
        ull sz=countr_zero(u);
        root.resize(sz+1,1),invroot.resize(sz+1,1);
        root[sz]=modpow(primitive_root(M),(M-1)>>sz,M);
        invroot[sz]=modpow(root[sz],M-2,M);
        for(int32_t i=sz-1;i>=1;--i){
            root[i]=root[i+1]*root[i+1]%M;
            invroot[i]=invroot[i+1]*invroot[i+1]%M;
        }
        rev_swp=bit_reverse_permutation(u);
        for(auto&x:a){ x%=M; if constexpr(is_signed_v<T>) if(x<0)x+=M; }
        a.resize(u,0),ntt<false>(a,sz);
        for(auto&x:b){ x%=M; if constexpr(is_signed_v<T>) if(x<0)x+=M; }
        b.resize(u,0),ntt<false>(b,sz);
        for(ull i=0;i<u;++i)
            if constexpr(is_integral_v<T>)
                if constexpr(is_same_v<ll,__int128_t>)
                    a[i]=mul64to128(a[i],b[i])%M;
                else a[i]=a[i]*b[i]%M;
            else a[i]*=b[i];
        ntt<true>(a,sz);
        a.resize(m);
        ll invN=modpow(u,M-2,M);
        for(auto&&x:a)x=invN*x%M;
        return a;
    }
};

template<class T,T M=998244353,size_t psz=2>
class convolution{
    static_assert(1<=psz&&psz<=11,"Even if you use a good prime as M, you still need to satisfy the condition 1<=psz<=11.");
    static constexpr bool f_prime=is_prime<T>(M);
    static constexpr array<size_t,11>mods{
        2'885'681'153/*43 * 2^26 +1*/,2'717'908'993/*81 * 2^25 +1*/,
        2'483'027'969/*37 * 2^26 +1*/,2'281'701'377/*17 * 2^27 +1*/,
        2'113'929'217/*63 * 2^25 +1*/,2'013'265'921/*15 * 2^27 +1*/,
        1'811'939'329/*27 * 2^26 +1*/,1'711'276'033/*51 * 2^25 +1*/,
        1'107'296'257/*33 * 2^25 +1*/,  469'762'049/* 7 * 2^26 +1*/,
          167'772'161/* 5 * 2^25 +1*/
    };
    template<size_t... I>
    vector<T>prod_impl(const vector<T>&a,const vector<T>&b,const size_t len,index_sequence<I...>)const{
        vector<vector<T>>r={convolution_fast<T,mods[I]>().prod(a,b)...};
        vector<T>ret(len);
        vector<T>q(psz),m(psz);
        for(size_t i=0;i<psz;++i)m[i]=mods[i];
        for(size_t i=0;i<len;++i){
            for(uint32_t j=0;j<psz;++j)
                q[j]=r[j][i];
            ret[i]=garner<T>(q,m,M);
        }
        return ret;
    }
    public:
    convolution(){}
    vector<T>prod(const vector<T>&a,const vector<T>&b)const{
        const size_t len=a.size()+b.size()-1;
        const size_t step=63-countl_zero(len)+(popcount(len)!=1);
        size_t max_step=0;
        if constexpr(sizeof(T)) for(T p=1;p<len;p<<=1)++max_step;
        else max_step=countr_zero(uint64_t(M)-1);
        if constexpr(f_prime)if(max_step>=step)
            return convolution_fast<T,M>().prod(a,b);
        return prod_impl(a,b,len,make_index_sequence<psz>{});
    }
};
}
#endif