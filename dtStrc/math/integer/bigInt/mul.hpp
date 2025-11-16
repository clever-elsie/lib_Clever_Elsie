#ifndef ELSIE_BIGINT_MUL
#define ELSIE_BIGINT_MUL
#include "dtStrc/math/integer/bigInt/bigInt_image.hpp"
namespace elsie{
void bigInt::naive_mul(sv64 a,sv64 b,mv64 r){
  sv64 s=a,t=b;
  if(a.size()<b.size())swap(s,t);
  for(size_t i=0;i<s.size();++i){
    __uint128_t carry=0;
    for(size_t j=0;j<t.size();++j){
      u64 hi,lo;
      asm("mulq %3" :"=a"(lo),"=d"(hi):"a"(s[i]),"r"(t[j]));
      carry+=__uint128_t(lo)+r[i+j];
      r[i+j]=(u64)carry;
      carry>>=64;
      carry+=__uint128_t(hi)+r[i+j+1];
      r[i+j+1]=(u64)carry;
      carry>>=64;
    }
  }
}

void bigInt::karatsuba(sv64 a,sv64 b,mv64 r){
  const size_t msize=std::min(a.size(),b.size());
  if(msize<Lp[0]) return naive_mul(a,b,r);
  const size_t hsize=msize>>1,wsize=hsize<<1;
  auto split=[&](sv64&s){ return std::pair<sv64,sv64>{sv64(s.begin()+hsize,s.end()),sv64(s.begin(),s.begin()+hsize)}; };
  auto[ahi,alow]=split(a);
  auto[bhi,blow]=split(b);
  mv64 low(r.begin(),wsize),hi(r.begin()+wsize,r.end());
  karatsuba(alow,blow,low);
  karatsuba(ahi,bhi,hi);
  // 正のspan同士の減算
  auto sub=[&](sv64 Hi,sv64 Lo)-> vu64 {
    vu64 r(Hi.size()+1);
    __uint128_t carry=1;
    for(size_t i=0;i<Lo.size();++i){
      carry=carry+Hi[i]+~Lo[i];
      r[i]=(u64)carry;
      carry>>=64;
    }
    for(size_t i=Lo.size();i<Hi.size();++i){
      carry=carry+Hi[i]+~u64(0);
      r[i]=(u64)carry;
      carry>>=64;
    }
    return r;
  };
  bigInt mid(sub(ahi,alow));
  mid*=bigInt(sub(bhi,blow));
  // mid-(hi+low)して，r-mid
  if(mid.data.size()-1<hi.size()) mid.data.resize(hi.size()+1,mid.data.back()); // 符号拡張
  __uint128_t carry=2;
  for(size_t i=0;i<low.size();++i){
    carry=carry+mid.data[i]+~hi[i]+~low[i];
    mid.data[i]=(u64)carry;
    carry>>=64;
  }
  for(size_t i=low.size();i<hi.size();++i){
    carry=carry+mid.data[i]+~hi[i];
    mid.data[i]=(u64)carry;
    carry>>=64;
  }
  for(size_t i=hi.size();i<mid.data.size()-1;++i){
    carry+=mid.data[i];
    mid.data[i]=(u64)carry;
    carry>>=64;
  }
  if(carry){
    mid.data.push_back(mid.data.back());
    mid.data[mid.data.size()-2]+=carry;
  }
  return bigInt(std::move(r))-=std::move(mid);
}

void bigInt::ntt_cnvlt(sv64 a,sv64 b,mv64 r){

}

void bigInt::multiply(cv64&a,cv64&b){
  // 符号を除いた配列サイズ
  const size_t m=std::min<size_t>(a.size(),b.size())-1;
  // 新しい配列のサイズ
  const size_t n=a.size()+b.size()-1;
  // unsignedで処理
  sv64 s(a.begin(),a.end()-1),t(b.begin(),b.end()-1);
  vu64 r(n,0);
  mv64 rspan(r.begin(),r.end()-1);
     if(m<=Lp[0]) naive_mul(s,t,rspan); // O(n^2)
  else if(m<=Lp[1]) karatsuba(s,t,rspan); // O(n^{1.58})
  else              ntt_cnvlt(s,t,rspan); // O(n\lg n)
  r.push_back(0);
}
} // namespace elsie
#endif