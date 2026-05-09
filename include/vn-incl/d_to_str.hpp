// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp
// vn-incl/d_to_str.hpp

#pragma once

#include <vn-incl/utility.hpp>
#include <vn-incl/i_to_str.hpp>
#include <vn-incl/zmij.hpp>

namespace vn {

	template<detail::float_types v_type>
	VN_FORCE_INLINE std::to_chars_result to_chars(char* first, char* last, v_type value, std::chars_format format = std::chars_format::fixed) noexcept {
		if (format != std::chars_format::hex) {
			char* end = vn_zmij::detail::write(value, first);
			return { end, std::errc{} };
		} else {
			auto r = std::to_chars(first, last, value);
			return { r.ptr, r.ec };
		}
	}

}
