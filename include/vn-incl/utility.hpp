// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp
// vn-incl/utility.hpp

#pragma once

#include <vn-incl/concepts.hpp>
#include <limits>

namespace vn {

	namespace detail {

		template<auto value_new> struct make_static {
			static constexpr auto value{ value_new };
		};

		template<uint8_types auto repeat, uint_types v_type> static constexpr uint64_t repeat_bytes_v = static_cast<v_type>(0x0101010101010101ull) * static_cast<v_type>(repeat);

		template<bool negative> static constexpr uint64_t comp_val_addition{ [] {
			if constexpr (negative) {
				return 1;
			} else {
				return 0;
			}
		}() };

		template<typename v_type, bool negative> static constexpr std::array<std::make_unsigned_t<v_type>, 256> gen_raw_comp_vals() {
			static constexpr auto max_value{ static_cast<std::make_unsigned_t<v_type>>(std::numeric_limits<base_t<v_type>>::max()) + comp_val_addition<negative> };
			std::array<std::make_unsigned_t<v_type>, 256> return_values_interna{};
			return_values_interna['0'] = (max_value - 0) / 10;
			return_values_interna['1'] = (max_value - 1) / 10;
			return_values_interna['2'] = (max_value - 2) / 10;
			return_values_interna['3'] = (max_value - 3) / 10;
			return_values_interna['4'] = (max_value - 4) / 10;
			return_values_interna['5'] = (max_value - 5) / 10;
			return_values_interna['6'] = (max_value - 6) / 10;
			return_values_interna['7'] = (max_value - 7) / 10;
			return_values_interna['8'] = (max_value - 8) / 10;
			return_values_interna['9'] = (max_value - 9) / 10;
			return return_values_interna;
		};

		template<typename v_type, bool negative> VN_ALIGN(64) static constexpr std::array<std::make_unsigned_t<v_type>, 256> raw_comp_vals{ gen_raw_comp_vals<v_type, negative>() };

		template<typename v_type, bool negative> VN_ALIGN(64) static constexpr const std::make_unsigned_t<v_type>* __restrict comp_vals{ raw_comp_vals<v_type, negative>.data() };

		template<uint8_types v_type> VN_FORCE_INLINE static constexpr bool is_digit(v_type c) noexcept {
			return c - static_cast<uint8_t>('0') < 10;
		}

		VN_ALIGN(64) inline constexpr char zero{ '0' };

		template<std::endian, uint64_t size = 0, typename = void> struct int_tables_impl;

		template<std::endian endianity, typename v_type> struct int_tables_impl<endianity, 1, v_type> {
			static constexpr std::array<char, 10> gen() {
				std::array<char, 10> t{};
				for (uint32_t i = 0; i < 10; ++i) {
					t[i] = static_cast<char>('0' + i);
				}
				return t;
			}
			VN_ALIGN(64) static constexpr std::array<char, 10> table{ gen() };
			VN_ALIGN(64) static constexpr const char* __restrict values{ table.data() };
		};

		template<uint64_t size> struct char_array {
			VN_FORCE_INLINE operator const char*() const {
				return values;
			}

			VN_FORCE_INLINE constexpr char& operator[](uint64_t index) noexcept {
				return values[index];
			}
			char values[size];
		};

		template<typename v_type> struct int_tables_impl<std::endian::little, 2, v_type> {
			static constexpr std::array<uint16_t, 100> gen() {
				std::array<uint16_t, 100> t{};
				for (uint32_t i = 0; i < 100; ++i) {
					t[i] |= static_cast<uint16_t>('0' + (i / 10));
					t[i] |= static_cast<uint16_t>('0' + (i % 10)) << 8;
				}
				return t;
			}
			VN_ALIGN(64) static constexpr std::array<uint16_t, 100> table{ gen() };
			VN_ALIGN(64) static constexpr const uint16_t* __restrict values{ table.data() };
		};

		template<typename v_type> struct int_tables_impl<std::endian::little, 3, v_type> {
			static constexpr std::array<char_array<3>, 1000> gen() {
				std::array<char_array<3>, 1000> t{};
				for (uint32_t i = 0; i < 1000; ++i) {
					t[i][0] = static_cast<char>('0' + (i / 100));
					t[i][1] = static_cast<char>('0' + (i / 10 % 10));
					t[i][2] = static_cast<char>('0' + (i % 10));
				}
				return t;
			}
			VN_ALIGN(64) static constexpr std::array<char_array<3>, 1000> table{ gen() };
			VN_ALIGN(64) static constexpr const char_array<3>* __restrict values{ table.data() };
		};

		template<typename v_type> struct int_tables_impl<std::endian::little, 4, v_type> {
			static constexpr std::array<uint32_t, 10000> gen() {
				std::array<uint32_t, 10000> t{};
				for (uint32_t i = 0; i < 10000; ++i) {
					t[i] |= static_cast<uint32_t>('0' + (i / 1000));
					t[i] |= static_cast<uint32_t>('0' + (i / 100 % 10)) << 8;
					t[i] |= static_cast<uint32_t>('0' + (i / 10 % 10)) << 16;
					t[i] |= static_cast<uint32_t>('0' + (i % 10)) << 24;
				}
				return t;
			}
			VN_ALIGN(64) static constexpr std::array<uint32_t, 10000> table{ gen() };
			VN_ALIGN(64) static constexpr const uint32_t* __restrict values{ table.data() };
		};

		template<typename v_type> struct int_tables_impl<std::endian::big, 2, v_type> {
			static constexpr std::array<uint16_t, 100> gen() {
				std::array<uint16_t, 100> t{};
				for (uint32_t i = 0; i < 100; ++i) {
					t[i] |= static_cast<uint16_t>('0' + (i / 10)) << 8;
					t[i] |= static_cast<uint16_t>('0' + (i % 10));
				}
				return t;
			}
			VN_ALIGN(64) static constexpr std::array<uint16_t, 100> table{ gen() };
			VN_ALIGN(64) static constexpr const uint16_t* __restrict values{ table.data() };
		};

		template<typename v_type> struct int_tables_impl<std::endian::big, 3, v_type> {
			static constexpr std::array<char_array<3>, 1000> gen() {
				std::array<char_array<3>, 1000> t{};
				for (uint32_t i = 0; i < 1000; ++i) {
					t[i][2] = static_cast<char>('0' + (i / 100));
					t[i][1] = static_cast<char>('0' + (i / 10 % 10));
					t[i][0] = static_cast<char>('0' + (i % 10));
				}
				return t;
			}
			VN_ALIGN(64) static constexpr std::array<char_array<3>, 1000> table{ gen() };
			VN_ALIGN(64) static constexpr const char_array<3>* __restrict values{ table.data() };
		};

		template<typename v_type> struct int_tables_impl<std::endian::big, 4, v_type> {
			static constexpr std::array<uint32_t, 10000> gen() {
				std::array<uint32_t, 10000> t{};
				for (uint32_t i = 0; i < 10000; ++i) {
					t[i] |= static_cast<uint32_t>('0' + (i / 1000)) << 24;
					t[i] |= static_cast<uint32_t>('0' + (i / 100 % 10)) << 16;
					t[i] |= static_cast<uint32_t>('0' + (i / 10 % 10)) << 8;
					t[i] |= static_cast<uint32_t>('0' + (i % 10));
				}
				return t;
			}
			VN_ALIGN(64) static constexpr std::array<uint32_t, 10000> table{ gen() };
			VN_ALIGN(64) static constexpr const uint32_t* __restrict values{ table.data() };
		};

		template<uint64_t size> using int_tables = int_tables_impl<std::endian::native, size>;

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
			} else if constexpr (divisor == 1000000000ULL) {
				return 8ULL;
			} else if constexpr (divisor == 10000000000ULL) {
				return 9ULL;
			} else if constexpr (divisor == 100000000000ULL) {
				return 10ULL;
			} else if constexpr (divisor == 1000000000000ULL) {
				return 11ULL;
			} else if constexpr (divisor == 10000000000000ULL) {
				return 12ULL;
			} else if constexpr (divisor == 100000000000000ULL) {
				return 13ULL;
			} else if constexpr (divisor == 1000000000000000ULL) {
				return 14ULL;
			} else if constexpr (divisor == 10000000000000000ULL) {
				return 15ULL;
			} else if constexpr (divisor == 100000000000000000ULL) {
				return 16ULL;
			} else if constexpr (divisor == 1000000000000000000ULL) {
				return 17ULL;
			} else if constexpr (divisor == 10000000000000000000ULL) {
				return 18ULL;
			} else {
				static_assert(false_type::value, "Sorry, but that divisor is not within this table!");
			}
		}

		template<typename v_type> struct mul_shift_table;

		template<typename v_type> struct mul_shift_entry {
			VN_ALIGN(64) v_type multiplicand;
			VN_ALIGN(64) uint64_t shift;
		};

		template<uint16_types v_type> struct mul_shift_table<v_type> {
			static constexpr std::array<mul_shift_entry<v_type>, 5> gen() {
				std::array<mul_shift_entry<v_type>, 5> t{
					mul_shift_entry<v_type>{ 0x999a, 0x18 },
					{ 0x8f5d, 0x18 },
					{ 0x8313, 0x19 },
					{ 0xd1b8, 0x1d },
					{ 0xf367, 0x1f },
				};
				return t;
			}
			VN_ALIGN(64) static constexpr std::array<mul_shift_entry<v_type>, 5> table{ gen() };
			VN_ALIGN(64) static constexpr const mul_shift_entry<v_type>* __restrict values{ table.data() };
		};

		template<uint32_types v_type> struct mul_shift_table<v_type> {
			static constexpr std::array<mul_shift_entry<v_type>, 8> gen() {
				std::array<mul_shift_entry<v_type>, 8> t{
					mul_shift_entry<v_type>{ 0xcccccccdU, 35 },
					{ 0xa3d70a3eU, 38 },
					{ 0x83126e98U, 41 },
					{ 0xd1b71759U, 45 },
					{ 0xa7c5ac48U, 48 },
					{ 0x8637bd06U, 51 },
					{ 0xd6bf94d6U, 55 },
					{ 0xabcc7712U, 58 },
				};
				return t;
			}
			VN_ALIGN(64) static constexpr std::array<mul_shift_entry<v_type>, 8> table{ gen() };
			VN_ALIGN(64) static constexpr const mul_shift_entry<v_type>* __restrict values{ table.data() };
		};

		template<uint64_types v_type> struct mul_shift_table<v_type> {
			static constexpr std::array<mul_shift_entry<v_type>, 16> gen() {
				std::array<mul_shift_entry<v_type>, 16> t{
					mul_shift_entry<v_type>{ 0xcccccccccccccccd, 67 },
					{ 0xa3d70a3d70a3d70b, 70 },
					{ 0x83126e978d4fdf3c, 73 },
					{ 0xd1b71758e219652c, 77 },
					{ 0xa7c5ac471b478424, 80 },
					{ 0x8637bd05af6c69b6, 83 },
					{ 0xd6bf94d5e57a42bd, 87 },
					{ 0xabcc77118461cefd, 90 },
					{ 0x89705f4136b4a598, 93 },
					{ 0xdbe6fecebdedd5bf, 97 },
					{ 0xafebff0bcb24aaff, 100 },
					{ 0x8cbccc096f5088cc, 103 },
					{ 0xe12e13424bb40e14, 107 },
					{ 0xb424dc35095cd810, 110 },
					{ 0x901d7cf73ab0acda, 113 },
					{ 0xe69594bec44de15c, 117 },
				};
				return t;
			}
			VN_ALIGN(64) static constexpr std::array<mul_shift_entry<v_type>, 16> table{ gen() };
			VN_ALIGN(64) static constexpr const mul_shift_entry<v_type>* __restrict values{ table.data() };
		};

#if !VN_COMPILER_CLANG && !VN_COMPILER_GNU && !VN_COMPILER_MSVC

		template<uint_types v_type_new> VN_FORCE_INLINE static v_type_new mulhi_portable(v_type_new a, v_type_new b) noexcept {
			using v_type						 = get_next_higher_int_t<v_type_new>;
			static constexpr uint64_t total_bits = sizeof(v_type_new) * 8;
			static constexpr uint64_t half_bits	 = total_bits / 2;
			static constexpr v_type mask		 = (static_cast<v_type>(1) << half_bits) - 1;
			const v_type a_lo					 = static_cast<v_type>(a) & mask;
			const v_type a_hi					 = static_cast<v_type>(a) >> half_bits;
			const v_type b_lo					 = static_cast<v_type>(b) & mask;
			const v_type b_hi					 = static_cast<v_type>(b) >> half_bits;
			const v_type lo_lo					 = a_lo * b_lo;
			const v_type hi_lo					 = a_hi * b_lo;
			const v_type lo_hi					 = a_lo * b_hi;
			const v_type hi_hi					 = a_hi * b_hi;
			const v_type cross					 = (lo_lo >> half_bits) + (hi_lo & mask) + (lo_hi & mask);
			return static_cast<v_type_new>(hi_hi + (hi_lo >> half_bits) + (lo_hi >> half_bits) + (cross >> half_bits));
		}

#endif

		template<typename v_type, uint64_t divisor> struct multiply_and_shift;

		template<uint_types v_type, uint64_t divisor> struct multiply_and_shift<v_type, divisor> {
			static constexpr auto entry = mul_shift_table<v_type>::values[divisor_to_index<divisor>()];
			VN_FORCE_INLINE static v_type impl(v_type value) noexcept {
				static constexpr v_type m	= entry.multiplicand;
				static constexpr uint64_t s = entry.shift;
				return static_cast<v_type>((static_cast<uint64_t>(value) * m) >> s);
			}
		};

		template<uint64_types v_type, uint64_t divisor> struct multiply_and_shift<v_type, divisor> {
			static constexpr auto entry = mul_shift_table<v_type>::values[divisor_to_index<divisor>()];
			static constexpr v_type m	= entry.multiplicand;
			static constexpr uint64_t s = entry.shift;
			static_assert(s >= 64ULL);
			VN_FORCE_INLINE static v_type impl(v_type value) noexcept {
#if VN_COMPILER_CLANG || VN_COMPILER_GNU
				return static_cast<v_type>(static_cast<__uint128_t>(value) * m >> s);
#elif VN_COMPILER_MSVC
				v_type high_part;
				_umul128(m, value, &high_part);
				return static_cast<v_type>(high_part >> (s - 64ULL));
#else
				return static_cast<v_type>(mulhi_portable(value, m) >> (s - 64ULL));
#endif
			}
		};

		VN_ALIGN(64) static constexpr const auto* __restrict char_table_1_digit_data = int_tables<1>::values;
		VN_ALIGN(64) static constexpr const auto* __restrict char_table_2_digit_data = int_tables<2>::values;
		VN_ALIGN(64) static constexpr const auto* __restrict char_table_3_digit_data = int_tables<3>::values;
		VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;

	}

}
