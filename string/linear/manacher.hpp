#ifndef ELSIE_MANACHER
#define ELSIE_MANACHER
#include <cstddef>
#include <cstdint>
#include <vector>
#include <concepts>
#include <type_traits>
namespace elsie{
using namespace std;

template<class T>
vector<uint32_t>manacher(const T&v){
    ssize_t i=0,j=0;
    vector<uint32_t>r(v.size());
    while(i<v.size()){
        while(i-j>=0&&i+j<v.size()&&v[i-j]==v[i+j])++j;
        r[i]=j;
        ssize_t k=1;
        while(i-k>=0&&k+r[i-k]<j)r[i+k]=r[i-k],++k;
        i+=k,j-=k;
    }
    return r;
}

/**
 * @brief 配列vのi番目の要素を中心とする回文の最長の長さを返す．
 * @return [(奇数長の回文の長さ，偶数長の回文の長さ)]
 * @details 偶数長が中心のどちら側かは前後の偶数長と比較し同じ方である．両方同じことは多分ない．知らんけど
 */
template<class T>
vector<pair<uint32_t,uint32_t>>LongestPalindrome(const vector<T>&v){
    vector<pair<uint32_t,uint32_t>>ret(v.size(),{0,0});
    vector<uint32_t>r=manacher(v);
    for(size_t i=0;i<v.size();++i)
        ret[i].first=2*r[i]-1;
    if(r.size()>1){
        vector<T>s;
        s.reserve(v.size()*2-1);
        s.push_back(r[0]);
        for(size_t i=1;i<v.size();++i){
            s.push_back(0);
            s.push_back(v[i]);
        }
        vector<uint32_t>p=manacher(s);
        for(size_t i=0,j=1;i+1<v.size();j+=2,++i){
            uint32_t c=(p[j]/2)*2;
            ret[i].second=max(ret[i].second,c);
            ret[i+1].second=c;
        }
    }
    return ret;
}
}
#endif