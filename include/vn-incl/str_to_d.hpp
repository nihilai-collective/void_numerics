// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp
// vn-incl/str_to_d.hpp

#pragma once

#include <vn-incl/tables.hpp>
#include <vn-incl/fast_float.h>

namespace vn {

	template<detail::float_types v_type> VN_FORCE_INLINE std::from_chars_result from_chars(const char* first, const char* last, v_type& value, int base = 10) noexcept {
		if (base == 10) {
			const char* end = detail::from_chars_impl<v_type>::impl(value, first, last);

			if (end == first) {
				return { first, std::errc::invalid_argument };
			}
			return { end, std::errc{} };
		} else {
			auto r = std::from_chars(first, last, value, base);
			return { r.ptr, r.ec };
		}
	}

}
