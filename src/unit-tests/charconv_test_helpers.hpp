//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// unit-tests/charconv_test_helpers.hpp

#ifndef SUPPORT_CHARCONV_TEST_HELPERS_H
#define SUPPORT_CHARCONV_TEST_HELPERS_H

#include <assert.h>
#include <cerrno>
#include <charconv>
#include <cstddef>
#include <ostream>
#include <string>
#include <limits>
#include <numeric>
#include <string.h>
#include <stdlib.h>
#include <type_traits>

#include <void-numerics>

#include <rt-ut-incl/index.hpp>

#include "test_macros.hpp"

#if TEST_STD_VER < 11
	#error This file requires C++11
#endif

using std::false_type;
using std::true_type;

template<typename To, typename From> constexpr auto is_non_narrowing(From a) -> decltype(To{ a }, true_type()) {
	return {};
}

template<typename To> constexpr auto is_non_narrowing(...) -> false_type {
	return {};
}

template<typename X, typename v_type, typename A, typename B> constexpr bool fits_in(v_type, true_type, A, B) {
	return true;
}

template<typename X, typename v_type, typename xl = std::numeric_limits<X>> constexpr bool fits_in(v_type v, false_type, true_type /* v_type signed*/, true_type /* X signed */) {
	return xl::lowest() <= v && v <= ( xl::max )();
}

template<typename X, typename v_type, typename xl = std::numeric_limits<X>>
constexpr bool fits_in(v_type v, false_type, true_type /* v_type signed */, false_type /* X unsigned*/) {
	return 0 <= v && typename std::make_unsigned<v_type>::type(v) <= ( xl::max )();
}

template<typename X, typename v_type, typename xl = std::numeric_limits<X>> constexpr bool fits_in(v_type v, false_type, false_type, false_type) {
	return v <= typename std::make_unsigned<X>::type(( xl::max )());
}
template<typename X, typename v_type, typename xl = std::numeric_limits<X>> constexpr bool fits_in(v_type v, false_type, false_type, true_type) {
	return v <= typename std::make_unsigned<X>::type(( xl::max )());
}

template<typename X, typename v_type> constexpr bool fits_in(v_type v) {
	return fits_in<X>(v, is_non_narrowing<X>(v), std::is_signed<v_type>{}, std::is_signed<X>{});
}

template<typename X> struct to_chars_test_base {
	template<typename v_type, std::size_t N, typename... Ts> TEST_CONSTEXPR_CXX23 void test(v_type v, [[maybe_unused]] char const (&expect)[N], Ts... args) {
		std::to_chars_result r;

		constexpr std::size_t len = N - 1;
		static_assert(len > 0, "expected output won't be empty");

		if (!fits_in<X>(v))
			return;

		r = vn::to_chars(buf, buf + len - 1, X(v), args...);
		assert(r.ptr == buf + len - 1);

		r = vn::to_chars(buf, buf + sizeof(buf), X(v), args...);
		assert(r.ptr == buf + len);
		assert(r.ec == std::errc{});
		assert(std::equal(buf, buf + len, expect));
	}

	template<typename... Ts> TEST_CONSTEXPR_CXX23 void test_value(X v, Ts... args) {
		std::to_chars_result r;
		std::iota(buf, buf + sizeof(buf), static_cast<uint8_t>(1));
		r = vn::to_chars(buf, buf + sizeof(buf), v, args...);
		assert(r.ec == std::errc{});
		for (auto i = r.ptr - buf; i < static_cast<decltype(i)>(sizeof(buf)); ++i)
			assert(static_cast<uint8_t>(buf[static_cast<uint64_t>(i)]) == i + 1);
		if (r.ptr >= buf && r.ptr < buf + sizeof(buf)) {
			*r.ptr = '\0';
		}
		{
			[[maybe_unused]] auto a = fromchars_impl(buf, r.ptr, args...);
			assert(v == a);
		}

		auto ep = r.ptr - 1;
		r		= vn::to_chars(buf, ep, v, args...);
		assert(r.ptr == ep);
	}

  private:
	static TEST_CONSTEXPR_CXX23 long long fromchars_impl(char const* p, char const* ep, int32_t base, true_type) {
		long long r;
		[[maybe_unused]] char* last = const_cast<char*>(vn::from_chars(p, ep, r, base).ptr);
		assert(last == ep);

		return r;
	}

	static TEST_CONSTEXPR_CXX23 uint64_t fromchars_impl(char const* p, char const* ep, int32_t base, false_type) {
		uint64_t r;
		[[maybe_unused]] char* last = const_cast<char*>(vn::from_chars(p, ep, r, base).ptr);
		assert(last == ep);

		return r;
	}

	static TEST_CONSTEXPR_CXX23 auto fromchars_impl(char const* p, char const* ep, int32_t base = 10) -> decltype(fromchars_impl(p, ep, base, std::is_signed<X>())) {
		return fromchars_impl(p, ep, base, std::is_signed<X>());
	}

	char buf[150];
};

template<typename X> struct roundtrip_test_base {
	template<typename v_type, typename... Ts> TEST_CONSTEXPR_CXX23 void test(v_type v, Ts... args) {
		std::from_chars_result r2;
		std::to_chars_result r;
		X x = 0xc;

		if (fits_in<X>(v)) {
			r = vn::to_chars(buf, buf + sizeof(buf), v, args...);

			r2 = vn::from_chars(buf, r.ptr, x, args...);
			assert(r2.ptr == r.ptr);
			assert(x == X(v));
		} else {
			r = vn::to_chars(buf, buf + sizeof(buf), v, args...);

			r2 = vn::from_chars(buf, r.ptr, x, args...);

			TEST_DIAGNOSTIC_PUSH
			TEST_MSVC_DIAGNOSTIC_IGNORED(4127)

			if (std::is_signed<v_type>::value && v < 0 && std::is_unsigned<X>::value) {
				assert(x == 0xc);
				assert(r2.ptr == buf);
			} else {
				assert(x == 0xc);
				assert(r2.ptr == r.ptr);
			}

			TEST_DIAGNOSTIC_POP
		}
	}

  private:
	char buf[150];
};

template<typename... v_type> struct type_list {};

template<typename L1, typename L2> struct type_concat;

template<typename... Xs, typename... Ys> struct type_concat<type_list<Xs...>, type_list<Ys...>> {
	using type = type_list<Xs..., Ys...>;
};

template<typename L1, typename L2> using concat_t = typename type_concat<L1, L2>::type;

template<typename L1, typename L2> constexpr auto concat(L1, L2) -> concat_t<L1, L2> {
	return {};
}

static constexpr auto all_floats = type_list<float, double>();
static constexpr auto all_signed   = type_list<char, signed char, short, int32_t, long, long long>();
static constexpr auto all_unsigned = type_list<uint8_t, unsigned short, uint32_t, uint64_t, uint64_t>();
static constexpr auto integrals	   = concat(all_unsigned, all_signed);

template<template<typename> class Fn, typename... Ts> TEST_CONSTEXPR_CXX23 void run(type_list<Ts...>) {
	int32_t ls[sizeof...(Ts)] = { (Fn<Ts>{}(), 0)... };
	( void )ls;
}

#endif
