// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp
// vn-incl/i_to_str.hpp

#pragma once

#include <vn-incl/tables.hpp>

namespace vn {

	VN_FORCE_INLINE static constexpr uint64_t mulhi64_portable(uint64_t a, uint64_t b) noexcept {
		const uint64_t a_lo	 = a & 0xFFFFFFFFULL;
		const uint64_t a_hi	 = a >> 32ULL;
		const uint64_t b_lo	 = b & 0xFFFFFFFFULL;
		const uint64_t b_hi	 = b >> 32ULL;
		const uint64_t lo_lo = a_lo * b_lo;
		const uint64_t hi_lo = a_hi * b_lo;
		const uint64_t lo_hi = a_lo * b_hi;
		const uint64_t hi_hi = a_hi * b_hi;
		const uint64_t cross = (lo_lo >> 32ULL) + (hi_lo & 0xFFFFFFFFULL) + lo_hi;
		return hi_hi + (hi_lo >> 32ULL) + (cross >> 32ULL);
	}

	template<typename v_type, uint64_t divisor> struct multiply_and_shift;

	template<uint32_types v_type, uint64_t divisor> struct multiply_and_shift<v_type, divisor> {
		static constexpr auto& entry = mul_shift_table<v_type>::values[divisor_to_index<divisor>()];
		VN_FORCE_INLINE static v_type impl(v_type value) noexcept {
			static constexpr v_type m = entry.multiplicand;
			static constexpr uint64_t s	  = entry.shift;
			return static_cast<uint32_t>((static_cast<uint64_t>(value) * m) >> s);
		}
	};

	template<uint64_types v_type, uint64_t divisor>
		requires(divisor < 100000000ULL)
	struct multiply_and_shift<v_type, divisor> {
		static constexpr auto& entry = mul_shift_table<uint32_t>::values[divisor_to_index<divisor>()];
		static constexpr uint32_t m	 = entry.multiplicand;
		static constexpr uint64_t s	 = entry.shift;
		VN_FORCE_INLINE static v_type impl(v_type value) noexcept {
			return static_cast<v_type>(static_cast<uint32_t>((static_cast<uint64_t>(static_cast<uint32_t>(value)) * m) >> s));
		}
	};

	template<uint64_types v_type, uint64_t divisor>
		requires(divisor >= 100000000ULL)
	struct multiply_and_shift<v_type, divisor> {
		static constexpr auto& entry  = mul_shift_table<v_type>::values[divisor_to_index<divisor>()];
		static constexpr v_type m = entry.multiplicand;
		static constexpr uint64_t s	  = entry.shift;
		static_assert(s >= 64ULL);
		VN_FORCE_INLINE static v_type impl(v_type value) noexcept {
#if VN_COMPILER_CLANG || VN_COMPILER_GCC
			return static_cast<v_type>(static_cast<__uint128_t>(value) * m >> s);
#elif VN_COMPILER_MSVC
			v_type high_part;
			_umul128(m, value, &high_part);
			return static_cast<v_type>(high_part >> (s - 64ULL));
#else
			return static_cast<v_type>(mulhi64_portable(value, m) >> (s - 64ULL));
#endif
		}
	};

	template<typename v_type, uint64_t digit_length> struct to_chars_internal;

	template<uint32_types v_type> struct to_chars_internal<v_type, 2ULL> {
		VN_FORCE_INLINE static char* impl(char* buf, const v_type value) noexcept {
			VN_ALIGN(64) static constexpr const auto* char_table_01_data = int_tables::char_table_01.data();
			const uint32_t lz											 = value < 10U;
			std::memcpy(buf, char_table_01_data + (value * 2U + lz), 2ULL);
			buf -= lz;
			return buf + 2ULL;
		}
	};

	template<uint32_types v_type> struct to_chars_internal<v_type, 4ULL> {
		VN_FORCE_INLINE static char* impl(char* buf, const v_type value) noexcept {
			VN_ALIGN(64) static constexpr const auto* char_table_01_data = int_tables::char_table_01.data();
			VN_ALIGN(64) static constexpr const auto* char_table_02_data = int_tables::char_table_02.data();
			const uint32_t aa											 = multiply_and_shift<v_type, 100ULL>::impl(value);
			const uint32_t lz											 = value < 1000U;
			std::memcpy(buf, char_table_01_data + (aa * 2U + lz), 2ULL);
			buf -= lz;
			std::memcpy(buf + 2ULL, char_table_02_data + (value - aa * 100U), 2ULL);
			return buf + 4ULL;
		}
	};

	template<uint32_types v_type> struct to_chars_internal<v_type, 6ULL> {
		VN_FORCE_INLINE static char* impl(char* buf, const v_type value) noexcept {
			VN_ALIGN(64) static constexpr const auto* char_table_01_data = int_tables::char_table_01.data();
#if !VN_COMPILER_MSVC
			VN_ALIGN(64) static constexpr const auto* char_table_04_data = int_tables::char_table_04.data();
			const uint32_t aa											 = multiply_and_shift<v_type, 10000ULL>::impl(value);
			const uint32_t lz											 = value < 100000U;
			const uint32_t remainder									 = value - aa * 10000U;
			std::memcpy(buf, char_table_01_data + (aa * 2U + lz), 2ULL);
			buf -= lz;
			std::memcpy(buf + 2ULL, char_table_04_data + remainder, 4ULL);
#else
			VN_ALIGN(64) static constexpr const auto* char_table_02_data = int_tables::char_table_02.data();
			const uint32_t aa											 = multiply_and_shift<v_type, 10000ULL>::impl(value);
			const uint32_t bbcc											 = value - aa * 10000U;
			const uint32_t bb											 = multiply_and_shift<v_type, 100ULL>::impl(bbcc);
			const uint32_t cc											 = bbcc - bb * 100U;
			const uint32_t lz											 = aa < 10U;
			std::memcpy(buf, char_table_01_data + (aa * 2U + lz), 2ULL);
			buf -= lz;
			std::memcpy(buf + 2ULL, char_table_02_data + bb, 2ULL);
			std::memcpy(buf + 4ULL, char_table_02_data + cc, 2ULL);
#endif
			return buf + 6ULL;
		}
	};

	template<uint32_types v_type> struct to_chars_internal<v_type, 8ULL> {
		VN_FORCE_INLINE static char* impl(char* buf, const v_type value) noexcept {
			VN_ALIGN(64) static constexpr const auto* char_table_01_data = int_tables::char_table_01.data();
			const uint32_t aabb											 = multiply_and_shift<v_type, 10000ULL>::impl(value);
			const uint32_t aa											 = multiply_and_shift<v_type, 100ULL>::impl(aabb);
			const uint32_t lz											 = value < 10000000U;
			std::memcpy(buf, char_table_01_data + (aa * 2U + lz), 2ULL);
			buf -= lz;
#if !VN_COMPILER_MSVC
			VN_ALIGN(64) static constexpr const auto* char_table_02_data = int_tables::char_table_02.data();
			VN_ALIGN(64) static constexpr const auto* char_table_04_data = int_tables::char_table_04.data();
			const uint32_t ccdd											 = value - aabb * 10000U;
			std::memcpy(buf + 2ULL, char_table_02_data + (aabb - aa * 100U), 2ULL);
			std::memcpy(buf + 4ULL, char_table_04_data + ccdd, 4ULL);
#else
			VN_ALIGN(64) static constexpr const auto* char_table_02_data = int_tables::char_table_02.data();
			const uint32_t ccdd											 = value - aabb * 10000U;
			const uint32_t cc											 = multiply_and_shift<v_type, 100ULL>::impl(ccdd);
			const uint32_t bb											 = aabb - aa * 100U;
			const uint32_t dd											 = ccdd - cc * 100U;
			std::memcpy(buf + 2ULL, char_table_02_data + bb, 2ULL);
			std::memcpy(buf + 4ULL, char_table_02_data + cc, 2ULL);
			std::memcpy(buf + 6ULL, char_table_02_data + dd, 2ULL);
#endif
			return buf + 8ULL;
		}
	};

	template<uint32_types v_type> struct to_chars_internal<v_type, 10ULL> {
		VN_FORCE_INLINE static char* impl(char* buf, const v_type value) noexcept {
			VN_ALIGN(64) static constexpr const auto* char_table_01_data = int_tables::char_table_01.data();
#if !VN_COMPILER_MSVC
			VN_ALIGN(64) static constexpr const auto* char_table_04_data = int_tables::char_table_04.data();
			const uint32_t high											 = multiply_and_shift<v_type, 100000000ULL>::impl(value);
			const uint32_t low											 = value - high * 100000000U;
			const uint32_t lz											 = high < 10U;
			const uint32_t aabb											 = multiply_and_shift<v_type, 10000ULL>::impl(low);
			const uint32_t ccdd											 = low - aabb * 10000U;
			std::memcpy(buf, char_table_01_data + (high * 2U + lz), 2ULL);
			buf -= lz;
			std::memcpy(buf + 2ULL, char_table_04_data + aabb, 4ULL);
			std::memcpy(buf + 6ULL, char_table_04_data + ccdd, 4ULL);
#else
			VN_ALIGN(64) static constexpr const auto* char_table_02_data = int_tables::char_table_02.data();
			const uint32_t aabbcc										 = multiply_and_shift<v_type, 10000ULL>::impl(value);
			const uint32_t aa											 = multiply_and_shift<v_type, 10000ULL>::impl(aabbcc);
			const uint32_t ddee											 = value - aabbcc * 10000U;
			const uint32_t bbcc											 = aabbcc - aa * 10000U;
			const uint32_t bb											 = multiply_and_shift<v_type, 100ULL>::impl(bbcc);
			const uint32_t dd											 = multiply_and_shift<v_type, 100ULL>::impl(ddee);
			const uint32_t cc											 = bbcc - bb * 100U;
			const uint32_t ee											 = ddee - dd * 100U;
			const uint32_t lz											 = aa < 10U;
			std::memcpy(buf, char_table_01_data + (aa * 2U + lz), 2ULL);
			buf -= lz;
			std::memcpy(buf + 2ULL, char_table_02_data + bb, 2ULL);
			std::memcpy(buf + 4ULL, char_table_02_data + cc, 2ULL);
			std::memcpy(buf + 6ULL, char_table_02_data + dd, 2ULL);
			std::memcpy(buf + 8ULL, char_table_02_data + ee, 2ULL);
#endif
			return buf + 10ULL;
		}
	};

	template<uint64_types v_type> struct to_chars_internal<v_type, 2ULL> {
		VN_FORCE_INLINE static char* impl(char* buf, const v_type value) noexcept {
			VN_ALIGN(64) static constexpr const auto* char_table_01_data = int_tables::char_table_01.data();
			const uint64_t lz											 = value < 10ULL;
			std::memcpy(buf, char_table_01_data + (value * 2ULL + lz), 2ULL);
			buf -= lz;
			return buf + 2ULL;
		}
	};

	template<uint64_types v_type> struct to_chars_internal<v_type, 4ULL> {
		VN_FORCE_INLINE static char* impl(char* buf, const v_type value) noexcept {
			VN_ALIGN(64) static constexpr const auto* char_table_01_data = int_tables::char_table_01.data();
			VN_ALIGN(64) static constexpr const auto* char_table_02_data = int_tables::char_table_02.data();
			const uint64_t aa											 = multiply_and_shift<v_type, 100ULL>::impl(value);
			const uint64_t lz											 = value < 1000ULL;
			std::memcpy(buf, char_table_01_data + (aa * 2ULL + lz), 2ULL);
			buf -= lz;
			std::memcpy(buf + 2ULL, char_table_02_data + (value - aa * 100ULL), 2ULL);
			return buf + 4ULL;
		}
	};

	template<uint64_types v_type> struct to_chars_internal<v_type, 6ULL> {
		VN_FORCE_INLINE static char* impl(char* buf, const v_type value) noexcept {
			VN_ALIGN(64) static constexpr const auto* char_table_01_data = int_tables::char_table_01.data();
			VN_ALIGN(64) static constexpr const auto* char_table_04_data = int_tables::char_table_04.data();
			const uint64_t aa											 = multiply_and_shift<v_type, 10000ULL>::impl(value);
			const uint64_t lz											 = value < 100000ULL;
			const uint64_t remainder									 = value - aa * 10000ULL;
			std::memcpy(buf, char_table_01_data + (aa * 2ULL + lz), 2ULL);
			buf -= lz;
			std::memcpy(buf + 2ULL, char_table_04_data + remainder, 4ULL);
			return buf + 6ULL;
		}
	};

	template<uint64_types v_type> struct to_chars_internal<v_type, 8ULL> {
		VN_FORCE_INLINE static char* impl(char* buf, const v_type value) noexcept {
			VN_ALIGN(64) static constexpr const auto* char_table_01_data = int_tables::char_table_01.data();
			VN_ALIGN(64) static constexpr const auto* char_table_02_data = int_tables::char_table_02.data();
			VN_ALIGN(64) static constexpr const auto* char_table_04_data = int_tables::char_table_04.data();
			const uint64_t aabb											 = multiply_and_shift<v_type, 10000ULL>::impl(value);
			const uint64_t aa											 = multiply_and_shift<v_type, 100ULL>::impl(aabb);
			const uint64_t lz											 = value < 10000000ULL;
			const uint64_t ccdd											 = value - aabb * 10000ULL;
			std::memcpy(buf, char_table_01_data + (aa * 2ULL + lz), 2ULL);
			buf -= lz;
			std::memcpy(buf + 2ULL, char_table_02_data + (aabb - aa * 100ULL), 2ULL);
			std::memcpy(buf + 4ULL, char_table_04_data + ccdd, 4ULL);
			return buf + 8ULL;
		}
	};

	template<uint64_types v_type> struct to_chars_internal<v_type, 10ULL> {
		VN_FORCE_INLINE static char* impl(char* buf, const v_type value) noexcept {
			VN_ALIGN(64) static constexpr const auto* char_table_01_data = int_tables::char_table_01.data();
			VN_ALIGN(64) static constexpr const auto* char_table_04_data = int_tables::char_table_04.data();
			const uint64_t high											 = multiply_and_shift<v_type, 100000000ULL>::impl(value);
			const uint64_t low											 = value - high * 100000000ULL;
			const uint64_t lz											 = high < 10ULL;
			const uint64_t aabb											 = multiply_and_shift<v_type, 10000ULL>::impl(low);
			const uint64_t ccdd											 = low - aabb * 10000ULL;
			std::memcpy(buf, char_table_01_data + (high * 2ULL + lz), 2ULL);
			buf -= lz;
			std::memcpy(buf + 2ULL, char_table_04_data + aabb, 4ULL);
			std::memcpy(buf + 6ULL, char_table_04_data + ccdd, 4ULL);
			return buf + 10ULL;
		}
	};

	template<uint64_types v_type> struct to_chars_internal<v_type, 12ULL> {
		VN_FORCE_INLINE static char* impl(char* buf, const v_type value) noexcept {
			VN_ALIGN(64) static constexpr const auto* char_table_01_data = int_tables::char_table_01.data();
			VN_ALIGN(64) static constexpr const auto* char_table_02_data = int_tables::char_table_02.data();
			VN_ALIGN(64) static constexpr const auto* char_table_04_data = int_tables::char_table_04.data();
			const uint64_t high											 = multiply_and_shift<v_type, 100000000ULL>::impl(value);
			const uint64_t low											 = value - high * 100000000ULL;
			const uint64_t aa											 = multiply_and_shift<v_type, 100ULL>::impl(high);
			const uint64_t lz											 = aa < 10ULL;
			const uint64_t aabb											 = multiply_and_shift<v_type, 10000ULL>::impl(low);
			const uint64_t ccdd											 = low - aabb * 10000ULL;
			std::memcpy(buf, char_table_01_data + (aa * 2ULL + lz), 2ULL);
			buf -= lz;
			std::memcpy(buf + 2ULL, char_table_02_data + (high - aa * 100ULL), 2ULL);
			std::memcpy(buf + 4ULL, char_table_04_data + aabb, 4ULL);
			std::memcpy(buf + 8ULL, char_table_04_data + ccdd, 4ULL);
			return buf + 12ULL;
		}
	};

	template<uint64_types v_type> struct to_chars_internal<v_type, 14ULL> {
		VN_FORCE_INLINE static char* impl(char* buf, const v_type value) noexcept {
			VN_ALIGN(64) static constexpr const auto* char_table_01_data = int_tables::char_table_01.data();
			VN_ALIGN(64) static constexpr const auto* char_table_04_data = int_tables::char_table_04.data();
			const uint64_t high											 = multiply_and_shift<v_type, 100000000ULL>::impl(value);
			const uint64_t low											 = value - high * 100000000ULL;
			const uint64_t aa											 = multiply_and_shift<v_type, 10000ULL>::impl(high);
			const uint64_t lz											 = aa < 10ULL;
			const uint64_t bbcc											 = high - aa * 10000ULL;
			const uint64_t aabb											 = multiply_and_shift<v_type, 10000ULL>::impl(low);
			const uint64_t ccdd											 = low - aabb * 10000ULL;
			std::memcpy(buf, char_table_01_data + (aa * 2ULL + lz), 2ULL);
			buf -= lz;
			std::memcpy(buf + 2ULL, char_table_04_data + bbcc, 4ULL);
			std::memcpy(buf + 6ULL, char_table_04_data + aabb, 4ULL);
			std::memcpy(buf + 10ULL, char_table_04_data + ccdd, 4ULL);
			return buf + 14ULL;
		}
	};

	template<uint64_types v_type> struct to_chars_internal<v_type, 16ULL> {
		VN_FORCE_INLINE static char* impl(char* buf, const v_type value) noexcept {
			VN_ALIGN(64) static constexpr const auto* char_table_01_data = int_tables::char_table_01.data();
			VN_ALIGN(64) static constexpr const auto* char_table_02_data = int_tables::char_table_02.data();
			VN_ALIGN(64) static constexpr const auto* char_table_04_data = int_tables::char_table_04.data();
			const uint64_t high											 = multiply_and_shift<v_type, 100000000ULL>::impl(value);
			const uint64_t low											 = value - high * 100000000ULL;
			const uint64_t aabb											 = multiply_and_shift<v_type, 10000ULL>::impl(high);
			const uint64_t ccdd											 = high - aabb * 10000ULL;
			const uint64_t aa											 = multiply_and_shift<v_type, 100ULL>::impl(aabb);
			const uint64_t lz											 = aa < 10ULL;
			const uint64_t bb											 = aabb - aa * 100ULL;
			const uint64_t eeff											 = multiply_and_shift<v_type, 10000ULL>::impl(low);
			const uint64_t gghh											 = low - eeff * 10000ULL;
			std::memcpy(buf, char_table_01_data + (aa * 2ULL + lz), 2ULL);
			buf -= lz;
			std::memcpy(buf + 2ULL, char_table_02_data + bb, 2ULL);
			std::memcpy(buf + 4ULL, char_table_04_data + ccdd, 4ULL);
			std::memcpy(buf + 8ULL, char_table_04_data + eeff, 4ULL);
			std::memcpy(buf + 12ULL, char_table_04_data + gghh, 4ULL);
			return buf + 16ULL;
		}
	};

	template<uint64_types v_type> struct to_chars_internal<v_type, 18ULL> {
		VN_FORCE_INLINE static char* impl(char* buf, const v_type value) noexcept {
			VN_ALIGN(64) static constexpr const auto* char_table_01_data = int_tables::char_table_01.data();
			VN_ALIGN(64) static constexpr const auto* char_table_04_data = int_tables::char_table_04.data();
			const uint64_t high											 = multiply_and_shift<v_type, 100000000ULL>::impl(value);
			const uint64_t low											 = value - high * 100000000ULL;
			const uint64_t high10										 = multiply_and_shift<v_type, 100000000ULL>::impl(high);
			const uint64_t low10										 = high - high10 * 100000000ULL;
			const uint64_t lz											 = high10 < 10ULL;
			const uint64_t aabb											 = multiply_and_shift<v_type, 10000ULL>::impl(low10);
			const uint64_t ccdd											 = low10 - aabb * 10000ULL;
			const uint64_t eeff											 = multiply_and_shift<v_type, 10000ULL>::impl(low);
			const uint64_t gghh											 = low - eeff * 10000ULL;
			std::memcpy(buf, char_table_01_data + (high10 * 2ULL + lz), 2ULL);
			buf -= lz;
			std::memcpy(buf + 2ULL, char_table_04_data + aabb, 4ULL);
			std::memcpy(buf + 6ULL, char_table_04_data + ccdd, 4ULL);
			std::memcpy(buf + 10ULL, char_table_04_data + eeff, 4ULL);
			std::memcpy(buf + 14ULL, char_table_04_data + gghh, 4ULL);
			return buf + 18ULL;
		}
	};

	template<uint64_types v_type> struct to_chars_internal<v_type, 20ULL> {
		VN_FORCE_INLINE static char* impl(char* buf, const v_type value) noexcept {
			VN_ALIGN(64) static constexpr const auto* char_table_01_data = int_tables::char_table_01.data();
			VN_ALIGN(64) static constexpr const auto* char_table_02_data = int_tables::char_table_02.data();
			VN_ALIGN(64) static constexpr const auto* char_table_04_data = int_tables::char_table_04.data();
			const uint64_t high											 = multiply_and_shift<v_type, 100000000ULL>::impl(value);
			const uint64_t low											 = value - high * 100000000ULL;
			const uint64_t high12										 = multiply_and_shift<v_type, 100000000ULL>::impl(high);
			const uint64_t low12										 = high - high12 * 100000000ULL;
			const uint64_t aa											 = multiply_and_shift<v_type, 100ULL>::impl(high12);
			const uint64_t lz											 = aa < 10ULL;
			const uint64_t aabb											 = multiply_and_shift<v_type, 10000ULL>::impl(low12);
			const uint64_t ccdd											 = low12 - aabb * 10000ULL;
			const uint64_t eeff											 = multiply_and_shift<v_type, 10000ULL>::impl(low);
			const uint64_t gghh											 = low - eeff * 10000ULL;
			std::memcpy(buf, char_table_01_data + (aa * 2ULL + lz), 2ULL);
			buf -= lz;
			std::memcpy(buf + 2ULL, char_table_02_data + (high12 - aa * 100ULL), 2ULL);
			std::memcpy(buf + 4ULL, char_table_04_data + aabb, 4ULL);
			std::memcpy(buf + 8ULL, char_table_04_data + ccdd, 4ULL);
			std::memcpy(buf + 12ULL, char_table_04_data + eeff, 4ULL);
			std::memcpy(buf + 16ULL, char_table_04_data + gghh, 4ULL);
			return buf + 20ULL;
		}
	};

	template<typename v_type> struct to_chars_impl;

	template<uint64_types v_type> struct to_chars_impl<v_type> {
		VN_FORCE_INLINE static char* impl(char* buf, const v_type value) noexcept {
			if (value < 10000ULL) {
				if (value < 100ULL) {
					return to_chars_internal<v_type, 2ULL>::impl(buf, value);
				} else {
					return to_chars_internal<v_type, 4ULL>::impl(buf, value);
				}
			} else if (value < 100000000ULL) {
				if (value < 1000000ULL) {
					return to_chars_internal<v_type, 6ULL>::impl(buf, value);
				} else {
					return to_chars_internal<v_type, 8ULL>::impl(buf, value);
				}
			} else if (value < 1000000000000ULL) {
				if (value < 10000000000ULL) {
					return to_chars_internal<v_type, 10ULL>::impl(buf, value);
				} else {
					return to_chars_internal<v_type, 12ULL>::impl(buf, value);
				}
			} else if (value < 10000000000000000ULL) {
				if (value < 100000000000000ULL) {
					return to_chars_internal<v_type, 14ULL>::impl(buf, value);
				} else {
					return to_chars_internal<v_type, 16ULL>::impl(buf, value);
				}
			} else if (value < 1000000000000000000ULL) {
				return to_chars_internal<v_type, 18ULL>::impl(buf, value);
			} else {
				return to_chars_internal<v_type, 20ULL>::impl(buf, value);
			}
		}
	};

	template<int64_types v_type> struct to_chars_impl<v_type> {
		VN_FORCE_INLINE static char* impl(char* buf, const v_type value) noexcept {
			using unsigned_type					 = std::make_unsigned_t<v_type>;
			constexpr unsigned_type shift_amount = sizeof(v_type) * 8ULL - 1ULL;
			*buf								 = '-';
			return to_chars_impl<unsigned_type>::impl(buf + (value < 0), (static_cast<unsigned_type>(value) ^ (value >> shift_amount)) - (value >> shift_amount));
		}
	};

	template<uint32_types v_type> struct to_chars_impl<v_type> {
		VN_FORCE_INLINE static char* impl(char* buf, const v_type value) noexcept {
			if (value < 10000U) {
				if (value < 100U) {
					return to_chars_internal<v_type, 2ULL>::impl(buf, value);
				} else {
					return to_chars_internal<v_type, 4ULL>::impl(buf, value);
				}
			} else if (value < 100000000U) {
				if (value < 1000000U) {
					return to_chars_internal<v_type, 6ULL>::impl(buf, value);
				} else {
					return to_chars_internal<v_type, 8ULL>::impl(buf, value);
				}
			} else {
				return to_chars_internal<v_type, 10ULL>::impl(buf, value);
			}
		}
	};

	template<int32_types v_type> struct to_chars_impl<v_type> {
		VN_FORCE_INLINE static char* impl(char* buf, const v_type value) noexcept {
			using unsigned_type					 = std::make_unsigned_t<v_type>;
			constexpr unsigned_type shift_amount = static_cast<unsigned_type>(sizeof(v_type) * 8ULL - 1ULL);
			*buf								 = '-';
			return to_chars_impl<unsigned_type>::impl(buf + (value < 0), (static_cast<unsigned_type>(value) ^ (value >> shift_amount)) - (value >> shift_amount));
		}
	};

	template<integer_types v_type> const char* to_chars(const v_type value, char* buf) noexcept {
		return to_chars_impl<v_type>::impl(buf, value);
	}

}