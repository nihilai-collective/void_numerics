//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <charconv>

// constexpr from_chars_result from_chars(const char* first, const char* last,
//                                        Integral& value, int base = 10)

#include <charconv>
#include <system_error>

#include "test_macros.hpp"
#include "charconv_test_helpers.hpp"

namespace integral_pass_llvm_tests {

	template<typename v_type> struct test_basics {
		TEST_CONSTEXPR_CXX23 void operator()() {
			std::from_chars_result r;
			v_type x;

			{
				char s[] = "001x";
				r		 = vn::from_chars(s, s + sizeof(s), x);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 3);
				assert(x == 1);
			}

			{
				char s[] = "0X7BAtSGHDkEIXZgQRfYChLpOzRnM ";
				r		 = vn::from_chars(s, s + sizeof(s), x, 36);
				assert(r.ec == std::errc::result_out_of_range);
				assert(r.ptr == s + sizeof(s) - 2);
				assert(x == 1);
				r = vn::from_chars(s, s + sizeof(s), x, 16);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 1);
				assert(x == 0);
				r = vn::from_chars(s + 2, s + sizeof(s), x, 12);
				if (!fits_in<v_type>(1150)) {
					assert(x == 0);
					assert(r.ec == std::errc::result_out_of_range);
				} else {
					assert(x == 1150);
					assert(r.ec == std::errc{});
				}
				assert(r.ptr == s + 5);
			}
		}
	};

	template<typename v_type> struct test_signed {
		TEST_CONSTEXPR_CXX23 void operator()() {
			std::from_chars_result r;
			v_type x = 42;

			{
				char s[] = "- 9+12";
				r		 = vn::from_chars(s, s + sizeof(s), x);
				assert(x == 42);
				assert(r.ptr == s);
				assert(r.ec == std::errc::invalid_argument);
			}

			{
				char s[] = "9+12";
				r		 = vn::from_chars(s, s + sizeof(s), x);
				assert(r.ec == std::errc{});

				assert(r.ptr == s + 1);
				assert(x == 9);
			}

			{
				char s[] = "12";
				r		 = vn::from_chars(s, s + 2, x);
				assert(r.ec == std::errc{});

				assert(r.ptr == s + 2);
				assert(x == 12);
			}

			{
				char s[] = "+30";

				r = vn::from_chars(s, s + sizeof(s), x);
				assert(x == 12);
				assert(r.ptr == s);
				assert(r.ec == std::errc::invalid_argument);
			}
		}
	};

	TEST_CONSTEXPR_CXX23 static bool test() {
		std::cout << "Running LLVM Integral-Pass tests" << std::endl;
		run<test_basics>(integrals);
		run<test_signed>(all_signed);
		std::cout << "LLVM Integral-Pass tests complete" << std::endl;
		return true;
	}

}
