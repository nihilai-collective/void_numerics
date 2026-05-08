// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp

#pragma once

#include "utility.hpp"
#include "zmij.hpp"
#include <vn-incl/d_to_str.hpp>

namespace d_to_str_tests {

	template<typename v_type, int64_t min_exp, int64_t max_exp> struct random_double_value {
		static_assert(std::is_floating_point_v<v_type>);
		VN_FORCE_INLINE static v_type impl() {
			bnch_swt::random_generator<int64_t> exp_rg{};
			bnch_swt::random_generator<v_type> mantissa_rg{};
			int64_t exp		= exp_rg.impl(min_exp, max_exp - 1);
			v_type mantissa = mantissa_rg.impl(static_cast<v_type>(1.0), static_cast<v_type>(10.0));
			return static_cast<v_type>(mantissa * std::pow(static_cast<v_type>(10.0), static_cast<v_type>(exp)));
		}
	};

	template<uint64_t test_size, uint64_t sub_iters, uint64_t total_iters, typename float_type, int64_t min_exp, int64_t max_exp, bool negative> struct double_generator {
		using test_data_type   = std::vector<float_type>;
		using output_data_type = std::vector<char>;
		static constexpr uint64_t type_size{ benchmarks::max_chars_v<float_type> };
		static test_data_type impl() {
			static_assert(std::is_floating_point_v<float_type>);
			test_data_type data{};
			data.resize(test_size * total_iters * sub_iters);
			for (uint64_t x = 0; x < data.size(); ++x) {
				float_type value = random_double_value<float_type, min_exp, max_exp>::impl();
				data[x]			 = negative ? -value : value;
			}
			return data;
		}
	};

	struct vn_double_op {
		VN_FORCE_INLINE static char* convert(double v, char* buf) noexcept {
			return const_cast<char*>(vn::to_chars(buf, buf + 32, v).ptr);
		}
	};

	struct zmij_double_op {
		VN_FORCE_INLINE static char* convert(double v, char* buf) noexcept {
			return const_cast<char*>(zmij::detail::write(v, buf));
		}
	};

	struct std_double_op {
		VN_FORCE_INLINE static char* convert(double v, char* buf) noexcept {
			return std::to_chars(buf, buf + 32, v).ptr;
		}
	};

	struct fmt_double_op {
		VN_FORCE_INLINE static char* convert(double v, char* buf) noexcept {
			return fmt::format_to(buf, FMT_COMPILE("{}"), v);
		}
	};

	struct verify_double_correctness {
		template<typename float_type> static void impl(const std::vector<float_type>& test_data, const char* test_label) {
			uint64_t vn_correct{}, vn_incorrect{};
			uint64_t std_correct{}, std_incorrect{};
			uint64_t fmt_correct{}, fmt_incorrect{};
			uint64_t zmij_correct{}, zmij_incorrect{};
			uint64_t dragonbox_correct{}, dragonbox_incorrect{};
			float_type first_bad_value{};
			bool found_bad{ false };
			auto round_trips = [](float_type original, char* buf, char* end) -> bool {
				float_type result{};
#if defined(__cpp_lib_charconv) && __cpp_lib_charconv >= 201611L
				auto [ptr, ec] = std::from_chars(buf, end, result, std::chars_format::general);
				if (ec != std::errc{})
					return false;
#else
				char* endptr = end;
				if constexpr (std::is_same_v<float_type, float>)
					result = std::strtof(buf, &endptr);
				else
					result = std::strtod(buf, &endptr);
				if (endptr == buf)
					return false;
#endif
				return result == original;
			};
			for (uint64_t x = 0; x < test_data.size(); ++x) {
				float_type v = test_data[x];
				char buf_vn[128]{};
				char buf_std[128]{};
				char buf_fmt[128]{};
				char buf_zmij[128]{};
				char buf_dragonbox[128]{};
				char* vn_end   = vn_double_op::convert(v, buf_vn);
				char* std_end  = std_double_op::convert(v, buf_std);
				char* fmt_end  = fmt_double_op::convert(v, buf_fmt);
				char* zmij_end = zmij_double_op::convert(v, buf_zmij);
				if (round_trips(v, buf_vn, vn_end)) {
					++vn_correct;
				} else {
					++vn_incorrect;
					if (!found_bad) {
						first_bad_value = v;
						found_bad		= true;
					}
				}
				round_trips(v, buf_zmij, zmij_end) ? ++zmij_correct : ++zmij_incorrect;
				round_trips(v, buf_std, std_end) ? ++std_correct : ++std_incorrect;
				round_trips(v, buf_fmt, fmt_end) ? ++fmt_correct : ++fmt_incorrect;
			}
			std::cout << "[" << test_label << "] vn correct: " << vn_correct << " | incorrect: " << vn_incorrect << " | std incorrect: " << std_incorrect
					  << " | fmt incorrect: " << fmt_incorrect << " | dragonbox incorrect: " << dragonbox_incorrect << " | zmij incorrect: " << zmij_incorrect << std::endl;
			if (vn_incorrect > 0) {
				std::cout << "  FIRST BAD vn VALUE: " << first_bad_value << std::endl;
			}
		}
	};

};