// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp
// vn-incl/d_to_str.hpp

#pragma once

#include <vn-incl/utility.hpp>
#include <vn-incl/i_to_str.hpp>
#include <vn-incl/zmij.hpp>

namespace vn {

	template<detail::float_types v_type> VN_FORCE_INLINE std::to_chars_result to_chars(char* first, char* last, v_type value, int32_t base = 10) noexcept {
		if (base == 10) {
			return std::to_chars(first, last, value);
		} else {
			auto r = std::to_chars(first, last, value);
			return { r.ptr, r.ec };
		}
	}

}
