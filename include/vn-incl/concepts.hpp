// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp
// vn-incl/concepts.hpp

#pragma once

#include <vn-incl/config.hpp>

namespace vn {

	namespace detail {

		template<typename v_type> using base_t = std::remove_cvref_t<v_type>;

		template<typename v_type>
		concept integer_types = std::integral<base_t<v_type>>;

		template<typename v_type>
		concept integer8_types = integer_types<v_type> && sizeof(base_t<v_type>) == 1;

		template<typename v_type>
		concept integer16_types = integer_types<v_type> && sizeof(base_t<v_type>) == 2;

		template<typename v_type>
		concept integer32_types = integer_types<v_type> && sizeof(base_t<v_type>) == 4;

		template<typename v_type>
		concept integer64_types = integer_types<v_type> && sizeof(base_t<v_type>) == 8;

		template<typename v_type>
		concept uint_types = std::unsigned_integral<base_t<v_type>>;

		template<typename v_type>
		concept uint8_types = uint_types<v_type> && sizeof(base_t<v_type>) == 1;

		template<typename v_type>
		concept uint16_types = uint_types<v_type> && sizeof(base_t<v_type>) == 2;

		template<typename v_type>
		concept uint32_types = uint_types<v_type> && sizeof(base_t<v_type>) == 4;

		template<typename v_type>
		concept uint64_types = uint_types<v_type> && sizeof(base_t<v_type>) == 8;

		template<typename v_type>
		concept int_types = std::signed_integral<base_t<v_type>>;

		template<typename v_type>
		concept int8_types = int_types<v_type> && sizeof(base_t<v_type>) == 1;

		template<typename v_type>
		concept int16_types = int_types<v_type> && sizeof(base_t<v_type>) == 2;

		template<typename v_type>
		concept int32_types = int_types<v_type> && sizeof(base_t<v_type>) == 4;

		template<typename v_type>
		concept int64_types = int_types<v_type> && sizeof(base_t<v_type>) == 8;

		template<typename v_type>
		concept float_types = std::floating_point<base_t<v_type>>;

		template<typename v_type>
		concept float32_types = float_types<v_type> && sizeof(base_t<v_type>) == 4;

		template<typename v_type>
		concept float64_types = float_types<v_type> && sizeof(base_t<v_type>) == 8;

	}

}
