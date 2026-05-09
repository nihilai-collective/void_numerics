// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp

#pragma once

#include <bnch_swt/index.hpp>
#include <void-numerics>
#include "utility.hpp"
#include <charconv>
#include <cstring>
#include <cstdlib>
#include <limits>
#include <vector>
#include <array>
#include <iostream>

namespace str_to_i_tests {

	template<typename v_type_new> struct string_entry {
		using v_type = v_type_new;
		char buf[benchmarks::max_digits_v<v_type> + 1]{};
		uint8_t len{};
		v_type expected{};
	};

	template<uint64_t test_size, uint64_t sub_iters, uint64_t total_iters, typename int_type, uint64_t min_length, uint64_t max_length, bool negative> struct string_generator {
		using entry_type	 = string_entry<int_type>;
		using test_data_type = std::vector<entry_type>;
		using output_data_type = std::vector<int_type>;
		static constexpr uint64_t type_size{ 1 };
		static test_data_type impl() {
			test_data_type data(test_size * total_iters * sub_iters);
			for (auto& e: data) {
				int_type value = benchmarks::random_digits<int_type, min_length, max_length>::impl();
				if constexpr (vn::detail::int_types<int_type>) {
					value *= negative ? -1 : 1;
				}
				char* end  = std::to_chars(e.buf, e.buf + benchmarks::max_digits_v<int_type> + 1, value).ptr;
				e.len	   = static_cast<uint8_t>(end - e.buf);
				e.expected = value;
			}
			return data;
		}
	};

	struct vn_from_op {
		template<typename v_type> VN_FORCE_INLINE static v_type convert(const char* buf, uint8_t len) noexcept {
			v_type result{};
			vn::from_chars(buf, buf + len, result);
			return result;
		}
	};

	struct std_from_op {
		template<typename v_type> VN_FORCE_INLINE static v_type convert(const char* buf, uint8_t len) noexcept {
			v_type result{};
			std::from_chars(buf, buf + len, result);
			return result;
		}
	};

	struct strto_op {
		template<typename v_type> VN_FORCE_INLINE static v_type convert(const char* buf, uint8_t) noexcept {
			if constexpr (vn::detail::int_types<v_type>) {
				return static_cast<v_type>(std::strtoll(buf, nullptr, 10));
			} else {
				return static_cast<v_type>(std::strtoull(buf, nullptr, 10));
			}
		}
	};

	template<typename Op> struct from_chars_benchmark {
		template<typename size_type_01, typename size_type_02, typename vector_type, typename output_buffer_type, typename index_type> VN_FORCE_INLINE static uint64_t impl(
			size_type_01 test_size, size_type_02 sub_iters, const vector_type& test_data, output_buffer_type& output_buffer, index_type& current_index) {
			using out_type		  = typename output_buffer_type::value_type;
			const auto* entries	  = test_data.data() + current_index * test_size;
			out_type* const begin = output_buffer.data() + current_index * test_size;
			out_type* p			  = begin;
			for (uint64_t y = 0; y < sub_iters; ++y) {
				uint64_t base_index = y * test_size;
				for (uint64_t x = 0; x < test_size; ++x) {
					const auto& e = entries[base_index + x];
					*p++		  = Op::template convert<out_type>(e.buf, e.len);
				}
			}
			++current_index;
			bnch_swt::do_not_optimize_away(p);
			return static_cast<uint64_t>(p - begin) * sizeof(out_type);
		}
	};

	struct verify_correctness {
		template<typename int_type> static void impl(const std::vector<string_entry<int_type>>& test_data, const char* test_label) {
			uint64_t vn_correct{}, vn_incorrect{};
			uint64_t strto_correct{}, strto_incorrect{}, total_incorrect{};
			int_type first_bad_value{};
			bool found_bad{ false };
			for (uint64_t x = 0; x < test_data.size(); ++x) {
				const auto& e = test_data[x];
				int_type vn_result{};
				vn::from_chars(e.buf, e.buf + e.len, vn_result);
				int_type strto_result{};
				if constexpr (vn::detail::int_types<int_type>) {
					strto_result = static_cast<int_type>(std::strtoll(e.buf, nullptr, 10));
				} else {
					strto_result = static_cast<int_type>(std::strtoull(e.buf, nullptr, 10));
				}
				if (vn_result == e.expected) {
					++vn_correct;
				} else {
					++vn_incorrect;
					++total_incorrect;
					if (!found_bad) {
						first_bad_value = e.expected;
						found_bad		= true;
					}
				}
				if (strto_result == e.expected) {
					++strto_correct;
				} else {
					++strto_incorrect;
					++total_incorrect;
				}
			}
			if (total_incorrect > 0) {
				std::cout << "[" << test_label << "] vn correct: " << vn_correct << " | incorrect: " << vn_incorrect << " | strto incorrect: " << strto_incorrect << std::endl;
			}
			if (vn_incorrect > 0) {
				std::cout << "  FIRST BAD vn VALUE: " << static_cast<int64_t>(first_bad_value) << std::endl;
			}
		}
	};

	static constexpr uint64_t max_leading_zeros = 20;

	template<typename v_type_new> struct leading_zero_string_entry {
		using v_type = v_type_new;
		char buf[benchmarks::max_digits_v<v_type> + max_leading_zeros + 2]{};
		uint8_t len{};
		v_type expected{};
	};

	template<uint64_t test_size, uint64_t sub_iters, uint64_t total_iters, typename int_type, uint64_t min_length, uint64_t max_length, bool negative>
	struct leading_zero_string_generator {
		using entry_type	   = leading_zero_string_entry<int_type>;
		using test_data_type   = std::vector<entry_type>;
		using output_data_type = std::vector<int_type>;
		static constexpr uint64_t type_size{ 1 };
		static test_data_type impl() {
			test_data_type data(test_size * total_iters * sub_iters);
			bnch_swt::random_generator<uint64_t> zrg{};
			for (auto& e: data) {
				int_type value = benchmarks::random_digits<int_type, min_length, max_length>::impl();
				if constexpr (vn::detail::int_types<int_type>) {
					value = negative ? -1 : 1;
				}
				uint64_t num_zeros = zrg.impl(uint64_t(1), max_leading_zeros);
				char* p			   = e.buf;
				if constexpr (vn::detail::int_types<int_type>) {
					if (value < 0) {
						*p++ = '-';
					}
				}
				for (uint64_t z = 0; z < num_zeros; ++z) {
					*p++ = '0';
				}
				using U = std::make_unsigned_t<int_type>;
				U abs_val{};
				if constexpr (vn::detail::int_types<int_type>) {
					abs_val = value < 0 ? static_cast<U>(0) - static_cast<U>(value) : static_cast<U>(value);
				} else {
					abs_val = value;
				}
				char* end   = std::to_chars(p, e.buf + sizeof(e.buf), abs_val).ptr;
				e.len	   = static_cast<uint8_t>(end - e.buf);
				e.expected = value;
			}
			return data;
		}
	};

	struct verify_correctness_leading_zeros {
		template<typename int_type> static void impl(const std::vector<leading_zero_string_entry<int_type>>& test_data, const char* test_label) {
			uint64_t vn_correct{}, vn_incorrect{};
			uint64_t strto_correct{}, strto_incorrect{}, total_incorrect{};
			int_type first_bad_value{};
			bool found_bad{ false };
			for (const auto& e: test_data) {
				int_type vn_result{};
				vn::from_chars(e.buf, e.buf + e.len, vn_result);
				int_type strto_result{};
				if constexpr (vn::detail::int_types<int_type>) {
					strto_result = static_cast<int_type>(std::strtoll(e.buf, nullptr, 10));
				} else {
					strto_result = static_cast<int_type>(std::strtoull(e.buf, nullptr, 10));
				}
				if (vn_result == e.expected) {
					++vn_correct;
				} else {
					++vn_incorrect;
					++total_incorrect;
					if (!found_bad) {
						first_bad_value = e.expected;
						found_bad		= true;
					}
				}
				if (strto_result == e.expected) {
					++strto_correct;
				} else {
					++strto_incorrect;
					++total_incorrect;
				}
			}
			if (total_incorrect > 0) {
				std::cout << "[" << test_label << "] vn correct: " << vn_correct << " | incorrect: " << vn_incorrect << " | strto incorrect: " << strto_incorrect << std::endl;
			}
			if (vn_incorrect > 0) {
				std::cout << "  FIRST BAD vn VALUE: " << static_cast<int64_t>(first_bad_value) << std::endl;
			}
		}
	};

}
