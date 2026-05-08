// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp
// vn-incl/str_to_i.hpp

#pragma once

#include <vn-incl/utility.hpp>
#include <iostream>

namespace vn {

	namespace detail {

		template<uint64_t byte_count> struct first_non_zero_byte;

		template<> struct first_non_zero_byte<1> {
			VN_FORCE_INLINE static uint64_t impl(const char* str) {
				return *str == '0';
			}
		};

		template<> struct first_non_zero_byte<2> {
			VN_FORCE_INLINE static uint64_t impl(const char* str) {
				uint16_t chunk;
				std::memcpy(&chunk, str, 2);
				uint16_t diff = chunk ^ repeat_bytes_v<static_cast<uint8_t>(0x30), 2>;

				return (diff == 0) ? 2 : tzcnt(diff) >> 3;
			}
		};

		template<> struct first_non_zero_byte<3> {
			VN_FORCE_INLINE static uint64_t impl(const char* str) {
				uint32_t chunk;
				std::memcpy(&chunk, str, 3);
				uint32_t diff = chunk ^ repeat_bytes_v<static_cast<uint8_t>(0x30), 3>;

				return (diff == 0) ? 3 : tzcnt(diff) >> 3;
			}
		};

		template<> struct first_non_zero_byte<4> {
			VN_FORCE_INLINE static uint64_t impl(const char* str) {
				uint32_t chunk;
				std::memcpy(&chunk, str, 4);
				uint32_t diff = chunk ^ repeat_bytes_v<static_cast<uint8_t>(0x30), 4>;
				return (diff == 0) ? 4 : tzcnt(diff) >> 3;
			}
		};

		template<> struct first_non_zero_byte<5> {
			VN_FORCE_INLINE static uint64_t impl(const char* str) {
				uint64_t chunk;
				std::memcpy(&chunk, str, 5);
				uint64_t diff = chunk ^ repeat_bytes_v<static_cast<uint8_t>(0x30), 5>;
				return (diff == 0) ? 5 : tzcnt(diff) >> 3;
			}
		};

		template<> struct first_non_zero_byte<6> {
			VN_FORCE_INLINE static uint64_t impl(const char* str) {
				uint64_t chunk;
				std::memcpy(&chunk, str, 6);
				uint64_t diff = chunk ^ repeat_bytes_v<static_cast<uint8_t>(0x30), 6>;
				return (diff == 0) ? 6 : tzcnt(diff) >> 3;
			}
		};

		template<> struct first_non_zero_byte<7> {
			VN_FORCE_INLINE static uint64_t impl(const char* str) {
				uint64_t chunk;
				std::memcpy(&chunk, str, 7);
				uint64_t diff = chunk ^ repeat_bytes_v<static_cast<uint8_t>(0x30), 7>;
				return (diff == 0) ? 7 : tzcnt(diff) >> 3;
			}
		};

		template<> struct first_non_zero_byte<8> {
			VN_FORCE_INLINE static uint64_t impl(const char* str) {
				uint64_t chunk;
				std::memcpy(&chunk, str, 8);
				uint64_t diff = chunk ^ repeat_bytes_v<static_cast<uint8_t>(0x30), 8>;
				return (diff == 0) ? 8 : tzcnt(diff) >> 3;
			}
		};

		template<> struct first_non_zero_byte<9> {
			VN_FORCE_INLINE static uint64_t impl(const char* str) {
				uint64_t chunk;
				std::memcpy(&chunk, str, 8);
				uint64_t diff = chunk ^ 0x3030303030303030ULL;
				if (diff != 0)
					return tzcnt(diff) >> 3;
				return (str[8] == '0') ? 9 : 8;
			}
		};

		template<> struct first_non_zero_byte<10> {
			VN_FORCE_INLINE static uint64_t impl(const char* str) {
				uint64_t chunk;
				std::memcpy(&chunk, str, 8);
				uint64_t diff = chunk ^ 0x3030303030303030ULL;
				if (diff != 0)
					return tzcnt(diff) >> 3;
				uint16_t chunk2;
				std::memcpy(&chunk2, str + 8, 2);
				uint16_t diff2 = chunk2 ^ 0x3030;
				return (diff2 == 0) ? 10 : 8 + (tzcnt(diff2) >> 3);
			}
		};

		template<> struct first_non_zero_byte<11> {
			VN_FORCE_INLINE static uint64_t impl(const char* str) {
				uint64_t chunk;
				std::memcpy(&chunk, str, 8);
				uint64_t diff = chunk ^ 0x3030303030303030ULL;
				if (diff != 0)
					return tzcnt(diff) >> 3;
				uint32_t chunk2;
				std::memcpy(&chunk2, str + 8, 3);
				uint32_t diff2 = chunk2 ^ 0x303030;
				return (diff2 == 0) ? 11 : 8 + (tzcnt(diff2) >> 3);
			}
		};

		template<> struct first_non_zero_byte<12> {
			VN_FORCE_INLINE static uint64_t impl(const char* str) {
				uint64_t chunk;
				std::memcpy(&chunk, str, 8);
				uint64_t diff = chunk ^ 0x3030303030303030ULL;
				if (diff != 0)
					return tzcnt(diff) >> 3;
				uint32_t chunk2;
				std::memcpy(&chunk2, str + 8, 4);
				uint32_t diff2 = chunk2 ^ 0x30303030;
				return (diff2 == 0) ? 12 : 8 + (tzcnt(diff2) >> 3);
			}
		};

		template<> struct first_non_zero_byte<13> {
			VN_FORCE_INLINE static uint64_t impl(const char* str) {
				uint64_t chunk;
				std::memcpy(&chunk, str, 8);
				uint64_t diff = chunk ^ 0x3030303030303030ULL;
				if (diff != 0)
					return tzcnt(diff) >> 3;
				uint64_t chunk2;
				std::memcpy(&chunk2, str + 8, 5);
				uint64_t diff2 = chunk2 ^ 0x3030303030ULL;
				return (diff2 == 0) ? 13 : 8 + (tzcnt(diff2) >> 3);
			}
		};

		template<> struct first_non_zero_byte<14> {
			VN_FORCE_INLINE static uint64_t impl(const char* str) {
				uint64_t chunk;
				std::memcpy(&chunk, str, 8);
				uint64_t diff = chunk ^ 0x3030303030303030ULL;
				if (diff != 0)
					return tzcnt(diff) >> 3;
				uint64_t chunk2;
				std::memcpy(&chunk2, str + 8, 6);
				uint64_t diff2 = chunk2 ^ 0x303030303030ULL;
				return (diff2 == 0) ? 14 : 8 + (tzcnt(diff2) >> 3);
			}
		};

		template<> struct first_non_zero_byte<15> {
			VN_FORCE_INLINE static uint64_t impl(const char* str) {
				uint64_t chunk;
				std::memcpy(&chunk, str, 8);
				uint64_t diff = chunk ^ 0x3030303030303030ULL;
				if (diff != 0)
					return tzcnt(diff) >> 3;
				uint64_t chunk2;
				std::memcpy(&chunk2, str + 8, 7);
				uint64_t diff2 = chunk2 ^ 0x30303030303030ULL;
				return (diff2 == 0) ? 15 : 8 + (tzcnt(diff2) >> 3);
			}
		};

		template<> struct first_non_zero_byte<16> {
			VN_FORCE_INLINE static uint64_t impl(const char* str) {
				uint64_t chunk;
				std::memcpy(&chunk, str, 8);
				uint64_t diff = chunk ^ 0x3030303030303030ULL;
				if (diff != 0)
					return tzcnt(diff) >> 3;
				uint64_t chunk2;
				std::memcpy(&chunk2, str + 8, 8);
				uint64_t diff2 = chunk2 ^ 0x3030303030303030ULL;
				return (diff2 == 0) ? 16 : 8 + (tzcnt(diff2) >> 3);
			}
		};

		template<> struct first_non_zero_byte<17> {
			VN_FORCE_INLINE static uint64_t impl(const char* str) {
				uint64_t chunk;
				std::memcpy(&chunk, str, 8);
				uint64_t diff = chunk ^ 0x3030303030303030ULL;
				if (diff != 0)
					return tzcnt(diff) >> 3;
				std::memcpy(&chunk, str + 8, 8);
				diff = chunk ^ 0x3030303030303030ULL;
				if (diff != 0)
					return 8 + (tzcnt(diff) >> 3);
				return (str[16] == '0') ? 17 : 16;
			}
		};

		template<> struct first_non_zero_byte<18> {
			VN_FORCE_INLINE static uint64_t impl(const char* str) {
				uint64_t chunk;
				std::memcpy(&chunk, str, 8);
				uint64_t diff = chunk ^ 0x3030303030303030ULL;
				if (diff != 0)
					return tzcnt(diff) >> 3;
				std::memcpy(&chunk, str + 8, 8);
				diff = chunk ^ 0x3030303030303030ULL;
				if (diff != 0)
					return 8 + (tzcnt(diff) >> 3);
				uint16_t chunk2;
				std::memcpy(&chunk2, str + 16, 2);
				uint16_t diff2 = chunk2 ^ 0x3030;
				return (diff2 == 0) ? 18 : 16 + (tzcnt(diff2) >> 3);
			}
		};

		template<> struct first_non_zero_byte<19> {
			VN_FORCE_INLINE static uint64_t impl(const char* str) {
				uint64_t chunk;
				std::memcpy(&chunk, str, 8);
				uint64_t diff = chunk ^ 0x3030303030303030ULL;
				if (diff != 0)
					return tzcnt(diff) >> 3;
				std::memcpy(&chunk, str + 8, 8);
				diff = chunk ^ 0x3030303030303030ULL;
				if (diff != 0)
					return 8 + (tzcnt(diff) >> 3);
				uint32_t chunk2;
				std::memcpy(&chunk2, str + 16, 3);
				uint32_t diff2 = chunk2 ^ 0x303030;
				return (diff2 == 0) ? 19 : 16 + (tzcnt(diff2) >> 3);
			}
		};

		template<> struct first_non_zero_byte<20> {
			VN_FORCE_INLINE static uint64_t impl(const char* str) {
				uint64_t chunk;
				std::memcpy(&chunk, str, 8);
				uint64_t diff = chunk ^ 0x3030303030303030ULL;
				if (diff != 0)
					return tzcnt(diff) >> 3;
				std::memcpy(&chunk, str + 8, 8);
				diff = chunk ^ 0x3030303030303030ULL;
				if (diff != 0)
					return 8 + (tzcnt(diff) >> 3);
				uint32_t chunk2;
				std::memcpy(&chunk2, str + 16, 4);
				uint32_t diff2 = chunk2 ^ 0x30303030;
				return (diff2 == 0) ? 20 : 16 + (tzcnt(diff2) >> 3);
			}
		};

		#if 0

		template<uint64_types auto index> struct int_selector;

		template<> struct int_selector<0ULL> {
			using type = uint64_t;
		};

		template<> struct int_selector<1ULL> {
			using type = uint32_t;
		};

		template<> struct int_selector<2ULL> {
			using type = uint16_t;
		};

		template<> struct int_selector<3ULL> {
			using type = uint8_t;
		};

		template<uint64_types auto index> using int_selector_t = typename int_selector<index>::type;

		template<uint64_types auto index> struct int_selector_by_byte;

		template<uint64_types auto byte_count> struct int_selector_by_byte {
			using type = uint64_t;
		};

		template<uint64_types auto byte_count>
			requires(byte_count == 1)
		struct int_selector_by_byte<byte_count> {
			using type = uint8_t;
		};

		template<uint64_types auto byte_count>
			requires(byte_count == 2)
		struct int_selector_by_byte<byte_count> {
			using type = uint16_t;
		};

		template<uint64_types auto byte_count>
			requires(byte_count > 2 && byte_count <= 4)
		struct int_selector_by_byte<byte_count> {
			using type = uint32_t;
		};

		template<uint64_types auto byte_count> using int_selector_by_byte_t = typename int_selector_by_byte<byte_count>::type;

		template<uint_types integer_type> struct swar_gather_values;

		template<> struct swar_gather_values<uint8_t> {
			template<typename char_type> VN_FORCE_INLINE static uint8_t impl(const char_type* values) noexcept {
				return static_cast<uint8_t>(values[0]);
			}
		};

		template<> struct swar_gather_values<uint16_t> {
			template<typename char_type> VN_FORCE_INLINE static uint16_t impl(const char_type* values) noexcept {
				uint16_t return_value;
				std::memcpy(&return_value, values, 2);
				return return_value;
			}
		};

		template<> struct swar_gather_values<uint32_t> {
			template<typename char_type> VN_FORCE_INLINE static uint32_t impl(const char_type* values) noexcept {
				uint32_t return_value;
				std::memcpy(&return_value, values, 4);
				return return_value;
			}
		};

		template<> struct swar_gather_values<uint64_t> {
			template<typename char_type> VN_FORCE_INLINE static uint64_t impl(const char_type* values) noexcept {
				uint64_t return_value;
				std::memcpy(&return_value, values, 8);
				return return_value;
			}
		};

		template<uint_types v_type> VN_FORCE_INLINE static v_type swar_cmplt_epu8(v_type a, v_type b) noexcept {
			constexpr v_type H{ static_cast<v_type>(0x8080808080808080ULL) };
			constexpr v_type L{ static_cast<v_type>(~H) };
			const v_type ua{ static_cast<v_type>(a) };
			const v_type ub{ static_cast<v_type>(b) };
			const v_type ah{ static_cast<v_type>(ua & H) };
			const v_type bh{ static_cast<v_type>(ub & H) };
			const v_type sub7{ static_cast<v_type>((ua | H) - (ub & L)) };
			const v_type borrow7{ static_cast<v_type>(static_cast<v_type>(~sub7) & H) };
			const v_type r80{ static_cast<v_type>((static_cast<v_type>(~ah & bh)) | (static_cast<v_type>(~(ah ^ bh)) & borrow7)) };
			return static_cast<v_type>(static_cast<v_type>(r80) | static_cast<v_type>(static_cast<v_type>(r80) - static_cast<v_type>(r80 >> 7)));
		}

		template<uint_types v_type> VN_FORCE_INLINE static v_type swar_sub_epu8(v_type a, v_type b) {
			static constexpr v_type mask = repeat_byte<static_cast<uint8_t>(0x80), v_type>();
			return ((a | mask) - (b & ~mask)) ^ mask;
		}

		template<typename integer_type> struct swar_collect_digit_indices {
			template<typename char_type> VN_FORCE_INLINE static auto impl(const char_type* values) noexcept {
				static constexpr integer_type zero_val	= repeat_byte<static_cast<uint8_t>('0'), integer_type>();
				static constexpr integer_type threshold = repeat_byte<static_cast<uint8_t>(10), integer_type>();
				const integer_type input				= swar_gather_values<integer_type>::impl(values);
				const integer_type offset				= swar_sub_epu8<integer_type>(input, zero_val);
				return swar_cmplt_epu8(offset, threshold);
			}
		};

		template<uint64_t length> struct find_first_not_of_swar_comparitor {
			template<uint64_types auto index, typename char_type> VN_FORCE_INLINE static bool impl(const char_type*& text, uint64_t i) {
				using v_type = int_selector_t<index>;
				static constexpr v_type n_bytes{ sizeof(v_type) };
				if (i + n_bytes <= length) {
					v_type non_digit_mask = static_cast<v_type>(~static_cast<v_type>(swar_collect_digit_indices<v_type>::impl(text)));
					if (non_digit_mask != static_cast<v_type>(0)) {
						i += tzcnt(non_digit_mask) >> 3;
						return false;
					}
					text += n_bytes;
					i += n_bytes;
				}
				return true;
			}
		};

		template<> struct find_first_not_of_swar_comparitor<2> {
			VN_FORCE_INLINE static void impl(const char*& text) {
				using v_type = uint16_t;
				static constexpr v_type n_bytes{ sizeof(v_type) };
				v_type non_digit_mask = static_cast<v_type>(~static_cast<v_type>(swar_collect_digit_indices<v_type>::impl(text)));
				if (non_digit_mask != static_cast<v_type>(0)) {
					return;
				}
				text += n_bytes;
				return;
			}
		};

		struct nope_fucker {

			template<uint64_t length, template<uint64_t> typename comparison_type, typename integer_sequence> struct string_compare_executor;

			template<uint64_t length, template<uint64_t> typename comparison_type, uint64_types auto... indices>
			struct string_compare_executor<length, comparison_type, std::integer_sequence<uint64_t, indices...>> {
				template<typename... arg_types> VN_FORCE_INLINE static bool impl(arg_types&&... args) {
					return (comparison_type<length>::template impl<4 - indices - 1ULL>(std::forward<arg_types>(args)...) && ...);
				}

				template<typename... arg_types> VN_FORCE_INLINE static bool swar_impl(arg_types&&... args) {
					return (comparison_type<length>::template impl<indices>(std::forward<arg_types>(args)...) && ...);
				}
			};

			template<uint64_t byte_count, uint_types v_type> VN_FORCE_INLINE static bool is_made_of_x_digits_no_sub(v_type val) noexcept {
				return !((
					(( val + repeat_bytes_v<static_cast<uint8_t>(0x76), byte_count> ) | (val)) & repeat_bytes_v<static_cast<uint8_t>(0x80), byte_count> ));
			}

			VN_FORCE_INLINE static uint32_t parse_eight_digits_unrolled_no_sub(uint64_t val) {
				constexpr uint64_t mask = 0x000000FF000000FF;
				constexpr uint64_t mul1 = 100 + (1000000ULL << 32);
				constexpr uint64_t mul2 = 1 + (10000ULL << 32);
				val						= (val * 10) + (val >> 8);
				val						= (((val & mask) * mul1) + (((val >> 16) & mask) * mul2)) >> 32;
				return static_cast<uint32_t>(val);
			}

			VN_FORCE_INLINE static uint32_t parse_seven_digits_unrolled_no_sub(uint64_t val) {
				constexpr uint64_t mask = 0xFF000000FF;
				constexpr uint64_t mul1 = 10 + (100000ULL << 32);
				constexpr uint64_t mul2 = (1000ULL << 32);
				uint32_t last_digit		= (static_cast<uint8_t>(val >> 48));
				val						= (val * 10) + (val >> 8);
				val						= (((val & mask) * mul1) + (((val >> 16) & mask) * mul2)) >> 32;
				return static_cast<uint32_t>(val) + last_digit;
			}

			VN_FORCE_INLINE static uint32_t parse_six_digits_unrolled_no_sub(uint64_t val) {
				constexpr uint64_t mask = 0xFF000000FF;
				constexpr uint64_t mul1 = 1 + (10000ULL << 32);
				constexpr uint64_t mul2 = (100ULL << 32);
				val						= (val * 10) + (val >> 8);
				val						= (((val & mask) * mul1) + (((val >> 16) & mask) * mul2)) >> 32;
				return static_cast<uint32_t>(val);
			}

			VN_FORCE_INLINE static uint32_t parse_five_digits_unrolled_no_sub(uint64_t val) {
				constexpr uint64_t mask = 0xFF000000FF;
				constexpr uint64_t mul1 = (1000ULL << 32);
				constexpr uint64_t mul2 = (10ULL << 32);
				uint32_t last_digit		= (static_cast<uint8_t>(val >> 32));
				val						= (val * 10) + (val >> 8);
				val						= (((val & mask) * mul1) + (((val >> 16) & mask) * mul2)) >> 32;
				return static_cast<uint32_t>(val) + last_digit;
			}

			VN_FORCE_INLINE static uint32_t parse_four_digits_unrolled_no_sub(uint64_t val) {
				constexpr uint64_t mask = 0xFF000000FF;
				constexpr uint64_t mul1 = (100ULL << 32);
				constexpr uint64_t mul2 = (1ULL << 32);
				val						= (val * 10) + (val >> 8);
				val						= (((val & mask) * mul1) + (((val >> 16) & mask) * mul2)) >> 32;
				return static_cast<uint32_t>(val);
			}

			VN_FORCE_INLINE static uint32_t parse_three_digits_unrolled_no_sub(uint64_t val) {
				constexpr uint64_t mask = 0xFF000000FF;
				constexpr uint64_t mul1 = (10ULL << 32);
				uint32_t last_digit		= (static_cast<uint8_t>(val >> 16));
				val						= (val * 10) + (val >> 8);
				val						= (((val & mask) * mul1)) >> 32;
				return static_cast<uint32_t>(val) + last_digit;
			}

			VN_FORCE_INLINE static uint32_t parse_two_digits_unrolled_no_sub(uint64_t val) {
				constexpr uint64_t mask = 0xFF000000FF;
				constexpr uint64_t mul1 = (1ULL << 32);
				val						= (val * 10) + (val >> 8);
				val						= (((val & mask) * mul1)) >> 32;
				return static_cast<uint32_t>(val);
			}

			VN_FORCE_INLINE static uint32_t parse_nine_digits_unrolled_no_sub(uint64_t val_low, uint32_t last_digit) {
				uint32_t first_eight = parse_eight_digits_unrolled_no_sub(val_low);
				return first_eight * 10 + last_digit;
			}

			VN_FORCE_INLINE static uint64_t parse_ten_digits_unrolled_no_sub(uint64_t val_low, uint64_t val_high) {
				uint32_t first_eight = parse_eight_digits_unrolled_no_sub(val_low);
				uint32_t last_two	 = parse_two_digits_unrolled_no_sub(val_high);
				return static_cast<uint64_t>(first_eight) * 100ULL + last_two;
			}

			VN_FORCE_INLINE static uint64_t parse_eleven_digits_unrolled_no_sub(uint64_t val_low, uint64_t val_high) {
				uint32_t first_eight = parse_eight_digits_unrolled_no_sub(val_low);
				uint32_t last_three	 = parse_three_digits_unrolled_no_sub(val_high);
				return static_cast<uint64_t>(first_eight) * 1000ULL + last_three;
			}

			VN_FORCE_INLINE static uint64_t parse_twelve_digits_unrolled_no_sub(uint64_t val_low, uint64_t val_high) {
				uint32_t first_eight = parse_eight_digits_unrolled_no_sub(val_low);
				uint32_t last_four	 = parse_four_digits_unrolled_no_sub(val_high);
				return static_cast<uint64_t>(first_eight) * 10000ULL + last_four;
			}

			VN_FORCE_INLINE static uint64_t parse_thirteen_digits_unrolled_no_sub(uint64_t val_low, uint64_t val_high) {
				uint32_t first_eight = parse_eight_digits_unrolled_no_sub(val_low);
				uint32_t last_five	 = parse_five_digits_unrolled_no_sub(val_high);
				return static_cast<uint64_t>(first_eight) * 100000ULL + last_five;
			}

			VN_FORCE_INLINE static uint64_t parse_fourteen_digits_unrolled_no_sub(uint64_t val_low, uint64_t val_high) {
				uint32_t first_eight = parse_eight_digits_unrolled_no_sub(val_low);
				uint32_t last_six	 = parse_six_digits_unrolled_no_sub(val_high);
				return static_cast<uint64_t>(first_eight) * 1000000ULL + last_six;
			}

			VN_FORCE_INLINE static uint64_t parse_fifteen_digits_unrolled_no_sub(uint64_t val_low, uint64_t val_high) {
				uint32_t first_eight = parse_eight_digits_unrolled_no_sub(val_low);
				uint32_t last_seven	 = parse_seven_digits_unrolled_no_sub(val_high);
				return static_cast<uint64_t>(first_eight) * 10000000ULL + last_seven;
			}

			VN_FORCE_INLINE static uint64_t parse_sixteen_digits_unrolled_no_sub(uint64_t val_low, uint64_t val_high) {
				uint32_t first_eight = parse_eight_digits_unrolled_no_sub(val_low);
				uint32_t last_eight	 = parse_eight_digits_unrolled_no_sub(val_high);
				return static_cast<uint64_t>(first_eight) * 100000000ULL + last_eight;
			}

			VN_FORCE_INLINE static uint64_t parse_seventeen_digits_unrolled_no_sub(uint64_t val_low, uint64_t val_mid, uint64_t val_high) {
				uint32_t first_eight = parse_eight_digits_unrolled_no_sub(val_low);
				uint32_t next_eight	 = parse_eight_digits_unrolled_no_sub(val_mid);
				uint32_t last_one	 = static_cast<uint8_t>(val_high);
				return static_cast<uint64_t>(first_eight) * 1000000000ULL + static_cast<uint64_t>(next_eight) * 10ULL + last_one;
			}

			VN_FORCE_INLINE static uint64_t parse_eighteen_digits_unrolled_no_sub(uint64_t val_low, uint64_t val_mid, uint64_t val_high) {
				uint32_t first_eight = parse_eight_digits_unrolled_no_sub(val_low);
				uint32_t next_eight	 = parse_eight_digits_unrolled_no_sub(val_mid);
				uint32_t last_two	 = parse_two_digits_unrolled_no_sub(val_high);
				return static_cast<uint64_t>(first_eight) * 10000000000ULL + static_cast<uint64_t>(next_eight) * 100ULL + last_two;
			}

			VN_FORCE_INLINE static uint64_t parse_nineteen_digits_unrolled_no_sub(uint64_t val_low, uint64_t val_mid, uint64_t val_high) {
				uint32_t first_eight = parse_eight_digits_unrolled_no_sub(val_low);
				uint32_t next_eight	 = parse_eight_digits_unrolled_no_sub(val_mid);
				uint32_t last_three	 = parse_three_digits_unrolled_no_sub(val_high);
				return static_cast<uint64_t>(first_eight) * 100000000000ULL + static_cast<uint64_t>(next_eight) * 1000ULL + last_three;
			}

			VN_FORCE_INLINE static uint64_t parse_twenty_digits_unrolled_no_sub(uint64_t val_low, uint64_t val_mid, uint64_t val_high) {
				uint32_t first_eight = parse_eight_digits_unrolled_no_sub(val_low);
				uint32_t next_eight	 = parse_eight_digits_unrolled_no_sub(val_mid);
				uint32_t last_four	 = parse_four_digits_unrolled_no_sub(val_high);
				return static_cast<uint64_t>(first_eight) * 1000000000000ULL + static_cast<uint64_t>(next_eight) * 10000ULL + last_four;
			}

			VN_FORCE_INLINE static bool parse_two_digits(uint16_t& value, const char*& str) {
				uint64_t value_new;
				std::memcpy(&value_new, str, 2);
				value_new -= 0x3030;
				bool valid{ is_made_of_x_digits_no_sub<2>(value_new) };
				return valid ? ((str += valid * 2), value = valid * static_cast<uint16_t>(parse_two_digits_unrolled_no_sub(value_new)), true) : false;
			}

			VN_FORCE_INLINE static bool parse_three_digits(uint32_t& value, const char*& str) {
				uint64_t value_new;
				std::memcpy(&value_new, str, 3);
				value_new -= 0x303030;
				bool valid{ is_made_of_x_digits_no_sub<3>(value_new) };
				return valid ? ((str += valid * 3), value = valid * static_cast<uint32_t>(parse_three_digits_unrolled_no_sub(value_new)), true) : false;
			}

			VN_FORCE_INLINE static bool parse_four_digits(uint32_t& value, const char*& str) {
				uint64_t value_new;
				std::memcpy(&value_new, str, 4);
				value_new -= 0x30303030;
				bool valid{ is_made_of_x_digits_no_sub<4>(value_new) };
				return valid ? ((str += valid * 4), value = valid * static_cast<uint32_t>(parse_four_digits_unrolled_no_sub(value_new)), true) : false;
			}

			VN_FORCE_INLINE static bool parse_five_digits(uint32_t& value, const char*& str) {
				uint64_t value_new;
				std::memcpy(&value_new, str, 5);
				value_new -= 0x3030303030;
				bool valid{ is_made_of_x_digits_no_sub<5>(value_new) };
				return valid ? ((str += valid * 5), value = valid * static_cast<uint32_t>(parse_five_digits_unrolled_no_sub(value_new)), true) : false;
			}

			VN_FORCE_INLINE static bool parse_six_digits(uint32_t& value, const char*& str) {
				uint64_t value_new;
				std::memcpy(&value_new, str, 6);
				value_new -= 0x303030303030;
				bool valid{ is_made_of_x_digits_no_sub<6>(value_new) };
				return valid ? ((str += valid * 6), value = valid * static_cast<uint32_t>(parse_six_digits_unrolled_no_sub(value_new)), true) : false;
			}

			VN_FORCE_INLINE static bool parse_seven_digits(uint32_t& value, const char*& str) {
				uint64_t value_new;
				std::memcpy(&value_new, str, 7);
				value_new -= 0x30303030303030;
				bool valid{ is_made_of_x_digits_no_sub<7>(value_new) };
				return valid ? ((str += valid * 7), value = valid * static_cast<uint32_t>(parse_seven_digits_unrolled_no_sub(value_new)), true) : false;
			}

			VN_FORCE_INLINE static bool parse_eight_digits(uint32_t& value, const char*& str) {
				uint64_t value_new;
				std::memcpy(&value_new, str, 8);
				value_new -= 0x3030303030303030;
				bool valid{ is_made_of_x_digits_no_sub<8>(value_new) };
				return valid ? ((str += valid * 8), value = valid * static_cast<uint32_t>(parse_eight_digits_unrolled_no_sub(value_new)), true) : false;
			}

			VN_FORCE_INLINE static bool parse_nine_digits(uint32_t& value, const char*& str) {
				uint64_t val_low;
				std::memcpy(&val_low, str, 8);
				uint8_t ninth_byte = str[8];
				val_low -= 0x3030303030303030;
				uint32_t last_digit = ninth_byte - '0';
				bool valid{ is_made_of_x_digits_no_sub<8>(val_low) && (last_digit <= 9) };
				return valid ? ((str += 9), value = parse_nine_digits_unrolled_no_sub(val_low, last_digit), true) : false;
			}

			VN_FORCE_INLINE static bool parse_ten_digits(uint64_t& value, const char*& str) {
				uint64_t val_low, val_high;
				std::memcpy(&val_low, str, 8);
				std::memcpy(&val_high, str + 8, 2);
				val_low -= 0x3030303030303030;
				val_high -= 0x3030;
				bool valid{ is_made_of_x_digits_no_sub<8>(val_low) && is_made_of_x_digits_no_sub<2>(val_high) };
				return valid ? ((str += valid * 10), value = valid * parse_ten_digits_unrolled_no_sub(val_low, val_high), true) : false;
			}

			VN_FORCE_INLINE static bool parse_eleven_digits(uint64_t& value, const char*& str) {
				uint64_t val_low, val_high;
				std::memcpy(&val_low, str, 8);
				std::memcpy(&val_high, str + 8, 3);
				val_low -= 0x3030303030303030;
				val_high -= 0x303030;
				bool valid{ is_made_of_x_digits_no_sub<8>(val_low) && is_made_of_x_digits_no_sub<3>(val_high) };
				return valid ? ((str += valid * 11), value = valid * parse_eleven_digits_unrolled_no_sub(val_low, val_high), true) : false;
			}

			VN_FORCE_INLINE static bool parse_twelve_digits(uint64_t& value, const char*& str) {
				uint64_t val_low, val_high;
				std::memcpy(&val_low, str, 8);
				std::memcpy(&val_high, str + 8, 4);
				val_low -= 0x3030303030303030;
				val_high -= 0x30303030;
				bool valid{ is_made_of_x_digits_no_sub<8>(val_low) && is_made_of_x_digits_no_sub<4>(val_high) };
				return valid ? ((str += valid * 12), value = valid * parse_twelve_digits_unrolled_no_sub(val_low, val_high), true) : false;
			}

			VN_FORCE_INLINE static bool parse_thirteen_digits(uint64_t& value, const char*& str) {
				uint64_t val_low, val_high;
				std::memcpy(&val_low, str, 8);
				std::memcpy(&val_high, str + 8, 5);
				val_low -= 0x3030303030303030;
				val_high -= 0x3030303030;
				bool valid{ is_made_of_x_digits_no_sub<8>(val_low) && is_made_of_x_digits_no_sub<5>(val_high) };
				return valid ? ((str += valid * 13), value = valid * parse_thirteen_digits_unrolled_no_sub(val_low, val_high), true) : false;
			}

			VN_FORCE_INLINE static bool parse_fourteen_digits(uint64_t& value, const char*& str) {
				uint64_t val_low, val_high;
				std::memcpy(&val_low, str, 8);
				std::memcpy(&val_high, str + 8, 6);
				val_low -= 0x3030303030303030;
				val_high -= 0x303030303030;
				bool valid{ is_made_of_x_digits_no_sub<8>(val_low) && is_made_of_x_digits_no_sub<6>(val_high) };
				return valid ? ((str += valid * 14), value = valid * parse_fourteen_digits_unrolled_no_sub(val_low, val_high), true) : false;
			}

			VN_FORCE_INLINE static bool parse_fifteen_digits(uint64_t& value, const char*& str) {
				uint64_t val_low, val_high;
				std::memcpy(&val_low, str, 8);
				std::memcpy(&val_high, str + 8, 7);
				val_low -= 0x3030303030303030;
				val_high -= 0x30303030303030;
				bool valid{ is_made_of_x_digits_no_sub<8>(val_low) && is_made_of_x_digits_no_sub<7>(val_high) };
				return valid ? ((str += valid * 15), value = valid * parse_fifteen_digits_unrolled_no_sub(val_low, val_high), true) : false;
			}

			VN_FORCE_INLINE static bool parse_sixteen_digits(uint64_t& value, const char*& str) {
				uint64_t val_low, val_high;
				std::memcpy(&val_low, str, 8);
				std::memcpy(&val_high, str + 8, 8);
				val_low -= 0x3030303030303030;
				val_high -= 0x3030303030303030;
				bool valid{ is_made_of_x_digits_no_sub<8>(val_low) && is_made_of_x_digits_no_sub<8>(val_high) };
				return valid ? ((str += valid * 16), value = valid * parse_sixteen_digits_unrolled_no_sub(val_low, val_high), true) : false;
			}

			VN_FORCE_INLINE static bool parse_seventeen_digits(uint64_t& value, const char*& str) {
				uint64_t val_low, val_mid, val_high;
				std::memcpy(&val_low, str, 8);
				std::memcpy(&val_mid, str + 8, 8);
				std::memcpy(&val_high, str + 16, 1);
				val_low -= 0x3030303030303030;
				val_mid -= 0x3030303030303030;
				val_high -= 0x30;
				bool valid{ is_made_of_x_digits_no_sub<8>(val_low) && is_made_of_x_digits_no_sub<8>(val_mid) && is_made_of_x_digits_no_sub<1>(val_high) };
				return valid ? ((str += valid * 17), value = valid * parse_seventeen_digits_unrolled_no_sub(val_low, val_mid, val_high), true) : false;
			}

			VN_FORCE_INLINE static bool parse_eighteen_digits(uint64_t& value, const char*& str) {
				uint64_t val_low, val_mid, val_high;
				std::memcpy(&val_low, str, 8);
				std::memcpy(&val_mid, str + 8, 8);
				std::memcpy(&val_high, str + 16, 2);
				val_low -= 0x3030303030303030;
				val_mid -= 0x3030303030303030;
				val_high -= 0x3030;
				bool valid{ is_made_of_x_digits_no_sub<8>(val_low) && is_made_of_x_digits_no_sub<8>(val_mid) && is_made_of_x_digits_no_sub<2>(val_high) };
				return valid ? ((str += valid * 18), value = valid * parse_eighteen_digits_unrolled_no_sub(val_low, val_mid, val_high), true) : false;
			}

			VN_FORCE_INLINE static bool parse_nineteen_digits(uint64_t& value, const char*& str) {
				uint64_t val_low, val_mid, val_high;
				std::memcpy(&val_low, str, 8);
				std::memcpy(&val_mid, str + 8, 8);
				std::memcpy(&val_high, str + 16, 3);
				val_low -= 0x3030303030303030;
				val_mid -= 0x3030303030303030;
				val_high -= 0x303030;
				bool valid{ is_made_of_x_digits_no_sub<8>(val_low) && is_made_of_x_digits_no_sub<8>(val_mid) && is_made_of_x_digits_no_sub<3>(val_high) };
				return valid ? ((str += valid * 19), value = valid * parse_nineteen_digits_unrolled_no_sub(val_low, val_mid, val_high), true) : false;
			}

			VN_FORCE_INLINE static bool parse_twenty_digits(uint64_t& value, const char*& str) {
				uint64_t val_low, val_mid, val_high;
				std::memcpy(&val_low, str, 8);
				std::memcpy(&val_mid, str + 8, 8);
				std::memcpy(&val_high, str + 16, 4);
				val_low -= 0x3030303030303030;
				val_mid -= 0x3030303030303030;
				val_high -= 0x30303030;
				bool valid{ is_made_of_x_digits_no_sub<8>(val_low) && is_made_of_x_digits_no_sub<8>(val_mid) && is_made_of_x_digits_no_sub<4>(val_high) };
				return valid ? ((str += valid * 20), value = valid * parse_twenty_digits_unrolled_no_sub(val_low, val_mid, val_high), true) : false;
			}
		};

#endif

		template<typename v_type> struct from_chars_impl;

		template<uint_types v_type> VN_FORCE_INLINE bool vn_is_digit(v_type value) {
			return ((static_cast<uint8_t>(value - '0')) < 10);
		}

		VN_FORCE_INLINE static const char* trim_leading_zeros(const char* iter, const char* end) noexcept {
			uint64_t length{ static_cast<uint64_t>(end - iter) };
			switch (length) {
				case 1: {
					iter += first_non_zero_byte<1>::impl(iter);
					return iter;
				}
				case 2: {
					iter += first_non_zero_byte<2>::impl(iter);
					return iter;
				}
				case 3: {
					iter += first_non_zero_byte<3>::impl(iter);
					return iter;
				}
				case 4: {
					iter += first_non_zero_byte<4>::impl(iter);
					return iter;
				}
				case 5: {
					iter += first_non_zero_byte<5>::impl(iter);
					return iter;
				}
				case 6: {
					iter += first_non_zero_byte<6>::impl(iter);
					return iter;
				}
				case 7: {
					iter += first_non_zero_byte<7>::impl(iter);
					return iter;
				}
				case 8: {
					iter += first_non_zero_byte<8>::impl(iter);
					return iter;
				}
				case 9: {
					iter += first_non_zero_byte<9>::impl(iter);
					return iter;
				}
				case 10: {
					iter += first_non_zero_byte<10>::impl(iter);
					return iter;
				}
				case 11: {
					iter += first_non_zero_byte<11>::impl(iter);
					return iter;
				}
				case 12: {
					iter += first_non_zero_byte<12>::impl(iter);
					return iter;
				}
				case 13: {
					iter += first_non_zero_byte<13>::impl(iter);
					return iter;
				}
				case 14: {
					iter += first_non_zero_byte<14>::impl(iter);
					return iter;
				}
				case 15: {
					iter += first_non_zero_byte<15>::impl(iter);
					return iter;
				}
				case 16: {
					iter += first_non_zero_byte<16>::impl(iter);
					return iter;
				}
				case 17: {
					iter += first_non_zero_byte<17>::impl(iter);
					return iter;
				}
				case 18: {
					iter += first_non_zero_byte<18>::impl(iter);
					return iter;
				}
				case 19: {
					iter += first_non_zero_byte<19>::impl(iter);
					return iter;
				}
				case 20: {
					iter += first_non_zero_byte<20>::impl(iter);
					return iter;
				}
				default: {
					while (iter < end && *iter == zero) {
						++iter;
					}
				}
			}
			return iter;
		}

		template<bool negative, int8_types v_type> VN_FORCE_INLINE static const char* parse_integer(v_type& value_new, const char* iter, const char* end) noexcept {
			using v_type_local = std::make_unsigned_t<v_type>;
			VN_ALIGN(64) static constexpr v_type_local zero_val{ 0 };

			if (iter >= end) [[unlikely]] {
				return iter;
			}

			v_type_local value;
			uint8_t c = static_cast<uint8_t>(*iter);

			if (c == zero) {

				iter = trim_leading_zeros(iter, end);
				if (iter < end && vn_is_digit(static_cast<uint8_t>(*iter))) {
					value = static_cast<v_type_local>(*iter - zero);
					++iter;
				} else {
					value_new = 0;
					return iter;
				}
			} else if VN_LIKELY (vn_is_digit(c)) {
				value = static_cast<v_type_local>(c - zero);
				++iter;
			} else {
				return iter;
			}

			if VN_LIKELY (iter < end) {
				c = static_cast<uint8_t>(*iter);
				if VN_LIKELY (vn_is_digit(c)) {
					value = static_cast<v_type_local>(value * 10 + (c - zero));
					++iter;
				} else {
					if constexpr (negative) {
						value_new = static_cast<v_type>(zero_val - value);
					} else {
						value_new = static_cast<v_type>(value);
					}
					return iter;
				}
			} else {
				if constexpr (negative) {
					value_new = static_cast<v_type>(zero_val - value);
				} else {
					value_new = static_cast<v_type>(value);
				}
				return iter;
			}

			if constexpr (sizeof(v_type) > 1) {
				if VN_LIKELY (iter < end) {
					c = static_cast<uint8_t>(*iter);
					if VN_LIKELY (vn_is_digit(c)) {
						value = static_cast<v_type_local>(value * 10 + (c - zero));
						++iter;
					} else {
						return iter;
					}
				} else {
					if constexpr (negative) {
						value_new = static_cast<v_type>(zero_val - value);
					} else {
						value_new = static_cast<v_type>(value);
					}
					return iter;
				}

				if VN_LIKELY (iter < end) {
					c = static_cast<uint8_t>(*iter);
					if VN_LIKELY (vn_is_digit(c)) {
						value = static_cast<v_type_local>(value * 10 + (c - zero));
						++iter;
					} else {
						return iter;
					}
				} else {
					if constexpr (negative) {
						value_new = static_cast<v_type>(zero_val - value);
					} else {
						value_new = static_cast<v_type>(value);
					}
					return iter;
				}

				if constexpr (sizeof(v_type) > 2) {
					if VN_LIKELY (iter < end) {
						c = static_cast<uint8_t>(*iter);
						if VN_LIKELY (vn_is_digit(c)) {
							value = static_cast<v_type_local>(value * 10 + (c - zero));
							++iter;
						} else {
							return iter;
						}
					} else {
						if constexpr (negative) {
							value_new = static_cast<v_type>(zero_val - value);
						} else {
							value_new = static_cast<v_type>(value);
						}
						return iter;
					}

					if VN_LIKELY (iter < end) {
						c = static_cast<uint8_t>(*iter);
						if VN_LIKELY (vn_is_digit(c)) {
							value = static_cast<v_type_local>(value * 10 + (c - zero));
							++iter;
						} else {
							return iter;
						}
					} else {
						if constexpr (negative) {
							value_new = static_cast<v_type>(zero_val - value);
						} else {
							value_new = static_cast<v_type>(value);
						}
						return iter;
					}

					if VN_LIKELY (iter < end) {
						c = static_cast<uint8_t>(*iter);
						if VN_LIKELY (vn_is_digit(c)) {
							value = static_cast<v_type_local>(value * 10 + (c - zero));
							++iter;
						} else {
							return iter;
						}
					} else {
						if constexpr (negative) {
							value_new = static_cast<v_type>(zero_val - value);
						} else {
							value_new = static_cast<v_type>(value);
						}
						return iter;
					}

					if VN_LIKELY (iter < end) {
						c = static_cast<uint8_t>(*iter);
						if VN_LIKELY (vn_is_digit(c)) {
							value = static_cast<v_type_local>(value * 10 + (c - zero));
							++iter;
						} else {
							return iter;
						}
					} else {
						if constexpr (negative) {
							value_new = static_cast<v_type>(zero_val - value);
						} else {
							value_new = static_cast<v_type>(value);
						}
						return iter;
					}

					if VN_LIKELY (iter < end) {
						c = static_cast<uint8_t>(*iter);
						if VN_LIKELY (vn_is_digit(c)) {
							value = static_cast<v_type_local>(value * 10 + (c - zero));
							++iter;
						} else {
							return iter;
						}
					} else {
						if constexpr (negative) {
							value_new = static_cast<v_type>(zero_val - value);
						} else {
							value_new = static_cast<v_type>(value);
						}
						return iter;
					}
					if constexpr (sizeof(v_type) > 4) {
						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}
						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if constexpr (uint_types<v_type>) {
							if VN_LIKELY (iter < end) {
								c = static_cast<uint8_t>(*iter);
								if VN_LIKELY (vn_is_digit(c)) {
									value = static_cast<v_type_local>(value * 10 + (c - zero));
									++iter;
								} else {
									return iter;
								}
							} else {
								if constexpr (negative) {
									value_new = static_cast<v_type>(zero_val - value);
								} else {
									value_new = static_cast<v_type>(value);
								}
								return iter;
							}
						}
					}
				}
			}

			if VN_LIKELY (iter < end) {
				c = static_cast<uint8_t>(*iter);
				if VN_LIKELY (vn_is_digit(c)) {
					if constexpr (negative) {
						if (static_cast<uint64_t>(value) > static_cast<uint64_t>(raw_comp_vals_neg<v_type>[c])) {
							while (++iter < end && vn_is_digit(static_cast<uint8_t>(*iter))) {
							}
							return iter;
						}
					} else {
						if (static_cast<uint64_t>(value) > static_cast<uint64_t>(raw_comp_vals_pos<v_type>[c])) {
							while (++iter < end && vn_is_digit(static_cast<uint8_t>(*iter))) {
							}
							return iter;
						}
					}
					value = static_cast<v_type_local>(value * 10 + (c - zero));
					++iter;
				} else {
					if constexpr (negative) {
						value_new = static_cast<v_type>(zero_val - value);
					} else {
						value_new = static_cast<v_type>(value);
					}
					return iter;
				}
			} else {
				if constexpr (negative) {
					value_new = static_cast<v_type>(zero_val - value);
				} else {
					value_new = static_cast<v_type>(value);
				}
				return iter;
			}

			if (iter < end && vn_is_digit(static_cast<uint8_t>(*iter))) {
				while (++iter < end && vn_is_digit(static_cast<uint8_t>(*iter))) {
				}
				return iter;
			}

			if constexpr (negative) {
				value_new = static_cast<v_type>(zero_val - value);
			} else {
				value_new = static_cast<v_type>(value);
			}
			return iter;
		}

		template<bool negative, integer_types v_type> VN_FORCE_INLINE static const char* parse_integer(v_type& value_new, const char* iter, const char* end) noexcept {
			using v_type_local					  = std::make_unsigned_t<v_type>;
			VN_ALIGN(64) static constexpr v_type_local zero_val{ 0 };

			if (iter >= end) [[unlikely]] {
				return iter;
			}

			v_type_local value;
			uint8_t c = static_cast<uint8_t>(*iter);

			if (c == zero) {
				iter = trim_leading_zeros(iter, end);
				if (iter < end && vn_is_digit(static_cast<uint8_t>(*iter))) {
					value = static_cast<v_type_local>(*iter - zero);
					++iter;
				} else {
					value_new = 0;
					return iter;
				}
			} else if VN_LIKELY (vn_is_digit(c)) {
				value = static_cast<v_type_local>(c - zero);
				++iter;
			} else {
				return iter;
			}

			if VN_LIKELY (iter < end) {
				c = static_cast<uint8_t>(*iter);
				if VN_LIKELY (vn_is_digit(c)) {
					value = static_cast<v_type_local>(value * 10 + (c - zero));
					++iter;
				} else {
					if constexpr (negative) {
						value_new = static_cast<v_type>(zero_val - value);
					} else {
						value_new = static_cast<v_type>(value);
					}
					return iter;
				}
			} else {
				if constexpr (negative) {
					value_new = static_cast<v_type>(zero_val - value);
				} else {
					value_new = static_cast<v_type>(value);
				}
				return iter;
			}

			if constexpr (sizeof(v_type) > 1) {
				if VN_LIKELY (iter < end) {
					c = static_cast<uint8_t>(*iter);
					if VN_LIKELY (vn_is_digit(c)) {
						value = static_cast<v_type_local>(value * 10 + (c - zero));
						++iter;
					} else {
						return iter;
					}
				} else {
					if constexpr (negative) {
						value_new = static_cast<v_type>(zero_val - value);
					} else {
						value_new = static_cast<v_type>(value);
					}
					return iter;
				}

				if VN_LIKELY (iter < end) {
					c = static_cast<uint8_t>(*iter);
					if VN_LIKELY (vn_is_digit(c)) {
						value = static_cast<v_type_local>(value * 10 + (c - zero));
						++iter;
					} else {
						return iter;
					}
				} else {
					if constexpr (negative) {
						value_new = static_cast<v_type>(zero_val - value);
					} else {
						value_new = static_cast<v_type>(value);
					}
					return iter;
				}

				if constexpr (sizeof(v_type) > 2) {
					if VN_LIKELY (iter < end) {
						c = static_cast<uint8_t>(*iter);
						if VN_LIKELY (vn_is_digit(c)) {
							value = static_cast<v_type_local>(value * 10 + (c - zero));
							++iter;
						} else {
							return iter;
						}
					} else {
						if constexpr (negative) {
							value_new = static_cast<v_type>(zero_val - value);
						} else {
							value_new = static_cast<v_type>(value);
						}
						return iter;
					}

					if VN_LIKELY (iter < end) {
						c = static_cast<uint8_t>(*iter);
						if VN_LIKELY (vn_is_digit(c)) {
							value = static_cast<v_type_local>(value * 10 + (c - zero));
							++iter;
						} else {
							return iter;
						}
					} else {
						if constexpr (negative) {
							value_new = static_cast<v_type>(zero_val - value);
						} else {
							value_new = static_cast<v_type>(value);
						}
						return iter;
					}

					if VN_LIKELY (iter < end) {
						c = static_cast<uint8_t>(*iter);
						if VN_LIKELY (vn_is_digit(c)) {
							value = static_cast<v_type_local>(value * 10 + (c - zero));
							++iter;
						} else {
							return iter;
						}
					} else {
						if constexpr (negative) {
							value_new = static_cast<v_type>(zero_val - value);
						} else {
							value_new = static_cast<v_type>(value);
						}
						return iter;
					}

					if VN_LIKELY (iter < end) {
						c = static_cast<uint8_t>(*iter);
						if VN_LIKELY (vn_is_digit(c)) {
							value = static_cast<v_type_local>(value * 10 + (c - zero));
							++iter;
						} else {
							return iter;
						}
					} else {
						if constexpr (negative) {
							value_new = static_cast<v_type>(zero_val - value);
						} else {
							value_new = static_cast<v_type>(value);
						}
						return iter;
					}

					if VN_LIKELY (iter < end) {
						c = static_cast<uint8_t>(*iter);
						if VN_LIKELY (vn_is_digit(c)) {
							value = static_cast<v_type_local>(value * 10 + (c - zero));
							++iter;
						} else {
							return iter;
						}
					} else {
						if constexpr (negative) {
							value_new = static_cast<v_type>(zero_val - value);
						} else {
							value_new = static_cast<v_type>(value);
						}
						return iter;
					}
					if constexpr (sizeof(v_type) > 4) {
						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}
						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if constexpr (uint_types<v_type>) {
							if VN_LIKELY (iter < end) {
								c = static_cast<uint8_t>(*iter);
								if VN_LIKELY (vn_is_digit(c)) {
									value = static_cast<v_type_local>(value * 10 + (c - zero));
									++iter;
								} else {
									return iter;
								}
							} else {
								if constexpr (negative) {
									value_new = static_cast<v_type>(zero_val - value);
								} else {
									value_new = static_cast<v_type>(value);
								}
								return iter;
							}
						}
					}
				}
			}

			if VN_LIKELY (iter < end) {
				c = static_cast<uint8_t>(*iter);
				if VN_LIKELY (vn_is_digit(c)) {
					if constexpr (negative) {
						if (static_cast<uint64_t>(value) > static_cast<uint64_t>(raw_comp_vals_neg<v_type>[c])) {
							while (++iter < end && vn_is_digit(static_cast<uint8_t>(*iter))) {
							}
							return iter;
						}
					} else {
						if (static_cast<uint64_t>(value) > static_cast<uint64_t>(raw_comp_vals_pos<v_type>[c])) {
							while (++iter < end && vn_is_digit(static_cast<uint8_t>(*iter))) {
							}
							return iter;
						}
					}
					value = static_cast<v_type_local>(value * 10 + (c - zero));
					++iter;
				} else {
					if constexpr (negative) {
						value_new = static_cast<v_type>(zero_val - value);
					} else {
						value_new = static_cast<v_type>(value);
					}
					return iter;
				}
			} else {
				if constexpr (negative) {
					value_new = static_cast<v_type>(zero_val - value);
				} else {
					value_new = static_cast<v_type>(value);
				}
				return iter;
			}

			if (iter < end && vn_is_digit(static_cast<uint8_t>(*iter))) {
				while (++iter < end && vn_is_digit(static_cast<uint8_t>(*iter))) {
				}
				return iter;
			}

			if constexpr (negative) {
				value_new = static_cast<v_type>(zero_val - value);
			} else {
				value_new = static_cast<v_type>(value);
			}
			return iter;
		}

		template<int_types v_type> struct from_chars_impl<v_type> {
			constexpr from_chars_impl() noexcept = default;

			VN_FORCE_INLINE static const char* impl(v_type& value, const char* iter, const char* end) noexcept {
				return (iter >= end) ? end
					: (*iter == '-') ? ((iter + 1 >= end || !vn_is_digit(static_cast<uint8_t>(*(iter + 1)))) ? iter : parse_integer<true>(value, iter + 1, end))
									 : parse_integer<false>(value, iter, end);
			}
		};

		template<uint_types v_type> struct from_chars_impl<v_type> {
			constexpr from_chars_impl() noexcept = default;

			VN_FORCE_INLINE static const char* impl(v_type& value, const char* iter, const char* end) noexcept {
				return (iter < end) ? (*iter != '-') ? (parse_integer<false>(value, iter, end)) : iter : end;
			}
		};

	}

	template<detail::integer_types v_type> VN_FORCE_INLINE std::from_chars_result from_chars(const char* first, const char* last, v_type& value, int32_t base = 10) noexcept {
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
