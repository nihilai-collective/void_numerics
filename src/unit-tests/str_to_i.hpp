// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp

#pragma once

#include <void-numerics>
#include <iostream>
#include <charconv>
#include <random>
#include <string>
#include <cstring>
#include <system_error>

namespace vn_from_chars_tests {

	template<typename v_type> inline v_type ref_val(const char* s) {
		v_type v{};
		std::from_chars(s, s + std::strlen(s), v);
		return v;
	}
	template<typename v_type> inline v_type vn_val(const char* s) {
		v_type v{};
		vn::from_chars(s, s + std::strlen(s), v);
		return v;
	}
	template<typename v_type> inline std::ptrdiff_t ref_len(const char* s) {
		v_type v{};
		auto r = std::from_chars(s, s + std::strlen(s), v);
		return r.ptr - s;
	}
	template<typename v_type> inline std::ptrdiff_t vn_len(const char* s) {
		v_type v{};
		auto r = vn::from_chars(s, s + std::strlen(s), v);
		return r.ptr - s;
	}
	template<typename v_type> inline bool ref_ok(const char* s) {
		v_type v{};
		auto r = std::from_chars(s, s + std::strlen(s), v);
		return r.ptr == s + std::strlen(s);
	}
	template<typename v_type> inline bool vn_ok(const char* s) {
		v_type v{};
		auto r = vn::from_chars(s, s + std::strlen(s), v);
		return r.ptr == s + std::strlen(s);
	}

	template<rt_ut::string_literal name, vn::detail::integer_types v_type> void test_function() {
		rt_ut::unit_test<name + "-from_chars zero", true>::assert_eq(ref_val<v_type>("0"), [] {
			return vn_val<v_type>("0");
		});

		rt_ut::unit_test<name + "-from_chars leading zeros 00", true>::assert_eq(ref_val<v_type>("00"), [] {
			return vn_val<v_type>("00");
		});

		rt_ut::unit_test<name + "-from_chars leading zeros 000", true>::assert_eq(ref_val<v_type>("000"), [] {
			return vn_val<v_type>("000");
		});

		rt_ut::unit_test<name + "-from_chars leading zeros 007", true>::assert_eq(ref_val<v_type>("007"), [] {
			return vn_val<v_type>("007");
		});

		rt_ut::unit_test<name + "-from_chars leading zeros 0001", true>::assert_eq(ref_val<v_type>("0001"), [] {
			return vn_val<v_type>("0001");
		});

		rt_ut::unit_test<name + "-from_chars single digits 1-9", true>::assert_eq(true, [] {
			for (char c = '1'; c <= '9'; ++c) {
				char s[2]{ c, '\0' };
				if (ref_val<v_type>(s) != vn_val<v_type>(s))
					return false;
			}
			return true;
		});

		rt_ut::unit_test<name + "-from_chars powers of 2", true>::assert_eq(true, [] {
			char buf[32]{};
			for (uint32_t i = 0; i < sizeof(v_type) * 8 - (vn::detail::int_types<v_type> ? 1 : 0); ++i) {
				auto v = static_cast<v_type>(v_type{ 1 } << i);
				auto r = std::to_chars(buf, buf + 32, v);
				*r.ptr = '\0';
				if (ref_val<v_type>(buf) != vn_val<v_type>(buf))
					return false;
			}
			return true;
		});

		rt_ut::unit_test<name + "-from_chars powers of 10", true>::assert_eq(true, [] {
			char buf[32]{};
			v_type p = 1;
			for (uint32_t i = 0; i < unit_tests::max_digits_v<v_type>; ++i) {
				auto r = std::to_chars(buf, buf + 32, p);
				*r.ptr = '\0';
				if (ref_val<v_type>(buf) != vn_val<v_type>(buf))
					return false;
				if (p > std::numeric_limits<v_type>::max() / 10)
					break;
				p *= 10;
			}
			return true;
		});

		rt_ut::unit_test<name + "-from_chars all digit lengths at 111...1", true>::assert_eq(true, [] {
			char buf[32]{};
			v_type p = 0;
			for (uint32_t i = 0; i < unit_tests::max_digits_v<v_type>; ++i) {
				p	   = static_cast<v_type>(p * 10 + 1);
				auto r = std::to_chars(buf, buf + 32, p);
				*r.ptr = '\0';
				if (ref_val<v_type>(buf) != vn_val<v_type>(buf))
					return false;
			}
			return true;
		});

		rt_ut::unit_test<name + "-from_chars all digit lengths at 999...9", true>::assert_eq(true, [] {
			char buf[32]{};
			v_type p = 0;
			for (uint32_t i = 0; i < unit_tests::max_digits_v<v_type>; ++i) {
				p	   = static_cast<v_type>(p * 10 + 9);
				auto r = std::to_chars(buf, buf + 32, p);
				*r.ptr = '\0';
				if (ref_val<v_type>(buf) != vn_val<v_type>(buf))
					return false;
				if (p > std::numeric_limits<v_type>::max() / 10)
					break;
			}
			return true;
		});

		rt_ut::unit_test<name + "-from_chars max", true>::assert_eq(true, [] {
			char buf[32]{};
			auto r = std::to_chars(buf, buf + 32, std::numeric_limits<v_type>::max());
			*r.ptr = '\0';
			return ref_val<v_type>(buf) == vn_val<v_type>(buf);
		});

		rt_ut::unit_test<name + "-from_chars max-1", true>::assert_eq(true, [] {
			char buf[32]{};
			auto r = std::to_chars(buf, buf + 32, static_cast<v_type>(std::numeric_limits<v_type>::max() - 1));
			*r.ptr = '\0';
			return ref_val<v_type>(buf) == vn_val<v_type>(buf);
		});

		rt_ut::unit_test<name + "-from_chars max/2", true>::assert_eq(true, [] {
			char buf[32]{};
			auto r = std::to_chars(buf, buf + 32, static_cast<v_type>(std::numeric_limits<v_type>::max() / 2));
			*r.ptr = '\0';
			return ref_val<v_type>(buf) == vn_val<v_type>(buf);
		});

		rt_ut::unit_test<name + "-from_chars max/3", true>::assert_eq(true, [] {
			char buf[32]{};
			auto r = std::to_chars(buf, buf + 32, static_cast<v_type>(std::numeric_limits<v_type>::max() / 3));
			*r.ptr = '\0';
			return ref_val<v_type>(buf) == vn_val<v_type>(buf);
		});

		rt_ut::unit_test<name + "-from_chars max/7", true>::assert_eq(true, [] {
			char buf[32]{};
			auto r = std::to_chars(buf, buf + 32, static_cast<v_type>(std::numeric_limits<v_type>::max() / 7));
			*r.ptr = '\0';
			return ref_val<v_type>(buf) == vn_val<v_type>(buf);
		});

		rt_ut::unit_test<name + "-from_chars stop at non-digit", true>::assert_eq(ref_len<v_type>("123abc"), [] {
			return vn_len<v_type>("123abc");
		});

		rt_ut::unit_test<name + "-from_chars stop at space", true>::assert_eq(ref_len<v_type>("42 99"), [] {
			return vn_len<v_type>("42 99");
		});

		rt_ut::unit_test<name + "-from_chars stop at dot", true>::assert_eq(ref_len<v_type>("1.5"), [] {
			return vn_len<v_type>("1.5");
		});

		rt_ut::unit_test<name + "-from_chars ok full consume", true>::assert_eq(ref_ok<v_type>("123"), [] {
			return vn_ok<v_type>("123");
		});

		rt_ut::unit_test<name + "-from_chars ok partial consume", true>::assert_eq(ref_ok<v_type>("123abc"), [] {
			return vn_ok<v_type>("123abc");
		});

		rt_ut::unit_test<name + "-from_chars ok leading space", true>::assert_eq(ref_ok<v_type>(" 123"), [] {
			return vn_ok<v_type>(" 123");
		});

		rt_ut::unit_test<name + "-from_chars ok zero", true>::assert_eq(ref_ok<v_type>("0"), [] {
			return vn_ok<v_type>("0");
		});

		rt_ut::unit_test<name + "-from_chars len zero", true>::assert_eq(ref_len<v_type>("0"), [] {
			return vn_len<v_type>("0");
		});

		rt_ut::unit_test<name + "-from_chars len leading zeros 007", true>::assert_eq(ref_len<v_type>("007"), [] {
			return vn_len<v_type>("007");
		});

		if constexpr (sizeof(v_type) <= 2) {
			rt_ut::unit_test<name + "-from_chars exhaustive all values", true>::assert_eq(true, [] {
				char buf[32]{};
				v_type v = std::numeric_limits<v_type>::min();
				do {
					auto r = std::to_chars(buf, buf + 32, v);
					*r.ptr = '\0';
					if (ref_val<v_type>(buf) != vn_val<v_type>(buf))
						return false;
					if (v == std::numeric_limits<v_type>::max())
						break;
					++v;
				} while (true);
				return true;
			});
		}

		if constexpr (vn::detail::int_types<v_type>) {
			rt_ut::unit_test<name + "-from_chars -1", true>::assert_eq(ref_val<v_type>("-1"), [] {
				return vn_val<v_type>("-1");
			});

			rt_ut::unit_test<name + "-from_chars min", true>::assert_eq(true, [] {
				char buf[32]{};
				auto r = std::to_chars(buf, buf + 32, std::numeric_limits<v_type>::min());
				*r.ptr = '\0';
				return ref_val<v_type>(buf) == vn_val<v_type>(buf);
			});

			rt_ut::unit_test<name + "-from_chars min+1", true>::assert_eq(true, [] {
				char buf[32]{};
				auto r = std::to_chars(buf, buf + 32, static_cast<v_type>(std::numeric_limits<v_type>::min() + 1));
				*r.ptr = '\0';
				return ref_val<v_type>(buf) == vn_val<v_type>(buf);
			});

			rt_ut::unit_test<name + "-from_chars min/2", true>::assert_eq(true, [] {
				char buf[32]{};
				auto r = std::to_chars(buf, buf + 32, static_cast<v_type>(std::numeric_limits<v_type>::min() / 2));
				*r.ptr = '\0';
				return ref_val<v_type>(buf) == vn_val<v_type>(buf);
			});

			rt_ut::unit_test<name + "-from_chars negative powers of 2", true>::assert_eq(true, [] {
				char buf[32]{};
				for (uint32_t i = 0; i < sizeof(v_type) * 8 - 1; ++i) {
					auto v = static_cast<v_type>(-(v_type{ 1 } << i));
					auto r = std::to_chars(buf, buf + 32, v);
					*r.ptr = '\0';
					if (ref_val<v_type>(buf) != vn_val<v_type>(buf))
						return false;
				}
				return true;
			});

			rt_ut::unit_test<name + "-from_chars negative powers of 10", true>::assert_eq(true, [] {
				char buf[32]{};
				v_type p = -1;
				for (uint32_t i = 0; i < unit_tests::max_digits_v<v_type>; ++i) {
					auto r = std::to_chars(buf, buf + 32, p);
					*r.ptr = '\0';
					if (ref_val<v_type>(buf) != vn_val<v_type>(buf))
						return false;
					if (p < std::numeric_limits<v_type>::min() / 10)
						break;
					p *= 10;
				}
				return true;
			});

			rt_ut::unit_test<name + "-from_chars negative all digit lengths at -111...1", true>::assert_eq(true, [] {
				char buf[32]{};
				v_type p = 0;
				for (uint32_t i = 0; i < unit_tests::max_digits_v<v_type>; ++i) {
					p	   = static_cast<v_type>(p * 10 - 1);
					auto r = std::to_chars(buf, buf + 32, p);
					*r.ptr = '\0';
					if (ref_val<v_type>(buf) != vn_val<v_type>(buf))
						return false;
					if (p < std::numeric_limits<v_type>::min() / 10)
						break;
				}
				return true;
			});

			rt_ut::unit_test<name + "-from_chars ok negative full consume", true>::assert_eq(ref_ok<v_type>("-42"), [] {
				return vn_ok<v_type>("-42");
			});

			rt_ut::unit_test<name + "-from_chars ok lone minus", true>::assert_eq(ref_ok<v_type>("-"), [] {
				return vn_ok<v_type>("-");
			});

			rt_ut::unit_test<name + "-from_chars len lone minus", true>::assert_eq(ref_len<v_type>("-"), [] {
				return vn_len<v_type>("-");
			});

			rt_ut::unit_test<name + "-from_chars stop after negative number", true>::assert_eq(ref_len<v_type>("-99xyz"), [] {
				return vn_len<v_type>("-99xyz");
			});
		}

		rt_ut::unit_test<name + "-from_chars round-trip via to_chars", true>::assert_eq(true, [] {
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
				auto end = std::to_chars(buf, buf + 32, v);
				*end.ptr = '\0';
				if (ref_val<v_type>(buf) != vn_val<v_type>(buf))
					return false;
			}
			return true;
		});

		if constexpr (vn::detail::int_types<v_type>) {
			rt_ut::unit_test<name + "-from_chars round-trip signed via to_chars", true>::assert_eq(true, [] {
				static constexpr v_type test_vals[] = {
					static_cast<v_type>(-1),
					static_cast<v_type>(std::numeric_limits<v_type>::min()),
					static_cast<v_type>(std::numeric_limits<v_type>::min() + 1),
					static_cast<v_type>(std::numeric_limits<v_type>::min() / 2),
				};
				for (auto v: test_vals) {
					char buf[32]{};
					auto end = std::to_chars(buf, buf + 32, v);
					*end.ptr = '\0';
					if (ref_val<v_type>(buf) != vn_val<v_type>(buf))
						return false;
				}
				return true;
			});
		}
	}
}

template<> struct unit_tests::tests<vn::detail::conversion_classes::str_to_i> {
	static void impl() {
		vn_from_chars_tests::test_function<"uint8", uint8_t>();
		vn_from_chars_tests::test_function<"int8", int8_t>();
		vn_from_chars_tests::test_function<"uint16", uint16_t>();
		vn_from_chars_tests::test_function<"int16", int16_t>();
		vn_from_chars_tests::test_function<"uint32", uint32_t>();
		vn_from_chars_tests::test_function<"int32", int32_t>();
		vn_from_chars_tests::test_function<"uint64", uint64_t>();
		vn_from_chars_tests::test_function<"int64", int64_t>();
	}
};
