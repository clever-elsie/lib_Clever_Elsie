#ifndef ELSIE_CONVOLUTION
#define ELSIE_CONVOLUTION
#include <array>
#include <vector>
#include <cstdint>
#include <variant>
#include <algorithm>
#include <type_traits>
#include <math/basic_math.hpp>
#include <math/prime_factor.hpp>
namespace elsie{
using namespace std;
template<uint32_t M=998244353>
class convolution_fast{
    static_assert(is_prime(M),"convolution_fast requires that M is prime");
    private:
    using i32 = int32_t; using u32 = uint32_t;
    using i64 = int64_t; using u64 = uint64_t;
    using ll = int64_t; using ull= uint64_t;
    vector<ll> root,invroot;
    inline u32 bit_rev(u32 x){
        x=(x<<16)|(x>>16);
        x=((x&0x00FF00FF)<<8)|((x&0xFF00FF00)>>8);
        x=((x&0x0F0F0F0F)<<4)|((x&0xF0F0F0F0)>>4);
        x=((x&0x33333333)<<2)|((x&0xCCCCCCCC)>>2);
        return((x&0x55555555)<<1)|((x&0xAAAAAAAA)>>1);
    }
    public:
    template<bool sign>
    void ntt(vector<u32>&s,uint32_t term){
        uint64_t N=1,shf=countl_zero((u32)s.size())+1;
        if(1<=term){ // 初回はbit reverse 昇順アクセス
            vector<u32>pre(s.size(),UINT32_MAX);
            for(size_t st=0;st+N<s.size();st+=N<<1){
                ll z,p=1;
                if constexpr(sign)z=invroot[1];
                else z=root[1];
                for(size_t j=0;j<N;++j){
                    u32 sj=st+j,jn=sj+N;
                    u32 pt=bit_rev(sj)>>shf,qt=bit_rev(jn)>>shf;
                    ll f,t;
                    if(pre[pt]!=UINT32_MAX)f=pre[pt];
                    else f=pre[pt]=s[pt];
                    if(pre[qt]!=UINT32_MAX)t=(u64)pre[qt]*p%M;
                    else t=(u64)p*(pre[qt]=s[qt])%M;
                    if(pre[sj]==UINT32_MAX)pre[sj]=s[sj];
                    s[sj]=(u64)f+t>=M?(u64)f+t-M:f+t;
                    if(pre[jn]==UINT32_MAX)pre[jn]=s[jn];
                    s[jn]=f>=t?f-t:(u64)f+M-t;
                    p=mul64to128(p,z)%M;
                }
            }
            N=2;
        }
        for(uint32_t cnt=2;cnt<=term;++cnt,N<<=1){
            for(size_t st=0;st+N<s.size();st+=N<<1){
                ll z,p=1;
                if constexpr(sign)z=invroot[cnt];
                else z=root[cnt];
                for(size_t j=0;j<N;++j){
                    size_t pt=st+j,qt=pt+N;
                    ll f=s[pt], t=p*s[qt]%M;
                    u64 ad=(u64)f+t;
                    if(ad>=M)ad-=M;
                    s[pt]=ad;
                    s[qt]=f>=t?f-t:(u64)f+M-t;
                    p=mul64to128(p,z)%M;
                }
            }
        }
    }
    public:
    int on=0;
    convolution_fast(){}
    template<bool is_Zmod=true>
    vector<u32>prod_move(vector<u32>&&a,vector<u32>&&b){
        if(a.empty()||b.empty())return{0};
        size_t m=a.size()+b.size()-1;
        size_t u=1ull<<(sizeof(ull)*8-1-countl_zero(m)+(popcount(m)!=1));
        size_t sz=countr_zero(u);
        root.resize(sz+1,1),invroot.resize(sz+1,1);
        root[sz]=modpow(primitive_root(M),(M-1)>>sz,M);
        invroot[sz]=modpow(root[sz],M-2,M);
        for(int32_t i=sz-1;i>=1;--i){
            root[i]=root[i+1]*root[i+1]%M;
            invroot[i]=invroot[i+1]*invroot[i+1]%M;
        }
        if constexpr(!is_Zmod)
            for(auto&x:a) x%=M;
        a.resize(u,0),ntt<false>(a,sz);
        if constexpr(!is_Zmod)
            for(auto&x:b) x%=M;
        b.resize(u,0),ntt<false>(b,sz);
        for(ull i=0;i<u;++i)
            a[i]=(u64)a[i]*b[i]%M;
        ntt<true>(a,sz);
        a.resize(m);
        ll invN=modpow(u,M-2,M);
        for(auto&&x:a)x=invN*x%M;
        return a;
    }
    template<bool is_Zmod=true>
    vector<u32>prod_amove(vector<u32>&&a,vector<u32>b){ return prod_move<is_Zmod>(move(a),move(b)); }
    template<bool is_Zmod=true>
    vector<u32>prod_bmove(vector<u32>a,vector<u32>&&b){ return prod_move<is_Zmod>(move(a),move(b)); }
    template<bool is_Zmod=true>
    vector<u32>prod(vector<u32>a,vector<u32>b){ return prod_move<is_Zmod>(move(a),move(b)); }
};

template<class T,T M=998244353,size_t psz=2>
class convolution{
    static_assert(1<=psz&&psz<=11,"Even if you use a good prime as M, you still need to satisfy the condition 1<=psz<=11.");
    static constexpr bool f_prime=is_prime<T>(M);
    using u32 = uint32_t;
    static constexpr array<u32,11>mods{
        2'885'681'153u/*43 * 2^26 +1*/,2'717'908'993u/*81 * 2^25 +1*/,
        2'483'027'969u/*37 * 2^26 +1*/,2'281'701'377u/*17 * 2^27 +1*/,
        2'113'929'217u/*63 * 2^25 +1*/,2'013'265'921u/*15 * 2^27 +1*/,
        1'811'939'329u/*27 * 2^26 +1*/,1'711'276'033u/*51 * 2^25 +1*/,
        1'107'296'257u/*33 * 2^25 +1*/,  469'762'049u/* 7 * 2^26 +1*/,
          167'772'161u/* 5 * 2^25 +1*/
    };
    template<size_t... I>
    static constexpr array<T,psz+1>make_mods_s(index_sequence<I...>){ return{{mods[I]...,M}}; }
    static constexpr array<T,psz+1>mods_s=make_mods_s(make_index_sequence<psz>{});

    T garner_impl(const array<u32,psz>&b)const
    requires is_integral_v<T>||same_as<T,__int128_t>||same_as<T,__uint128_t>{
        using int_s=conditional_t<(sizeof(T)>8),__int128_t,int64_t>;
        using vc=vector<int_s>;
        constexpr __uint128_t threshold=__uint128_t(1)<<64;
        array<int_s,psz+1>coeffs,constants;
        coeffs.fill(1); constants.fill(0);
        if constexpr(M<=threshold){
            for(size_t k=0;k<b.size();++k){
                int_s tmp=b[k]>=constants[k]?b[k]-constants[k]:((int_s)b[k]+mods_s[k])-constants[k];
                int_s t=tmp*mod_inv<int_s>(coeffs[k],mods_s[k])%mods_s[k];
                for(size_t i=k+1;i<=b.size();++i){
                    constants[i]=(t*coeffs[i]%mods_s[i]+constants[i])%mods_s[i];
                    coeffs[i]=coeffs[i]*mods_s[k]%mods_s[i];
                }
            }
        }else{
            for(size_t k=0;k<b.size();++k){
                int_s tmp=b[k]>=constants[k]?b[k]-constants[k]:(b[k]+mods_s[k])-constants[k];
                int_s t=tmp*mod_inv<int_s>(coeffs[k],mods_s[k])%mods_s[k];
                for(size_t i=k+1;i<=b.size();++i){
                    constants[i]=(constants[i]+rem256(mul128(t,coeffs[i]),mods_s[i]))%mods_s[i];
                    coeffs[i]=rem256(mul128(coeffs[i],mods_s[k]),mods_s[i]);
                }
            }
        }
        return constants.back();
    }
    template<u32 mod> vector<u32>mod_vec(const vector<T>&a)const{
        vector<u32>r(a.size());
        for(auto rtr=r.begin(),atr=a.begin();atr!=a.end();++atr,++rtr)
            *rtr=*atr%mod;
        return r;
    }
    template<size_t... I>
    vector<T>prod_impl(const vector<T>&a,const vector<T>&b,const size_t len,index_sequence<I...>)const{
        vector<vector<u32>>r={convolution_fast<mods_s[I]>().prod_move(mod_vec<mods_s[I]>(a),mod_vec<mods[I]>(b))...};
        vector<T>ret(len);
        array<u32,psz>q;
        for(size_t i=0;i<len;++i){
            for(uint32_t j=0;j<psz;++j)
                q[j]=r[j][i];
            ret[i]=garner_impl(q);
        }
        return ret;
    }
    public:
    convolution(){}
    vector<T>prod(const vector<T>&a,const vector<T>&b)const{
        const size_t len=a.size()+b.size()-1;
        const size_t step=63-countl_zero(len)+(popcount(len)!=1);
        size_t max_step=0;
        if constexpr(sizeof(T)>8) for(T p=M-1;(p&1==0);p>>=1)++max_step;
        else max_step=countr_zero(uint64_t(M)-1);
        if constexpr(f_prime)if(max_step>=step)
            return convolution_fast<M>().prod(a,b);
        return prod_impl(a,b,len,make_index_sequence<psz>{});
    }
};
}
#endif