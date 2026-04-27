// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp
// vn-incl/tables.hpp

#pragma once

#include <vn-incl/concepts.hpp>
#include <limits>

namespace vn {

	template<typename v_type> inline static constexpr std::array<uint64_t, 256> raw_comp_vals_pos{ []() {
		constexpr auto max_value{ std::numeric_limits<std::decay_t<v_type>>::max() };
		std::array<uint64_t, 256> return_values{};
		return_values['0'] = (max_value - 0) / 10;
		return_values['1'] = (max_value - 1) / 10;
		return_values['2'] = (max_value - 2) / 10;
		return_values['3'] = (max_value - 3) / 10;
		return_values['4'] = (max_value - 4) / 10;
		return_values['5'] = (max_value - 5) / 10;
		return_values['6'] = (max_value - 6) / 10;
		return_values['7'] = (max_value - 7) / 10;
		return_values['8'] = (max_value - 8) / 10;
		return_values['9'] = (max_value - 9) / 10;
		return return_values;
	}() };

	template<typename v_type>
	inline static constexpr std::array<uint64_t, 256> raw_comp_vals_neg{ []() {
		constexpr auto max_value{ static_cast<uint64_t>(std::numeric_limits<std::make_signed_t<v_type>>::max()) + 1 };
		std::array<uint64_t, 256> return_values{};
		return_values['0'] = (max_value - 0) / 10;
		return_values['1'] = (max_value - 1) / 10;
		return_values['2'] = (max_value - 2) / 10;
		return_values['3'] = (max_value - 3) / 10;
		return_values['4'] = (max_value - 4) / 10;
		return_values['5'] = (max_value - 5) / 10;
		return_values['6'] = (max_value - 6) / 10;
		return_values['7'] = (max_value - 7) / 10;
		return_values['8'] = (max_value - 8) / 10;
		return_values['9'] = (max_value - 9) / 10;
		return return_values;
	}() };

	template<typename = void> struct pow_tables {
		static constexpr uint64_t power_of_ten_uint[]{ 1ull, 10ull, 100ull, 1000ull, 10000ull, 100000ull, 1000000ull, 10000000ull, 100000000ull, 1000000000ull, 10000000000ull,
			100000000000ull, 1000000000000ull, 10000000000000ull, 100000000000000ull, 1000000000000000ull, 10000000000000000ull, 100000000000000000ull, 1000000000000000000ull,
			10000000000000000000ull };

		static constexpr int64_t power_of_ten_int[]{ 1ll, 10ll, 100ll, 1000ll, 10000ll, 100000ll, 1000000ll, 10000000ll, 100000000ll, 1000000000ll, 10000000000ll, 100000000000ll,
			1000000000000ll, 10000000000000ll, 100000000000000ll, 1000000000000000ll, 10000000000000000ll, 100000000000000000ll, 1000000000000000000ll };
	};

	template<typename = void> struct exp_tables {
		VN_ALIGN(64)
		static constexpr std::array<bool, 256> exp_table = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };

		VN_ALIGN(64)
		static constexpr std::array<bool, 256> exp_frac_table = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			false };
	};

	VN_ALIGN(64) inline constexpr char decimal{ '.' };
	VN_ALIGN(64) inline constexpr char minus{ '-' };
	VN_ALIGN(64) inline constexpr char plus{ '+' };
	VN_ALIGN(64) inline constexpr char zero{ '0' };
	VN_ALIGN(64) inline constexpr char nine{ '9' };

	template<std::endian, typename v_type> struct int_tables_impl {};

	template<typename v_type> struct int_tables_impl<std::endian::little, v_type> {
		VN_ALIGN(64ULL)
		inline static constexpr auto char_table_01{ [] {
			std::array<char, 200> t{};
			for (uint32_t i = 0; i < 100; ++i) {
				t[i * 2]	 = static_cast<char>('0' + i / 10);
				t[i * 2 + 1] = static_cast<char>('0' + i % 10);
			}
			return t;
		}() };
		VN_ALIGN(64ULL)
		inline static constexpr auto char_table_02{ [] {
			std::array<uint16_t, 100> t{};
			for (uint32_t i = 0; i < 100; ++i) {
				t[i] = static_cast<uint16_t>(('0' + i % 10) << 8 | ('0' + i / 10));
			}
			return t;
		}() };
		VN_ALIGN(64ULL)
		inline static constexpr auto char_table_04{ [] {
			std::array<uint32_t, 10000> t{};
			for (uint32_t i = 0; i < 10000; ++i) {
				t[i] = (0x30 + (i / 1000)) | ((0x30 + ((i / 100) % 10)) << 8) | ((0x30 + ((i / 10) % 10)) << 16) | ((0x30 + (i % 10)) << 24);
			}
			return t;
		}() };
	};

	template<typename v_type> struct int_tables_impl<std::endian::big, v_type> {
		VN_ALIGN(64ULL)
		inline static constexpr auto char_table_01{ [] {
			std::array<char, 200> t{};
			for (uint32_t i = 0; i < 100; ++i) {
				t[i * 2]	 = static_cast<char>('0' + i / 10);
				t[i * 2 + 1] = static_cast<char>('0' + i % 10);
			}
			return t;
		}() };
		VN_ALIGN(64ULL)
		inline static constexpr auto char_table_02{ [] {
			std::array<uint16_t, 100> t{};
			for (uint32_t i = 0; i < 100; ++i) {
				t[i] = static_cast<uint16_t>(('0' + i / 10) << 8 | ('0' + i % 10));
			}
			return t;
		}() };
		VN_ALIGN(64ULL)
		inline static constexpr auto char_table_04{ [] {
			std::array<uint32_t, 10000> t{};
			for (uint32_t i = 0; i < 10000; ++i) {
				t[i] = (0x30 + (i / 1000)) << 24 | ((0x30 + ((i / 100) % 10)) << 16) | ((0x30 + ((i / 10) % 10)) << 8) | (0x30 + (i % 10));
			}
			return t;
		}() };
	};

	using int_tables = int_tables_impl<std::endian::native, void>;

	template<uint64_t divisor> consteval uint64_t divisor_to_index() {
		if constexpr (divisor == 10ULL) {
			return 0ULL;
		} else if constexpr (divisor == 100ULL) {
			return 1ULL;
		} else if constexpr (divisor == 1000ULL) {
			return 2ULL;
		} else if constexpr (divisor == 10000ULL) {
			return 3ULL;
		} else if constexpr (divisor == 100000ULL) {
			return 4ULL;
		} else if constexpr (divisor == 1000000ULL) {
			return 5ULL;
		} else if constexpr (divisor == 10000000ULL) {
			return 6ULL;
		} else if constexpr (divisor == 100000000ULL) {
			return 7ULL;
		} else {
			static_assert(false_type::value, "Sorry, but that divisor is not within this table!");
		}
	}

	template<typename v_type> struct mul_shift_table;

	template<typename v_type> struct mul_shift_entry {
		v_type multiplicand;
		uint64_t shift;
	};

	template<uint32_types v_type> struct mul_shift_table<v_type> {
		static constexpr mul_shift_entry<v_type> values[8] = {
			{ 0xcccccccdU, 35 },
			{ 0xa3d70a3eU, 38 },
			{ 0x83126e98U, 41 },
			{ 0xd1b71759U, 45 },
			{ 0xa7c5ac48U, 48 },
			{ 0x8637bd06U, 51 },
			{ 0xd6bf94d6U, 55 },
			{ 0xabcc7712U, 58 },
		};
	};

	template<uint64_types v_type> struct mul_shift_table<v_type> {
		static constexpr mul_shift_entry<v_type> values[8] = {
			{ 0xcccccccccccccccdULL, 67 },
			{ 0xa3d70a3d70a3d70bULL, 70 },
			{ 0x83126e978d4fdf3cULL, 73 },
			{ 0xd1b71758e219652cULL, 77 },
			{ 0xa7c5ac471b478424ULL, 80 },
			{ 0x8637bd05af6c69b6ULL, 83 },
			{ 0xd6bf94d5e57a42bdULL, 87 },
			{ 0xabcc77118461cefdULL, 90 },
		};
	};

}