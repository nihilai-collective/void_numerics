// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp
#pragma once

#include <void-numerics>
#include <rt-ut-incl/index.hpp>
#include <iostream>
#include <charconv>
#include <random>
#include <string>

using namespace rt_ut;
using namespace vn;

namespace vn_to_chars_tests {

	template<typename T> inline std::string ref_str(T v) {
		char buf[32] = {};
		auto r		 = std::to_chars(buf, buf + 32, v);
		*r.ptr		 = '\0';
		return std::string{ buf };
	}

	template<typename T> inline std::string vn_str(T v) {
		char buf[32]	= {};
		const char* end = to_chars(v, buf);
		buf[end - buf]	= '\0';
		return std::string{ buf };
	}

}

using vn_to_chars_tests::ref_str;
using vn_to_chars_tests::vn_str;

template<conversion_classes> struct to_chars_tests;

template<> struct to_chars_tests<conversion_classes::i_to_str> {
	static void impl() {
		using vn_to_chars_tests::ref_str;
		using vn_to_chars_tests::vn_str;
		
		std::cout << "\n=== void_numerics::to_chars Unit Tests ===" << std::endl;

		unit_test<"mulhi64_portable: 0 * 0">::assert_eq(uint64_t{ 0 }, [] {
			return vn::mulhi64_portable(0ULL, 0ULL);
		});

		unit_test<"mulhi64_portable: 1 * 1">::assert_eq(uint64_t{ 0 }, [] {
			return vn::mulhi64_portable(1ULL, 1ULL);
		});

		unit_test<"mulhi64_portable: anything * 0">::assert_eq(uint64_t{ 0 }, [] {
			return vn::mulhi64_portable(0xDEADBEEFCAFEBABEULL, 0ULL);
		});

		unit_test<"mulhi64_portable: 0 * anything">::assert_eq(uint64_t{ 0 }, [] {
			return vn::mulhi64_portable(0ULL, 0xDEADBEEFCAFEBABEULL);
		});

		unit_test<"mulhi64_portable: anything * 1">::assert_eq(uint64_t{ 0 }, [] {
			return vn::mulhi64_portable(0xDEADBEEFCAFEBABEULL, 1ULL);
		});

		unit_test<"mulhi64_portable: UINT32_MAX * UINT32_MAX (no overflow)">::assert_eq(uint64_t{ 0 }, [] {
			return vn::mulhi64_portable(0xFFFFFFFFULL, 0xFFFFFFFFULL);
		});

		unit_test<"mulhi64_portable: 2^32 * 2^32 == 1">::assert_eq(uint64_t{ 1 }, [] {
			return vn::mulhi64_portable(0x100000000ULL, 0x100000000ULL);
		});

		unit_test<"mulhi64_portable: UINT64_MAX * 2 == 1">::assert_eq(uint64_t{ 1 }, [] {
			return vn::mulhi64_portable(0xFFFFFFFFFFFFFFFFULL, 2ULL);
		});

		unit_test<"mulhi64_portable: 2 * UINT64_MAX == 1">::assert_eq(uint64_t{ 1 }, [] {
			return vn::mulhi64_portable(2ULL, 0xFFFFFFFFFFFFFFFFULL);
		});

		unit_test<"mulhi64_portable: UINT64_MAX * UINT64_MAX">::assert_eq(uint64_t{ 0xFFFFFFFFFFFFFFFEULL }, [] {
			return vn::mulhi64_portable(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
		});

		unit_test<"mulhi64_portable: 2^63 * 2^63 == 2^62">::assert_eq(uint64_t{ 0x4000000000000000ULL }, [] {
			return vn::mulhi64_portable(0x8000000000000000ULL, 0x8000000000000000ULL);
		});

		unit_test<"mulhi64_portable: 2^63 * 2 == 1">::assert_eq(uint64_t{ 1 }, [] {
			return vn::mulhi64_portable(0x8000000000000000ULL, 2ULL);
		});

		unit_test<"mulhi64_portable: low_only * low_only">::assert_eq(uint64_t{ 0 }, [] {
			return vn::mulhi64_portable(0x12345678ULL, 0x9ABCDEF0ULL);
		});

		unit_test<"mulhi64_portable: 0xFFFFFFFF00000000 * 0xEEEEEEEE00000000">::assert_eq(uint64_t{ 0xEEEEEEED11111112ULL }, [] {
			return vn::mulhi64_portable(0xFFFFFFFF00000000ULL, 0xEEEEEEEE00000000ULL);
		});

#if defined(__SIZEOF_INT128__)
		unit_test<"mulhi64_portable: matches uint128 multiply DEADBEEF * 1234ABCD">::assert_eq(
			uint64_t{ static_cast<uint64_t>((static_cast<unsigned __int128>(0xDEADBEEFCAFEBABEULL) * 0x1234ABCD5678EF01ULL) >> 64) }, [] {
				return vn::mulhi64_portable(0xDEADBEEFCAFEBABEULL, 0x1234ABCD5678EF01ULL);
			});

		unit_test<"mulhi64_portable: matches uint128 multiply prime-like">::assert_eq(
			uint64_t{ static_cast<uint64_t>((static_cast<unsigned __int128>(0xFEDCBA9876543210ULL) * 0x0123456789ABCDEFULL) >> 64) }, [] {
				return vn::mulhi64_portable(0xFEDCBA9876543210ULL, 0x0123456789ABCDEFULL);
			});

		unit_test<"mulhi64_portable: matches uint128 multiply alternating bits">::assert_eq(
			uint64_t{ static_cast<uint64_t>((static_cast<unsigned __int128>(0xAAAAAAAAAAAAAAAAULL) * 0x5555555555555555ULL) >> 64) }, [] {
				return vn::mulhi64_portable(0xAAAAAAAAAAAAAAAAULL, 0x5555555555555555ULL);
			});
#elif defined(_MSC_VER)
		unit_test<"mulhi64_portable: matches _umul128 DEADBEEF * 1234ABCD">::assert_eq(
			[] {
				uint64_t hi;
				_umul128(0xDEADBEEFCAFEBABEULL, 0x1234ABCD5678EF01ULL, &hi);
				return hi;
			}(),
			[] {
				return vn::mulhi64_portable(0xDEADBEEFCAFEBABEULL, 0x1234ABCD5678EF01ULL);
			});

		unit_test<"mulhi64_portable: matches _umul128 prime-like">::assert_eq(
			[] {
				uint64_t hi;
				_umul128(0xFEDCBA9876543210ULL, 0x0123456789ABCDEFULL, &hi);
				return hi;
			}(),
			[] {
				return vn::mulhi64_portable(0xFEDCBA9876543210ULL, 0x0123456789ABCDEFULL);
			});

		unit_test<"mulhi64_portable: matches _umul128 alternating bits">::assert_eq(
			[] {
				uint64_t hi;
				_umul128(0xAAAAAAAAAAAAAAAAULL, 0x5555555555555555ULL, &hi);
				return hi;
			}(),
			[] {
				return vn::mulhi64_portable(0xAAAAAAAAAAAAAAAAULL, 0x5555555555555555ULL);
			});
#endif

		unit_test<"to_chars u32 2digit: 0 (lz path)">::assert_eq(ref_str(uint32_t{ 0 }), [] {
			return vn_str(uint32_t{ 0 });
		});

		unit_test<"to_chars u32 2digit: 1 (lz path)">::assert_eq(ref_str(uint32_t{ 1 }), [] {
			return vn_str(uint32_t{ 1 });
		});

		unit_test<"to_chars u32 2digit: 5 (lz path mid)">::assert_eq(ref_str(uint32_t{ 5 }), [] {
			return vn_str(uint32_t{ 5 });
		});

		unit_test<"to_chars u32 2digit: 9 (last lz)">::assert_eq(ref_str(uint32_t{ 9 }), [] {
			return vn_str(uint32_t{ 9 });
		});

		unit_test<"to_chars u32 2digit: 10 (first non-lz)">::assert_eq(ref_str(uint32_t{ 10 }), [] {
			return vn_str(uint32_t{ 10 });
		});

		unit_test<"to_chars u32 2digit: 42">::assert_eq(ref_str(uint32_t{ 42 }), [] {
			return vn_str(uint32_t{ 42 });
		});

		unit_test<"to_chars u32 2digit: 99 (max in branch)">::assert_eq(ref_str(uint32_t{ 99 }), [] {
			return vn_str(uint32_t{ 99 });
		});

		unit_test<"to_chars u32 4digit: 100 (first in branch)">::assert_eq(ref_str(uint32_t{ 100 }), [] {
			return vn_str(uint32_t{ 100 });
		});

		unit_test<"to_chars u32 4digit: 500 (lz path)">::assert_eq(ref_str(uint32_t{ 500 }), [] {
			return vn_str(uint32_t{ 500 });
		});

		unit_test<"to_chars u32 4digit: 999 (last lz)">::assert_eq(ref_str(uint32_t{ 999 }), [] {
			return vn_str(uint32_t{ 999 });
		});

		unit_test<"to_chars u32 4digit: 1000 (first non-lz)">::assert_eq(ref_str(uint32_t{ 1000 }), [] {
			return vn_str(uint32_t{ 1000 });
		});

		unit_test<"to_chars u32 4digit: 5678">::assert_eq(ref_str(uint32_t{ 5678 }), [] {
			return vn_str(uint32_t{ 5678 });
		});

		unit_test<"to_chars u32 4digit: 9999 (max in branch)">::assert_eq(ref_str(uint32_t{ 9999 }), [] {
			return vn_str(uint32_t{ 9999 });
		});

		unit_test<"to_chars u32 6digit: 10000 (first in branch)">::assert_eq(ref_str(uint32_t{ 10000 }), [] {
			return vn_str(uint32_t{ 10000 });
		});

		unit_test<"to_chars u32 6digit: 50000 (lz path)">::assert_eq(ref_str(uint32_t{ 50000 }), [] {
			return vn_str(uint32_t{ 50000 });
		});

		unit_test<"to_chars u32 6digit: 99999 (last lz)">::assert_eq(ref_str(uint32_t{ 99999 }), [] {
			return vn_str(uint32_t{ 99999 });
		});

		unit_test<"to_chars u32 6digit: 100000 (first non-lz)">::assert_eq(ref_str(uint32_t{ 100000 }), [] {
			return vn_str(uint32_t{ 100000 });
		});

		unit_test<"to_chars u32 6digit: 314159">::assert_eq(ref_str(uint32_t{ 314159 }), [] {
			return vn_str(uint32_t{ 314159 });
		});

		unit_test<"to_chars u32 6digit: 999999 (max in branch)">::assert_eq(ref_str(uint32_t{ 999999 }), [] {
			return vn_str(uint32_t{ 999999 });
		});

		unit_test<"to_chars u32 8digit: 1000000 (first in branch)">::assert_eq(ref_str(uint32_t{ 1000000 }), [] {
			return vn_str(uint32_t{ 1000000 });
		});

		unit_test<"to_chars u32 8digit: 5000000 (lz path)">::assert_eq(ref_str(uint32_t{ 5000000 }), [] {
			return vn_str(uint32_t{ 5000000 });
		});

		unit_test<"to_chars u32 8digit: 9999999 (last lz)">::assert_eq(ref_str(uint32_t{ 9999999 }), [] {
			return vn_str(uint32_t{ 9999999 });
		});

		unit_test<"to_chars u32 8digit: 10000000 (first non-lz)">::assert_eq(ref_str(uint32_t{ 10000000 }), [] {
			return vn_str(uint32_t{ 10000000 });
		});

		unit_test<"to_chars u32 8digit: 31415926">::assert_eq(ref_str(uint32_t{ 31415926 }), [] {
			return vn_str(uint32_t{ 31415926 });
		});

		unit_test<"to_chars u32 8digit: 99999999 (max in branch)">::assert_eq(ref_str(uint32_t{ 99999999 }), [] {
			return vn_str(uint32_t{ 99999999 });
		});

		unit_test<"to_chars u32 10digit: 100000000 (first in branch)">::assert_eq(ref_str(uint32_t{ 100000000 }), [] {
			return vn_str(uint32_t{ 100000000 });
		});

		unit_test<"to_chars u32 10digit: 500000000 (lz path)">::assert_eq(ref_str(uint32_t{ 500000000 }), [] {
			return vn_str(uint32_t{ 500000000 });
		});

		unit_test<"to_chars u32 10digit: 999999999 (last lz)">::assert_eq(ref_str(uint32_t{ 999999999 }), [] {
			return vn_str(uint32_t{ 999999999 });
		});

		unit_test<"to_chars u32 10digit: 1000000000 (first non-lz)">::assert_eq(ref_str(uint32_t{ 1000000000U }), [] {
			return vn_str(uint32_t{ 1000000000U });
		});

		unit_test<"to_chars u32 10digit: 3141592653">::assert_eq(ref_str(uint32_t{ 3141592653U }), [] {
			return vn_str(uint32_t{ 3141592653U });
		});

		unit_test<"to_chars u32 10digit: 4000000000">::assert_eq(ref_str(uint32_t{ 4000000000U }), [] {
			return vn_str(uint32_t{ 4000000000U });
		});

		unit_test<"to_chars u32 10digit: UINT32_MAX">::assert_eq(ref_str(std::numeric_limits<uint32_t>::max()), [] {
			return vn_str(std::numeric_limits<uint32_t>::max());
		});

		unit_test<"to_chars u32 10digit: UINT32_MAX - 1">::assert_eq(ref_str(std::numeric_limits<uint32_t>::max() - 1), [] {
			return vn_str(std::numeric_limits<uint32_t>::max() - 1);
		});

		unit_test<"to_chars u32 powers: 10^0">::assert_eq(ref_str(uint32_t{ 1 }), [] {
			return vn_str(uint32_t{ 1 });
		});

		unit_test<"to_chars u32 powers: 10^1">::assert_eq(ref_str(uint32_t{ 10 }), [] {
			return vn_str(uint32_t{ 10 });
		});

		unit_test<"to_chars u32 powers: 10^2">::assert_eq(ref_str(uint32_t{ 100 }), [] {
			return vn_str(uint32_t{ 100 });
		});

		unit_test<"to_chars u32 powers: 10^3">::assert_eq(ref_str(uint32_t{ 1000 }), [] {
			return vn_str(uint32_t{ 1000 });
		});

		unit_test<"to_chars u32 powers: 10^4">::assert_eq(ref_str(uint32_t{ 10000 }), [] {
			return vn_str(uint32_t{ 10000 });
		});

		unit_test<"to_chars u32 powers: 10^5">::assert_eq(ref_str(uint32_t{ 100000 }), [] {
			return vn_str(uint32_t{ 100000 });
		});

		unit_test<"to_chars u32 powers: 10^6">::assert_eq(ref_str(uint32_t{ 1000000 }), [] {
			return vn_str(uint32_t{ 1000000 });
		});

		unit_test<"to_chars u32 powers: 10^7">::assert_eq(ref_str(uint32_t{ 10000000 }), [] {
			return vn_str(uint32_t{ 10000000 });
		});

		unit_test<"to_chars u32 powers: 10^8">::assert_eq(ref_str(uint32_t{ 100000000 }), [] {
			return vn_str(uint32_t{ 100000000 });
		});

		unit_test<"to_chars u32 powers: 10^9">::assert_eq(ref_str(uint32_t{ 1000000000U }), [] {
			return vn_str(uint32_t{ 1000000000U });
		});

		unit_test<"to_chars u32 nines: 9">::assert_eq(ref_str(uint32_t{ 9 }), [] {
			return vn_str(uint32_t{ 9 });
		});

		unit_test<"to_chars u32 nines: 99">::assert_eq(ref_str(uint32_t{ 99 }), [] {
			return vn_str(uint32_t{ 99 });
		});

		unit_test<"to_chars u32 nines: 999">::assert_eq(ref_str(uint32_t{ 999 }), [] {
			return vn_str(uint32_t{ 999 });
		});

		unit_test<"to_chars u32 nines: 9999">::assert_eq(ref_str(uint32_t{ 9999 }), [] {
			return vn_str(uint32_t{ 9999 });
		});

		unit_test<"to_chars u32 nines: 99999">::assert_eq(ref_str(uint32_t{ 99999 }), [] {
			return vn_str(uint32_t{ 99999 });
		});

		unit_test<"to_chars u32 nines: 999999">::assert_eq(ref_str(uint32_t{ 999999 }), [] {
			return vn_str(uint32_t{ 999999 });
		});

		unit_test<"to_chars u32 nines: 9999999">::assert_eq(ref_str(uint32_t{ 9999999 }), [] {
			return vn_str(uint32_t{ 9999999 });
		});

		unit_test<"to_chars u32 nines: 99999999">::assert_eq(ref_str(uint32_t{ 99999999 }), [] {
			return vn_str(uint32_t{ 99999999 });
		});

		unit_test<"to_chars u32 nines: 999999999">::assert_eq(ref_str(uint32_t{ 999999999 }), [] {
			return vn_str(uint32_t{ 999999999 });
		});

		unit_test<"to_chars u32 powers2: 1">::assert_eq(ref_str(uint32_t{ 1U << 0 }), [] {
			return vn_str(uint32_t{ 1U << 0 });
		});

		unit_test<"to_chars u32 powers2: 256">::assert_eq(ref_str(uint32_t{ 1U << 8 }), [] {
			return vn_str(uint32_t{ 1U << 8 });
		});

		unit_test<"to_chars u32 powers2: 65536">::assert_eq(ref_str(uint32_t{ 1U << 16 }), [] {
			return vn_str(uint32_t{ 1U << 16 });
		});

		unit_test<"to_chars u32 powers2: 16777216">::assert_eq(ref_str(uint32_t{ 1U << 24 }), [] {
			return vn_str(uint32_t{ 1U << 24 });
		});

		unit_test<"to_chars u32 powers2: 2147483648">::assert_eq(ref_str(uint32_t{ 1U << 31 }), [] {
			return vn_str(uint32_t{ 1U << 31 });
		});

		unit_test<"to_chars u32 stress 1000 deterministic samples">::assert_eq(true, [] {
			std::mt19937 rng{ 0xC0FFEEU };
			std::uniform_int_distribution<uint32_t> dist{ 0, std::numeric_limits<uint32_t>::max() };
			for (uint32_t i = 0; i < 1000; ++i) {
				uint32_t v = dist(rng);
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars u32 stress sweep small range 0..2000">::assert_eq(true, [] {
			for (uint32_t v = 0; v < 2000; ++v) {
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars u32 stress branch-boundary sweep">::assert_eq(true, [] {
			static constexpr uint32_t boundaries[] = { 99, 100, 101, 999, 1000, 1001, 9999, 10000, 10001, 99999, 100000, 100001, 999999, 1000000, 1000001, 9999999, 10000000,
				10000001, 99999999, 100000000, 100000001, 999999999, 1000000000U, 1000000001U, std::numeric_limits<uint32_t>::max() - 1U, std::numeric_limits<uint32_t>::max() };
			for (uint32_t v: boundaries) {
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars u64 2digit: 0">::assert_eq(ref_str(uint64_t{ 0 }), [] {
			return vn_str(uint64_t{ 0 });
		});

		unit_test<"to_chars u64 2digit: 7">::assert_eq(ref_str(uint64_t{ 7 }), [] {
			return vn_str(uint64_t{ 7 });
		});

		unit_test<"to_chars u64 2digit: 9 (last lz)">::assert_eq(ref_str(uint64_t{ 9 }), [] {
			return vn_str(uint64_t{ 9 });
		});

		unit_test<"to_chars u64 2digit: 10 (first non-lz)">::assert_eq(ref_str(uint64_t{ 10 }), [] {
			return vn_str(uint64_t{ 10 });
		});

		unit_test<"to_chars u64 2digit: 99 (max)">::assert_eq(ref_str(uint64_t{ 99 }), [] {
			return vn_str(uint64_t{ 99 });
		});

		unit_test<"to_chars u64 4digit: 100">::assert_eq(ref_str(uint64_t{ 100 }), [] {
			return vn_str(uint64_t{ 100 });
		});

		unit_test<"to_chars u64 4digit: 999 (last lz)">::assert_eq(ref_str(uint64_t{ 999 }), [] {
			return vn_str(uint64_t{ 999 });
		});

		unit_test<"to_chars u64 4digit: 1000">::assert_eq(ref_str(uint64_t{ 1000 }), [] {
			return vn_str(uint64_t{ 1000 });
		});

		unit_test<"to_chars u64 4digit: 9999">::assert_eq(ref_str(uint64_t{ 9999 }), [] {
			return vn_str(uint64_t{ 9999 });
		});

		unit_test<"to_chars u64 6digit: 10000">::assert_eq(ref_str(uint64_t{ 10000 }), [] {
			return vn_str(uint64_t{ 10000 });
		});

		unit_test<"to_chars u64 6digit: 99999 (last lz)">::assert_eq(ref_str(uint64_t{ 99999 }), [] {
			return vn_str(uint64_t{ 99999 });
		});

		unit_test<"to_chars u64 6digit: 100000">::assert_eq(ref_str(uint64_t{ 100000 }), [] {
			return vn_str(uint64_t{ 100000 });
		});

		unit_test<"to_chars u64 6digit: 999999">::assert_eq(ref_str(uint64_t{ 999999 }), [] {
			return vn_str(uint64_t{ 999999 });
		});

		unit_test<"to_chars u64 8digit: 1000000">::assert_eq(ref_str(uint64_t{ 1000000 }), [] {
			return vn_str(uint64_t{ 1000000 });
		});

		unit_test<"to_chars u64 8digit: 9999999 (last lz)">::assert_eq(ref_str(uint64_t{ 9999999 }), [] {
			return vn_str(uint64_t{ 9999999 });
		});

		unit_test<"to_chars u64 8digit: 10000000">::assert_eq(ref_str(uint64_t{ 10000000 }), [] {
			return vn_str(uint64_t{ 10000000 });
		});

		unit_test<"to_chars u64 8digit: 99999999">::assert_eq(ref_str(uint64_t{ 99999999 }), [] {
			return vn_str(uint64_t{ 99999999 });
		});

		unit_test<"to_chars u64 10digit: 100000000">::assert_eq(ref_str(uint64_t{ 100000000ULL }), [] {
			return vn_str(uint64_t{ 100000000ULL });
		});

		unit_test<"to_chars u64 10digit: 999999999 (last lz)">::assert_eq(ref_str(uint64_t{ 999999999ULL }), [] {
			return vn_str(uint64_t{ 999999999ULL });
		});

		unit_test<"to_chars u64 10digit: 1000000000">::assert_eq(ref_str(uint64_t{ 1000000000ULL }), [] {
			return vn_str(uint64_t{ 1000000000ULL });
		});

		unit_test<"to_chars u64 10digit: 9999999999">::assert_eq(ref_str(uint64_t{ 9999999999ULL }), [] {
			return vn_str(uint64_t{ 9999999999ULL });
		});

		unit_test<"to_chars u64 12digit: 10000000000">::assert_eq(ref_str(uint64_t{ 10000000000ULL }), [] {
			return vn_str(uint64_t{ 10000000000ULL });
		});

		unit_test<"to_chars u64 12digit: 99999999999 (last lz)">::assert_eq(ref_str(uint64_t{ 99999999999ULL }), [] {
			return vn_str(uint64_t{ 99999999999ULL });
		});

		unit_test<"to_chars u64 12digit: 100000000000">::assert_eq(ref_str(uint64_t{ 100000000000ULL }), [] {
			return vn_str(uint64_t{ 100000000000ULL });
		});

		unit_test<"to_chars u64 12digit: 999999999999">::assert_eq(ref_str(uint64_t{ 999999999999ULL }), [] {
			return vn_str(uint64_t{ 999999999999ULL });
		});

		unit_test<"to_chars u64 14digit: 1000000000000">::assert_eq(ref_str(uint64_t{ 1000000000000ULL }), [] {
			return vn_str(uint64_t{ 1000000000000ULL });
		});

		unit_test<"to_chars u64 14digit: 9999999999999 (last lz)">::assert_eq(ref_str(uint64_t{ 9999999999999ULL }), [] {
			return vn_str(uint64_t{ 9999999999999ULL });
		});

		unit_test<"to_chars u64 14digit: 10000000000000">::assert_eq(ref_str(uint64_t{ 10000000000000ULL }), [] {
			return vn_str(uint64_t{ 10000000000000ULL });
		});

		unit_test<"to_chars u64 14digit: 99999999999999">::assert_eq(ref_str(uint64_t{ 99999999999999ULL }), [] {
			return vn_str(uint64_t{ 99999999999999ULL });
		});

		unit_test<"to_chars u64 16digit: 100000000000000">::assert_eq(ref_str(uint64_t{ 100000000000000ULL }), [] {
			return vn_str(uint64_t{ 100000000000000ULL });
		});

		unit_test<"to_chars u64 16digit: 999999999999999 (last lz)">::assert_eq(ref_str(uint64_t{ 999999999999999ULL }), [] {
			return vn_str(uint64_t{ 999999999999999ULL });
		});

		unit_test<"to_chars u64 16digit: 1000000000000000">::assert_eq(ref_str(uint64_t{ 1000000000000000ULL }), [] {
			return vn_str(uint64_t{ 1000000000000000ULL });
		});

		unit_test<"to_chars u64 16digit: 9999999999999999">::assert_eq(ref_str(uint64_t{ 9999999999999999ULL }), [] {
			return vn_str(uint64_t{ 9999999999999999ULL });
		});

		unit_test<"to_chars u64 18digit: 10000000000000000">::assert_eq(ref_str(uint64_t{ 10000000000000000ULL }), [] {
			return vn_str(uint64_t{ 10000000000000000ULL });
		});

		unit_test<"to_chars u64 18digit: 99999999999999999 (last lz)">::assert_eq(ref_str(uint64_t{ 99999999999999999ULL }), [] {
			return vn_str(uint64_t{ 99999999999999999ULL });
		});

		unit_test<"to_chars u64 18digit: 100000000000000000">::assert_eq(ref_str(uint64_t{ 100000000000000000ULL }), [] {
			return vn_str(uint64_t{ 100000000000000000ULL });
		});

		unit_test<"to_chars u64 18digit: 999999999999999999">::assert_eq(ref_str(uint64_t{ 999999999999999999ULL }), [] {
			return vn_str(uint64_t{ 999999999999999999ULL });
		});

		unit_test<"to_chars u64 20digit: 1000000000000000000">::assert_eq(ref_str(uint64_t{ 1000000000000000000ULL }), [] {
			return vn_str(uint64_t{ 1000000000000000000ULL });
		});

		unit_test<"to_chars u64 20digit: 9999999999999999999 (last lz)">::assert_eq(ref_str(uint64_t{ 9999999999999999999ULL }), [] {
			return vn_str(uint64_t{ 9999999999999999999ULL });
		});

		unit_test<"to_chars u64 20digit: 10000000000000000000">::assert_eq(ref_str(uint64_t{ 10000000000000000000ULL }), [] {
			return vn_str(uint64_t{ 10000000000000000000ULL });
		});

		unit_test<"to_chars u64 20digit: UINT64_MAX">::assert_eq(ref_str(std::numeric_limits<uint64_t>::max()), [] {
			return vn_str(std::numeric_limits<uint64_t>::max());
		});

		unit_test<"to_chars u64 20digit: UINT64_MAX - 1">::assert_eq(ref_str(std::numeric_limits<uint64_t>::max() - 1), [] {
			return vn_str(std::numeric_limits<uint64_t>::max() - 1);
		});

		unit_test<"to_chars u64 nines: 9">::assert_eq(ref_str(uint64_t{ 9ULL }), [] {
			return vn_str(uint64_t{ 9ULL });
		});

		unit_test<"to_chars u64 nines: 99">::assert_eq(ref_str(uint64_t{ 99ULL }), [] {
			return vn_str(uint64_t{ 99ULL });
		});

		unit_test<"to_chars u64 nines: 999">::assert_eq(ref_str(uint64_t{ 999ULL }), [] {
			return vn_str(uint64_t{ 999ULL });
		});

		unit_test<"to_chars u64 nines: 9999">::assert_eq(ref_str(uint64_t{ 9999ULL }), [] {
			return vn_str(uint64_t{ 9999ULL });
		});

		unit_test<"to_chars u64 nines: 99999">::assert_eq(ref_str(uint64_t{ 99999ULL }), [] {
			return vn_str(uint64_t{ 99999ULL });
		});

		unit_test<"to_chars u64 nines: 999999">::assert_eq(ref_str(uint64_t{ 999999ULL }), [] {
			return vn_str(uint64_t{ 999999ULL });
		});

		unit_test<"to_chars u64 nines: 9999999">::assert_eq(ref_str(uint64_t{ 9999999ULL }), [] {
			return vn_str(uint64_t{ 9999999ULL });
		});

		unit_test<"to_chars u64 nines: 99999999">::assert_eq(ref_str(uint64_t{ 99999999ULL }), [] {
			return vn_str(uint64_t{ 99999999ULL });
		});

		unit_test<"to_chars u64 nines: 999999999">::assert_eq(ref_str(uint64_t{ 999999999ULL }), [] {
			return vn_str(uint64_t{ 999999999ULL });
		});

		unit_test<"to_chars u64 nines: 9999999999">::assert_eq(ref_str(uint64_t{ 9999999999ULL }), [] {
			return vn_str(uint64_t{ 9999999999ULL });
		});

		unit_test<"to_chars u64 nines: 99999999999">::assert_eq(ref_str(uint64_t{ 99999999999ULL }), [] {
			return vn_str(uint64_t{ 99999999999ULL });
		});

		unit_test<"to_chars u64 nines: 999999999999">::assert_eq(ref_str(uint64_t{ 999999999999ULL }), [] {
			return vn_str(uint64_t{ 999999999999ULL });
		});

		unit_test<"to_chars u64 nines: 9999999999999">::assert_eq(ref_str(uint64_t{ 9999999999999ULL }), [] {
			return vn_str(uint64_t{ 9999999999999ULL });
		});

		unit_test<"to_chars u64 nines: 99999999999999">::assert_eq(ref_str(uint64_t{ 99999999999999ULL }), [] {
			return vn_str(uint64_t{ 99999999999999ULL });
		});

		unit_test<"to_chars u64 nines: 999999999999999">::assert_eq(ref_str(uint64_t{ 999999999999999ULL }), [] {
			return vn_str(uint64_t{ 999999999999999ULL });
		});

		unit_test<"to_chars u64 nines: 9999999999999999">::assert_eq(ref_str(uint64_t{ 9999999999999999ULL }), [] {
			return vn_str(uint64_t{ 9999999999999999ULL });
		});

		unit_test<"to_chars u64 nines: 99999999999999999">::assert_eq(ref_str(uint64_t{ 99999999999999999ULL }), [] {
			return vn_str(uint64_t{ 99999999999999999ULL });
		});

		unit_test<"to_chars u64 nines: 999999999999999999">::assert_eq(ref_str(uint64_t{ 999999999999999999ULL }), [] {
			return vn_str(uint64_t{ 999999999999999999ULL });
		});

		unit_test<"to_chars u64 nines: 9999999999999999999">::assert_eq(ref_str(uint64_t{ 9999999999999999999ULL }), [] {
			return vn_str(uint64_t{ 9999999999999999999ULL });
		});

		unit_test<"to_chars u64 powers2: 2^32">::assert_eq(ref_str(uint64_t{ 1ULL << 32 }), [] {
			return vn_str(uint64_t{ 1ULL << 32 });
		});

		unit_test<"to_chars u64 powers2: 2^40">::assert_eq(ref_str(uint64_t{ 1ULL << 40 }), [] {
			return vn_str(uint64_t{ 1ULL << 40 });
		});

		unit_test<"to_chars u64 powers2: 2^48">::assert_eq(ref_str(uint64_t{ 1ULL << 48 }), [] {
			return vn_str(uint64_t{ 1ULL << 48 });
		});

		unit_test<"to_chars u64 powers2: 2^56">::assert_eq(ref_str(uint64_t{ 1ULL << 56 }), [] {
			return vn_str(uint64_t{ 1ULL << 56 });
		});

		unit_test<"to_chars u64 powers2: 2^63">::assert_eq(ref_str(uint64_t{ 1ULL << 63 }), [] {
			return vn_str(uint64_t{ 1ULL << 63 });
		});

		unit_test<"to_chars u64 mid range: 1234567890">::assert_eq(ref_str(uint64_t{ 1234567890ULL }), [] {
			return vn_str(uint64_t{ 1234567890ULL });
		});

		unit_test<"to_chars u64 mid range: 12345678901">::assert_eq(ref_str(uint64_t{ 12345678901ULL }), [] {
			return vn_str(uint64_t{ 12345678901ULL });
		});

		unit_test<"to_chars u64 mid range: 123456789012">::assert_eq(ref_str(uint64_t{ 123456789012ULL }), [] {
			return vn_str(uint64_t{ 123456789012ULL });
		});

		unit_test<"to_chars u64 mid range: 1234567890123">::assert_eq(ref_str(uint64_t{ 1234567890123ULL }), [] {
			return vn_str(uint64_t{ 1234567890123ULL });
		});

		unit_test<"to_chars u64 mid range: 12345678901234">::assert_eq(ref_str(uint64_t{ 12345678901234ULL }), [] {
			return vn_str(uint64_t{ 12345678901234ULL });
		});

		unit_test<"to_chars u64 mid range: 123456789012345">::assert_eq(ref_str(uint64_t{ 123456789012345ULL }), [] {
			return vn_str(uint64_t{ 123456789012345ULL });
		});

		unit_test<"to_chars u64 mid range: 1234567890123456">::assert_eq(ref_str(uint64_t{ 1234567890123456ULL }), [] {
			return vn_str(uint64_t{ 1234567890123456ULL });
		});

		unit_test<"to_chars u64 mid range: 12345678901234567">::assert_eq(ref_str(uint64_t{ 12345678901234567ULL }), [] {
			return vn_str(uint64_t{ 12345678901234567ULL });
		});

		unit_test<"to_chars u64 mid range: 123456789012345678">::assert_eq(ref_str(uint64_t{ 123456789012345678ULL }), [] {
			return vn_str(uint64_t{ 123456789012345678ULL });
		});

		unit_test<"to_chars u64 mid range: 1234567890123456789">::assert_eq(ref_str(uint64_t{ 1234567890123456789ULL }), [] {
			return vn_str(uint64_t{ 1234567890123456789ULL });
		});

		unit_test<"to_chars u64 mid range: 12345678901234567890">::assert_eq(ref_str(uint64_t{ 12345678901234567890ULL }), [] {
			return vn_str(uint64_t{ 12345678901234567890ULL });
		});

		unit_test<"to_chars u64 stress 1000 random samples">::assert_eq(true, [] {
			std::mt19937_64 rng{ 0xDEADBEEFCAFEBABEULL };
			std::uniform_int_distribution<uint64_t> dist{ 0ULL, std::numeric_limits<uint64_t>::max() };
			for (uint32_t i = 0; i < 1000; ++i) {
				uint64_t v = dist(rng);
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars u64 stress sweep 0..2000">::assert_eq(true, [] {
			for (uint64_t v = 0; v < 2000ULL; ++v) {
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars u64 stress branch boundaries">::assert_eq(true, [] {
			static constexpr uint64_t boundaries[] = {
				99ULL,
				100ULL,
				101ULL,
				999ULL,
				1000ULL,
				1001ULL,
				9999ULL,
				10000ULL,
				10001ULL,
				99999ULL,
				100000ULL,
				100001ULL,
				999999ULL,
				1000000ULL,
				1000001ULL,
				9999999ULL,
				10000000ULL,
				10000001ULL,
				99999999ULL,
				100000000ULL,
				100000001ULL,
				999999999ULL,
				1000000000ULL,
				1000000001ULL,
				9999999999ULL,
				10000000000ULL,
				10000000001ULL,
				99999999999ULL,
				100000000000ULL,
				100000000001ULL,
				999999999999ULL,
				1000000000000ULL,
				1000000000001ULL,
				9999999999999ULL,
				10000000000000ULL,
				10000000000001ULL,
				99999999999999ULL,
				100000000000000ULL,
				100000000000001ULL,
				999999999999999ULL,
				1000000000000000ULL,
				1000000000000001ULL,
				9999999999999999ULL,
				10000000000000000ULL,
				10000000000000001ULL,
				99999999999999999ULL,
				100000000000000000ULL,
				100000000000000001ULL,
				999999999999999999ULL,
				1000000000000000000ULL,
				1000000000000000001ULL,
				9999999999999999999ULL,
				10000000000000000000ULL,
				std::numeric_limits<uint64_t>::max() - 1ULL,
				std::numeric_limits<uint64_t>::max(),
			};
			for (uint64_t v: boundaries) {
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars i32: 0">::assert_eq(ref_str(int32_t{ 0 }), [] {
			return vn_str(int32_t{ 0 });
		});

		unit_test<"to_chars i32: 1">::assert_eq(ref_str(int32_t{ 1 }), [] {
			return vn_str(int32_t{ 1 });
		});

		unit_test<"to_chars i32: -1">::assert_eq(ref_str(int32_t{ -1 }), [] {
			return vn_str(int32_t{ -1 });
		});

		unit_test<"to_chars i32: -9">::assert_eq(ref_str(int32_t{ -9 }), [] {
			return vn_str(int32_t{ -9 });
		});

		unit_test<"to_chars i32: -10">::assert_eq(ref_str(int32_t{ -10 }), [] {
			return vn_str(int32_t{ -10 });
		});

		unit_test<"to_chars i32: -42">::assert_eq(ref_str(int32_t{ -42 }), [] {
			return vn_str(int32_t{ -42 });
		});

		unit_test<"to_chars i32: -99">::assert_eq(ref_str(int32_t{ -99 }), [] {
			return vn_str(int32_t{ -99 });
		});

		unit_test<"to_chars i32: -100">::assert_eq(ref_str(int32_t{ -100 }), [] {
			return vn_str(int32_t{ -100 });
		});

		unit_test<"to_chars i32: -999">::assert_eq(ref_str(int32_t{ -999 }), [] {
			return vn_str(int32_t{ -999 });
		});

		unit_test<"to_chars i32: -1000">::assert_eq(ref_str(int32_t{ -1000 }), [] {
			return vn_str(int32_t{ -1000 });
		});

		unit_test<"to_chars i32: -12345">::assert_eq(ref_str(int32_t{ -12345 }), [] {
			return vn_str(int32_t{ -12345 });
		});

		unit_test<"to_chars i32: -123456789">::assert_eq(ref_str(int32_t{ -123456789 }), [] {
			return vn_str(int32_t{ -123456789 });
		});

		unit_test<"to_chars i32: -2147483647 (INT32_MIN+1)">::assert_eq(ref_str(int32_t{ -2147483647 }), [] {
			return vn_str(int32_t{ -2147483647 });
		});

		unit_test<"to_chars i32: INT32_MIN (negation edge case)">::assert_eq(ref_str(std::numeric_limits<int32_t>::min()), [] {
			return vn_str(std::numeric_limits<int32_t>::min());
		});

		unit_test<"to_chars i32: INT32_MAX">::assert_eq(ref_str(std::numeric_limits<int32_t>::max()), [] {
			return vn_str(std::numeric_limits<int32_t>::max());
		});

		unit_test<"to_chars i32: INT32_MAX - 1">::assert_eq(ref_str(std::numeric_limits<int32_t>::max() - 1), [] {
			return vn_str(std::numeric_limits<int32_t>::max() - 1);
		});

		unit_test<"to_chars i32: 2147483647">::assert_eq(ref_str(int32_t{ 2147483647 }), [] {
			return vn_str(int32_t{ 2147483647 });
		});

		unit_test<"to_chars i32: 12345">::assert_eq(ref_str(int32_t{ 12345 }), [] {
			return vn_str(int32_t{ 12345 });
		});

		unit_test<"to_chars i32: 123456789">::assert_eq(ref_str(int32_t{ 123456789 }), [] {
			return vn_str(int32_t{ 123456789 });
		});

		unit_test<"to_chars i32 stress 1000 random samples">::assert_eq(true, [] {
			std::mt19937 rng{ 0x1337C0DEU };
			std::uniform_int_distribution<int32_t> dist{ std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max() };
			for (uint32_t i = 0; i < 1000; ++i) {
				int32_t v = dist(rng);
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars i32 stress sweep -1000..1000">::assert_eq(true, [] {
			for (int32_t v = -1000; v < 1000; ++v) {
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars i64: 0">::assert_eq(ref_str(int64_t{ 0 }), [] {
			return vn_str(int64_t{ 0 });
		});

		unit_test<"to_chars i64: 1">::assert_eq(ref_str(int64_t{ 1 }), [] {
			return vn_str(int64_t{ 1 });
		});

		unit_test<"to_chars i64: -1">::assert_eq(ref_str(int64_t{ -1 }), [] {
			return vn_str(int64_t{ -1 });
		});

		unit_test<"to_chars i64: -9">::assert_eq(ref_str(int64_t{ -9 }), [] {
			return vn_str(int64_t{ -9 });
		});

		unit_test<"to_chars i64: -10">::assert_eq(ref_str(int64_t{ -10 }), [] {
			return vn_str(int64_t{ -10 });
		});

		unit_test<"to_chars i64: -99">::assert_eq(ref_str(int64_t{ -99 }), [] {
			return vn_str(int64_t{ -99 });
		});

		unit_test<"to_chars i64: -100">::assert_eq(ref_str(int64_t{ -100 }), [] {
			return vn_str(int64_t{ -100 });
		});

		unit_test<"to_chars i64: -999">::assert_eq(ref_str(int64_t{ -999 }), [] {
			return vn_str(int64_t{ -999 });
		});

		unit_test<"to_chars i64: -123456789012345">::assert_eq(ref_str(int64_t{ -123456789012345LL }), [] {
			return vn_str(int64_t{ -123456789012345LL });
		});

		unit_test<"to_chars i64: -1234567890123456789">::assert_eq(ref_str(int64_t{ -1234567890123456789LL }), [] {
			return vn_str(int64_t{ -1234567890123456789LL });
		});

		unit_test<"to_chars i64: INT64_MIN+1 (-9223372036854775807)">::assert_eq(ref_str(int64_t{ -9223372036854775807LL }), [] {
			return vn_str(int64_t{ -9223372036854775807LL });
		});

		unit_test<"to_chars i64: INT64_MIN (negation edge case)">::assert_eq(ref_str(std::numeric_limits<int64_t>::min()), [] {
			return vn_str(std::numeric_limits<int64_t>::min());
		});

		unit_test<"to_chars i64: INT64_MAX">::assert_eq(ref_str(std::numeric_limits<int64_t>::max()), [] {
			return vn_str(std::numeric_limits<int64_t>::max());
		});

		unit_test<"to_chars i64: INT64_MAX - 1">::assert_eq(ref_str(std::numeric_limits<int64_t>::max() - 1), [] {
			return vn_str(std::numeric_limits<int64_t>::max() - 1);
		});

		unit_test<"to_chars i64: 9223372036854775806">::assert_eq(ref_str(int64_t{ 9223372036854775806LL }), [] {
			return vn_str(int64_t{ 9223372036854775806LL });
		});

		unit_test<"to_chars i64: 9223372036854775807">::assert_eq(ref_str(int64_t{ 9223372036854775807LL }), [] {
			return vn_str(int64_t{ 9223372036854775807LL });
		});

		unit_test<"to_chars i64: large positive 1234567890123456789">::assert_eq(ref_str(int64_t{ 1234567890123456789LL }), [] {
			return vn_str(int64_t{ 1234567890123456789LL });
		});

		unit_test<"to_chars i64 stress 1000 random samples">::assert_eq(true, [] {
			std::mt19937_64 rng{ 0xBADCAFE5BABE5EEDULL };
			std::uniform_int_distribution<int64_t> dist{ std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max() };
			for (uint32_t i = 0; i < 1000; ++i) {
				int64_t v = dist(rng);
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars i64 stress sweep -1000..1000">::assert_eq(true, [] {
			for (int64_t v = -1000; v < 1000; ++v) {
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars i64 stress all signed branch boundaries">::assert_eq(true, [] {
			static constexpr int64_t boundaries[] = {
				-1LL,
				-9LL,
				-10LL,
				-99LL,
				-100LL,
				-999LL,
				-1000LL,
				-9999LL,
				-10000LL,
				-99999LL,
				-100000LL,
				-999999LL,
				-1000000LL,
				-9999999LL,
				-10000000LL,
				-99999999LL,
				-100000000LL,
				-999999999LL,
				-1000000000LL,
				-9999999999LL,
				-10000000000LL,
				-99999999999LL,
				-100000000000LL,
				-999999999999LL,
				-1000000000000LL,
				-9999999999999LL,
				-10000000000000LL,
				-99999999999999LL,
				-100000000000000LL,
				-999999999999999LL,
				-1000000000000000LL,
				-9999999999999999LL,
				-10000000000000000LL,
				-99999999999999999LL,
				-100000000000000000LL,
				-999999999999999999LL,
				-1000000000000000000LL,
				std::numeric_limits<int64_t>::min(),
				std::numeric_limits<int64_t>::min() + 1,
				std::numeric_limits<int64_t>::max(),
				std::numeric_limits<int64_t>::max() - 1,
			};
			for (int64_t v: boundaries) {
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars: returned pointer length u32 single digit">::assert_eq(uint64_t{ 1 }, [] {
			char buf[32]	= {};
			const char* end = vn::to_chars(uint32_t{ 7 }, buf);
			return static_cast<uint64_t>(end - buf);
		});

		unit_test<"to_chars: returned pointer length u32 max digits">::assert_eq(uint64_t{ 10 }, [] {
			char buf[32]	= {};
			const char* end = vn::to_chars(std::numeric_limits<uint32_t>::max(), buf);
			return static_cast<uint64_t>(end - buf);
		});

		unit_test<"to_chars: returned pointer length u64 single digit">::assert_eq(uint64_t{ 1 }, [] {
			char buf[32]	= {};
			const char* end = vn::to_chars(uint64_t{ 7 }, buf);
			return static_cast<uint64_t>(end - buf);
		});

		unit_test<"to_chars: returned pointer length u64 max digits (20)">::assert_eq(uint64_t{ 20 }, [] {
			char buf[32]	= {};
			const char* end = vn::to_chars(std::numeric_limits<uint64_t>::max(), buf);
			return static_cast<uint64_t>(end - buf);
		});

		unit_test<"to_chars: returned pointer length i64 INT64_MIN (20 chars)">::assert_eq(uint64_t{ 20 }, [] {
			char buf[32]	= {};
			const char* end = vn::to_chars(std::numeric_limits<int64_t>::min(), buf);
			return static_cast<uint64_t>(end - buf);
		});

		unit_test<"to_chars: returned pointer length i32 -1 (2 chars)">::assert_eq(uint64_t{ 2 }, [] {
			char buf[32]	= {};
			const char* end = vn::to_chars(int32_t{ -1 }, buf);
			return static_cast<uint64_t>(end - buf);
		});

		unit_test<"to_chars: cross-check i32 stress full byte range -128..127">::assert_eq(true, [] {
			for (int32_t v = -256; v <= 256; ++v) {
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars: cross-check uint8-range u32 0..255">::assert_eq(true, [] {
			for (uint32_t v = 0; v <= 255; ++v) {
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars: cross-check uint16-range u32 every 100">::assert_eq(true, [] {
			for (uint32_t v = 0; v <= 65535; v += 13) {
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars: u64 powers of 10 exhaustive">::assert_eq(true, [] {
			uint64_t pow10 = 1;
			for (uint32_t i = 0; i <= 19; ++i) {
				if (ref_str(pow10) != vn_str(pow10))
					return false;
				if (ref_str(pow10 - 1) != vn_str(pow10 - 1))
					return false;
				if (i < 19)
					pow10 *= 10ULL;
			}
			return true;
		});

		unit_test<"to_chars: u64 every power of 2">::assert_eq(true, [] {
			for (uint32_t i = 0; i < 64; ++i) {
				uint64_t v = 1ULL << i;
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars: u32 every power of 2">::assert_eq(true, [] {
			for (uint32_t i = 0; i < 32; ++i) {
				uint32_t v = 1U << i;
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars: i64 INT64_MIN_PLUS_K sweep">::assert_eq(true, [] {
			static constexpr int64_t base = std::numeric_limits<int64_t>::min();
			for (int64_t k = 0; k < 100; ++k) {
				int64_t v = base + k;
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars: i64 INT64_MAX_MINUS_K sweep">::assert_eq(true, [] {
			static constexpr int64_t base = std::numeric_limits<int64_t>::max();
			for (int64_t k = 0; k < 100; ++k) {
				int64_t v = base - k;
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars: u64 UINT64_MAX_MINUS_K sweep">::assert_eq(true, [] {
			static constexpr uint64_t base = std::numeric_limits<uint64_t>::max();
			for (uint64_t k = 0; k < 100; ++k) {
				uint64_t v = base - k;
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars: u32 UINT32_MAX_MINUS_K sweep">::assert_eq(true, [] {
			static constexpr uint32_t base = std::numeric_limits<uint32_t>::max();
			for (uint32_t k = 0; k < 100; ++k) {
				uint32_t v = base - k;
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars: u64 sweep around 2^32 boundary">::assert_eq(true, [] {
			static constexpr uint64_t base = 1ULL << 32;
			for (int64_t k = -50; k <= 50; ++k) {
				uint64_t v = base + static_cast<uint64_t>(k);
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars: u64 sweep around 2^53 boundary">::assert_eq(true, [] {
			static constexpr uint64_t base = 1ULL << 53;
			for (int64_t k = -50; k <= 50; ++k) {
				uint64_t v = base + static_cast<uint64_t>(k);
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars: i32 every digit-length each sign">::assert_eq(true, [] {
			static constexpr int32_t samples[] = { 0, 1, -1, 9, -9, 10, -10, 99, -99, 100, -100, 999, -999, 1000, -1000, 9999, -9999, 10000, -10000, 99999, -99999, 100000, -100000,
				999999, -999999, 1000000, -1000000, 9999999, -9999999, 10000000, -10000000, 99999999, -99999999, 100000000, -100000000, 999999999, -999999999, 1000000000,
				-1000000000, std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::min() };
			for (int32_t v: samples) {
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars: u32 dispatcher logic each ladder rung">::assert_eq(true, [] {
			static constexpr uint32_t samples[] = { 0U, 99U, 100U, 9999U, 10000U, 999999U, 1000000U, 99999999U, 100000000U, std::numeric_limits<uint32_t>::max() };
			for (uint32_t v: samples) {
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		unit_test<"to_chars: u64 dispatcher logic each ladder rung">::assert_eq(true, [] {
			static constexpr uint64_t samples[] = { 0ULL, 99ULL, 100ULL, 9999ULL, 10000ULL, 999999ULL, 1000000ULL, 99999999ULL, 100000000ULL, 9999999999ULL, 10000000000ULL,
				999999999999ULL, 1000000000000ULL, 99999999999999ULL, 100000000000000ULL, 9999999999999999ULL, 10000000000000000ULL, 999999999999999999ULL, 1000000000000000000ULL,
				std::numeric_limits<uint64_t>::max() };
			for (uint64_t v: samples) {
				if (ref_str(v) != vn_str(v))
					return false;
			}
			return true;
		});

		std::cout << "=== Unit Tests Complete ===" << std::endl;
	}
};
