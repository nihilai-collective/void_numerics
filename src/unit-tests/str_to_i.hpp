// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp
#pragma once

#include <void-numerics>
#include <rt-ut-incl/index.hpp>
#include <iostream>
#include <charconv>
#include <random>
#include <string>
#include <cstring>
#include <system_error>

using namespace rt_ut;
using namespace vn;

namespace vn_from_chars_tests {

	template<typename T> inline T ref_val(const char* s) {
		T v{};
		std::from_chars(s, s + std::strlen(s), v);
		return v;
	}

	template<typename T> inline T vn_val(const char* s) {
		T v{};
		vn::from_chars(s, s + std::strlen(s), v);
		return v;
	}

	template<typename T> inline std::ptrdiff_t ref_len(const char* s) {
		T v{};
		auto r = std::from_chars(s, s + std::strlen(s), v);
		return r.ptr - s;
	}

	template<typename T> inline std::ptrdiff_t vn_len(const char* s) {
		T v{};
		auto s_orig = s;
		auto r = vn::from_chars(s, s + std::strlen(s), v);
		return s- s_orig;
	}

	template<typename T> inline bool ref_ok(const char* s) {
		T v{};
		auto r = std::from_chars(s, s + std::strlen(s), v);
		return r.ec == std::errc{};
	}

	template<typename T> inline bool vn_ok(const char* s) {
		T v{};
		auto r = vn::from_chars(s, s + std::strlen(s), v);
		return r;
	}

}

using vn_from_chars_tests::ref_val;
using vn_from_chars_tests::vn_val;
using vn_from_chars_tests::ref_len;
using vn_from_chars_tests::vn_len;
using vn_from_chars_tests::ref_ok;
using vn_from_chars_tests::vn_ok;

template<conversion_classes> struct from_chars_tests;

template<> struct from_chars_tests<conversion_classes::str_to_i> {
	static void impl() {
		std::cout << "\n=== void_numerics::from_chars Unit Tests ===" << std::endl;

		unit_test<"from_chars u32 1digit: 0">::assert_eq(ref_val<uint32_t>("0"), [] {
			return vn_val<uint32_t>("0");
		});

		unit_test<"from_chars u32 1digit: 1">::assert_eq(ref_val<uint32_t>("1"), [] {
			return vn_val<uint32_t>("1");
		});

		unit_test<"from_chars u32 1digit: 5">::assert_eq(ref_val<uint32_t>("5"), [] {
			return vn_val<uint32_t>("5");
		});

		unit_test<"from_chars u32 1digit: 9">::assert_eq(ref_val<uint32_t>("9"), [] {
			return vn_val<uint32_t>("9");
		});

		unit_test<"from_chars u32 2digit: 10">::assert_eq(ref_val<uint32_t>("10"), [] {
			return vn_val<uint32_t>("10");
		});

		unit_test<"from_chars u32 2digit: 42">::assert_eq(ref_val<uint32_t>("42"), [] {
			return vn_val<uint32_t>("42");
		});

		unit_test<"from_chars u32 2digit: 99">::assert_eq(ref_val<uint32_t>("99"), [] {
			return vn_val<uint32_t>("99");
		});

		unit_test<"from_chars u32 3digit: 100">::assert_eq(ref_val<uint32_t>("100"), [] {
			return vn_val<uint32_t>("100");
		});

		unit_test<"from_chars u32 3digit: 500">::assert_eq(ref_val<uint32_t>("500"), [] {
			return vn_val<uint32_t>("500");
		});

		unit_test<"from_chars u32 3digit: 999">::assert_eq(ref_val<uint32_t>("999"), [] {
			return vn_val<uint32_t>("999");
		});

		unit_test<"from_chars u32 4digit: 1000">::assert_eq(ref_val<uint32_t>("1000"), [] {
			return vn_val<uint32_t>("1000");
		});

		unit_test<"from_chars u32 4digit: 5678">::assert_eq(ref_val<uint32_t>("5678"), [] {
			return vn_val<uint32_t>("5678");
		});

		unit_test<"from_chars u32 4digit: 9999">::assert_eq(ref_val<uint32_t>("9999"), [] {
			return vn_val<uint32_t>("9999");
		});

		unit_test<"from_chars u32 5digit: 10000">::assert_eq(ref_val<uint32_t>("10000"), [] {
			return vn_val<uint32_t>("10000");
		});

		unit_test<"from_chars u32 5digit: 50000">::assert_eq(ref_val<uint32_t>("50000"), [] {
			return vn_val<uint32_t>("50000");
		});

		unit_test<"from_chars u32 5digit: 99999">::assert_eq(ref_val<uint32_t>("99999"), [] {
			return vn_val<uint32_t>("99999");
		});

		unit_test<"from_chars u32 6digit: 100000">::assert_eq(ref_val<uint32_t>("100000"), [] {
			return vn_val<uint32_t>("100000");
		});

		unit_test<"from_chars u32 6digit: 314159">::assert_eq(ref_val<uint32_t>("314159"), [] {
			return vn_val<uint32_t>("314159");
		});

		unit_test<"from_chars u32 6digit: 999999">::assert_eq(ref_val<uint32_t>("999999"), [] {
			return vn_val<uint32_t>("999999");
		});

		unit_test<"from_chars u32 7digit: 1000000">::assert_eq(ref_val<uint32_t>("1000000"), [] {
			return vn_val<uint32_t>("1000000");
		});

		unit_test<"from_chars u32 7digit: 5000000">::assert_eq(ref_val<uint32_t>("5000000"), [] {
			return vn_val<uint32_t>("5000000");
		});

		unit_test<"from_chars u32 7digit: 9999999">::assert_eq(ref_val<uint32_t>("9999999"), [] {
			return vn_val<uint32_t>("9999999");
		});

		unit_test<"from_chars u32 8digit: 10000000">::assert_eq(ref_val<uint32_t>("10000000"), [] {
			return vn_val<uint32_t>("10000000");
		});

		unit_test<"from_chars u32 8digit: 31415926">::assert_eq(ref_val<uint32_t>("31415926"), [] {
			return vn_val<uint32_t>("31415926");
		});

		unit_test<"from_chars u32 8digit: 99999999">::assert_eq(ref_val<uint32_t>("99999999"), [] {
			return vn_val<uint32_t>("99999999");
		});

		unit_test<"from_chars u32 9digit: 100000000">::assert_eq(ref_val<uint32_t>("100000000"), [] {
			return vn_val<uint32_t>("100000000");
		});

		unit_test<"from_chars u32 9digit: 500000000">::assert_eq(ref_val<uint32_t>("500000000"), [] {
			return vn_val<uint32_t>("500000000");
		});

		unit_test<"from_chars u32 9digit: 999999999">::assert_eq(ref_val<uint32_t>("999999999"), [] {
			return vn_val<uint32_t>("999999999");
		});

		unit_test<"from_chars u32 10digit: 1000000000">::assert_eq(ref_val<uint32_t>("1000000000"), [] {
			return vn_val<uint32_t>("1000000000");
		});

		unit_test<"from_chars u32 10digit: 3141592653">::assert_eq(ref_val<uint32_t>("3141592653"), [] {
			return vn_val<uint32_t>("3141592653");
		});

		unit_test<"from_chars u32 10digit: 4000000000">::assert_eq(ref_val<uint32_t>("4000000000"), [] {
			return vn_val<uint32_t>("4000000000");
		});

		unit_test<"from_chars u32 10digit: UINT32_MAX">::assert_eq(ref_val<uint32_t>("4294967295"), [] {
			return vn_val<uint32_t>("4294967295");
		});

		unit_test<"from_chars u32 10digit: UINT32_MAX-1">::assert_eq(ref_val<uint32_t>("4294967294"), [] {
			return vn_val<uint32_t>("4294967294");
		});

		unit_test<"from_chars u32 powers: 10^0">::assert_eq(ref_val<uint32_t>("1"), [] {
			return vn_val<uint32_t>("1");
		});

		unit_test<"from_chars u32 powers: 10^1">::assert_eq(ref_val<uint32_t>("10"), [] {
			return vn_val<uint32_t>("10");
		});

		unit_test<"from_chars u32 powers: 10^2">::assert_eq(ref_val<uint32_t>("100"), [] {
			return vn_val<uint32_t>("100");
		});

		unit_test<"from_chars u32 powers: 10^3">::assert_eq(ref_val<uint32_t>("1000"), [] {
			return vn_val<uint32_t>("1000");
		});

		unit_test<"from_chars u32 powers: 10^4">::assert_eq(ref_val<uint32_t>("10000"), [] {
			return vn_val<uint32_t>("10000");
		});

		unit_test<"from_chars u32 powers: 10^5">::assert_eq(ref_val<uint32_t>("100000"), [] {
			return vn_val<uint32_t>("100000");
		});

		unit_test<"from_chars u32 powers: 10^6">::assert_eq(ref_val<uint32_t>("1000000"), [] {
			return vn_val<uint32_t>("1000000");
		});

		unit_test<"from_chars u32 powers: 10^7">::assert_eq(ref_val<uint32_t>("10000000"), [] {
			return vn_val<uint32_t>("10000000");
		});

		unit_test<"from_chars u32 powers: 10^8">::assert_eq(ref_val<uint32_t>("100000000"), [] {
			return vn_val<uint32_t>("100000000");
		});

		unit_test<"from_chars u32 powers: 10^9">::assert_eq(ref_val<uint32_t>("1000000000"), [] {
			return vn_val<uint32_t>("1000000000");
		});

		unit_test<"from_chars u32 nines: 9">::assert_eq(ref_val<uint32_t>("9"), [] {
			return vn_val<uint32_t>("9");
		});
		unit_test<"from_chars u32 nines: 99">::assert_eq(ref_val<uint32_t>("99"), [] {
			return vn_val<uint32_t>("99");
		});
		unit_test<"from_chars u32 nines: 999">::assert_eq(ref_val<uint32_t>("999"), [] {
			return vn_val<uint32_t>("999");
		});
		unit_test<"from_chars u32 nines: 9999">::assert_eq(ref_val<uint32_t>("9999"), [] {
			return vn_val<uint32_t>("9999");
		});
		unit_test<"from_chars u32 nines: 99999">::assert_eq(ref_val<uint32_t>("99999"), [] {
			return vn_val<uint32_t>("99999");
		});
		unit_test<"from_chars u32 nines: 999999">::assert_eq(ref_val<uint32_t>("999999"), [] {
			return vn_val<uint32_t>("999999");
		});
		unit_test<"from_chars u32 nines: 9999999">::assert_eq(ref_val<uint32_t>("9999999"), [] {
			return vn_val<uint32_t>("9999999");
		});
		unit_test<"from_chars u32 nines: 99999999">::assert_eq(ref_val<uint32_t>("99999999"), [] {
			return vn_val<uint32_t>("99999999");
		});
		unit_test<"from_chars u32 nines: 999999999">::assert_eq(ref_val<uint32_t>("999999999"), [] {
			return vn_val<uint32_t>("999999999");
		});


		unit_test<"from_chars u32 powers2: 1">::assert_eq(ref_val<uint32_t>("1"), [] {
			return vn_val<uint32_t>("1");
		});
		unit_test<"from_chars u32 powers2: 256">::assert_eq(ref_val<uint32_t>("256"), [] {
			return vn_val<uint32_t>("256");
		});
		unit_test<"from_chars u32 powers2: 65536">::assert_eq(ref_val<uint32_t>("65536"), [] {
			return vn_val<uint32_t>("65536");
		});
		unit_test<"from_chars u32 powers2: 16777216">::assert_eq(ref_val<uint32_t>("16777216"), [] {
			return vn_val<uint32_t>("16777216");
		});
		unit_test<"from_chars u32 powers2: 2147483648">::assert_eq(ref_val<uint32_t>("2147483648"), [] {
			return vn_val<uint32_t>("2147483648");
		});

		unit_test<"from_chars u32 every power of 2">::assert_eq(true, [] {
			static constexpr const char* strs[] = { "1", "2", "4", "8", "16", "32", "64", "128", "256", "512", "1024", "2048", "4096", "8192", "16384", "32768", "65536", "131072",
				"262144", "524288", "1048576", "2097152", "4194304", "8388608", "16777216", "33554432", "67108864", "134217728", "268435456", "536870912", "1073741824",
				"2147483648" };
			for (auto s: strs) {
				if (ref_val<uint32_t>(s) != vn_val<uint32_t>(s))
					return false;
			}
			return true;
		});

		unit_test<"from_chars u32 stress 1000 deterministic samples">::assert_eq(true, [] {
			std::mt19937 rng{ 0xC0FFEEU };
			std::uniform_int_distribution<uint32_t> dist{ 0, std::numeric_limits<uint32_t>::max() };
			char buf[16]{};
			for (uint32_t i = 0; i < 1000; ++i) {
				uint32_t v = dist(rng);
				auto r	   = std::to_chars(buf, buf + 16, v);
				*r.ptr	   = '\0';
				if (ref_val<uint32_t>(buf) != vn_val<uint32_t>(buf))
					return false;
			}
			return true;
		});

		unit_test<"from_chars u32 stress sweep 0..2000">::assert_eq(true, [] {
			char buf[16]{};
			for (uint32_t v = 0; v < 2000; ++v) {
				auto r = std::to_chars(buf, buf + 16, v);
				*r.ptr = '\0';
				if (ref_val<uint32_t>(buf) != vn_val<uint32_t>(buf))
					return false;
			}
			return true;
		});

		unit_test<"from_chars u32 stress branch-boundary sweep">::assert_eq(true, [] {
			static constexpr uint32_t boundaries[] = { 99U, 100U, 101U, 999U, 1000U, 1001U, 9999U, 10000U, 10001U, 99999U, 100000U, 100001U, 999999U, 1000000U, 1000001U, 9999999U,
				10000000U, 10000001U, 99999999U, 100000000U, 100000001U, 999999999U, 1000000000U, 1000000001U, std::numeric_limits<uint32_t>::max() - 1U,
				std::numeric_limits<uint32_t>::max() };
			char buf[16]{};
			for (uint32_t v: boundaries) {
				auto r = std::to_chars(buf, buf + 16, v);
				*r.ptr = '\0';
				if (ref_val<uint32_t>(buf) != vn_val<uint32_t>(buf))
					return false;
			}
			return true;
		});

		unit_test<"from_chars u32 UINT32_MAX_MINUS_K sweep">::assert_eq(true, [] {
			static constexpr uint32_t base = std::numeric_limits<uint32_t>::max();
			char buf[16]{};
			for (uint32_t k = 0; k < 100; ++k) {
				uint32_t v = base - k;
				auto r	   = std::to_chars(buf, buf + 16, v);
				*r.ptr	   = '\0';
				if (ref_val<uint32_t>(buf) != vn_val<uint32_t>(buf))
					return false;
			}
			return true;
		});

		unit_test<"from_chars u32 dispatcher logic each ladder rung">::assert_eq(true, [] {
			static constexpr const char* samples[] = { "0", "99", "100", "9999", "10000", "999999", "1000000", "99999999", "100000000", "4294967295" };
			for (auto s: samples) {
				if (ref_val<uint32_t>(s) != vn_val<uint32_t>(s))
					return false;
			}
			return true;
		});

		unit_test<"from_chars u64 1digit: 0">::assert_eq(ref_val<uint64_t>("0"), [] {
			return vn_val<uint64_t>("0");
		});
		unit_test<"from_chars u64 1digit: 7">::assert_eq(ref_val<uint64_t>("7"), [] {
			return vn_val<uint64_t>("7");
		});
		unit_test<"from_chars u64 1digit: 9">::assert_eq(ref_val<uint64_t>("9"), [] {
			return vn_val<uint64_t>("9");
		});

		unit_test<"from_chars u64 2digit: 10">::assert_eq(ref_val<uint64_t>("10"), [] {
			return vn_val<uint64_t>("10");
		});
		unit_test<"from_chars u64 2digit: 99">::assert_eq(ref_val<uint64_t>("99"), [] {
			return vn_val<uint64_t>("99");
		});

		unit_test<"from_chars u64 3digit: 100">::assert_eq(ref_val<uint64_t>("100"), [] {
			return vn_val<uint64_t>("100");
		});
		unit_test<"from_chars u64 3digit: 999">::assert_eq(ref_val<uint64_t>("999"), [] {
			return vn_val<uint64_t>("999");
		});

		unit_test<"from_chars u64 4digit: 1000">::assert_eq(ref_val<uint64_t>("1000"), [] {
			return vn_val<uint64_t>("1000");
		});
		unit_test<"from_chars u64 4digit: 9999">::assert_eq(ref_val<uint64_t>("9999"), [] {
			return vn_val<uint64_t>("9999");
		});

		unit_test<"from_chars u64 5digit: 10000">::assert_eq(ref_val<uint64_t>("10000"), [] {
			return vn_val<uint64_t>("10000");
		});
		unit_test<"from_chars u64 5digit: 99999">::assert_eq(ref_val<uint64_t>("99999"), [] {
			return vn_val<uint64_t>("99999");
		});

		unit_test<"from_chars u64 6digit: 100000">::assert_eq(ref_val<uint64_t>("100000"), [] {
			return vn_val<uint64_t>("100000");
		});
		unit_test<"from_chars u64 6digit: 999999">::assert_eq(ref_val<uint64_t>("999999"), [] {
			return vn_val<uint64_t>("999999");
		});

		unit_test<"from_chars u64 7digit: 1000000">::assert_eq(ref_val<uint64_t>("1000000"), [] {
			return vn_val<uint64_t>("1000000");
		});
		unit_test<"from_chars u64 7digit: 9999999">::assert_eq(ref_val<uint64_t>("9999999"), [] {
			return vn_val<uint64_t>("9999999");
		});

		unit_test<"from_chars u64 8digit: 10000000">::assert_eq(ref_val<uint64_t>("10000000"), [] {
			return vn_val<uint64_t>("10000000");
		});
		unit_test<"from_chars u64 8digit: 99999999">::assert_eq(ref_val<uint64_t>("99999999"), [] {
			return vn_val<uint64_t>("99999999");
		});

		unit_test<"from_chars u64 9digit: 100000000">::assert_eq(ref_val<uint64_t>("100000000"), [] {
			return vn_val<uint64_t>("100000000");
		});
		unit_test<"from_chars u64 9digit: 999999999">::assert_eq(ref_val<uint64_t>("999999999"), [] {
			return vn_val<uint64_t>("999999999");
		});

		unit_test<"from_chars u64 10digit: 1000000000">::assert_eq(ref_val<uint64_t>("1000000000"), [] {
			return vn_val<uint64_t>("1000000000");
		});
		unit_test<"from_chars u64 10digit: 9999999999">::assert_eq(ref_val<uint64_t>("9999999999"), [] {
			return vn_val<uint64_t>("9999999999");
		});

		unit_test<"from_chars u64 11digit: 10000000000">::assert_eq(ref_val<uint64_t>("10000000000"), [] {
			return vn_val<uint64_t>("10000000000");
		});
		unit_test<"from_chars u64 11digit: 99999999999">::assert_eq(ref_val<uint64_t>("99999999999"), [] {
			return vn_val<uint64_t>("99999999999");
		});

		unit_test<"from_chars u64 12digit: 100000000000">::assert_eq(ref_val<uint64_t>("100000000000"), [] {
			return vn_val<uint64_t>("100000000000");
		});
		unit_test<"from_chars u64 12digit: 999999999999">::assert_eq(ref_val<uint64_t>("999999999999"), [] {
			return vn_val<uint64_t>("999999999999");
		});

		unit_test<"from_chars u64 13digit: 1000000000000">::assert_eq(ref_val<uint64_t>("1000000000000"), [] {
			return vn_val<uint64_t>("1000000000000");
		});
		unit_test<"from_chars u64 13digit: 9999999999999">::assert_eq(ref_val<uint64_t>("9999999999999"), [] {
			return vn_val<uint64_t>("9999999999999");
		});

		unit_test<"from_chars u64 14digit: 10000000000000">::assert_eq(ref_val<uint64_t>("10000000000000"), [] {
			return vn_val<uint64_t>("10000000000000");
		});
		unit_test<"from_chars u64 14digit: 99999999999999">::assert_eq(ref_val<uint64_t>("99999999999999"), [] {
			return vn_val<uint64_t>("99999999999999");
		});

		unit_test<"from_chars u64 15digit: 100000000000000">::assert_eq(ref_val<uint64_t>("100000000000000"), [] {
			return vn_val<uint64_t>("100000000000000");
		});
		unit_test<"from_chars u64 15digit: 999999999999999">::assert_eq(ref_val<uint64_t>("999999999999999"), [] {
			return vn_val<uint64_t>("999999999999999");
		});

		unit_test<"from_chars u64 16digit: 1000000000000000">::assert_eq(ref_val<uint64_t>("1000000000000000"), [] {
			return vn_val<uint64_t>("1000000000000000");
		});
		unit_test<"from_chars u64 16digit: 9999999999999999">::assert_eq(ref_val<uint64_t>("9999999999999999"), [] {
			return vn_val<uint64_t>("9999999999999999");
		});

		unit_test<"from_chars u64 17digit: 10000000000000000">::assert_eq(ref_val<uint64_t>("10000000000000000"), [] {
			return vn_val<uint64_t>("10000000000000000");
		});
		unit_test<"from_chars u64 17digit: 99999999999999999">::assert_eq(ref_val<uint64_t>("99999999999999999"), [] {
			return vn_val<uint64_t>("99999999999999999");
		});

		unit_test<"from_chars u64 18digit: 100000000000000000">::assert_eq(ref_val<uint64_t>("100000000000000000"), [] {
			return vn_val<uint64_t>("100000000000000000");
		});
		unit_test<"from_chars u64 18digit: 999999999999999999">::assert_eq(ref_val<uint64_t>("999999999999999999"), [] {
			return vn_val<uint64_t>("999999999999999999");
		});

		unit_test<"from_chars u64 19digit: 1000000000000000000">::assert_eq(ref_val<uint64_t>("1000000000000000000"), [] {
			return vn_val<uint64_t>("1000000000000000000");
		});
		unit_test<"from_chars u64 19digit: 9999999999999999999">::assert_eq(ref_val<uint64_t>("9999999999999999999"), [] {
			return vn_val<uint64_t>("9999999999999999999");
		});

		unit_test<"from_chars u64 20digit: 10000000000000000000">::assert_eq(ref_val<uint64_t>("10000000000000000000"), [] {
			return vn_val<uint64_t>("10000000000000000000");
		});
		unit_test<"from_chars u64 20digit: UINT64_MAX">::assert_eq(ref_val<uint64_t>("18446744073709551615"), [] {
			return vn_val<uint64_t>("18446744073709551615");
		});
		unit_test<"from_chars u64 20digit: UINT64_MAX-1">::assert_eq(ref_val<uint64_t>("18446744073709551614"), [] {
			return vn_val<uint64_t>("18446744073709551614");
		});

		unit_test<"from_chars u64 nines: 9">::assert_eq(ref_val<uint64_t>("9"), [] {
			return vn_val<uint64_t>("9");
		});
		unit_test<"from_chars u64 nines: 99">::assert_eq(ref_val<uint64_t>("99"), [] {
			return vn_val<uint64_t>("99");
		});
		unit_test<"from_chars u64 nines: 999">::assert_eq(ref_val<uint64_t>("999"), [] {
			return vn_val<uint64_t>("999");
		});
		unit_test<"from_chars u64 nines: 9999">::assert_eq(ref_val<uint64_t>("9999"), [] {
			return vn_val<uint64_t>("9999");
		});
		unit_test<"from_chars u64 nines: 99999">::assert_eq(ref_val<uint64_t>("99999"), [] {
			return vn_val<uint64_t>("99999");
		});
		unit_test<"from_chars u64 nines: 999999">::assert_eq(ref_val<uint64_t>("999999"), [] {
			return vn_val<uint64_t>("999999");
		});
		unit_test<"from_chars u64 nines: 9999999">::assert_eq(ref_val<uint64_t>("9999999"), [] {
			return vn_val<uint64_t>("9999999");
		});
		unit_test<"from_chars u64 nines: 99999999">::assert_eq(ref_val<uint64_t>("99999999"), [] {
			return vn_val<uint64_t>("99999999");
		});
		unit_test<"from_chars u64 nines: 999999999">::assert_eq(ref_val<uint64_t>("999999999"), [] {
			return vn_val<uint64_t>("999999999");
		});
		unit_test<"from_chars u64 nines: 9999999999">::assert_eq(ref_val<uint64_t>("9999999999"), [] {
			return vn_val<uint64_t>("9999999999");
		});
		unit_test<"from_chars u64 nines: 99999999999">::assert_eq(ref_val<uint64_t>("99999999999"), [] {
			return vn_val<uint64_t>("99999999999");
		});
		unit_test<"from_chars u64 nines: 999999999999">::assert_eq(ref_val<uint64_t>("999999999999"), [] {
			return vn_val<uint64_t>("999999999999");
		});
		unit_test<"from_chars u64 nines: 9999999999999">::assert_eq(ref_val<uint64_t>("9999999999999"), [] {
			return vn_val<uint64_t>("9999999999999");
		});
		unit_test<"from_chars u64 nines: 99999999999999">::assert_eq(ref_val<uint64_t>("99999999999999"), [] {
			return vn_val<uint64_t>("99999999999999");
		});
		unit_test<"from_chars u64 nines: 999999999999999">::assert_eq(ref_val<uint64_t>("999999999999999"), [] {
			return vn_val<uint64_t>("999999999999999");
		});
		unit_test<"from_chars u64 nines: 9999999999999999">::assert_eq(ref_val<uint64_t>("9999999999999999"), [] {
			return vn_val<uint64_t>("9999999999999999");
		});
		unit_test<"from_chars u64 nines: 99999999999999999">::assert_eq(ref_val<uint64_t>("99999999999999999"), [] {
			return vn_val<uint64_t>("99999999999999999");
		});
		unit_test<"from_chars u64 nines: 999999999999999999">::assert_eq(ref_val<uint64_t>("999999999999999999"), [] {
			return vn_val<uint64_t>("999999999999999999");
		});
		unit_test<"from_chars u64 nines: 9999999999999999999">::assert_eq(ref_val<uint64_t>("9999999999999999999"), [] {
			return vn_val<uint64_t>("9999999999999999999");
		});

		unit_test<"from_chars u64 powers2: 2^32">::assert_eq(ref_val<uint64_t>("4294967296"), [] {
			return vn_val<uint64_t>("4294967296");
		});
		unit_test<"from_chars u64 powers2: 2^40">::assert_eq(ref_val<uint64_t>("1099511627776"), [] {
			return vn_val<uint64_t>("1099511627776");
		});
		unit_test<"from_chars u64 powers2: 2^48">::assert_eq(ref_val<uint64_t>("281474976710656"), [] {
			return vn_val<uint64_t>("281474976710656");
		});
		unit_test<"from_chars u64 powers2: 2^56">::assert_eq(ref_val<uint64_t>("72057594037927936"), [] {
			return vn_val<uint64_t>("72057594037927936");
		});
		unit_test<"from_chars u64 powers2: 2^63">::assert_eq(ref_val<uint64_t>("9223372036854775808"), [] {
			return vn_val<uint64_t>("9223372036854775808");
		});

		unit_test<"from_chars u64 every power of 2">::assert_eq(true, [] {
			char buf[32]{};
			for (uint32_t i = 0; i < 64; ++i) {
				uint64_t v = 1ULL << i;
				auto r	   = std::to_chars(buf, buf + 32, v);
				*r.ptr	   = '\0';
				if (ref_val<uint64_t>(buf) != vn_val<uint64_t>(buf))
					return false;
			}
			return true;
		});

		unit_test<"from_chars u64 mid: 1234567890">::assert_eq(ref_val<uint64_t>("1234567890"), [] {
			return vn_val<uint64_t>("1234567890");
		});
		unit_test<"from_chars u64 mid: 12345678901">::assert_eq(ref_val<uint64_t>("12345678901"), [] {
			return vn_val<uint64_t>("12345678901");
		});
		unit_test<"from_chars u64 mid: 123456789012">::assert_eq(ref_val<uint64_t>("123456789012"), [] {
			return vn_val<uint64_t>("123456789012");
		});
		unit_test<"from_chars u64 mid: 1234567890123">::assert_eq(ref_val<uint64_t>("1234567890123"), [] {
			return vn_val<uint64_t>("1234567890123");
		});
		unit_test<"from_chars u64 mid: 12345678901234">::assert_eq(ref_val<uint64_t>("12345678901234"), [] {
			return vn_val<uint64_t>("12345678901234");
		});
		unit_test<"from_chars u64 mid: 123456789012345">::assert_eq(ref_val<uint64_t>("123456789012345"), [] {
			return vn_val<uint64_t>("123456789012345");
		});
		unit_test<"from_chars u64 mid: 1234567890123456">::assert_eq(ref_val<uint64_t>("1234567890123456"), [] {
			return vn_val<uint64_t>("1234567890123456");
		});
		unit_test<"from_chars u64 mid: 12345678901234567">::assert_eq(ref_val<uint64_t>("12345678901234567"), [] {
			return vn_val<uint64_t>("12345678901234567");
		});
		unit_test<"from_chars u64 mid: 123456789012345678">::assert_eq(ref_val<uint64_t>("123456789012345678"), [] {
			return vn_val<uint64_t>("123456789012345678");
		});
		unit_test<"from_chars u64 mid: 1234567890123456789">::assert_eq(ref_val<uint64_t>("1234567890123456789"), [] {
			return vn_val<uint64_t>("1234567890123456789");
		});
		unit_test<"from_chars u64 mid: 12345678901234567890">::assert_eq(ref_val<uint64_t>("12345678901234567890"), [] {
			return vn_val<uint64_t>("12345678901234567890");
		});

		unit_test<"from_chars u64 stress 1000 random samples">::assert_eq(true, [] {
			std::mt19937_64 rng{ 0xDEADBEEFCAFEBABEULL };
			std::uniform_int_distribution<uint64_t> dist{ 0ULL, std::numeric_limits<uint64_t>::max() };
			char buf[32]{};
			for (uint32_t i = 0; i < 1000; ++i) {
				uint64_t v = dist(rng);
				auto r	   = std::to_chars(buf, buf + 32, v);
				*r.ptr	   = '\0';
				if (ref_val<uint64_t>(buf) != vn_val<uint64_t>(buf))
					return false;
			}
			return true;
		});

		unit_test<"from_chars u64 stress sweep 0..2000">::assert_eq(true, [] {
			char buf[32]{};
			for (uint64_t v = 0; v < 2000ULL; ++v) {
				auto r = std::to_chars(buf, buf + 32, v);
				*r.ptr = '\0';
				if (ref_val<uint64_t>(buf) != vn_val<uint64_t>(buf))
					return false;
			}
			return true;
		});

		unit_test<"from_chars u64 stress branch boundaries">::assert_eq(true, [] {
			static constexpr const char* boundaries[] = { "99", "100", "101", "999", "1000", "1001", "9999", "10000", "10001", "99999", "100000", "100001", "999999", "1000000",
				"1000001", "9999999", "10000000", "10000001", "99999999", "100000000", "100000001", "999999999", "1000000000", "1000000001", "9999999999", "10000000000",
				"10000000001", "99999999999", "100000000000", "100000000001", "999999999999", "1000000000000", "1000000000001", "9999999999999", "10000000000000", "10000000000001",
				"99999999999999", "100000000000000", "100000000000001", "999999999999999", "1000000000000000", "1000000000000001", "9999999999999999", "10000000000000000",
				"10000000000000001", "99999999999999999", "100000000000000000", "100000000000000001", "999999999999999999", "1000000000000000000", "1000000000000000001",
				"9999999999999999999", "10000000000000000000", "18446744073709551614", "18446744073709551615" };
			for (auto s: boundaries) {
				if (ref_val<uint64_t>(s) != vn_val<uint64_t>(s))
					return false;
			}
			return true;
		});

		unit_test<"from_chars u64 UINT64_MAX_MINUS_K sweep">::assert_eq(true, [] {
			static constexpr uint64_t base = std::numeric_limits<uint64_t>::max();
			char buf[32]{};
			for (uint64_t k = 0; k < 100; ++k) {
				uint64_t v = base - k;
				auto r	   = std::to_chars(buf, buf + 32, v);
				*r.ptr	   = '\0';
				if (ref_val<uint64_t>(buf) != vn_val<uint64_t>(buf))
					return false;
			}
			return true;
		});

		unit_test<"from_chars u64 sweep around 2^32 boundary">::assert_eq(true, [] {
			static constexpr uint64_t base = 1ULL << 32;
			char buf[32]{};
			for (int64_t k = -50; k <= 50; ++k) {
				uint64_t v = base + static_cast<uint64_t>(k);
				auto r	   = std::to_chars(buf, buf + 32, v);
				*r.ptr	   = '\0';
				if (ref_val<uint64_t>(buf) != vn_val<uint64_t>(buf))
					return false;
			}
			return true;
		});

		unit_test<"from_chars u64 sweep around 2^53 boundary">::assert_eq(true, [] {
			static constexpr uint64_t base = 1ULL << 53;
			char buf[32]{};
			for (int64_t k = -50; k <= 50; ++k) {
				uint64_t v = base + static_cast<uint64_t>(k);
				auto r	   = std::to_chars(buf, buf + 32, v);
				*r.ptr	   = '\0';
				if (ref_val<uint64_t>(buf) != vn_val<uint64_t>(buf))
					return false;
			}
			return true;
		});

		unit_test<"from_chars u64 powers of 10 exhaustive">::assert_eq(true, [] {
			char buf[32]{};
			uint64_t pow10 = 1;
			for (uint32_t i = 0; i <= 19; ++i) {
				auto r = std::to_chars(buf, buf + 32, pow10);
				*r.ptr = '\0';
				if (ref_val<uint64_t>(buf) != vn_val<uint64_t>(buf))
					return false;
				auto rm = std::to_chars(buf, buf + 32, pow10 - 1);
				*rm.ptr = '\0';
				if (ref_val<uint64_t>(buf) != vn_val<uint64_t>(buf))
					return false;
				if (i < 19)
					pow10 *= 10ULL;
			}
			return true;
		});

		unit_test<"from_chars u64 dispatcher logic each ladder rung">::assert_eq(true, [] {
			static constexpr const char* samples[] = { "0", "99", "100", "9999", "10000", "999999", "1000000", "99999999", "100000000", "9999999999", "10000000000", "999999999999",
				"1000000000000", "99999999999999", "100000000000000", "9999999999999999", "10000000000000000", "999999999999999999", "1000000000000000000",
				"18446744073709551615" };
			for (auto s: samples) {
				if (ref_val<uint64_t>(s) != vn_val<uint64_t>(s))
					return false;
			}
			return true;
		});

		unit_test<"from_chars i32: 0">::assert_eq(ref_val<int32_t>("0"), [] {
			return vn_val<int32_t>("0");
		});
		unit_test<"from_chars i32: 1">::assert_eq(ref_val<int32_t>("1"), [] {
			return vn_val<int32_t>("1");
		});
		unit_test<"from_chars i32: -1">::assert_eq(ref_val<int32_t>("-1"), [] {
			return vn_val<int32_t>("-1");
		});
		unit_test<"from_chars i32: -9">::assert_eq(ref_val<int32_t>("-9"), [] {
			return vn_val<int32_t>("-9");
		});
		unit_test<"from_chars i32: -10">::assert_eq(ref_val<int32_t>("-10"), [] {
			return vn_val<int32_t>("-10");
		});
		unit_test<"from_chars i32: -42">::assert_eq(ref_val<int32_t>("-42"), [] {
			return vn_val<int32_t>("-42");
		});
		unit_test<"from_chars i32: -99">::assert_eq(ref_val<int32_t>("-99"), [] {
			return vn_val<int32_t>("-99");
		});
		unit_test<"from_chars i32: -100">::assert_eq(ref_val<int32_t>("-100"), [] {
			return vn_val<int32_t>("-100");
		});
		unit_test<"from_chars i32: -999">::assert_eq(ref_val<int32_t>("-999"), [] {
			return vn_val<int32_t>("-999");
		});
		unit_test<"from_chars i32: -1000">::assert_eq(ref_val<int32_t>("-1000"), [] {
			return vn_val<int32_t>("-1000");
		});
		unit_test<"from_chars i32: -12345">::assert_eq(ref_val<int32_t>("-12345"), [] {
			return vn_val<int32_t>("-12345");
		});
		unit_test<"from_chars i32: -123456789">::assert_eq(ref_val<int32_t>("-123456789"), [] {
			return vn_val<int32_t>("-123456789");
		});
		unit_test<"from_chars i32: -2147483647 (INT32_MIN+1)">::assert_eq(ref_val<int32_t>("-2147483647"), [] {
			return vn_val<int32_t>("-2147483647");
		});
		unit_test<"from_chars i32: INT32_MIN">::assert_eq(ref_val<int32_t>("-2147483648"), [] {
			return vn_val<int32_t>("-2147483648");
		});
		unit_test<"from_chars i32: INT32_MAX">::assert_eq(ref_val<int32_t>("2147483647"), [] {
			return vn_val<int32_t>("2147483647");
		});
		unit_test<"from_chars i32: INT32_MAX-1">::assert_eq(ref_val<int32_t>("2147483646"), [] {
			return vn_val<int32_t>("2147483646");
		});
		unit_test<"from_chars i32: 12345">::assert_eq(ref_val<int32_t>("12345"), [] {
			return vn_val<int32_t>("12345");
		});
		unit_test<"from_chars i32: 123456789">::assert_eq(ref_val<int32_t>("123456789"), [] {
			return vn_val<int32_t>("123456789");
		});
		unit_test<"from_chars i32: 2147483647">::assert_eq(ref_val<int32_t>("2147483647"), [] {
			return vn_val<int32_t>("2147483647");
		});

		unit_test<"from_chars i32 every digit-length each sign">::assert_eq(true, [] {
			static constexpr const char* samples[] = { "0", "1", "-1", "9", "-9", "10", "-10", "99", "-99", "100", "-100", "999", "-999", "1000", "-1000", "9999", "-9999", "10000",
				"-10000", "99999", "-99999", "100000", "-100000", "999999", "-999999", "1000000", "-1000000", "9999999", "-9999999", "10000000", "-10000000", "99999999",
				"-99999999", "100000000", "-100000000", "999999999", "-999999999", "1000000000", "-1000000000", "2147483647", "-2147483648" };
			for (auto s: samples) {
				if (ref_val<int32_t>(s) != vn_val<int32_t>(s))
					return false;
			}
			return true;
		});

		unit_test<"from_chars i32 stress 1000 random samples">::assert_eq(true, [] {
			std::mt19937 rng{ 0x1337C0DEU };
			std::uniform_int_distribution<int32_t> dist{ std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max() };
			char buf[16]{};
			for (uint32_t i = 0; i < 1000; ++i) {
				int32_t v = dist(rng);
				auto r	  = std::to_chars(buf, buf + 16, v);
				*r.ptr	  = '\0';
				if (ref_val<int32_t>(buf) != vn_val<int32_t>(buf))
					return false;
			}
			return true;
		});

		unit_test<"from_chars i32 stress sweep -1000..1000">::assert_eq(true, [] {
			char buf[16]{};
			for (int32_t v = -1000; v < 1000; ++v) {
				auto r = std::to_chars(buf, buf + 16, v);
				*r.ptr = '\0';
				if (ref_val<int32_t>(buf) != vn_val<int32_t>(buf))
					return false;
			}
			return true;
		});

		unit_test<"from_chars i32 stress full byte range -256..256">::assert_eq(true, [] {
			char buf[16]{};
			for (int32_t v = -256; v <= 256; ++v) {
				auto r = std::to_chars(buf, buf + 16, v);
				*r.ptr = '\0';
				if (ref_val<int32_t>(buf) != vn_val<int32_t>(buf))
					return false;
			}
			return true;
		});

		unit_test<"from_chars i32 INT32_MIN_PLUS_K sweep">::assert_eq(true, [] {
			char buf[16]{};
			static constexpr int32_t base = std::numeric_limits<int32_t>::min();
			for (int32_t k = 0; k < 100; ++k) {
				int32_t v = base + k;
				auto r	  = std::to_chars(buf, buf + 16, v);
				*r.ptr	  = '\0';
				if (ref_val<int32_t>(buf) != vn_val<int32_t>(buf))
					return false;
			}
			return true;
		});

		unit_test<"from_chars i32 INT32_MAX_MINUS_K sweep">::assert_eq(true, [] {
			char buf[16]{};
			static constexpr int32_t base = std::numeric_limits<int32_t>::max();
			for (int32_t k = 0; k < 100; ++k) {
				int32_t v = base - k;
				auto r	  = std::to_chars(buf, buf + 16, v);
				*r.ptr	  = '\0';
				if (ref_val<int32_t>(buf) != vn_val<int32_t>(buf))
					return false;
			}
			return true;
		});

		unit_test<"from_chars i64: 0">::assert_eq(ref_val<int64_t>("0"), [] {
			return vn_val<int64_t>("0");
		});
		unit_test<"from_chars i64: 1">::assert_eq(ref_val<int64_t>("1"), [] {
			return vn_val<int64_t>("1");
		});
		unit_test<"from_chars i64: -1">::assert_eq(ref_val<int64_t>("-1"), [] {
			return vn_val<int64_t>("-1");
		});
		unit_test<"from_chars i64: -9">::assert_eq(ref_val<int64_t>("-9"), [] {
			return vn_val<int64_t>("-9");
		});
		unit_test<"from_chars i64: -10">::assert_eq(ref_val<int64_t>("-10"), [] {
			return vn_val<int64_t>("-10");
		});
		unit_test<"from_chars i64: -99">::assert_eq(ref_val<int64_t>("-99"), [] {
			return vn_val<int64_t>("-99");
		});
		unit_test<"from_chars i64: -100">::assert_eq(ref_val<int64_t>("-100"), [] {
			return vn_val<int64_t>("-100");
		});
		unit_test<"from_chars i64: -999">::assert_eq(ref_val<int64_t>("-999"), [] {
			return vn_val<int64_t>("-999");
		});
		unit_test<"from_chars i64: -123456789012345">::assert_eq(ref_val<int64_t>("-123456789012345"), [] {
			return vn_val<int64_t>("-123456789012345");
		});
		unit_test<"from_chars i64: -1234567890123456789">::assert_eq(ref_val<int64_t>("-1234567890123456789"), [] {
			return vn_val<int64_t>("-1234567890123456789");
		});
		unit_test<"from_chars i64: INT64_MIN+1">::assert_eq(ref_val<int64_t>("-9223372036854775807"), [] {
			return vn_val<int64_t>("-9223372036854775807");
		});
		unit_test<"from_chars i64: INT64_MIN">::assert_eq(ref_val<int64_t>("-9223372036854775808"), [] {
			return vn_val<int64_t>("-9223372036854775808");
		});
		unit_test<"from_chars i64: INT64_MAX">::assert_eq(ref_val<int64_t>("9223372036854775807"), [] {
			return vn_val<int64_t>("9223372036854775807");
		});
		unit_test<"from_chars i64: INT64_MAX-1">::assert_eq(ref_val<int64_t>("9223372036854775806"), [] {
			return vn_val<int64_t>("9223372036854775806");
		});
		unit_test<"from_chars i64: large positive 1234567890123456789">::assert_eq(ref_val<int64_t>("1234567890123456789"), [] {
			return vn_val<int64_t>("1234567890123456789");
		});

		unit_test<"from_chars i64 all signed branch boundaries">::assert_eq(true, [] {
			static constexpr const char* boundaries[] = { "-1", "-9", "-10", "-99", "-100", "-999", "-1000", "-9999", "-10000", "-99999", "-100000", "-999999", "-1000000",
				"-9999999", "-10000000", "-99999999", "-100000000", "-999999999", "-1000000000", "-9999999999", "-10000000000", "-99999999999", "-100000000000", "-999999999999",
				"-1000000000000", "-9999999999999", "-10000000000000", "-99999999999999", "-100000000000000", "-999999999999999", "-1000000000000000", "-9999999999999999",
				"-10000000000000000", "-99999999999999999", "-100000000000000000", "-999999999999999999", "-1000000000000000000", "-9223372036854775808", "-9223372036854775807",
				"9223372036854775807", "9223372036854775806" };
			for (auto s: boundaries) {
				if (ref_val<int64_t>(s) != vn_val<int64_t>(s))
					return false;
			}
			return true;
		});

		unit_test<"from_chars i64 stress 1000 random samples">::assert_eq(true, [] {
			std::mt19937_64 rng{ 0xBADCAFE5BABE5EEDULL };
			std::uniform_int_distribution<int64_t> dist{ std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max() };
			char buf[32]{};
			for (uint32_t i = 0; i < 1000; ++i) {
				int64_t v = dist(rng);
				auto r	  = std::to_chars(buf, buf + 32, v);
				*r.ptr	  = '\0';
				if (ref_val<int64_t>(buf) != vn_val<int64_t>(buf))
					return false;
			}
			return true;
		});

		unit_test<"from_chars i64 stress sweep -1000..1000">::assert_eq(true, [] {
			char buf[32]{};
			for (int64_t v = -1000; v < 1000; ++v) {
				auto r = std::to_chars(buf, buf + 32, v);
				*r.ptr = '\0';
				if (ref_val<int64_t>(buf) != vn_val<int64_t>(buf))
					return false;
			}
			return true;
		});

		unit_test<"from_chars i64 INT64_MIN_PLUS_K sweep">::assert_eq(true, [] {
			char buf[32]{};
			static constexpr int64_t base = std::numeric_limits<int64_t>::min();
			for (int64_t k = 0; k < 100; ++k) {
				int64_t v = base + k;
				auto r	  = std::to_chars(buf, buf + 32, v);
				*r.ptr	  = '\0';
				if (ref_val<int64_t>(buf) != vn_val<int64_t>(buf))
					return false;
			}
			return true;
		});

		unit_test<"from_chars i64 INT64_MAX_MINUS_K sweep">::assert_eq(true, [] {
			char buf[32]{};
			static constexpr int64_t base = std::numeric_limits<int64_t>::max();
			for (int64_t k = 0; k < 100; ++k) {
				int64_t v = base - k;
				auto r	  = std::to_chars(buf, buf + 32, v);
				*r.ptr	  = '\0';
				if (ref_val<int64_t>(buf) != vn_val<int64_t>(buf))
					return false;
			}
			return true;
		});

		unit_test<"from_chars ptr: u32 single digit consumes 1 char">::assert_eq(std::ptrdiff_t{ 1 }, [] {
			return vn_len<uint32_t>("7");
		});

		unit_test<"from_chars ptr: u32 UINT32_MAX consumes 10 chars">::assert_eq(std::ptrdiff_t{ 10 }, [] {
			return vn_len<uint32_t>("4294967295");
		});

		unit_test<"from_chars ptr: u64 single digit consumes 1 char">::assert_eq(std::ptrdiff_t{ 1 }, [] {
			return vn_len<uint64_t>("3");
		});

		unit_test<"from_chars ptr: u64 UINT64_MAX consumes 20 chars">::assert_eq(std::ptrdiff_t{ 20 }, [] {
			return vn_len<uint64_t>("18446744073709551615");
		});

		unit_test<"from_chars ptr: i32 -1 consumes 2 chars">::assert_eq(std::ptrdiff_t{ 2 }, [] {
			return vn_len<int32_t>("-1");
		});

		unit_test<"from_chars ptr: i64 INT64_MIN consumes 20 chars">::assert_eq(std::ptrdiff_t{ 20 }, [] {
			return vn_len<int64_t>("-9223372036854775808");
		});

		unit_test<"from_chars ptr: u32 stops at trailing non-digit">::assert_eq(std::ptrdiff_t{ 3 }, [] {
			return vn_len<uint32_t>("123abc");
		});

		unit_test<"from_chars ptr: u64 stops at trailing non-digit">::assert_eq(std::ptrdiff_t{ 5 }, [] {
			return vn_len<uint64_t>("98765xyz");
		});

		unit_test<"from_chars ptr: i32 stops at trailing non-digit">::assert_eq(std::ptrdiff_t{ 4 }, [] {
			return vn_len<int32_t>("-123abc");
		});

		unit_test<"from_chars ptr: i64 stops at trailing non-digit">::assert_eq(std::ptrdiff_t{ 7 }, [] {
			return vn_len<int64_t>("-654321xyz");
		});

		unit_test<"from_chars ptr: u32 matches ref on trailing garbage">::assert_eq(ref_len<uint32_t>("999x"), [] {
			return vn_len<uint32_t>("999x");
		});

		unit_test<"from_chars ptr: u64 matches ref on trailing garbage">::assert_eq(ref_len<uint64_t>("18446744073709551615z"), [] {
			return vn_len<uint64_t>("18446744073709551615z");
		});

		unit_test<"from_chars ptr: i32 matches ref on trailing garbage">::assert_eq(ref_len<int32_t>("-2147483648q"), [] {
			return vn_len<int32_t>("-2147483648q");
		});

		unit_test<"from_chars ptr: i64 matches ref on trailing garbage">::assert_eq(ref_len<int64_t>("-9223372036854775808q"), [] {
			return vn_len<int64_t>("-9223372036854775808q");
		});

		unit_test<"from_chars err: u32 overflow UINT32_MAX+1">::assert_eq(ref_ok<uint32_t>("4294967296"), [] {
			return vn_ok<uint32_t>("4294967296");
		});

		unit_test<"from_chars err: u32 overflow all-nines 11digit">::assert_eq(ref_ok<uint32_t>("99999999999"), [] {
			return vn_ok<uint32_t>("99999999999");
		});

		unit_test<"from_chars err: u64 overflow UINT64_MAX+1">::assert_eq(ref_ok<uint64_t>("18446744073709551616"), [] {
			return vn_ok<uint64_t>("18446744073709551616");
		});

		unit_test<"from_chars err: u64 overflow 21 nines">::assert_eq(ref_ok<uint64_t>("999999999999999999999"), [] {
			return vn_ok<uint64_t>("999999999999999999999");
		});

		unit_test<"from_chars err: i32 overflow INT32_MAX+1">::assert_eq(ref_ok<int32_t>("2147483648"), [] {
			return vn_ok<int32_t>("2147483648");
		});

		unit_test<"from_chars err: i32 underflow INT32_MIN-1">::assert_eq(ref_ok<int32_t>("-2147483649"), [] {
			return vn_ok<int32_t>("-2147483649");
		});

		unit_test<"from_chars err: i64 overflow INT64_MAX+1">::assert_eq(ref_ok<int64_t>("9223372036854775808"), [] {
			return vn_ok<int64_t>("9223372036854775808");
		});

		unit_test<"from_chars err: i64 underflow INT64_MIN-1">::assert_eq(ref_ok<int64_t>("-9223372036854775809"), [] {
			return vn_ok<int64_t>("-9223372036854775809");
		});

		unit_test<"from_chars err: u32 empty string">::assert_eq(ref_ok<uint32_t>(""), [] {
			return vn_ok<uint32_t>("");
		});

		unit_test<"from_chars err: u64 empty string">::assert_eq(ref_ok<uint64_t>(""), [] {
			return vn_ok<uint64_t>("");
		});

		unit_test<"from_chars err: i32 minus only">::assert_eq(ref_ok<int32_t>("-"), [] {
			return vn_ok<int32_t>("-");
		});

		unit_test<"from_chars err: i64 minus only">::assert_eq(ref_ok<int64_t>("-"), [] {
			return vn_ok<int64_t>("-");
		});

		unit_test<"from_chars err: u32 leading alpha">::assert_eq(ref_ok<uint32_t>("abc"), [] {
			return vn_ok<uint32_t>("abc");
		});

		unit_test<"from_chars err: u64 leading alpha">::assert_eq(ref_ok<uint64_t>("xyz"), [] {
			return vn_ok<uint64_t>("xyz");
		});

		unit_test<"from_chars err: u32 leading whitespace">::assert_eq(ref_ok<uint32_t>(" 42"), [] {
			return vn_ok<uint32_t>(" 42");
		});

		unit_test<"from_chars err: i32 leading whitespace">::assert_eq(ref_ok<int32_t>(" -1"), [] {
			return vn_ok<int32_t>(" -1");
		});

		unit_test<"from_chars err: u32 plus sign prefix">::assert_eq(ref_ok<uint32_t>("+1"), [] {
			return vn_ok<uint32_t>("+1");
		});

		unit_test<"from_chars err: i32 plus sign prefix">::assert_eq(ref_ok<int32_t>("+1"), [] {
			return vn_ok<int32_t>("+1");
		});

		unit_test<"from_chars err: u32 overflow ec matches ref on wide overflow">::assert_eq(ref_ok<uint32_t>("99999999999999999999"), [] {
			return vn_ok<uint32_t>("99999999999999999999");
		});

		unit_test<"from_chars err: i64 overflow ec matches ref on wide overflow">::assert_eq(ref_ok<int64_t>("99999999999999999999"), [] {
			return vn_ok<int64_t>("99999999999999999999");
		});

		unit_test<"from_chars roundtrip u32 1000 random">::assert_eq(true, [] {
			std::mt19937 rng{ 0xABCD1234U };
			std::uniform_int_distribution<uint32_t> dist{ 0, std::numeric_limits<uint32_t>::max() };
			char buf[16]{};
			const char* buf_ptr{ buf };
			for (uint32_t i = 0; i < 1000; ++i) {
				std::memset(buf, 0, sizeof(buf));
				buf_ptr	   = buf; 
				uint32_t v = dist(rng);
				auto tw	   = std::to_chars(buf, buf + 16, v);
				*tw.ptr	   = '\0';
				uint32_t out{};
				vn::from_chars(buf_ptr, tw.ptr, out);
				if (out != v)
					return false;
			}
			return true;
		});

		unit_test<"from_chars roundtrip u64 1000 random">::assert_eq(true, [] {
			std::mt19937_64 rng{ 0x1234ABCD5678EF00ULL };
			std::uniform_int_distribution<uint64_t> dist{ 0ULL, std::numeric_limits<uint64_t>::max() };
			char buf[32]{};
			const char* buf_ptr{ buf };
			for (uint32_t i = 0; i < 1000; ++i) {
				std::memset(buf, 0, sizeof(buf));
				buf_ptr	   = buf; 
				uint64_t v = dist(rng);
				auto tw	   = std::to_chars(buf, buf + 32, v);
				*tw.ptr	   = '\0';
				uint64_t out{};
				vn::from_chars(buf_ptr, tw.ptr, out);
				if (out != v)
					return false;
			}
			return true;
		});

		unit_test<"from_chars roundtrip i32 1000 random">::assert_eq(true, [] {
			std::mt19937 rng{ 0xDEAD5AFEU };
			std::uniform_int_distribution<int32_t> dist{ std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max() };
			char buf[16]{};
			const char* buf_ptr{ buf };
			for (uint32_t i = 0; i < 1000; ++i) {
				std::memset(buf, 0, sizeof(buf));
				buf_ptr	  = buf; 
				int32_t v = dist(rng);
				auto tw	  = std::to_chars(buf, buf + 16, v);
				*tw.ptr	  = '\0';
				int32_t out{};
				vn::from_chars(buf_ptr, tw.ptr, out);
				if (out != v)
					return false;
			}
			return true;
		});

		unit_test<"from_chars roundtrip i64 1000 random">::assert_eq(true, [] {
			std::mt19937_64 rng{ 0xCAFEBABEDEADBEEFULL };
			std::uniform_int_distribution<int64_t> dist{ std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max() };
			char buf[32]{};
			const char* buf_ptr{ buf };
			for (uint32_t i = 0; i < 1000; ++i) {
				std::memset(buf, 0, sizeof(buf));
				buf_ptr	  = buf; 
				int64_t v = dist(rng);
				auto tw	  = std::to_chars(buf, buf + 32, v);
				*tw.ptr	  = '\0';
				int64_t out{};
				vn::from_chars(buf_ptr, tw.ptr, out);
				if (out != v)
					return false;
			}
			return true;
		});

		unit_test<"from_chars roundtrip u32 sweep 0..2000">::assert_eq(true, [] {
			char buf[16]{};
			const char* buf_ptr{ buf };
			for (uint32_t v = 0; v < 2000; ++v) {
				std::memset(buf, 0, sizeof(buf));
				buf_ptr = buf; 
				auto tw = std::to_chars(buf, buf + 16, v);
				uint32_t out{};
				vn::from_chars(buf_ptr, tw.ptr, out);
				if (out != v)
					return false;
			}
			return true;
		});

		unit_test<"from_chars roundtrip i32 sweep -1000..1000">::assert_eq(true, [] {
			char buf[16]{};
			const char* buf_ptr{ buf };
			for (int32_t v = -1000; v < 1000; ++v) {
				std::memset(buf, 0, sizeof(buf));
				buf_ptr = buf; 
				auto tw = std::to_chars(buf, buf + 16, v);
				int32_t out{};
				vn::from_chars(buf_ptr, tw.ptr, out);
				if (out != v) {
					std::cout << "MY_VAL: " << out << std::endl;
					std::cout << "REF_VAL: " << v << std::endl;
					return false;
				}
			}
			return true;
		});

		unit_test<"from_chars roundtrip u32 all powers of 2">::assert_eq(true, [] {
			char buf[16]{};
			const char* buf_ptr{ buf };
			for (uint32_t i = 0; i < 32; ++i) {
				std::memset(buf, 0, sizeof(buf));
				buf_ptr	   = buf; 
				uint32_t v = 1U << i;
				auto tw	   = std::to_chars(buf, buf + 16, v);
				uint32_t out{};
				vn::from_chars(buf_ptr, tw.ptr, out);
				if (out != v)
					return false;
			}
			return true;
		});

		unit_test<"from_chars roundtrip u64 all powers of 2">::assert_eq(true, [] {
			char buf[32]{};
			const char* buf_ptr{ buf };
			for (uint32_t i = 0; i < 64; ++i) {
				std::memset(buf, 0, sizeof(buf));
				buf_ptr	   = buf; 
				uint64_t v = 1ULL << i;
				auto tw	   = std::to_chars(buf, buf + 32, v);
				uint64_t out{};
				vn::from_chars(buf_ptr, tw.ptr, out);
				if (out != v)
					return false;
			}
			return true;
		});

		std::cout << "=== Unit Tests Complete ===" << std::endl;
	}
};