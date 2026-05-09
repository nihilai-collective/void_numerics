// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp
// vn-incl/str_to_d.hpp

#pragma once

#include <vn-incl/utility.hpp>
#include <vn-incl/fast_float.h>

namespace vn {

	template<detail::float_types v_type>
	VN_FORCE_INLINE std::from_chars_result from_chars(const char* first, const char* last, v_type& value, std::chars_format format = std::chars_format::general) noexcept {
		if (format != std::chars_format::hex) {
			return fast_float::from_chars(first, last, value, fast_float::std_to_vn_fmt_table[static_cast<uint64_t>(format)]);
		} else {
			return std::from_chars(first, last, value, format);
		}
	}

}
