// SPDX-License-Identifier: MIT
// Copyright (c) 2026 CleverElsie
// AVX512 / AVX2 (4-lane) 専用の行列 FMA 実装
// - 対象: elsie::matrix<montgomery_modint<MOD>>
// - ここでは 998244353 を例示しているが、mod()/Mdash() を持つ Montgomery 型なら同様に拡張可能
#ifndef ELSIE_SIMD_MATRIX_MODINT_HPP
#define ELSIE_SIMD_MATRIX_MODINT_HPP

#include <immintrin.h>
#include <algorithm>
#include <cstdint>
#include <charconv>
#include <dtStrc/math/modint/montgomery.hpp>
#include <dtStrc/math/matrix/matrix.hpp>

namespace elsie {

#ifdef MODE
#define ELSIE_SIMD_MODINT_MATRIX_MODE MODE
#endif
#ifdef select
#define ELSIE_SIMD_MODINT_MATRIX_SELECT select
#endif

#ifdef __AVX512F__
#define MODE 1
#elif defined(__AVX2__)
#define MODE 0
#endif

#if defined(MODE)

// -----------------------------------------------------------------------------
// `select(x, y)` を「プリプロセッサでの分岐」にする。
// - MODE==1 なら x を採用 (AVX512)
// - MODE==0 なら y を採用 (AVX2)
// これにより型や intrinsic 名も混在させずに 1 本の実装にできる。
// -----------------------------------------------------------------------------
#define ELSIE_PP_CAT_IMPL(a, b) a##b
#define ELSIE_PP_CAT(a, b) ELSIE_PP_CAT_IMPL(a, b)
#define ELSIE_PP_SELECT_0(x, y) y
#define ELSIE_PP_SELECT_1(x, y) x
#define select(x, y) ELSIE_PP_CAT(ELSIE_PP_SELECT_, MODE)(x, y)


using mmint_t = elsie::montgomery_modint<998244353>;
using mmat_t  = elsie::matrix<mmint_t>;

namespace simd_modint_matrix_detail {

// AVX2: 4x u64 の各 lane の下位 32bit を [0..3] の u32 に詰めて返す。
// (AVX2 だけで完結するパックを使い、AVX512VL に依存しない)
__attribute__((target("avx2")))
inline __m128i cvt_epi64_to_epi32_avx2(__m256i v) {
  // v は [q0 q1 q2 q3] (各 qi の下位 32bit が欲しい)
  // shuffle_epi32 で各 128bit lane から dword0,2 を [0,1] に並べ、
  // unpacklo_epi64 で (q0,q1) と (q2,q3) を連結する。
  __m256i sh = _mm256_shuffle_epi32(v, _MM_SHUFFLE(2, 0, 2, 0));
  __m128i lo = _mm256_castsi256_si128(sh);
  __m128i hi = _mm256_extracti128_si256(sh, 1);
  return _mm_unpacklo_epi64(lo, hi);
}

} // namespace simd_modint_matrix_detail

#define VSET1_EPI32(x) select(_mm512_set1_epi32((x)), _mm256_set1_epi32((x)))
#define VLOAD_EPI32(ptr) select(_mm512_load_epi32((const __m512i*)(ptr)), _mm256_load_si256((const __m256i*)(ptr)))
#define VSTORE_EPI32(ptr,v) select(_mm512_store_epi32((__m512i*)(ptr),(v)), _mm256_store_si256((__m256i*)(ptr),(v)))
#define VADD_EPI32(a,b) select(_mm512_add_epi32((a),(b)), _mm256_add_epi32((a),(b)))
#define VCMPGT_EPI32(a,b) select(_mm512_cmpgt_epi32_mask((a),(b)), _mm256_cmpgt_epi32((a),(b)))
#define VAND_EPI32(a,b) select(_mm512_and_epi32((a),(b)), _mm256_and_si256((a),(b)))
#define VSUB_EPI32(a,b) select(_mm512_sub_epi32((a),(b)), _mm256_sub_epi32((a),(b)))

template<>
template<>
__attribute__((target(select("avx512f", "avx2"))))
__attribute__((optimize("O3")))
inline mmat_t& mmat_t::operator+=(const mmat_t& b) {
  [[assume(dim().row == b.dim().row)]];
  [[assume(dim().col == b.dim().col)]];
  constexpr size_t simd = select(16, 8);
  const auto[row,col]=b.dim();
  const size_t M = mmint_t::mod();
  const size_t Mminus1 = M - 1;
  const auto Mv32 = VSET1_EPI32(M);
  const auto Mminus1_32 = VSET1_EPI32(Mminus1);
  for(size_t i=0;i<row;++i){
    size_t j;
    auto as = (*this)[i];
    auto bs = b[i];
    for(j=0;j+simd-1<col;j+=simd){
      const auto a32 = VLOAD_EPI32(&as[j]);
      const auto b32 = VLOAD_EPI32(&bs[j]);
      auto c32 = VADD_EPI32(a32, b32);
      const auto mask_c=VCMPGT_EPI32(c32, Mminus1_32);
      select(
        c32 = _mm512_mask_sub_epi32(c32, mask_c, c32, Mv32),
        do{
          __m256i masked = _mm256_and_si256(mask_c, Mv32);
          c32 = _mm256_sub_epi32(c32, masked);
        }while(0);
      );
      VSTORE_EPI32(&as[j], c32);
    }
    for(;j<col;++j)
      (*this)[i,j]+=b[i,j];
  }
  return *this;
}

template<>
__attribute__((target(select("avx512f", "avx2"))))
__attribute__((optimize("O3")))
inline mmat_t operator+(const mmat_t& a, const mmat_t& b) {
  [[assume(a.dim().row == b.dim().row)]];
  [[assume(a.dim().col == b.dim().col)]];
  constexpr size_t simd = select(16, 8);
  const auto[row,col]=b.dim();
  const size_t M = mmint_t::mod();
  const auto Mminus1 = M - 1;
  const auto Mv32 = VSET1_EPI32(M);
  const auto Mminus1_32 = VSET1_EPI32(Mminus1);
  mmat_t ret(a.dim());
  for(size_t i=0;i<row;++i){
    size_t j;
    const auto as = a[i];
    const auto bs = b[i];
    auto ret_row = ret[i];
    for(j=0;j+simd-1<col;j+=simd){
      const auto a32 = VLOAD_EPI32(&as[j]);
      const auto b32 = VLOAD_EPI32(&bs[j]);
      auto c32 = VADD_EPI32(a32, b32);
      const auto mask_c=VCMPGT_EPI32(c32, Mminus1_32);
      select(
        c32 = _mm512_mask_sub_epi32(c32, mask_c, c32, Mv32),
        do{
          __m256i masked = _mm256_and_si256(mask_c, Mv32);
          c32 = _mm256_sub_epi32(c32, masked);
        }while(0);
      );
      VSTORE_EPI32(&ret_row[j], c32);
    }
    for(;j<col;++j)
      ret_row[j]=a[i,j]+b[i,j];
  }
  return ret;
}

template<>
template<>
__attribute__((target(select("avx512f", "avx2"))))
__attribute__((optimize("O3")))
inline mmat_t& mmat_t::operator-=(const mmat_t& b) {
  [[assume(dim().row == b.dim().row)]];
  [[assume(dim().col == b.dim().col)]];
  constexpr size_t simd = select(16, 8);
  const auto[row,col]=b.dim();
  const size_t M = mmint_t::mod();
  const auto Mv32 = VSET1_EPI32(M);
  for(size_t i=0;i<row;++i){
    size_t j;
    auto as = (*this)[i];
    auto bs = b[i];
    for(j=0;j+simd-1<col;j+=simd){
      const auto a32 = VLOAD_EPI32(&as[j]);
      const auto b32 = VLOAD_EPI32(&bs[j]);
      auto c32 = VSUB_EPI32(a32, b32);
      const auto mask_c=VCMPGT_EPI32(b32, a32);
      select(
        c32 = _mm512_mask_add_epi32(c32, mask_c, c32, Mv32),
        do{
          __m256i masked = _mm256_and_si256(mask_c, Mv32);
          c32 = _mm256_add_epi32(c32, masked);
        }while(0);
      );
      VSTORE_EPI32(&as[j], c32);
    }
    for(;j<col;++j) (*this)[i,j]-=b[i,j];
  }
  return *this;
}

template<>
__attribute__((target(select("avx512f", "avx2"))))
__attribute__((optimize("O3")))
inline mmat_t operator-(const mmat_t& a, const mmat_t& b) {
  [[assume(a.dim().row == b.dim().row)]];
  [[assume(a.dim().col == b.dim().col)]];
  constexpr size_t simd = select(16, 8);
  const auto[row,col]=b.dim();
  const size_t M = mmint_t::mod();
  const auto Mv32 = VSET1_EPI32(M);
  mmat_t ret(a.dim());
  for(size_t i=0;i<row;++i){
    size_t j;
    const auto as = a[i];
    const auto bs = b[i];
    auto ret_row = ret[i];
    for(j=0;j+simd-1<col;j+=simd){
      const auto a32 = VLOAD_EPI32(&as[j]);
      const auto b32 = VLOAD_EPI32(&bs[j]);
      auto c32 = VSUB_EPI32(a32, b32);
      const auto mask_c=VCMPGT_EPI32(b32, a32);
      select(
        c32 = _mm512_mask_add_epi32(c32, mask_c, c32, Mv32),
        do{
          __m256i masked = _mm256_and_si256(mask_c, Mv32);
          c32 = _mm256_add_epi32(c32, masked);
        }while(0);
      );
      VSTORE_EPI32(&ret_row[j], c32);
    }
    for(;j<col;++j)
      ret_row[j]=a[i,j]-b[i,j];
  }
  return ret;
}

// +, +=, -, -= で使ったdefineの削除
#undef VSET1_EPI32
#undef VLOAD_EPI32
#undef VADD_EPI32
#undef VCMPGT_EPI32
#undef VCMPGE_EPI32
#undef VAND_EPI32
#undef VSUB_EPI32
#undef VSTORE_EPI32

#define setv1(x) select(_mm512_set1_epi32((x)), _mm256_set1_epi32((x)))
#define setv64(x) select(_mm512_set1_epi64(x), _mm256_set1_epi64x(x))
#define loadv(ptr) select(_mm512_load_epi32((const __m512i*)(ptr)), _mm256_load_si256((const __m256i*)(ptr)))
#define storev32(ptr,v) select(_mm512_store_epi32((__m512i*)(ptr),(v)), _mm256_store_si256((__m256i*)(ptr),(v)))

template<>
template<>
__attribute__((target(select("avx512f", "avx2"))))
__attribute__((optimize("O3")))
inline mmat_t& mmat_t::fma_impl_blocked(const mmat_t& a, const mmat_t& b) {
  [[assume(dim().row == a.dim().row)]];
  [[assume(dim().col == b.dim().col)]];
  [[assume(a.dim().col == b.dim().row)]];

  constexpr size_t BLOCK_SIZE = 64;
  const auto [row, col] = dim();
  const size_t midCR = a.dim().col;
  
  size_t I, J, K, i, j, k, it, jt, kt;
  constexpr static size_t M = mmint_t::mod();
  constexpr static size_t Mdash = mmint_t::Mdash();
  constexpr static size_t simd = select(16, 8);

  using p32_t = select(__m512i, __m256i);
  using p64_t = p32_t; // 中身が64bitのときつまり半分0になってることを明示するためのalias
  // R=2^32, MM'=-1(mod R)
  // s:64=M*(xM' mod R)
  // t= (x+s)/R
  // wb = t mod M
  const p32_t Mv32 = setv1(M), Mminus1v32 = setv1(M-1);
  const p64_t Mv64 = setv64(M), Mdashv64 = setv64(Mdash);
  const p64_t filter_lower_32 = setv64(0xFFFF'FFFF);
  const p64_t filter_upper_32 = setv64(0xFFFF'FFFF'0000'0000);
  const static auto split32v_64 = [&filter_lower_32,&filter_upper_32](select(__m512i, __m256i) v) {
    return std::make_pair(
      select(_mm512_and_epi64(v,filter_lower_32), _mm256_and_si256(v,filter_lower_32)),
      select(_mm512_srli_epi64(v,32), _mm256_srli_epi64(v,32))
    );
  };

  for (I = 0, it = std::min(BLOCK_SIZE, row); I < row; I += BLOCK_SIZE, it = std::min(it + BLOCK_SIZE, row)) {
  for (K = 0, kt = std::min(BLOCK_SIZE, midCR); K < midCR; K += BLOCK_SIZE, kt = std::min(kt + BLOCK_SIZE, midCR)) {
  for (J = 0, jt = std::min(BLOCK_SIZE, col); J < col; J += BLOCK_SIZE, jt = std::min(jt + BLOCK_SIZE, col)) {
    for (i = I; i < it; ++i) {
      const auto as = a[i];
      auto cs = (*this)[i];

      p32_t cache[BLOCK_SIZE/simd];
      for(size_t l=0;l<BLOCK_SIZE && J+l+simd<=jt;l+=simd)
        cache[l/simd] = loadv(&cs[J+l]);

      for (k = K; k < kt; ++k) {
        const auto& ak = as[k];
        const auto b_row = b[k];
        const uint32_t ak_raw = *reinterpret_cast<const uint32_t*>(&ak);
        const auto ak64=setv64(ak_raw);

        for (j = J; j + simd - 1 < jt; j += simd) {
          const auto [b64_lo, b64_hi] = split32v_64(loadv(&b_row[j]));
          p32_t&c32 = cache[(j-J)/simd];//loadv(&cs[j]);
          // xM'*M + x の上位32bitが必要となる
          // s=xM' mod R
          // (x+ xM'M)/R = x/R + (xM'M)/R
          #if defined(__AVX512F__)
            const p64_t x_lo = _mm512_mul_epu32(b64_lo,ak64);
            const p64_t x_hi = _mm512_mul_epu32(b64_hi,ak64);
            const p64_t xMdash_lo = _mm512_mul_epu32(x_lo,Mdashv64);
            const p64_t xMdash_hi = _mm512_mul_epu32(x_hi,Mdashv64);
            const p64_t Ms_lo = _mm512_mul_epu32(xMdash_lo,Mv64); // mul_epu32は下位32bitしか使わないのでxMdash_*はmod Rになる
            const p64_t Ms_hi = _mm512_mul_epu32(xMdash_hi,Mv64);
            const p64_t t_lo = _mm512_add_epi64(x_lo,Ms_lo);
            const p64_t t_hi = _mm512_add_epi64(x_hi,Ms_hi);
            const p32_t t32 = _mm512_or_epi64(_mm512_srli_epi64(t_lo,32), _mm512_and_epi64(t_hi, filter_upper_32));
            const __mmask16 mask = _mm512_cmpgt_epu32_mask(t32,Mminus1v32);
            const p32_t u = _mm512_mask_sub_epi32(t32, mask, t32, Mv32);
            const p32_t n32 = _mm512_add_epi32(c32,u);
            const __mmask16 mask_n = _mm512_cmpgt_epi32_mask(n32,Mv32);
            const p32_t wb = _mm512_mask_sub_epi32(n32, mask_n, n32, Mv32);
          #elif defined(__AVX2__)
            const p64_t x_lo = _mm256_mul_epu32(b64_lo,ak64);
            const p64_t x_hi = _mm256_mul_epu32(b64_hi,ak64);
            const p64_t xMdash_lo = _mm256_mul_epu32(x_lo,Mdashv64);
            const p64_t xMdash_hi = _mm256_mul_epu32(x_hi,Mdashv64);
            const p64_t Ms_lo = _mm256_mul_epu32(xMdash_lo,Mv64); // mul_epu32は下位32bitしか使わないのでxMdash_*はmod Rになる
            const p64_t Ms_hi = _mm256_mul_epu32(xMdash_hi,Mv64);
            const p64_t t_lo = _mm256_add_epi64(x_lo,Ms_lo);
            const p64_t t_hi = _mm256_add_epi64(x_hi,Ms_hi);
            const p32_t t32 = _mm256_or_si256(_mm256_srli_epi64(t_lo,32), _mm256_and_si256(t_hi, filter_upper_32));
            // unsigned比較: t32 > Mminus1v32 をsigned比較で実現（符号ビットXOR）
            const __m256i sign_bit = _mm256_set1_epi32(static_cast<int>(0x80000000u));
            const __m256i mask = _mm256_cmpgt_epi32(_mm256_xor_si256(t32, sign_bit), _mm256_xor_si256(Mminus1v32, sign_bit));
            const p32_t u = _mm256_sub_epi32(t32, _mm256_and_si256(mask, Mv32));
            const p32_t n32 = _mm256_add_epi32(c32,u);
            const __m256i mask_n = _mm256_cmpgt_epi32(n32,Mv32);
            const p32_t wb = _mm256_sub_epi32(n32, _mm256_and_si256(mask_n, Mv32));
          #endif
          c32 = wb;
        }
        for (; j < jt; ++j) cs[j] += ak * b_row[j];
      }
      for(size_t l=0;l<BLOCK_SIZE && J+l+simd<=jt;l+=simd)
        storev32(&cs[J+l], cache[l/simd]);
    }
  }
  }
  }
  return *this;
}

template<class Char, class Traits>
std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& os, const mmat_t& mat) {
  char element_buf[128];
  typename std::basic_ostream<Char, Traits>::sentry sent(os);
  if(!sent)[[unlikely]] return os;
  const auto [row_sz,col_sz]=mat.dim();
  constexpr static size_t simd = select(16, 8);
  using p32_t = select(__m512i, __m256i);
  using p64_t = p32_t;
  const p64_t filter_lower_32 = select(_mm512_set1_epi64(0xFFFF'FFFF), _mm256_set1_epi64x(0xFFFF'FFFF));
  const p64_t filter_upper_32 = select(_mm512_set1_epi64(0xFFFF'FFFF'0000'0000), _mm256_set1_epi64x(0xFFFF'FFFF'0000'0000));
  constexpr static size_t M = mmint_t::mod();
  constexpr static size_t Mdash = mmint_t::Mdash();
  const p32_t Mv32 = setv1(M), Mminus1v32 = setv1(M-1);
  const p64_t Mv64 = setv64(M), Mdashv64 = setv64(Mdash);
  const static auto split32v_64 = [&filter_lower_32,&filter_upper_32](select(__m512i, __m256i) v) {
    return std::make_pair(
      select(_mm512_and_epi64(v,filter_lower_32), _mm256_and_si256(v,filter_lower_32)),
      select(_mm512_srli_epi64(v,32), _mm256_srli_epi64(v,32))
    );
  };

  for(size_t i=0;i<row_sz;++i){
    const auto& row = mat[i];
    size_t j=0;
    uint32_t c32[simd];
    for(;j+simd-1<col_sz;j+=simd){
      #if defined(__AVX512F__)
        const auto[x64_lo, x64_hi] = split32v_64(loadv(&row[j]));
        const static auto x_s_Ms_t = [&](const p64_t& x) {
          const p64_t s = _mm512_and_epi64(_mm512_mul_epu32(x,Mdashv64), filter_lower_32);
          const p64_t Ms = _mm512_mul_epu32(s,Mv64);
          const p64_t t = _mm512_add_epi64(x,Ms);
          return t;
        };
        const p32_t t32 = _mm512_or_epi64(_mm512_srli_epi64(x_s_Ms_t(x64_lo),32), _mm512_and_epi64(x_s_Ms_t(x64_hi), filter_upper_32));
        const __mmask16 mask = _mm512_cmpgt_epu32_mask(t32,Mminus1v32);
        const p32_t wb = _mm512_mask_sub_epi32(t32, mask, t32, Mv32);
      #elif defined(__AVX2__)
        const auto[x64_lo, x64_hi] = split32v_64(loadv(&row[j]));
        const static auto x_s_Ms_t = [&](const p64_t& x) {
          const p64_t s = _mm256_and_si256(_mm256_mul_epu32(x,Mdashv64), filter_lower_32);
          const p64_t Ms = _mm256_mul_epu32(s,Mv64);
          const p64_t t = _mm256_add_epi64(x,Ms);
          return t;
        };
        const p32_t t32 = _mm256_or_si256(_mm256_srli_epi64(x_s_Ms_t(x64_lo),32), _mm256_and_si256(x_s_Ms_t(x64_hi), filter_upper_32));
        // unsigned比較: t32 > Mminus1v32 をsigned比較で実現（符号ビットXOR）
        const __m256i sign_bit = _mm256_set1_epi32(static_cast<int>(0x80000000u));
        const __m256i mask = _mm256_cmpgt_epi32(_mm256_xor_si256(t32, sign_bit), _mm256_xor_si256(Mminus1v32, sign_bit));
        const p32_t wb = _mm256_sub_epi32(t32, _mm256_and_si256(mask, Mv32));
      #endif
      storev32(c32, wb);
      for(size_t k=0;k<simd;++k){
        std::to_chars_result res = std::to_chars(element_buf, element_buf + sizeof(element_buf), c32[k]);
        os.rdbuf()->sputn(element_buf, res.ptr-element_buf);
        if(j+k+1<col_sz)[[likely]]
          os.rdbuf()->sputc(' ');
      }
    }
    for(;j<col_sz;++j){
      const auto& element = row[j];
      auto [ptr, len] = element.write(element_buf);
      os.rdbuf()->sputn(ptr, len);
      if(j+1<row.size())[[likely]]
        os.rdbuf()->sputc(' ');
    }
    if(i+1<mat.dim().row)[[likely]]
      os.rdbuf()->sputc('\n');
  }
  return os;
}

template<>
template<class Char, class Traits>
void mmat_t::read(std::basic_istream<Char, Traits>& is) {
  typename std::basic_istream<Char, Traits>::sentry sent(is);
  if(!sent)[[unlikely]] return;
  const auto [row_sz,col_sz]=dim();
  constexpr static size_t simd = select(16, 8);
  constexpr static size_t Rmod=(1ull<<32)%mmint_t::mod();
  constexpr static size_t R2 = Rmod*Rmod%mmint_t::mod();
  using p32_t = select(__m512i, __m256i);
  using p64_t = p32_t;
  const p64_t filter_lower_32 = select(_mm512_set1_epi64(0xFFFF'FFFF), _mm256_set1_epi64x(0xFFFF'FFFF));
  const p64_t filter_upper_32 = select(_mm512_set1_epi64(0xFFFF'FFFF'0000'0000), _mm256_set1_epi64x(0xFFFF'FFFF'0000'0000));
  constexpr static size_t M = mmint_t::mod();
  constexpr static size_t Mdash = mmint_t::Mdash();
  const p32_t Mv32 = setv1(M), Mminus1v32 = setv1(M-1);
  const p64_t Mv64 = setv64(M), Mdashv64 = setv64(Mdash);
  const p64_t R2v64 = setv64(R2);
  const static auto split32v_64 = [&filter_lower_32,&filter_upper_32](select(__m512i, __m256i) v) {
    return std::make_pair(
      select(_mm512_and_epi64(v,filter_lower_32), _mm256_and_si256(v,filter_lower_32)),
      select(_mm512_srli_epi64(v,32), _mm256_srli_epi64(v,32))
    );
  };

  char buf[32];
  alignas(64) mmint_t::value_type values[simd];
  const static auto read_number = [&](char (&buf)[32])->mmint_t::value_type {
    auto* sb = is.rdbuf();
    char c;
    do{
      const auto ic = sb->sbumpc();
      if(Traits::eq_int_type(ic, Traits::eof())) return mmint_t::value_type{};
      c = Traits::to_char_type(ic);
    }while(c==' ' || c=='\n');

    size_t len = 0;
    for(;;){
      buf[len++] = c;
      const auto ic = sb->sbumpc();
      if(Traits::eq_int_type(ic, Traits::eof())) break;
      c = Traits::to_char_type(ic);
      if(c<'0' || c>'9') break;
    }
    typename mmint_t::value_type value;
    (void)std::from_chars(buf, buf + len, value, 10);
    return value;
  };
  for(size_t i=0;i<row_sz;++i){
    auto row = (*this)[i];
    size_t j=0;
    for(;j+simd-1<col_sz;j+=simd){
      for(size_t k=0;k<simd;++k) values[k] = read_number(buf);
      #if defined(__AVX512F__)
        const auto[v64_lo, v64_hi] = split32v_64(loadv(values));
        const static auto x_s_Ms_t = [&](const p64_t& v) {
          const p64_t x = _mm512_mul_epu32(v,R2v64);
          const p64_t s = _mm512_and_epi64(_mm512_mul_epu32(x,Mdashv64), filter_lower_32);
          const p64_t Ms = _mm512_mul_epu32(s,Mv64);
          const p64_t t = _mm512_add_epi64(x,Ms);
          return t;
        };
        const p32_t t32 = _mm512_or_epi64(_mm512_srli_epi64(x_s_Ms_t(v64_lo),32), _mm512_and_epi64(x_s_Ms_t(v64_hi), filter_upper_32));
        const __mmask16 mask = _mm512_cmpgt_epu32_mask(t32,Mminus1v32);
        const p32_t wb = _mm512_mask_sub_epi32(t32, mask, t32, Mv32);
      #elif defined(__AVX2__)
        const auto[v64_lo, v64_hi] = split32v_64(loadv(values));
        const static auto x_s_Ms_t = [&](const p64_t& v) {
          const p64_t x = _mm256_mul_epu32(v,R2v64);
          const p64_t s = _mm256_and_si256(_mm256_mul_epu32(x,Mdashv64), filter_lower_32);
          const p64_t Ms = _mm256_mul_epu32(s,Mv64);
          const p64_t t = _mm256_add_epi64(x,Ms);
          return t;
        };
        const p32_t t32 = _mm256_or_si256(_mm256_srli_epi64(x_s_Ms_t(v64_lo),32), _mm256_and_si256(x_s_Ms_t(v64_hi), filter_upper_32));
        // unsigned比較: t32 > Mminus1v32 をsigned比較で実現（符号ビットXOR）
        const __m256i sign_bit = _mm256_set1_epi32(static_cast<int>(0x80000000u));
        const __m256i mask = _mm256_cmpgt_epi32(_mm256_xor_si256(t32, sign_bit), _mm256_xor_si256(Mminus1v32, sign_bit));
        const p32_t wb = _mm256_sub_epi32(t32, _mm256_and_si256(mask, Mv32));
      #endif
      storev32(&row[j], wb);
    }
    for(;j<col_sz;++j)
      row[j]=mmint_t::raw(read_number(buf));
  }
}

#undef setv1
#undef setv64
#undef loadv
#undef storev32

#undef MODE
#undef select
#undef ELSIE_PP_CAT_IMPL
#undef ELSIE_PP_CAT
#undef ELSIE_PP_SELECT_0
#undef ELSIE_PP_SELECT_1
#ifdef ELSIE_SIMD_MODINT_MATRIX_MODE
#define MODE ELSIE_SIMD_MODINT_MATRIX_MODE
#undef ELSIE_SIMD_MODINT_MATRIX_MODE
#endif
#ifdef ELSIE_SIMD_MODINT_MATRIX_SELECT
#define select ELSIE_SIMD_MODINT_MATRIX_SELECT
#undef ELSIE_SIMD_MODINT_MATRIX_SELECT
#endif

#endif // defined(MODE)

} // namespace elsie

#endif // ELSIE_SIMD_MATRIX_MODINT_HPP