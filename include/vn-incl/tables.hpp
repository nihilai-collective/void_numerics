// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp
// vn-incl/tables.hpp

#pragma once

#include <vn-incl/utility.hpp>
#include <limits>

namespace vn {

	namespace detail {

		template<typename v_type> VN_ALIGN(64)
		static constexpr const std::make_unsigned_t<v_type>* __restrict raw_comp_vals_pos{ [] {
			VN_ALIGN(64)
			constexpr std::array<std::make_unsigned_t<v_type>, 256> return_values{ [] {
				constexpr auto max_value{ static_cast<std::make_unsigned_t<v_type>>(std::numeric_limits<std::decay_t<v_type>>::max()) };
				std::array<std::make_unsigned_t<v_type>, 256> return_values_internal{};
				return_values_internal['0'] = (max_value - 0) / 10;
				return_values_internal['1'] = (max_value - 1) / 10;
				return_values_internal['2'] = (max_value - 2) / 10;
				return_values_internal['3'] = (max_value - 3) / 10;
				return_values_internal['4'] = (max_value - 4) / 10;
				return_values_internal['5'] = (max_value - 5) / 10;
				return_values_internal['6'] = (max_value - 6) / 10;
				return_values_internal['7'] = (max_value - 7) / 10;
				return_values_internal['8'] = (max_value - 8) / 10;
				return_values_internal['9'] = (max_value - 9) / 10;
				return return_values_internal;
			}() };
			return make_static<return_values>::value.data();
		}() };

		template<typename v_type> VN_ALIGN(64)
		static constexpr const std::make_unsigned_t<v_type>* __restrict raw_comp_vals_neg{ [] {
			VN_ALIGN(64)
			constexpr std::array<std::make_unsigned_t<v_type>, 256> return_values{ [] {
				constexpr auto max_value{ static_cast<std::make_unsigned_t<v_type>>(std::numeric_limits<std::make_signed_t<v_type>>::max()) + 1 };
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
			}() };
			return make_static<return_values>::value.data();
		}() };

		template<typename = void> struct pow_tables {
			VN_ALIGN(64)
			static constexpr const uint64_t* __restrict power_of_ten_uint{ [] {
				VN_ALIGN(64)
				constexpr auto table{ [] {
					std::array<uint64_t, 20> t{ 1ull, 10ull, 100ull, 1000ull, 10000ull, 100000ull, 1000000ull, 10000000ull, 100000000ull, 1000000000ull, 10000000000ull,
						100000000000ull, 1000000000000ull, 10000000000000ull, 100000000000000ull, 1000000000000000ull, 10000000000000000ull, 100000000000000000ull,
						1000000000000000000ull, 10000000000000000000ull };
					return t;
				}() };
				return make_static<table>::value.data();
			}() };

			VN_ALIGN(64)
			static constexpr const int64_t* __restrict power_of_ten_int{ [] {
				VN_ALIGN(64)
				constexpr auto table{ [] {
					std::array<int64_t, 19> t{ 1ll, 10ll, 100ll, 1000ll, 10000ll, 100000ll, 1000000ll, 10000000ll, 100000000ll, 1000000000ll, 10000000000ll, 100000000000ll,
						1000000000000ll, 10000000000000ll, 100000000000000ll, 1000000000000000ll, 10000000000000000ll, 100000000000000000ll, 1000000000000000000ll };
					return t;
				}() };
				return make_static<table>::value.data();
			}() };
		};

		template<uint8_types v_type> VN_FORCE_INLINE static constexpr bool is_digit(v_type c) noexcept {
			return c - static_cast<uint8_t>('0') < 10;
		}

		template<typename = void> struct exp_tables {
			VN_ALIGN(64)
			static constexpr const bool* __restrict exp_table{ [] {
				VN_ALIGN(64)
				constexpr auto table{ [] {
					std::array<bool, 256> t{};
					t['E'] = true;
					t['e'] = true;
					return t;
				}() };
				return make_static<table>::value.data();
			}() };

			VN_ALIGN(64)
			static constexpr const bool* __restrict exp_frac_table{ [] {
				VN_ALIGN(64)
				constexpr auto table{ [] {
					std::array<bool, 256> t{};
					t['.'] = true;
					t['E'] = true;
					t['e'] = true;
					return t;
				}() };
				return make_static<table>::value.data();
			}() };
		};

		VN_ALIGN(64) inline constexpr char decimal{ '.' };
		VN_ALIGN(64) inline constexpr char minus{ '-' };
		VN_ALIGN(64) inline constexpr char plus{ '+' };
		VN_ALIGN(64) inline constexpr char zero{ '0' };
		VN_ALIGN(64) inline constexpr char nine{ '9' };

		template<std::endian, uint64_t size = 0, typename = void> struct int_tables_impl;

		template<std::endian endianity, typename v_type> struct int_tables_impl<endianity, 1, v_type> {
			VN_ALIGN(64)
			static constexpr const char* __restrict values{ [] {
				VN_ALIGN(64)
				constexpr auto table{ [] {
					std::array<char, 10> t{};
					for (uint32_t i = 0; i < 10; ++i) {
						t[i] = static_cast<char>('0' + i);
					}
					return t;
				}() };
				return make_static<table>::value.data();
			}() };
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
			VN_ALIGN(64)
			static constexpr const uint16_t* __restrict values{ [] {
				VN_ALIGN(64)
				constexpr auto table{ [] {
					std::array<uint16_t, 100> t{};
					for (uint32_t i = 0; i < 100; ++i) {
						t[i] |= static_cast<uint16_t>('0' + (i / 10));
						t[i] |= static_cast<uint16_t>('0' + (i % 10)) << 8;
					}
					return t;
				}() };
				return make_static<table>::value.data();
			}() };
		};

		template<typename v_type> struct int_tables_impl<std::endian::little, 3, v_type> {
			VN_ALIGN(64)
			static constexpr const char_array<3>* __restrict values{ [] {
				VN_ALIGN(64)
				constexpr auto table{ [] {
					std::array<char_array<3>, 1000> t{};
					for (uint32_t i = 0; i < 1000; ++i) {
						t[i][0] = static_cast<char>('0' + (i / 100));
						t[i][1] = static_cast<char>('0' + (i / 10 % 10));
						t[i][2] = static_cast<char>('0' + (i % 10));
					}
					return t;
				}() };
				return make_static<table>::value.data();
			}() };
		};

		template<typename v_type> struct int_tables_impl<std::endian::little, 4, v_type> {
			VN_ALIGN(64)
			static constexpr const uint32_t* __restrict values{ [] {
				VN_ALIGN(64)
				constexpr auto table{ [] {
					std::array<uint32_t, 10000> t{};
					for (uint32_t i = 0; i < 10000; ++i) {
						t[i] |= static_cast<uint32_t>('0' + (i / 1000));
						t[i] |= static_cast<uint32_t>('0' + (i / 100 % 10)) << 8;
						t[i] |= static_cast<uint32_t>('0' + (i / 10 % 10)) << 16;
						t[i] |= static_cast<uint32_t>('0' + (i % 10)) << 24;
					}
					return t;
				}() };
				return make_static<table>::value.data();
			}() };
		};

		template<typename v_type> struct int_tables_impl<std::endian::big, 2, v_type> {
			VN_ALIGN(64)
			static constexpr const uint16_t* __restrict values{ [] {
				VN_ALIGN(64)
				constexpr auto table{ [] {
					std::array<uint16_t, 100> t{};
					for (uint32_t i = 0; i < 100; ++i) {
						t[i] |= static_cast<uint16_t>('0' + (i / 10)) << 8;
						t[i] |= static_cast<uint16_t>('0' + (i % 10));
					}
					return t;
				}() };
				return make_static<table>::value.data();
			}() };
		};

		template<typename v_type> struct int_tables_impl<std::endian::big, 3, v_type> {
			VN_ALIGN(64)
			static constexpr const char_array<3>* __restrict values{ [] {
				VN_ALIGN(64)
				constexpr auto table{ [] {
					std::array<char_array<3>, 1000> t{};
					for (uint32_t i = 0; i < 1000; ++i) {
						t[i][2] = static_cast<char>('0' + (i / 100));
						t[i][1] = static_cast<char>('0' + (i / 10 % 10));
						t[i][0] = static_cast<char>('0' + (i % 10));
					}
					return t;
				}() };
				return make_static<table>::value.data();
			}() };
		};

		template<typename v_type> struct int_tables_impl<std::endian::big, 4, v_type> {
			VN_ALIGN(64)
			static constexpr const uint32_t* __restrict values{ [] {
				VN_ALIGN(64)
				constexpr auto table{ [] {
					std::array<uint32_t, 10000> t{};
					for (uint32_t i = 0; i < 10000; ++i) {
						t[i] |= static_cast<uint32_t>('0' + (i / 1000)) << 24;
						t[i] |= static_cast<uint32_t>('0' + (i / 100 % 10)) << 16;
						t[i] |= static_cast<uint32_t>('0' + (i / 10 % 10)) << 8;
						t[i] |= static_cast<uint32_t>('0' + (i % 10));
					}
					return t;
				}() };
				return make_static<table>::value.data();
			}() };
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
			VN_ALIGN(64)
			static constexpr const mul_shift_entry<v_type>* __restrict values{ [] {
				VN_ALIGN(64)
				constexpr auto table{ [] {
					std::array<mul_shift_entry<v_type>, 5> t{ {
						{ 0x999a, 0x18 },
						{ 0x8f5d, 0x18 },
						{ 0x8313, 0x19 },
						{ 0xd1b8, 0x1d },
						{ 0xf367, 0x1f },
					} };
					return t;
				}() };
				return make_static<table>::value.data();
			}() };
		};

		template<uint32_types v_type> struct mul_shift_table<v_type> {
			VN_ALIGN(64)
			static constexpr const mul_shift_entry<v_type>* __restrict values{ [] {
				VN_ALIGN(64)
				constexpr auto table{ [] {
					std::array<mul_shift_entry<v_type>, 8> t{ {
						{ 0xcccccccdU, 35 },
						{ 0xa3d70a3eU, 38 },
						{ 0x83126e98U, 41 },
						{ 0xd1b71759U, 45 },
						{ 0xa7c5ac48U, 48 },
						{ 0x8637bd06U, 51 },
						{ 0xd6bf94d6U, 55 },
						{ 0xabcc7712U, 58 },
					} };
					return t;
				}() };
				return make_static<table>::value.data();
			}() };
		};

		template<uint64_types v_type> struct mul_shift_table<v_type> {
			VN_ALIGN(64)
			static constexpr const mul_shift_entry<v_type>* __restrict values{ [] {
				VN_ALIGN(64)
				constexpr auto table{ [] {
					std::array<mul_shift_entry<v_type>, 8> t{ {
						{ 0xcccccccccccccccdULL, 67 },
						{ 0xa3d70a3d70a3d70bULL, 70 },
						{ 0x83126e978d4fdf3cULL, 73 },
						{ 0xd1b71758e219652cULL, 77 },
						{ 0xa7c5ac471b478424ULL, 80 },
						{ 0x8637bd05af6c69b6ULL, 83 },
						{ 0xd6bf94d5e57a42bdULL, 87 },
						{ 0xabcc77118461cefdULL, 90 },
					} };
					return t;
				}() };
				return make_static<table>::value.data();
			}() };
		};

	}

}
