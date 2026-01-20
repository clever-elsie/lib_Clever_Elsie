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

using mmint_t = elsie::montgomery_modint<998244353>;
using mmat_t  = elsie::matrix<mmint_t>;

#ifdef __AVX512F__
// -----------------------------------------------------------------------------
// AVX512 実装 (8 lane, 512bit)
// ブロック分割 64x64x64。Montgomery 乗算:
//   t = (x * M') mod 2^32
//   u = (x + t * M) / 2^32
//   if u >= M then u -= M
// ここで x = A[i][k] * B[k][j] (64bit)。この u を C[i][j] に加算し、再度 M で調整。
// -----------------------------------------------------------------------------
template<>
template<>
__attribute__((target("avx512f")))
__attribute__((optimize("O3")))
inline mmat_t& mmat_t::fma(const mmat_t& a, const mmat_t& b) {
  [[assume(dim().row == a.dim().row)]];
  [[assume(dim().col == b.dim().col)]];
  [[assume(a.dim().col == b.dim().row)]];

  constexpr size_t BLOCK_SIZE = 64;
  const auto [row, col] = dim();
  const size_t midCR = a.dim().col;
  
  size_t I, J, K, i, j, k, it, jt, kt;
  constexpr static size_t M = mmint_t::mod();
  const __m512i Mv = _mm512_set1_epi64(M);
  const __m512i mv = _mm512_set1_epi64(mmint_t::Mdash());
  const __m256i M32 = _mm256_set1_epi32(M); // 加算後の再正規化用

  for (I = 0, it = std::min(BLOCK_SIZE, row); I < row; I += BLOCK_SIZE, it = std::min(it + BLOCK_SIZE, row)) {
    for (J = 0, jt = std::min(BLOCK_SIZE, col); J < col; J += BLOCK_SIZE, jt = std::min(jt + BLOCK_SIZE, col)) {
      for (K = 0, kt = std::min(BLOCK_SIZE, midCR); K < midCR; K += BLOCK_SIZE, kt = std::min(kt + BLOCK_SIZE, midCR)) {
        for (i = I; i < it; ++i) {
          const auto as = a[i];
          auto cs = (*this)[i];
          
          for (k = K; k < kt; ++k) {
            const auto& ak = as[k];
            const auto b_row = b[k];

            uint32_t ak_raw = *reinterpret_cast<const uint32_t*>(&ak);
            constexpr static size_t simd = 8; // 8 要素同時処理
            __m512i a_vec = _mm512_set1_epi32(ak_raw);
            __m512i a64 = _mm512_cvtepu32_epi64(_mm512_castsi512_si256(a_vec));

            for (j = J; j + simd - 1 < jt; j += simd) {
              __m256i b32 = _mm256_loadu_si256((__m256i*)&b_row[j]);
              __m256i c32 = _mm256_loadu_si256((__m256i*)&cs[j]);

              __m512i b64 = _mm512_cvtepu32_epi64(b32);

              // x = a*b (64bit)
              __m512i prod = _mm512_mul_epu32(a64, b64);
              // t = (x * M') mod 2^32
              __m512i t = _mm512_mul_epu32(prod, mv);
              // u = (x + t*M) / 2^32
              __m512i u = _mm512_add_epi64(prod, _mm512_mul_epu32(t, Mv));
              u = _mm512_srli_epi64(u, 32);
              // if u >= M then u -= M
              __mmask8 ge = _mm512_cmpge_epu64_mask(u, Mv);
              u = _mm512_mask_sub_epi64(u, ge, u, Mv);

              // c += u
              __m256i u32_ = _mm512_cvtepi64_epi32(u);
              c32 = _mm256_add_epi32(c32, u32_);
              // if c >= M then c -= M
              __mmask8 ge2 = _mm256_cmpge_epu32_mask(c32, M32);
              c32 = _mm256_mask_sub_epi32(c32, ge2, c32, M32);

              _mm256_storeu_si256((__m256i*)&cs[j], c32);
            }
            for (; j < jt; ++j) cs[j] += ak * b_row[j];
          }
        }
      }
    }
  }
  return *this;
}
#elif defined(__AVX2__)
// -----------------------------------------------------------------------------
// AVX2 実装 (4 lane, 256bit)
// 4 要素ずつ:
//   x = A[i][k] * B[k][j] (64bit)
//   t = (x * M') mod 2^32
//   u = (x + t * M) / 2^32
//   if u >= M then u -= M
//   C += u; if C >= M then C -= M
// 先に uint32 を uint64 に拡張してから Montgomery を行い、加算後も 32bit で正規化。
// -----------------------------------------------------------------------------
#include <immintrin.h>
#include <algorithm>
#include <cstdint>

template<>
template<>
__attribute__((target("avx2")))
__attribute__((optimize("O3")))
inline mmat_t& mmat_t::fma(const mmat_t& a, const mmat_t& b) {
  [[assume(dim().row == a.dim().row)]];
  [[assume(dim().col == b.dim().col)]];
  [[assume(a.dim().col == b.dim().row)]];

  constexpr size_t BLOCK_SIZE = 64;
  const auto [row, col] = dim();
  const size_t midCR = a.dim().col;
  
  size_t I, J, K, i, j, k, it, jt, kt;
  constexpr static size_t M = mmint_t::mod();

  constexpr static size_t simd = 4; // 4 要素同時
  const __m256i Mv64 = _mm256_set1_epi64x(M);
  const __m256i mDash64 = _mm256_set1_epi64x(mmint_t::Mdash());
  const __m256i Mminus1_64 = _mm256_set1_epi64x(M - 1);
  const __m128i Mv32 = _mm_set1_epi32(M);
  const __m128i Mminus1_32 = _mm_set1_epi32(M - 1);

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
            __m128i a32 = _mm_set1_epi32(ak_raw);
            __m256i a64 = _mm256_cvtepu32_epi64(a32);

            for (j = J; j + simd - 1 < jt; j += simd) {
              __m128i b32 = _mm_loadu_si128((__m128i*)&b_row[j]);
              __m128i c32 = _mm_loadu_si128((__m128i*)&cs[j]);

              __m256i b64 = _mm256_cvtepu32_epi64(b32);
              __m256i prod = _mm256_mul_epu32(a64, b64);      // x

              __m256i t = _mm256_mul_epu32(prod, mDash64);    // t = x * M'
              __m256i u = _mm256_add_epi64(prod, _mm256_mul_epu32(t, Mv64));
              u = _mm256_srli_epi64(u, 32);                   // /2^32

              __m256i mask_u = _mm256_cmpgt_epi64(u, Mminus1_64);
              u = _mm256_sub_epi64(u, _mm256_and_si256(mask_u, Mv64)); // u -= M if needed

              __m128i u32 = _mm256_cvtepi64_epi32(u);
              c32 = _mm_add_epi32(c32, u32);                  // C += u

              __m128i mask_c = _mm_cmpgt_epi32(c32, Mminus1_32);
              c32 = _mm_sub_epi32(c32, _mm_and_si128(mask_c, Mv32)); // C -= M if needed

              _mm_storeu_si128((__m128i*)&cs[j], c32);
            }
            for (; j < jt; ++j) cs[j] += ak * b_row[j];
          }
        }
      }
    }
  }
  return *this;
}
#endif

} // namespace elsie
