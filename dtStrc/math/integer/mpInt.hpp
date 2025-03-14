#ifndef ELSIE_MULTIPRECISION_INTEGER
#define ELSIE_MULTIPRECISION_INTEGER
#include <array>
#include <cstddef>
#include <cstdint>
namespace elsie{
using namespace std;
// mod_cntは1毎に9桁精度が増える．
template<size_t mod_cnt=10>
class mpInt{
    using it=uint32_t;
    using ut=uint64_t;
    constexpr static size_t max_mod=128;
    constexpr static it P_=1'000'000'000;
    constexpr static array<it,max_mod>mods{P_+7,P_+9,P_+21,P_+33,P_+87,P_+93,P_+97,P_+103,P_+123,P_+181,P_+207,P_+223,P_+241,P_+271,P_+289,P_+297,P_+321,P_+349,P_+363,P_+403,P_+409,P_+411,P_+427,P_+433,P_+439,P_+447,P_+453,P_+459,P_+483,P_+513,P_+531,P_+579,P_+607,P_+613,P_+637,P_+663,P_+711,P_+753,P_+787,P_+801,P_+829,P_+861,P_+871,P_+891,P_+901,P_+919,P_+931,P_+933,P_+993,P_+1011,P_+1021,P_+1053,P_+1087,P_+1099,P_+1137,P_+1161,P_+1203,P_+1213,P_+1237,P_+1263,P_+1269,P_+1273,P_+1279,P_+1311,P_+1329,P_+1333,P_+1351,P_+1371,P_+1393,P_+1413,P_+1447,P_+1449,P_+1491,P_+1501,P_+1531,P_+1537,P_+1539,P_+1581,P_+1617,P_+1621,P_+1633,P_+1647,P_+1663,P_+1677,P_+1699,P_+1759,P_+1773,P_+1789,P_+1791,P_+1801,P_+1803,P_+1819,P_+1857,P_+1887,P_+1917,P_+1927,P_+1957,P_+1963,P_+1969,P_+2043,P_+2089,P_+2103,P_+2139,P_+2149,P_+2161,P_+2173,P_+2187,P_+2193,P_+2233,P_+2239,P_+2277,P_+2307,P_+2359,P_+2361,P_+2431,P_+2449,P_+2457,P_+2499,P_+2571,P_+2581,P_+2607,P_+2631,P_+2637,P_+2649,P_+2667,P_+2727,P_+2791,P_+2803};
    private:
    bool sign;
    array<it,mod_cnt>val;
    void u64toVal(ut x){
    }
    public:
    mpInt():sign(0){}
    mpInt(uint64_t x):sign(0){ u64toVal(x); }
    mpInt(int64_t x):sign(x<0){ u64toVal(sign?~ut(x)+1:x); }
    mpInt(const mpInt&v):sign(v.sign),val(v.val){}
    mpInt(mpInt&&v):sign(v.sign){ val=move(v.val); }
};
}
#endif