// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp

#pragma once

#include "utility.hpp"
#include <void-numerics>
#include <iostream>
#include <charconv>
#include <random>
#include <string>

namespace vn_to_chars_tests {

	template<typename v_type> inline bool chars_match(v_type v) {
		char ref[32]{};
		char tst[32]{};
		auto r1		 = std::to_chars(ref, ref + 32, v);
		auto r2		 = vn::to_chars(tst, tst + 32, v);
		auto ref_len = static_cast<uint64_t>(r1.ptr - ref);
		auto tst_len = static_cast<uint64_t>(r2.ptr - tst);
		return ref_len == tst_len && std::memcmp(ref, tst, ref_len) == 0;
	}

	template<rt_ut::string_literal name, vn::detail::integer_types v_type> void test_function() {
		rt_ut::unit_test<name + "-to_chars zero", true>::assert_eq(true, [] {
			return chars_match<v_type>(0);
		});

		rt_ut::unit_test<name + "-to_chars single digits 1-9", true>::assert_eq(true, [] {
			for (v_type i = 1; i <= 9; ++i)
				if (!chars_match<v_type>(i))
					return false;
			return true;
		});

		rt_ut::unit_test<name + "-to_chars powers of 2", true>::assert_eq(true, [] {
			for (uint32_t i = 0; i < sizeof(v_type) * 8 - (vn::detail::int_types<v_type> ? 1 : 0); ++i)
				if (!chars_match<v_type>(static_cast<v_type>(v_type{ 1 } << i)))
					return false;
			return true;
		});

		rt_ut::unit_test<name + "-to_chars powers of 10", true>::assert_eq(true, [] {
			v_type p = 1;
			for (uint32_t i = 0; i < unit_tests::max_digits_v<v_type>; ++i) {
				if (!chars_match<v_type>(p))
					return false;
				if (p > std::numeric_limits<v_type>::max() / 10)
					break;
				p *= 10;
			}
			return true;
		});

		rt_ut::unit_test<name + "-to_chars all digit lengths at 111...1", true>::assert_eq(true, [] {
			v_type p = 0;
			for (uint32_t i = 0; i < unit_tests::max_digits_v<v_type>; ++i) {
				p = static_cast<v_type>(p * 10 + 1);
				if (!chars_match<v_type>(p))
					return false;
			}
			return true;
		});

		rt_ut::unit_test<name + "-to_chars all digit lengths at 999...9", true>::assert_eq(true, [] {
			v_type p = 0;
			for (uint32_t i = 0; i < unit_tests::max_digits_v<v_type>; ++i) {
				p = static_cast<v_type>(p * 10 + 9);
				if (p < 0 || !chars_match<v_type>(p))
					return false;
				if (p > std::numeric_limits<v_type>::max() / 10)
					break;
			}
			return true;
		});

		rt_ut::unit_test<name + "-to_chars max", true>::assert_eq(true, [] {
			return chars_match<v_type>(std::numeric_limits<v_type>::max());
		});

		rt_ut::unit_test<name + "-to_chars max-1", true>::assert_eq(true, [] {
			return chars_match<v_type>(static_cast<v_type>(std::numeric_limits<v_type>::max() - 1));
		});

		rt_ut::unit_test<name + "-to_chars max/2", true>::assert_eq(true, [] {
			return chars_match<v_type>(static_cast<v_type>(std::numeric_limits<v_type>::max() / 2));
		});

		rt_ut::unit_test<name + "-to_chars max/3", true>::assert_eq(true, [] {
			return chars_match<v_type>(static_cast<v_type>(std::numeric_limits<v_type>::max() / 3));
		});

		rt_ut::unit_test<name + "-to_chars max/7", true>::assert_eq(true, [] {
			return chars_match<v_type>(static_cast<v_type>(std::numeric_limits<v_type>::max() / 7));
		});

		if constexpr (sizeof(v_type) <= 2) {
			rt_ut::unit_test<name + "-to_chars exhaustive all values", true>::assert_eq(true, [] {
				v_type v = std::numeric_limits<v_type>::min();
				do {
					if (!chars_match<v_type>(v))
						return false;
					if (v == std::numeric_limits<v_type>::max())
						break;
					++v;
				} while (true);
				return true;
			});
		}

		if constexpr (vn::detail::int_types<v_type>) {
			rt_ut::unit_test<name + "-to_chars min", true>::assert_eq(true, [] {
				return chars_match<v_type>(std::numeric_limits<v_type>::min());
			});

			rt_ut::unit_test<name + "-to_chars min+1", true>::assert_eq(true, [] {
				return chars_match<v_type>(static_cast<v_type>(std::numeric_limits<v_type>::min() + 1));
			});

			rt_ut::unit_test<name + "-to_chars min/2", true>::assert_eq(true, [] {
				return chars_match<v_type>(static_cast<v_type>(std::numeric_limits<v_type>::min() / 2));
			});

			rt_ut::unit_test<name + "-to_chars -1", true>::assert_eq(true, [] {
				return chars_match<v_type>(static_cast<v_type>(-1));
			});

			rt_ut::unit_test<name + "-to_chars negative powers of 2", true>::assert_eq(true, [] {
				for (uint32_t i = 0; i < sizeof(v_type) * 8 - 1; ++i)
					if (!chars_match<v_type>(static_cast<v_type>(-(v_type{ 1 } << i))))
						return false;
				return true;
			});

			rt_ut::unit_test<name + "-to_chars negative powers of 10", true>::assert_eq(true, [] {
				v_type p = -1;
				for (uint32_t i = 0; i < unit_tests::max_digits_v<v_type>; ++i) {
					if (!chars_match<v_type>(p))
						return false;
					if (p < std::numeric_limits<v_type>::min() / 10)
						break;
					p *= 10;
				}
				return true;
			});

			rt_ut::unit_test<name + "-to_chars negative all digit lengths at -111...1", true>::assert_eq(true, [] {
				v_type p = 0;
				for (uint32_t i = 0; i < unit_tests::max_digits_v<v_type>; ++i) {
					p = static_cast<v_type>(p * 10 - 1);
					if (!chars_match<v_type>(p))
						return false;
					if (p < std::numeric_limits<v_type>::min() / 10)
						break;
				}
				return true;
			});
		}

		rt_ut::unit_test<name + "-to_chars round-trip via from_chars", true>::assert_eq(true, [] {
			static constexpr v_type test_vals[] = {
				0,
				1,
				static_cast<v_type>(std::numeric_limits<v_type>::max()),
				static_cast<v_type>(std::numeric_limits<v_type>::max() / 2),
				static_cast<v_type>(std::numeric_limits<v_type>::max() / 3),
				static_cast<v_type>(std::numeric_limits<v_type>::max() - 1),
			};
			for (auto v: test_vals) {
				char buf[32]{};
				auto end = vn::to_chars(buf, buf + 32, v);
				v_type parsed{};
				vn::from_chars(buf, end.ptr, parsed);
				if (parsed != v)
					return false;
			}
			return true;
		});

		if constexpr (vn::detail::int_types<v_type>) {
			rt_ut::unit_test<name + "-to_chars round-trip signed via from_chars", true>::assert_eq(true, [] {
				static constexpr v_type test_vals[] = {
					static_cast<v_type>(-1),
					static_cast<v_type>(std::numeric_limits<v_type>::min()),
					static_cast<v_type>(std::numeric_limits<v_type>::min() + 1),
					static_cast<v_type>(std::numeric_limits<v_type>::min() / 2),
				};
				for (auto v: test_vals) {
					char buf[32]{};
					auto end = vn::to_chars(buf, buf + 32, v);
					v_type parsed{};
					vn::from_chars(buf, end.ptr, parsed);
					if (parsed != v)
						return false;
				}
				return true;
			});
		}
	}

}

template<> struct unit_tests::tests<vn::detail::conversion_classes::i_to_str> {
	static void impl() {
		vn_to_chars_tests::test_function<"uint8", uint8_t>();
		vn_to_chars_tests::test_function<"int8", int8_t>();
		vn_to_chars_tests::test_function<"uint16", uint16_t>();
		vn_to_chars_tests::test_function<"int16", int16_t>();
		vn_to_chars_tests::test_function<"uint32", uint32_t>();
		vn_to_chars_tests::test_function<"int32", int32_t>();
		vn_to_chars_tests::test_function<"uint64", uint64_t>();
		vn_to_chars_tests::test_function<"int64", int64_t>();
	}
};
