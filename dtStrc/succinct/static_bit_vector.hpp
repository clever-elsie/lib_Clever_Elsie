#ifndef ELSIE_STATIC_BIT_VECTOR
#define ELSIE_STATIC_BIT_VECTOR
#include <cstddef>
#include <cstdint>
#include <vector>
#include <concepts>
#include <type_traits>
#include <math/basic_math.hpp>
namespace elsie{
using namespace std;

template<integral int_t>
class static_bit_vector{
	using u8=uint8_t;
	using u16=uint16_t;
	using u32=uint32_t;
	using u64=uint64_t;
	constexpr static u16 lbsize=255,sbsize=15;
	size_t c_size;
	vector<u64>bit;
	vector<u8>lb,sb;
	public:
	static_bit_vector():bit(0),lb(0),sb(0),c_size(0){}
	static_bit_vector(size_t N,int_t init_v)
	 :c_size(0),bit(N,init_v),
	 lb(ceil(N,lbsize)),sb(ceil(N,sbsize))
	{}
};
}// namespace elsie
#endif // include guard