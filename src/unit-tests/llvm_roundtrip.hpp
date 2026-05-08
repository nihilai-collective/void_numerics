//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// ADDITIONAL_COMPILE_FLAGS(has-fconstexpr-steps): -fconstexpr-steps=12712420

// <charconv>

// constexpr from_chars_result from_chars(const char* first, const char* last,
//                                        Integral& value, int32_t base = 10)

#include <charconv>
#include <system_error>

#include "test_macros.hpp"
#include "charconv_test_helpers.hpp"

namespace roundtrip_llvm_tests {

	template<typename v_type> struct test_basics : roundtrip_test_base<v_type> {
		using roundtrip_test_base<v_type>::test;

		TEST_CONSTEXPR_CXX23 void operator()() {
			test(0);
			test(42);
			test(32768);
			test(0, 10);
			test(42, 10);
			test(32768, 10);
			test(0xf, 16);
			test(0xdeadbeaf, 16);
			test(0755, 8);

			for (int32_t b = 2; b < 37; ++b) {
				using xl = std::numeric_limits<v_type>;

				test(1, b);
				test(-1, b);
				test(xl::lowest(), b);
				test(( xl::max )(), b);
				test(( xl::max )() / 2, b);
			}
		}
	};

	template<typename v_type> struct test_signed : roundtrip_test_base<v_type> {
		using roundtrip_test_base<v_type>::test;

		TEST_CONSTEXPR_CXX23 void operator()() {
			test(-1);
			test(-12);
			test(-1, 10);
			test(-12, 10);
			test(-21734634, 10);
			test(-2647, 2);
			test(-0xcc1, 16);

			for (int32_t b = 2; b < 37; ++b) {
				using xl = std::numeric_limits<v_type>;

				test(0, b);
				test(xl::lowest(), b);
				test(( xl::max )(), b);
			}
		}
	};

	TEST_CONSTEXPR_CXX23 static bool test() {
		std::cout << "Running LLVM Roundtrip tests" << std::endl;
		run<test_basics>(integrals);
		run<test_signed>(all_signed);
		std::cout << "LLVM Roundtrip tests complete" << std::endl;
		return true;
	}

}
