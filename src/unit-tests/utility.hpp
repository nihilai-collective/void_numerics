// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp
// unit-tests/utility.hpp

#pragma once

#include <void-numerics>
#include <charconv>
#include <cstring>
#include <limits>
#include <vector>
#include <memory>
#include <map>

namespace unit_tests {

	template<vn::detail::integer_types v_type> struct max_digits {
	  private:
		using limits = std::numeric_limits<v_type>;
		using U		 = std::make_unsigned_t<v_type>;

		static consteval uint64_t count_digits(U value_new) {
			uint64_t digits = 0;
			do {
				value_new /= 10;
				++digits;
			} while (value_new != 0);
			return digits;
		}

	  public:
		static constexpr uint64_t value = count_digits(static_cast<U>(limits::max()));
	};

	template<vn::detail::integer_types v_type> static constexpr uint64_t max_digits_v = max_digits<v_type>::value;

	template<vn::detail::conversion_classes> struct tests;

}
