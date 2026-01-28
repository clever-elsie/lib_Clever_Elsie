// Copyright (c) Elsie
// AVX512 / AVX2 (4-lane) 専用の行列 FMA 実装
// - 対象: elsie::matrix<montgomery_modint<MOD>>
// - ここでは 998244353 を例示しているが、mod()/Mdash() を持つ Montgomery 型なら同様に拡張可能
#pragma once

#include <immintrin.h>
#include <algorithm>
#include <cstdint>
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

// -----------------------------------------------------------------------------
// AVX512/AVX2 混在実装（`select` で 1 本化）
// - MODE==1: AVX512F で 8 要素同時 (u64 は 8lane / u32 は 8lane)
// - MODE==0: AVX2    で 4 要素同時 (u64 は 4lane / u32 は 4lane)
// Montgomery:
//   x = a*b (u64), t = (x*M') mod 2^32, u = (x + t*M) >> 32, if u>=M u-=M
//   C += u, if C>=M C-=M
// -----------------------------------------------------------------------------
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

  constexpr static size_t simd = select(8, 4);

  using v64_t = select(__m512i, __m256i);
  using v32_t = select(__m256i, __m128i);

#define ELSIE_VSET1_EPI32(x) select(_mm256_set1_epi32((x)), _mm_set1_epi32((x)))
#define ELSIE_VSET1_EPI64(x) select(_mm512_set1_epi64((x)), _mm256_set1_epi64x((x)))

#define ELSIE_VLOADU_EPI32(ptr) select(_mm256_loadu_si256((const __m256i*)(ptr)), _mm_loadu_si128((const __m128i*)(ptr)))
#define ELSIE_VSTOREU_EPI32(ptr, v) select(_mm256_storeu_si256((__m256i*)(ptr), (v)), _mm_storeu_si128((__m128i*)(ptr), (v)))

#define ELSIE_CVTEPU32_EPI64(v32) select(_mm512_cvtepu32_epi64((v32)), _mm256_cvtepu32_epi64((v32)))
#define ELSIE_MUL_EPU32(v64a, v64b) select(_mm512_mul_epu32((v64a), (v64b)), _mm256_mul_epu32((v64a), (v64b)))
#define ELSIE_ADD_EPI64(v64a, v64b) select(_mm512_add_epi64((v64a), (v64b)), _mm256_add_epi64((v64a), (v64b)))
#define ELSIE_SRLI_EPI64_32(v64) select(_mm512_srli_epi64((v64), 32), _mm256_srli_epi64((v64), 32))

#define ELSIE_CVT_EPI64_TO_EPI32(v64) select(_mm512_cvtepi64_epi32((v64)), simd_modint_matrix_detail::cvt_epi64_to_epi32_avx2((v64)))
#define ELSIE_ADD_EPI32(v32a, v32b) select(_mm256_add_epi32((v32a), (v32b)), _mm_add_epi32((v32a), (v32b)))

  const v64_t Mv64 = ELSIE_VSET1_EPI64(M);
  const v64_t mDash64 = ELSIE_VSET1_EPI64(mmint_t::Mdash());
  const v64_t Mminus1_64 = ELSIE_VSET1_EPI64(M - 1); // AVX2 reduce 用
  const v32_t Mv32 = ELSIE_VSET1_EPI32(M);
  const v32_t Mminus1_32 = ELSIE_VSET1_EPI32(M - 1);

  for (I = 0, it = std::min(BLOCK_SIZE, row); I < row; I += BLOCK_SIZE, it = std::min(it + BLOCK_SIZE, row)) {
    for (K = 0, kt = std::min(BLOCK_SIZE, midCR); K < midCR; K += BLOCK_SIZE, kt = std::min(kt + BLOCK_SIZE, midCR)) {
      for (J = 0, jt = std::min(BLOCK_SIZE, col); J < col; J += BLOCK_SIZE, jt = std::min(jt + BLOCK_SIZE, col)) {
        for (i = I; i < it; ++i) {
          const auto as = a[i];
          auto cs = (*this)[i];
          
          for (k = K; k < kt; ++k) {
            const auto& ak = as[k];
            const auto b_row = b[k];
            
            uint32_t ak_raw = *reinterpret_cast<const uint32_t*>(&ak);
            const v32_t a32 = ELSIE_VSET1_EPI32(ak_raw);
            const v64_t a64 = ELSIE_CVTEPU32_EPI64(a32);

            for (j = J; j + simd - 1 < jt; j += simd) {
              v32_t b32 = ELSIE_VLOADU_EPI32(&b_row[j]);
              v32_t c32 = ELSIE_VLOADU_EPI32(&cs[j]);

              const v64_t b64 = ELSIE_CVTEPU32_EPI64(b32);
              const v64_t prod = ELSIE_MUL_EPU32(a64, b64); // x = a*b

              const v64_t t = ELSIE_MUL_EPU32(prod, mDash64); // t = x * M'
              v64_t u = ELSIE_ADD_EPI64(prod, ELSIE_MUL_EPU32(t, Mv64));
              u = ELSIE_SRLI_EPI64_32(u); // u = (x + t*M) >> 32

              // u の reduce
              select(
                do {
                  // u, M は 2^32 未満なので signed compare で十分
                  __mmask8 ge = _mm512_cmpgt_epi64_mask(u, Mminus1_64);
                  u = _mm512_mask_sub_epi64(u, ge, u, Mv64);
                } while (0),
                do {
                  __m256i mask_u = _mm256_cmpgt_epi64(u, Mminus1_64);
                  u = _mm256_sub_epi64(u, _mm256_and_si256(mask_u, Mv64));
                } while (0)
              );

              // c += u (u64 -> u32)
              const v32_t u32 = ELSIE_CVT_EPI64_TO_EPI32(u);
              c32 = ELSIE_ADD_EPI32(c32, u32);

              // c の reduce (M < 2^31 かつ c < 2M なので signed compare で安全)
              select(
                do {
                  __m256i mask_c = _mm256_cmpgt_epi32(c32, Mminus1_32);
                  c32 = _mm256_sub_epi32(c32, _mm256_and_si256(mask_c, Mv32));
                } while (0),
                do {
                  __m128i mask_c = _mm_cmpgt_epi32(c32, Mminus1_32);
                  c32 = _mm_sub_epi32(c32, _mm_and_si128(mask_c, Mv32));
                } while (0)
              );

              ELSIE_VSTOREU_EPI32(&cs[j], c32);
            }
            for (; j < jt; ++j) cs[j] += ak * b_row[j];
          }
        }
      }
    }
  }

#undef ELSIE_VSET1_EPI32
#undef ELSIE_VSET1_EPI64
#undef ELSIE_VLOADU_EPI32
#undef ELSIE_VSTOREU_EPI32
#undef ELSIE_CVTEPU32_EPI64
#undef ELSIE_MUL_EPU32
#undef ELSIE_ADD_EPI64
#undef ELSIE_SRLI_EPI64_32
#undef ELSIE_CVT_EPI64_TO_EPI32
#undef ELSIE_ADD_EPI32

  return *this;
}

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
