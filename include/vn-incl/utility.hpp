// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp
// vn-incl/utility.hpp

#pragma once

#include <vn-incl/concepts.hpp>
#include <limits>

namespace vn {

	namespace detail {

		template<typename v_type> struct get_int_type {
			using type = std::conditional_t<std::is_unsigned_v<v_type>, uint8_t, int8_t>;
		};

		template<auto value_new> struct make_static {
			static constexpr auto value{ value_new };
		};

		template<typename v_type> struct get_next_higher_int;

		template<uint_types v_type> struct get_next_higher_int<v_type> {
			using type = std::conditional_t<std::same_as<uint8_t, base_t<v_type>>, uint16_t, std::conditional_t<std::same_as<uint16_t, base_t<v_type>>, uint32_t, uint64_t>>;
		};

		template<int_types v_type> struct get_next_higher_int<v_type> {
			using type = std::conditional_t<std::same_as<int8_t, base_t<v_type>>, int16_t, std::conditional_t<std::same_as<int16_t, base_t<v_type>>, int32_t, int64_t>>;
		};

		template<integer_types v_type> using get_next_higher_int_t = typename get_next_higher_int<v_type>::type;

		template<uint8_types auto repeat, uint_types return_type> static consteval return_type repeat_byte() noexcept {
			return static_cast<return_type>(0x0101010101010101ull) * static_cast<return_type>(repeat);
		}

		template<uint8_types auto repeat, uint64_t repetition_count> struct repeat_bytes {
			VN_FORCE_INLINE static consteval auto impl() {
				if constexpr (repetition_count == 1) {
					return static_cast<uint8_t>(0x01) * static_cast<uint8_t>(repeat);
				} else if constexpr (repetition_count == 2) {
					return static_cast<uint16_t>(0x0101) * static_cast<uint16_t>(repeat);
				} else if constexpr (repetition_count == 3) {
					return static_cast<uint32_t>(0x010101) * static_cast<uint32_t>(repeat);
				} else if constexpr (repetition_count == 4) {
					return static_cast<uint32_t>(0x01010101) * static_cast<uint32_t>(repeat);
				} else if constexpr (repetition_count == 5) {
					return static_cast<uint64_t>(0x0101010101) * static_cast<uint64_t>(repeat);
				} else if constexpr (repetition_count == 6) {
					return static_cast<uint64_t>(0x010101010101) * static_cast<uint64_t>(repeat);
				} else if constexpr (repetition_count == 7) {
					return static_cast<uint64_t>(0x01010101010101) * static_cast<uint64_t>(repeat);
				} else {
					return static_cast<uint64_t>(0x0101010101010101) * static_cast<uint64_t>(repeat);
				}
			}
		};

		template<uint8_types auto repeat, uint64_t repitition_count> static constexpr uint64_t repeat_bytes_v = repeat_bytes<repeat, repitition_count>::impl();

		template<uint_types v_type> VN_FORCE_INLINE constexpr v_type tzcnt_constexpr(const v_type value) noexcept {
			if (value == 0)
				return device_cast<v_type>(sizeof(v_type) * 8);

			v_type count = 0;
			while ((value & 1) == 0) {
				value >>= 1;
				++count;
			}
			return count;
		}

		template<uint8_types v_type> VN_FORCE_INLINE constexpr v_type tzcnt_constexpr(const v_type value) noexcept {
			return tzcnt_constexpr(value);
		}

		template<uint16_types v_type> VN_FORCE_INLINE constexpr v_type tzcnt(const v_type value) noexcept {
#if VN_COMPILER_MSVC
	#if VN_ARCH_ARM64
			uint64_t trailing_zero = 0;
			if (_BitScanForward(&trailing_zero, static_cast<uint64_t>(value))) {
				return static_cast<v_type>(trailing_zero);
			} else {
				return 16;
			}
	#else
			return static_cast<v_type>(_tzcnt_u16(value));
	#endif
#elif VN_COMPILER_CLANG || VN_COMPILER_GNU
			return (value == 0) ? 16 : static_cast<v_type>(__builtin_ctz(static_cast<uint32_t>(value)));
#else
			return tzcnt_constexpr(value);
#endif
		}

		template<uint32_types v_type> VN_FORCE_INLINE constexpr v_type tzcnt(const v_type value) noexcept {
#if VN_COMPILER_MSVC
	#if VN_ARCH_ARM64
			uint64_t trailing_zero = 0;
			if (_BitScanForward(&trailing_zero, static_cast<uint64_t>(value))) {
				return static_cast<v_type>(trailing_zero);
			} else {
				return 32;
			}
	#else
			return static_cast<v_type>(_tzcnt_u32(value));
	#endif
#elif VN_COMPILER_CLANG || VN_COMPILER_GNU
			return (value == 0) ? 32 : static_cast<v_type>(__builtin_ctz(static_cast<uint32_t>(value)));
#else
			return tzcnt_constexpr(value);
#endif
		}

		template<uint64_types v_type> VN_FORCE_INLINE constexpr v_type tzcnt(const v_type value) noexcept {
#if VN_COMPILER_MSVC
	#if VN_ARCH_ARM64
			uint64_t trailing_zero = 0;
			if (_BitScanForward64(&trailing_zero, static_cast<uint32_t __int64>(value))) {
				return static_cast<v_type>(trailing_zero);
			} else {
				return 64;
			}
	#else
			return static_cast<v_type>(_tzcnt_u64(value));
	#endif
#elif VN_COMPILER_CLANG || VN_COMPILER_GNU
			return (value == 0) ? 64 : static_cast<v_type>(__builtin_ctzll(static_cast<uint64_t>(value)));
#else
			return tzcnt_constexpr(value);
#endif
		}

	}

	

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

			VN_ALIGN(64)
			static constexpr const char* __restrict values_02{ [] {
				VN_ALIGN(64)
				constexpr auto table{ [] {
					std::array<char, 200> t{};
					for (uint32_t n = 0; n < 100; ++n) {
						t[n * 2] |= '0' + (n / 10);
						t[n * 2 + 1] |= '0' + (n % 10);
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
					std::array<mul_shift_entry<v_type>, 16> t{ mul_shift_entry<v_type>{ 0xcccccccccccccccd, 67 }, { 0xa3d70a3d70a3d70b, 70 }, { 0x83126e978d4fdf3c, 73 },
						{ 0xd1b71758e219652c, 77 }, { 0xa7c5ac471b478424, 80 }, { 0x8637bd05af6c69b6, 83 }, { 0xd6bf94d5e57a42bd, 87 }, { 0xabcc77118461cefd, 90 },
						{ 0x89705f4136b4a598, 93 }, { 0xdbe6fecebdedd5bf, 97 }, { 0xafebff0bcb24aaff, 100 }, { 0x8cbccc096f5088cc, 103 }, { 0xe12e13424bb40e14, 107 },
						{ 0xb424dc35095cd810, 110 }, { 0x901d7cf73ab0acda, 113 }, { 0xe69594bec44de15c, 117 } };
					return t;
				}() };
				return make_static<table>::value.data();
			}() };
		};

		template<typename v_type, uint64_t divisor> struct multiply_and_shift;

		template<uint_types v_type, uint64_t divisor> struct multiply_and_shift<v_type, divisor> {
			static constexpr auto entry = mul_shift_table<v_type>::values[divisor_to_index<divisor>()];
			VN_FORCE_INLINE static v_type impl(v_type value) noexcept {
				static constexpr v_type m	= entry.multiplicand;
				static constexpr uint64_t s = entry.shift;
				return static_cast<v_type>((static_cast<uint64_t>(value) * m) >> s);
			}
		};

		template<uint64_types v_type, uint64_t divisor>
			requires(divisor < 100000000ULL)
		struct multiply_and_shift<v_type, divisor> {
			static constexpr auto entry = mul_shift_table<uint32_t>::values[divisor_to_index<divisor>()];
			static constexpr v_type m	= entry.multiplicand;
			static constexpr uint64_t s = entry.shift;
			VN_FORCE_INLINE static v_type impl(v_type value) noexcept {
				return static_cast<v_type>((static_cast<uint64_t>(value) * m) >> s);
			}
		};

		template<uint64_types v_type, uint64_t divisor>
			requires(divisor >= 100000000ULL)
		struct multiply_and_shift<v_type, divisor> {
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

		template<typename v_type> struct fast_div_entry {
			VN_ALIGN(64) v_type inv_odd;
			VN_ALIGN(64) v_type max_quotient;
			VN_ALIGN(64) v_type even_mask;
		};

		template<typename v_type> struct fast_div_table;

		template<uint16_types v_type> struct fast_div_table<v_type> {
			VN_ALIGN(64)
			static constexpr const fast_div_entry<uint32_t>* __restrict values{ [] {
				VN_ALIGN(64)
				constexpr auto table{ [] {
					std::array<fast_div_entry<uint32_t>, 4> t{
						fast_div_entry<uint32_t>{ 0xcccccccd, 858993459, 1 },
						{ 0xc28f5c29, 171798691, 3 },
						{ 0x26e978d5, 34359738, 7 },
						{ 0x3afb7e91, 6871947, 15 },
					};
					return t;
				}() };
				return make_static<table>::value.data();
			}() };
		};

		template<uint32_types v_type> struct fast_div_table<v_type> {
			VN_ALIGN(64)
			static constexpr const fast_div_entry<uint32_t>* __restrict values{ [] {
				VN_ALIGN(64)
				constexpr auto table{ [] {
					std::array<fast_div_entry<uint32_t>, 8> t{
						fast_div_entry<uint32_t>{ 0xcccccccd, 858993459, 1 },
						{ 0xc28f5c29, 171798691, 3 },
						{ 0x26e978d5, 34359738, 7 },
						{ 0x3afb7e91, 6871947, 15 },
						{ 0xbcbe61d, 1374389, 31 },
						{ 0x68c26139, 274877, 63 },
						{ 0xae8d46a5, 54975, 127 },
						{ 0x22e90e21, 10995, 255 },
					};
					return t;
				}() };
				return make_static<table>::value.data();
			}() };
		};

		template<uint64_types v_type> struct fast_div_table<v_type> {
			VN_ALIGN(64)
			static constexpr const fast_div_entry<uint64_t>* __restrict values{ [] {
				VN_ALIGN(64)
				constexpr auto table{ [] {
					std::array<fast_div_entry<uint64_t>, 18> t{
						fast_div_entry<uint64_t>{ 0xcccccccccccccccd, 3689348814741910323, 1 },
						{ 0x8f5c28f5c28f5c29, 737869762948382064, 3 },
						{ 0x1cac083126e978d5, 147573952589676412, 7 },
						{ 0xd288ce703afb7e91, 29514790517935282, 15 },
						{ 0x5d4e8fb00bcbe61d, 5902958103587056, 31 },
						{ 0x790fb65668c26139, 1180591620717411, 63 },
						{ 0xe5032477ae8d46a5, 236118324143482, 127 },
						{ 0xc767074b22e90e21, 47223664828696, 255 },
						{ 0x8e47ce423a2e9c6d, 9444732965739, 511 },
						{ 0x4fa7f60d3ed61f49, 1888946593147, 1023 },
						{ 0xfee64690c913975, 377789318629, 2047 },
						{ 0x3662e0e1cf503eb1, 75557863725, 4095 },
						{ 0xa47a2cf9f6433fbd, 15111572745, 8191 },
						{ 0x54186f653140a659, 3022314549, 16383 },
						{ 0x7738164770402145, 604462909, 32767 },
						{ 0xe4a4d1417cd9a041, 120892581, 65535 },
						{ 0xc75429d9e5c5200d, 24178516, 131071 },
						{ 0xc1773b91fac10669, 4835703, 262143 },
					};
					return t;
				}() };
				return make_static<table>::value.data();
			}() };
		};

		template<uint_types v_type, uint64_t divisor> struct fast_divisibility;

		template<uint16_types v_type, uint64_t divisor> struct fast_divisibility<v_type, divisor> {
			static constexpr auto entry = fast_div_table<v_type>::values[divisor_to_index<divisor>()];
			VN_FORCE_INLINE static bool impl(v_type value) noexcept {
				const uint32_t v = static_cast<uint32_t>(value);
				return ((v & entry.even_mask) == 0) && ((v * entry.inv_odd) <= entry.max_quotient);
			}
		};

		template<uint32_types v_type, uint64_t divisor> struct fast_divisibility<v_type, divisor> {
			static constexpr auto entry = fast_div_table<v_type>::values[divisor_to_index<divisor>()];
			VN_FORCE_INLINE static bool impl(v_type value) noexcept {
				return ((value & entry.even_mask) == 0) && ((value * entry.inv_odd) <= entry.max_quotient);
			}
		};

		template<uint64_types v_type, uint64_t divisor> struct fast_divisibility<v_type, divisor> {
			static constexpr auto entry = fast_div_table<v_type>::values[divisor_to_index<divisor>()];
			VN_FORCE_INLINE static bool impl(v_type value) noexcept {
				return ((value & entry.even_mask) == 0) && ((value * entry.inv_odd) <= entry.max_quotient);
			}
		};

		VN_FORCE_INLINE static uint32_t count_trailing_decimal_zeros(uint64_t n) noexcept {
			if (n == 0) [[unlikely]]
				return 0;
			uint32_t z = 0;
			if (fast_divisibility<uint64_t, 10000000000000000ULL>::impl(n)) {
				n = multiply_and_shift<uint64_t, 10000000000000000ULL>::impl(n);
				z += 16;
			}
			if (fast_divisibility<uint64_t, 100000000ULL>::impl(n)) {
				n = multiply_and_shift<uint64_t, 100000000ULL>::impl(n);
				z += 8;
			}
			if (fast_divisibility<uint64_t, 10000>::impl(n)) {
				n = multiply_and_shift<uint64_t, 10000>::impl(n);
				z += 4;
			}
			if (fast_divisibility<uint64_t, 100>::impl(n)) {
				n = multiply_and_shift<uint64_t, 100>::impl(n);
				z += 2;
			}
			if (fast_divisibility<uint64_t, 10>::impl(n)) {
				z += 1;
			}
			return z;
		}

		static constexpr const auto* __restrict char_table_1_digit_data = int_tables<1>::values;
		static constexpr const auto* __restrict char_table_2_digit_data = int_tables<2>::values;
		static constexpr const auto* __restrict char_table_3_digit_data = int_tables<3>::values;
		static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;

		VN_ALIGN(64)
		inline static constexpr uint8_t digit_counts[]{ 19, 19, 19, 19, 18, 18, 18, 17, 17, 17, 16, 16, 16, 16, 15, 15, 15, 14, 14, 14, 13, 13, 13, 13, 12, 12, 12, 11, 11, 11, 10,
			10, 10, 10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 7, 6, 6, 6, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1 };

		VN_ALIGN(64)
		inline static constexpr uint64_t digit_count_thresholds[]{ 0ULL, 9ULL, 99ULL, 999ULL, 9999ULL, 99999ULL, 999999ULL, 9999999ULL, 99999999ULL, 999999999ULL, 9999999999ULL,
			99999999999ULL, 999999999999ULL, 9999999999999ULL, 99999999999999ULL, 999999999999999ULL, 9999999999999999ULL, 99999999999999999ULL, 999999999999999999ULL,
			9999999999999999999ULL };

		VN_FORCE_INLINE static uint64_t fast_digit_count(const uint64_t inputValue) {
			const uint64_t digit_count{ digit_counts[std::countl_zero(inputValue)] };
			return digit_count + static_cast<uint64_t>(inputValue > digit_count_thresholds[digit_count]);
		}
	}

}
