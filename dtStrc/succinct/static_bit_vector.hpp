#ifndef ELSIE_STATIC_BIT_VECTOR
#define ELSIE_STATIC_BIT_VECTOR
#include <bit>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <cassert>
#include <utility>
namespace elsie{

class static_bit_vector{
    using u32=std::uint32_t;
    using u64=std::uint64_t;
    constexpr static u32 bsz=64;
    bool fixed;
    size_t sz;
    std::vector<u32>cnt;
    std::vector<u64>bit;
    u32 ceil(u64 a,u64 b){return(a+b-1)/b;}
    public:
    static_bit_vector():fixed(false),bit(0),cnt(0),sz(0){}
    static_bit_vector(size_t N):fixed(false),sz(N),bit(ceil(N,bsz),0),cnt(ceil(N,bsz),0){}
    bool get(size_t idx)const{return(bit[idx>>6]&u64(1)<<(idx&0x3F))!=0;}
    void set(size_t idx){
        assert(!fixed);
        bit[idx>>6]|=u64(1)<<(idx&0x3F);
    }
    void reset(size_t idx){
        assert(!fixed);
        bit[idx>>6]&=~(u64(1)<<(idx&0x3F));
    }
    void fix(){
        for(u32 i=1;i<cnt.size();++i)
            cnt[i]=cnt[i-1]+std::popcount(bit[i-1]);
        fixed=true;
    }
    // [0,idx)
    size_t rank1(size_t idx)const{
        assert(fixed);
        return cnt[idx>>6]+std::popcount(bit[idx>>6]&((u64(1)<<(idx&0x3F))-1));
    }
    size_t rank0(size_t idx)const{
        assert(fixed);
        return idx+1-rank1(idx);
    }
    // [l,r)
    size_t rank1(size_t l,size_t r)const{
        assert(fixed);
        return rank1(r)-rank1(l);
    }
    size_t rank0(size_t l,size_t r)const{
        assert(fixed);
        return rank0(r)-rank0(l);
    }

    const static_bit_vector&operator=(const static_bit_vector&v){
        sz=v.sz;
        fixed=v.fixed;
        cnt=v.cnt;
        bit=v.cnt;
    }
    const static_bit_vector&operator=(static_bit_vector&&v){
        sz=v.sz;
        fixed=v.fixed;
        bit=std::move(v.bit);
        cnt=std::move(v.cnt);
    }
};
}// namespace elsie
#endif // include guard