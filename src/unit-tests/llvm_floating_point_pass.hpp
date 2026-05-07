//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// XFAIL: availability-fp_from_chars-missing

// from_chars_result from_chars(const char* first, const char* last,
//                              float& value, chars_format fmt = chars_format::general)
//
// from_chars_result from_chars(const char* first, const char* last,
//                              double& value, chars_format fmt = chars_format::general)

#pragma once

#include <array>
#include <charconv>
#include <cmath>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <system_error>

#include "charconv_test_helpers.hpp"
#include "test_macros.hpp"

namespace floating_point_pass_llvm_tests {

	template<class F> void test_infinity(std::chars_format fmt) {
		const char* s = "-InFiNiTyXXX";
		{
			F value						  = 0.25;
			std::from_chars_result result = std::from_chars(s + 1, s + 2, value, fmt);

			assert(result.ec == std::errc::invalid_argument);
			assert(result.ptr == s + 1);
			assert(value == F(0.25));
		}
		{
			F value						  = 0.25;
			std::from_chars_result result = std::from_chars(s + 1, s + 3, value, fmt);

			assert(result.ec == std::errc::invalid_argument);
			assert(result.ptr == s + 1);
			assert(value == F(0.25));
		}
		{
			F value						  = 0.25;
			std::from_chars_result result = std::from_chars(s + 1, s + 4, value, fmt);

			assert(result.ec == std::errc{});
			assert(result.ptr == s + 4);
			assert(value == std::numeric_limits<F>::infinity());
		}
		{
			F value						  = 0.25;
			std::from_chars_result result = std::from_chars(s, s + 4, value, fmt);

			assert(result.ec == std::errc{});
			assert(result.ptr == s + 4);
			assert(value == -std::numeric_limits<F>::infinity());
		}
		{
			F value						  = 0.25;
			std::from_chars_result result = std::from_chars(s + 1, s + 5, value, fmt);

			assert(result.ec == std::errc{});
			assert(result.ptr == s + 4);
			assert(value == std::numeric_limits<F>::infinity());
		}
		{
			F value						  = 0.25;
			std::from_chars_result result = std::from_chars(s, s + 6, value, fmt);

			assert(result.ec == std::errc{});
			assert(result.ptr == s + 4);
			assert(value == -std::numeric_limits<F>::infinity());
		}
		{
			F value						  = 0.25;
			std::from_chars_result result = std::from_chars(s + 1, s + 7, value, fmt);

			assert(result.ec == std::errc{});
			assert(result.ptr == s + 4);
			assert(value == std::numeric_limits<F>::infinity());
		}
		{
			F value						  = 0.25;
			std::from_chars_result result = std::from_chars(s, s + 8, value, fmt);

			assert(result.ec == std::errc{});
			assert(result.ptr == s + 4);
			assert(value == -std::numeric_limits<F>::infinity());
		}
		{
			F value						  = 0.25;
			std::from_chars_result result = std::from_chars(s + 1, s + 9, value, fmt);

			assert(result.ec == std::errc{});
			assert(result.ptr == s + 9);
			assert(value == std::numeric_limits<F>::infinity());
		}
		{
			F value						  = 0.25;
			std::from_chars_result result = std::from_chars(s, s + 9, value, fmt);

			assert(result.ec == std::errc{});
			assert(result.ptr == s + 9);
			assert(value == -std::numeric_limits<F>::infinity());
		}
		{
			F value						  = 0.25;
			std::from_chars_result result = std::from_chars(s + 1, s + 12, value, fmt);

			assert(result.ec == std::errc{});
			assert(result.ptr == s + 9);
			assert(value == std::numeric_limits<F>::infinity());
		}
		{
			F value						  = 0.25;
			std::from_chars_result result = std::from_chars(s, s + 12, value, fmt);

			assert(result.ec == std::errc{});
			assert(result.ptr == s + 9);
			assert(value == -std::numeric_limits<F>::infinity());
		}
	}

	template<class F> void test_nan(std::chars_format fmt) {
		{
			const char* s = "-NaN(1_A)XXX";
			{
				F value						  = 0.25;
				std::from_chars_result result = std::from_chars(s + 1, s + 2, value, fmt);

				assert(result.ec == std::errc::invalid_argument);
				assert(result.ptr == s + 1);
				assert(value == F(0.25));
			}
			{
				F value						  = 0.25;
				std::from_chars_result result = std::from_chars(s + 1, s + 3, value, fmt);

				assert(result.ec == std::errc::invalid_argument);
				assert(result.ptr == s + 1);
				assert(value == F(0.25));
			}
			{
				F value						  = 0.25;
				std::from_chars_result result = std::from_chars(s + 1, s + 4, value, fmt);

				assert(result.ec == std::errc{});
				assert(result.ptr == s + 4);
				assert(std::isnan(value));
				assert(!std::signbit(value));
			}
			{
				F value						  = 0.25;
				std::from_chars_result result = std::from_chars(s + 0, s + 4, value, fmt);

				assert(result.ec == std::errc{});
				assert(result.ptr == s + 4);
				assert(std::isnan(value));
				assert(std::signbit(value));
			}
			{
				F value						  = 0.25;
				std::from_chars_result result = std::from_chars(s + 1, s + 5, value, fmt);

				assert(result.ec == std::errc{});
				assert(result.ptr == s + 4);
				assert(std::isnan(value));
				assert(!std::signbit(value));
			}
			{
				F value						  = 0.25;
				std::from_chars_result result = std::from_chars(s, s + 6, value, fmt);

				assert(result.ec == std::errc{});
				assert(result.ptr == s + 4);
				assert(std::isnan(value));
				assert(std::signbit(value));
			}
			{
				F value						  = 0.25;
				std::from_chars_result result = std::from_chars(s + 1, s + 7, value, fmt);

				assert(result.ec == std::errc{});
				assert(result.ptr == s + 4);
				assert(std::isnan(value));
				assert(!std::signbit(value));
			}
			{
				F value						  = 0.25;
				std::from_chars_result result = std::from_chars(s, s + 8, value, fmt);

				assert(result.ec == std::errc{});
				assert(result.ptr == s + 4);
				assert(std::isnan(value));
				assert(std::signbit(value));
			}
			{
				F value						  = 0.25;
				std::from_chars_result result = std::from_chars(s + 1, s + 9, value, fmt);

				assert(result.ec == std::errc{});
				assert(result.ptr == s + 9);
				assert(std::isnan(value));
				assert(!std::signbit(value));
			}
			{
				F value						  = 0.25;
				std::from_chars_result result = std::from_chars(s, s + 9, value, fmt);

				assert(result.ec == std::errc{});
				assert(result.ptr == s + 9);
				assert(std::isnan(value));
				assert(std::signbit(value));
			}
			{
				F value						  = 0.25;
				std::from_chars_result result = std::from_chars(s + 1, s + 12, value, fmt);

				assert(result.ec == std::errc{});
				assert(result.ptr == s + 9);
				assert(std::isnan(value));
				assert(!std::signbit(value));
			}
			{
				F value						  = 0.25;
				std::from_chars_result result = std::from_chars(s, s + 12, value, fmt);

				assert(result.ec == std::errc{});
				assert(result.ptr == s + 9);
				assert(std::isnan(value));
				assert(std::signbit(value));
			}
		}
		{
			const char* s				  = "NaN()";
			F value						  = 0.25;
			std::from_chars_result result = std::from_chars(s, s + std::strlen(s), value, fmt);

			assert(result.ec == std::errc{});
			assert(result.ptr == s + 5);
			assert(std::isnan(value));
			assert(!std::signbit(value));
		}
		{
			std::array s = { 'N', 'a', 'N', '(', ' ', ')' };
			s[4]		 = 'a';
			{
				F value						  = 0.25;
				std::from_chars_result result = std::from_chars(s.data(), s.data() + s.size(), value, fmt);

				assert(result.ec == std::errc{});
				assert(result.ptr == s.data() + s.size());
				assert(std::isnan(value));
				assert(!std::signbit(value));
			}
			for (auto c: "!@#$%^&*(-=+[]{}|\\;:'\",./<>?~` \t\v\r\n") {
				F value						  = 0.25;
				s[4]						  = c;
				std::from_chars_result result = std::from_chars(s.data(), s.data() + s.size(), value, fmt);

				assert(result.ec == std::errc{});
				assert(result.ptr == s.data() + 3);
				assert(std::isnan(value));
				assert(!std::signbit(value));
			}
		}
	}

	template<class F> void test_fmt_independent(std::chars_format fmt) {
		test_infinity<F>(fmt);
		test_nan<F>(fmt);

		{
			F value						  = 0.25;
			std::from_chars_result result = std::from_chars(nullptr, nullptr, value, fmt);

			assert(result.ec == std::errc::invalid_argument);
			assert(result.ptr == nullptr);
			assert(value == F(0.25));
		}
		{
			F value						  = 0.25;
			const char* s				  = "-";
			std::from_chars_result result = std::from_chars(s, s + std::strlen(s), value, fmt);

			assert(result.ec == std::errc::invalid_argument);
			assert(result.ptr == s);
			assert(value == F(0.25));
		}
		{
			F value						  = 0.25;
			const char* s				  = ".";
			std::from_chars_result result = std::from_chars(s, s + std::strlen(s), value, fmt);

			assert(result.ec == std::errc::invalid_argument);
			assert(result.ptr == s);
			assert(value == F(0.25));
		}
		{
			F value						  = 0.25;
			const char* s				  = "-.";
			std::from_chars_result result = std::from_chars(s, s + std::strlen(s), value, fmt);

			assert(result.ec == std::errc::invalid_argument);
			assert(result.ptr == s);
			assert(value == F(0.25));
		}
		{
			F value						  = 0.25;
			const char* s				  = "+0.25";
			std::from_chars_result result = std::from_chars(s, s + std::strlen(s), value, fmt);

			assert(result.ec == std::errc::invalid_argument);
			assert(result.ptr == s);
			assert(value == F(0.25));
		}
	}

	template<class F> struct test_basics {
		void operator()() {
			for (auto fmt: { std::chars_format::scientific, std::chars_format::fixed,
					 /*std::chars_format::hex,*/ std::chars_format::general })
				test_fmt_independent<F>(fmt);
		}
	};

	template<class F> struct test_fixed {
		void operator()() {
			std::from_chars_result r;
			F x = 0.25;


			{
				std::array s = { ' ', '1' };
				for (auto c: "abcdefghijklmnopqrstuvwxyz"
							 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
							 "`~!@#$%^&*()_=[]{}\\|;:'\",/<>? \t\v\r\n") {
					s[0] = c;
					r	 = std::from_chars(s.data(), s.data() + s.size(), x, std::chars_format::fixed);

					assert(r.ec == std::errc::invalid_argument);
					assert(r.ptr == s.data());
					assert(x == F(0.25));
				}
			}


			{
				const char* s = "001x";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 3);
				assert(x == F(1.0));
			}
			{
				const char* s = ".5";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 2);
				assert(x == F(0.5));
			}
			{
				const char* s = "-.5";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 3);
				assert(x == F(-0.5));
			}

			{
				const char* s = "1.25.78";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 4);
				assert(x == F(1.25));
			}
			{
				const char* s = "1.5e10";
				r			  = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);

				assert(r.ec == std::errc{});
				assert(r.ptr == s + 3);
				assert(x == F(1.5));
			}
			{
				const char* s = "1.5E10";
				r			  = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);

				assert(r.ec == std::errc{});
				assert(r.ptr == s + 3);
				assert(x == F(1.5));
			}
			{
				const char* s = "1.5e+10";
				r			  = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);

				assert(r.ec == std::errc{});
				assert(r.ptr == s + 3);
				assert(x == F(1.5));
			}
			{
				const char* s = "1.5e-10";
				r			  = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);

				assert(r.ec == std::errc{});
				assert(r.ptr == s + 3);
				assert(x == F(1.5));
			}
			{
				const char* s = "1.5e";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 3);
				assert(x == F(1.5));
			}
			{
				{
					const char* s = "1.5e+";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 3);
					assert(x == F(1.5));
				}
				{
					const char* s = "1.5e-";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 3);
					assert(x == F(1.5));
				}
			}
			{
				{
					const char* s = "1.5e +1";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 3);
					assert(x == F(1.5));
				}
				{
					const char* s = "1.5e+ 1";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 3);
					assert(x == F(1.5));
				}
				{
					const char* s = "1.5e -1";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 3);
					assert(x == F(1.5));
				}
				{
					const char* s = "1.5e- 1";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 3);
					assert(x == F(1.5));
				}
			}
			{
				const char* s = "1.25e0e12";
				r			  = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);

				assert(r.ec == std::errc{});
				assert(r.ptr == s + 4);
				assert(x == F(1.25));
			}
			{
				{
					const char* s = "1.25e++12";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 4);
					assert(x == F(1.25));
				}
				{
					const char* s = "1.25e+-12";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 4);
					assert(x == F(1.25));
				}
				{
					const char* s = "1.25e-+12";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 4);
					assert(x == F(1.25));
				}
				{
					const char* s = "1.25e--12";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 4);
					assert(x == F(1.25));
				}
			}
			{
				const char* s = "1.25e0x12";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 4);
				assert(x == F(1.25));
			}
			{
				const char* s = "20040229";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 8);
				assert(x == F(20040229));
			}
			{
				const char* s = "123.456";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 7);
				assert(x == F(1.23456e2));
			}
			{
				const char* s = "123.456e3";
				r			  = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);

				assert(r.ec == std::errc{});
				assert(r.ptr == s + 7);
				assert(x == F(123.456));
			}
			{
				{
					const char* s = "0.111111111111111111111111111111111111111111";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + std::strlen(s));
					assert(x == F(0.111111111111111111111111111111111111111111));
				}
				{
					const char* s = "111111111111.111111111111111111111111111111111111111111";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + std::strlen(s));
					assert(x == F(111111111111.111111111111111111111111111111111111111111));
				}
			}
			{
				const char* s = "-0.25";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::fixed);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + std::strlen(s));
				assert(x == F(-0.25));
			}
		}
	};

	template<class F> struct test_scientific {
		void operator()() {
			std::from_chars_result r;
			F x = 0.25;


			{
				std::array s = { ' ', '1', 'e', '0' };
				for (auto c: "abcdefghijklmnopqrstuvwxyz"
							 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
							 "`~!@#$%^&*()_=[]{}\\|;:'\",/<>? \t\v\r\n") {
					s[0] = c;
					r	 = std::from_chars(s.data(), s.data() + s.size(), x, std::chars_format::scientific);

					assert(r.ec == std::errc::invalid_argument);
					assert(r.ptr == s.data());
					assert(x == F(0.25));
				}
			}
			{
				const char* s = "1.23";
				r			  = std::from_chars(s, s + strlen(s), x, std::chars_format::scientific);

				assert(r.ec == std::errc::invalid_argument);
				assert(r.ptr == s);
				assert(x == F(0.25));
			}
			{
				const char* s = "1.23e";
				r			  = std::from_chars(s, s + strlen(s), x, std::chars_format::scientific);

				assert(r.ec == std::errc::invalid_argument);
				assert(r.ptr == s);
				assert(x == F(0.25));
			}
			{
				{
					const char* s = "1.5e+";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
					assert(r.ec == std::errc::invalid_argument);
					assert(r.ptr == s);
					assert(x == F(0.25));
				}
				{
					const char* s = "1.5e-";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
					assert(r.ec == std::errc::invalid_argument);
					assert(r.ptr == s);
					assert(x == F(0.25));
				}
			}
			{
				{
					const char* s = "1.5e +1";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
					assert(r.ec == std::errc::invalid_argument);
					assert(r.ptr == s);
					assert(x == F(0.25));
				}
				{
					const char* s = "1.5e+ 1";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
					assert(r.ec == std::errc::invalid_argument);
					assert(r.ptr == s);
					assert(x == F(0.25));
				}
				{
					const char* s = "1.5e -1";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
					assert(r.ec == std::errc::invalid_argument);
					assert(r.ptr == s);
					assert(x == F(0.25));
				}
				{
					const char* s = "1.5e- 1";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
					assert(r.ec == std::errc::invalid_argument);
					assert(r.ptr == s);
					assert(x == F(0.25));
				}
			}
			{
				{
					const char* s = "1.25e++12";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
					assert(r.ec == std::errc::invalid_argument);
					assert(r.ptr == s);
					assert(x == F(0.25));
				}
				{
					const char* s = "1.25e+-12";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
					assert(r.ec == std::errc::invalid_argument);
					assert(r.ptr == s);
					assert(x == F(0.25));
				}
				{
					const char* s = "1.25e-+12";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
					assert(r.ec == std::errc::invalid_argument);
					assert(r.ptr == s);
					assert(x == F(0.25));
				}
				{
					const char* s = "1.25e--12";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
					assert(r.ec == std::errc::invalid_argument);
					assert(r.ptr == s);
					assert(x == F(0.25));
				}
			}


			{
				const char* s = "001e0x";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 5);
				assert(x == F(1.0));
			}

			{
				const char* s = "1.25e0.78";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 6);
				assert(x == F(1.25));
			}

			{
				const char* s = "1.5e10";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 6);
				assert(x == F(1.5e10));
			}
			{
				const char* s = "1.5E10";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 6);
				assert(x == F(1.5e10));
			}
			{
				const char* s = "1.5e+10";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 7);
				assert(x == F(1.5e10));
			}
			{
				const char* s = "1.5e-10";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 7);
				assert(x == F(1.5e-10));
			}
			{
				const char* s = "1.25e0x12";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 6);
				assert(x == F(1.25));
			}
			{
				const char* s = "1.25e0e12";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 6);
				assert(x == F(1.25));
			}
			{
				const char* s = "20040229e0";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 10);
				assert(x == F(20040229));
			}
			{
				const char* s = "123.456e3";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 9);
				assert(x == F(1.23456e5));
			}
			{
				{
					const char* s = "0.111111111111111111111111111111111111111111e0";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + std::strlen(s));
					assert(x == F(0.111111111111111111111111111111111111111111));
				}
				{
					const char* s = "111111111111.111111111111111111111111111111111111111111e0";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + std::strlen(s));
					assert(x == F(111111111111.111111111111111111111111111111111111111111));
				}
			}
			{
				const char* s = "-0.25e0";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + std::strlen(s));
				assert(x == F(-0.25));
			}
			{
				const char* s = "1e9999999999999999999999999999999999999999";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
				assert(r.ec == std::errc::result_out_of_range);
				assert(r.ptr == s + strlen(s));
				assert(x == std::numeric_limits<F>::infinity());
			}
			{
				const char* s = "-1e9999999999999999999999999999999999999999";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
				assert(r.ec == std::errc::result_out_of_range);
				assert(r.ptr == s + strlen(s));
				assert(x == -std::numeric_limits<F>::infinity());
			}
			{
				const char* s = "1e-9999999999999999999999999999999999999999";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
				assert(r.ec == std::errc::result_out_of_range);
				assert(r.ptr == s + strlen(s));
				assert(x == F(0.0));
			}
			{
				const char* s = "-1e-9999999999999999999999999999999999999999";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::scientific);
				assert(r.ec == std::errc::result_out_of_range);
				assert(r.ptr == s + strlen(s));
				assert(x == F(-0.0));
			}
		}
	};

	template<class F> struct test_general {
		void operator()() {
			std::from_chars_result r;
			F x = 0.25;


			{
				std::array s = { ' ', '1' };
				for (auto c: "abcdefghijklmnopqrstuvwxyz"
							 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
							 "`~!@#$%^&*()_=[]{}\\|;:'\",/<>? \t\v\r\n") {
					s[0] = c;
					r	 = std::from_chars(s.data(), s.data() + s.size(), x);

					assert(r.ec == std::errc::invalid_argument);
					assert(r.ptr == s.data());
					assert(x == F(0.25));
				}
			}


			{
				const char* s = "001x";

				r = std::from_chars(s, s + std::strlen(s), x);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 3);
				assert(x == F(1.0));
			}
			{
				const char* s = ".5e0";

				r = std::from_chars(s, s + std::strlen(s), x);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 4);
				assert(x == F(0.5));
			}
			{
				const char* s = "-.5e0";

				r = std::from_chars(s, s + std::strlen(s), x);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 5);
				assert(x == F(-0.5));
			}
			{
				const char* s = ".5";

				r = std::from_chars(s, s + std::strlen(s), x);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 2);
				assert(x == F(0.5));
			}
			{
				const char* s = "-.5";

				r = std::from_chars(s, s + std::strlen(s), x);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 3);
				assert(x == F(-0.5));
			}
			{
				const char* s = "1.25.78";

				r = std::from_chars(s, s + std::strlen(s), x);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 4);
				assert(x == F(1.25));
			}
			{
				const char* s = "1.5e10";

				r = std::from_chars(s, s + std::strlen(s), x);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 6);
				assert(x == F(1.5e10));
			}
			{
				const char* s = "1.5E10";

				r = std::from_chars(s, s + std::strlen(s), x);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 6);
				assert(x == F(1.5e10));
			}
			{
				const char* s = "1.5e+10";

				r = std::from_chars(s, s + std::strlen(s), x);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 7);
				assert(x == F(1.5e10));
			}
			{
				const char* s = "1.5e-10";

				r = std::from_chars(s, s + std::strlen(s), x);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 7);
				assert(x == F(1.5e-10));
			}
			{
				const char* s = "1.5e";

				r = std::from_chars(s, s + std::strlen(s), x);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 3);
				assert(x == F(1.5));
			}
			{
				{
					const char* s = "1.5e+";

					r = std::from_chars(s, s + std::strlen(s), x);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 3);
					assert(x == F(1.5));
				}
				{
					const char* s = "1.5e-";

					r = std::from_chars(s, s + std::strlen(s), x);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 3);
					assert(x == F(1.5));
				}
			}
			{
				{
					const char* s = "1.5e +1";

					r = std::from_chars(s, s + std::strlen(s), x);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 3);
					assert(x == F(1.5));
				}
				{
					const char* s = "1.5e+ 1";

					r = std::from_chars(s, s + std::strlen(s), x);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 3);
					assert(x == F(1.5));
				}
				{
					const char* s = "1.5e -1";

					r = std::from_chars(s, s + std::strlen(s), x);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 3);
					assert(x == F(1.5));
				}
				{
					const char* s = "1.5e- 1";

					r = std::from_chars(s, s + std::strlen(s), x);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 3);
					assert(x == F(1.5));
				}
			}
			{
				{
					const char* s = "1.25e++12";

					r = std::from_chars(s, s + std::strlen(s), x);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 4);
					assert(x == F(1.25));
				}
				{
					const char* s = "1.25e+-12";

					r = std::from_chars(s, s + std::strlen(s), x);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 4);
					assert(x == F(1.25));
				}
				{
					const char* s = "1.25e-+12";

					r = std::from_chars(s, s + std::strlen(s), x);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 4);
					assert(x == F(1.25));
				}
				{
					const char* s = "1.25e--12";

					r = std::from_chars(s, s + std::strlen(s), x);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 4);
					assert(x == F(1.25));
				}
			}
			{
				const char* s = "1.25e0x12";

				r = std::from_chars(s, s + std::strlen(s), x);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 6);
				assert(x == F(1.25));
			}
			{
				const char* s = "1.25e0e12";

				r = std::from_chars(s, s + std::strlen(s), x);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 6);
				assert(x == F(1.25));
			}
			{
				const char* s = "20040229";

				r = std::from_chars(s, s + std::strlen(s), x);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 8);
				assert(x == F(20040229));
			}
			{
				const char* s = "123.456";

				r = std::from_chars(s, s + std::strlen(s), x);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 7);
				assert(x == F(1.23456e2));
			}
			{
				const char* s = "123.456e3";

				r = std::from_chars(s, s + std::strlen(s), x);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 9);
				assert(x == F(1.23456e5));
			}
			{
				{
					const char* s = "0.111111111111111111111111111111111111111111";

					r = std::from_chars(s, s + std::strlen(s), x);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + std::strlen(s));
					assert(x == F(0.111111111111111111111111111111111111111111));
				}
				{
					const char* s = "111111111111.111111111111111111111111111111111111111111";

					r = std::from_chars(s, s + std::strlen(s), x);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + std::strlen(s));
					assert(x == F(111111111111.111111111111111111111111111111111111111111));
				}
			}
			{
				const char* s = "-0.25";

				r = std::from_chars(s, s + std::strlen(s), x);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + std::strlen(s));
				assert(x == F(-0.25));
			}
			{
				const char* s = "1e9999999999999999999999999999999999999999";

				r = std::from_chars(s, s + std::strlen(s), x);
				assert(r.ec == std::errc::result_out_of_range);
				assert(r.ptr == s + strlen(s));
				assert(x == std::numeric_limits<F>::infinity());
			}
			{
				const char* s = "-1e9999999999999999999999999999999999999999";

				r = std::from_chars(s, s + std::strlen(s), x);
				assert(r.ec == std::errc::result_out_of_range);
				assert(r.ptr == s + strlen(s));
				assert(x == -std::numeric_limits<F>::infinity());
			}
			{
				const char* s = "1e-9999999999999999999999999999999999999999";

				r = std::from_chars(s, s + std::strlen(s), x);
				assert(r.ec == std::errc::result_out_of_range);
				assert(r.ptr == s + strlen(s));
				assert(x == F(0.0));
			}
			{
				const char* s = "-1e-9999999999999999999999999999999999999999";

				r = std::from_chars(s, s + std::strlen(s), x);
				assert(r.ec == std::errc::result_out_of_range);
				assert(r.ptr == s + strlen(s));
				assert(x == F(-0.0));
			}
		}
	};

	template<class F> struct test_hex {
		void operator()() {
			std::from_chars_result r;
			F x = 0.25;


			{
				std::array s = { ' ', '1', 'e', '0' };
				for (auto c: "ghijklmnopqrstuvwxyz"
							 "GHIJKLMNOPQRSTUVWXYZ"
							 "`~!@#$%^&*()_=[]{}\\|;:'\",/<>? \t\v\r\n") {
					s[0] = c;
					r	 = std::from_chars(s.data(), s.data() + s.size(), x, std::chars_format::hex);

					assert(r.ec == std::errc::invalid_argument);
					assert(r.ptr == s.data());
					assert(x == F(0.25));
				}
			}


			{
				const char* s = "001x";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 3);
				assert(x == F(1.0));
			}
			{
				const char* s = ".5p0";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 4);
				assert(x == F(0x0.5p0));
			}
			{
				const char* s = "-.5p0";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 5);
				assert(x == F(-0x0.5p0));
			}
			{
				const char* s = ".5";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 2);
				assert(x == F(0x0.5p0));
			}
			{
				const char* s = "-.5";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 3);
				assert(x == F(-0x0.5p0));
			}
			{
				const char* s = "1.25.78";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 4);
				assert(x == F(0x1.25p0));
			}
			{
				const char* s = "1.5p10";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 6);
				assert(x == F(0x1.5p10));
			}
			{
				const char* s = "1.5P10";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 6);
				assert(x == F(0x1.5p10));
			}
			{
				const char* s = "1.5p+10";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 7);
				assert(x == F(0x1.5p10));
			}
			{
				const char* s = "1.5p-10";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 7);
				assert(x == F(0x1.5p-10));
			}
			{
				const char* s = "1.5p";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 3);
				assert(x == F(0x1.5p0));
			}
			{
				{
					const char* s = "1.5p+";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 3);
					assert(x == F(0x1.5p0));
				}
				{
					const char* s = "1.5p-";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 3);
					assert(x == F(0x1.5p0));
				}
			}
			{
				{
					const char* s = "1.5p +1";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 3);
					assert(x == F(0x1.5p0));
				}
				{
					const char* s = "1.5p+ 1";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 3);
					assert(x == F(0x1.5p0));
				}
				{
					const char* s = "1.5p -1";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 3);
					assert(x == F(0x1.5p0));
				}
				{
					const char* s = "1.5p- 1";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 3);
					assert(x == F(0x1.5p0));
				}
			}
			{
				{
					const char* s = "1.25p++12";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 4);
					assert(x == F(0x1.25p0));
				}
				{
					const char* s = "1.25p+-12";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 4);
					assert(x == F(0x1.25p0));
				}
				{
					const char* s = "1.25p-+12";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 4);
					assert(x == F(0x1.25p0));
				}
				{
					const char* s = "1.25p--12";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + 4);
					assert(x == F(0x1.25p0));
				}
			}
			{
				const char* s = "1.25p0x12";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 6);
				assert(x == F(0x1.25p0));
			}
			{
				const char* s = "1.25p0p12";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 6);
				assert(x == F(0x1.25p0));
			}
			{
				const char* s = "131CA25";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 7);
				assert(x == F(0x131CA25p0));
			}
			{
				const char* s = "123.456";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 7);
				assert(x == F(0x123.456p0));
			}
			{
				const char* s = "123.456p3";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + 9);
				assert(x == F(0x123.456p3));
			}
			{
				{
					const char* s = "0.111111111111111111111111111111111111111111";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + std::strlen(s));
					assert(x == F(0x0.111111111111111111111111111111111111111111p0));
				}
				{
					const char* s = "111111111111.111111111111111111111111111111111111111111";

					r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
					assert(r.ec == std::errc{});
					assert(r.ptr == s + std::strlen(s));
					assert(x == F(0x111111111111.111111111111111111111111111111111111111111p0));
				}
			}
			{
				const char* s = "-0.25";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
				assert(r.ec == std::errc{});
				assert(r.ptr == s + std::strlen(s));
				assert(x == F(-0x0.25p0));
			}
			{
				const char* s = "1p9999999999999999999999999999999999999999";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
				assert(r.ec == std::errc::result_out_of_range);
				assert(r.ptr == s + strlen(s));
				assert(x == std::numeric_limits<F>::infinity());
			}
			{
				const char* s = "-1p9999999999999999999999999999999999999999";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
				assert(r.ec == std::errc::result_out_of_range);
				assert(r.ptr == s + strlen(s));
				assert(x == -std::numeric_limits<F>::infinity());
			}
			{
				const char* s = "1p-9999999999999999999999999999999999999999";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
				assert(r.ec == std::errc::result_out_of_range);
				assert(r.ptr == s + strlen(s));
				assert(x == F(0.0));
			}
			{
				const char* s = "-1p-9999999999999999999999999999999999999999";

				r = std::from_chars(s, s + std::strlen(s), x, std::chars_format::hex);
				assert(r.ec == std::errc::result_out_of_range);
				assert(r.ptr == s + strlen(s));
				assert(x == F(-0.0));
			}
		}
	};

	static void test_random_errors() {
		{
			const char* s	 = "4.219902180869891e-2788";
			const char* last = s + std::strlen(s) - 1;


			double value				  = 0.25;
			std::from_chars_result result = std::from_chars(s, last, value);

			assert(result.ec == std::errc{});
			assert(result.ptr == last);
			assert(value == 4.219902180869891e-278);
		}
		{
			const char* s	 = "7.411412e-39U";
			const char* last = s + std::strlen(s) - 1;

			float value					  = 0.25;
			std::from_chars_result result = std::from_chars(s, last, value);

			assert(result.ec == std::errc{});
			assert(result.ptr == last);
			assert(value == 7.411412e-39F);
		}
	}

	static void test() {
		run<test_basics>(all_floats);
		run<test_scientific>(all_floats);
		run<test_fixed>(all_floats);
		run<test_general>(all_floats);

		run<test_hex>(all_floats);

		test_random_errors();
	}

}
