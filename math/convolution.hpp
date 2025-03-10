#ifndef ELSIE_CONVOLUTION
#define ELSIE_CONVOLUTION
#include <vector>
#include <cstdint>
#include <algorithm>
#include <type_traits>
#include <math/basic_math.hpp>
#include <math/crt_garner.hpp>
#include <math/prime_factor.hpp>
namespace elsie{
using namespace std;
template<class T,size_t M=998244353>
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
        return move(ret);
    }
    void bit_reverse_sort(vector<T>&s){
        uint32_t n=s.size();
        for(uint32_t i=0;i<n;++i)
            if(i<rev_swp[i])swap(s[i],s[rev_swp[i]]);
    }
    template<bool sign>
    void ntt(vector<T>&s,int term){
        bit_reverse_sort(s);
        uint64_t N=1;
        for(uint32_t cnt=1;cnt<=term;++cnt,N<<=1){
            for(int64_t st=0;st+N<s.size();st+=N<<1){
                ll z,p=1;
                if constexpr(sign)z=invroot[cnt];
                else z=root[cnt];
                for(int64_t j=0;j<N;++j){
                    int64_t pt=st+j,qt=pt+N;
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
        root[sz]=modpow(primitive_root(M),M-1>>sz,M);
        invroot[sz]=modpow(root[sz],M-2,M);
        for(int32_t i=sz-1;i>=1;--i){
            root[i]=root[i+1]*root[i+1]%M;
            invroot[i]=invroot[i+1]*invroot[i+1]%M;
        }
        rev_swp=bit_reverse_permutation(u);
        a.resize(u,0),ntt<false>(a,sz);
        b.resize(u,0),ntt<false>(b,sz);
        for(int i=0;i<u;i++)
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

template<class T,size_t M=998244353>
class convolution{
    static constexpr bool f_prime=is_prime(M);
    public:
    convolution(){}
    vector<T>prod(const vector<T>&a,const vector<T>&b)const{
        const size_t len=a.size()+b.size()-1;
        const size_t step=63-countl_zero(len)+(popcount(len)!=1);
        const size_t max_step=countr_zero(M-1);
        if(max_step>=step&&f_prime)
            return convolution_fast<T,M>().prod(a,b);
        constexpr size_t p25=  167'772'161; // 5 * 2^25 +1
        constexpr size_t p26=  469'762'049; // 7 * 2^26 +1
        constexpr size_t p27=2'013'265'921; // 15* 2^27 +1
        convolution_fast<T,p25> c25;
        convolution_fast<T,p26> c26;
        convolution_fast<T,p27> c27;
        vector<T>ret(len);
        vector<vector<T>>r(3);
        r[0]=c25.prod(a,b);
        r[1]=c26.prod(a,b);
        r[2]=c27.prod(a,b);
        vector<T>q(3),m{p25,p26,p27};
        for(size_t i=0;i<len;++i){
            for(uint32_t j=0;j<3;++j)
                q[j]=r[j][i];
            ret[i]=garner<T>(q,m,M);
        }
        return ret;
    }
};
}
#endif