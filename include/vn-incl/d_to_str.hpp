// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp
// vn-incl/d_to_str.hpp

#pragma once

#include <vn-incl/tables.hpp>
#include <vn-incl/dragonbox.hpp>

namespace vn {

	namespace detail {

		VN_FORCE_INLINE auto* write_64_len_15_to_17_trim(auto* buf, uint64_t sig) noexcept {
			const uint32_t tz = count_trailing_decimal_zeros(sig);

			const uint64_t abbccddee = multiply_and_shift<uint64_t, 100000000>::impl(sig);
			const uint64_t ffgghhii	 = sig - abbccddee * 100000000;
			const uint32_t abbcc	 = static_cast<uint32_t>(multiply_and_shift<uint64_t, 10000>::impl(abbccddee));
			const uint32_t ddee		 = static_cast<uint32_t>(abbccddee - abbcc * 10000);
			const uint32_t abb		 = static_cast<uint32_t>((static_cast<uint64_t>(abbcc) * 167773) >> 24);
			const uint32_t a		 = (abb * 41) >> 12;
			const uint32_t bb		 = abb - a * 100;
			const uint32_t cc		 = abbcc - abb * 100;
			const uint32_t ffgg		 = static_cast<uint32_t>((static_cast<uint64_t>(ffgghhii) * 109951163) >> 40);
			const uint32_t hhii		 = static_cast<uint32_t>(ffgghhii - ffgg * 10000);

			buf[0] = static_cast<char>(a) + '0';
			buf += a > 0;
			std::memcpy(buf, char_table_2_digit_data + bb, 2);
			std::memcpy(buf + 2, char_table_2_digit_data + cc, 2);
			std::memcpy(buf + 4, char_table_4_digit_data + ddee, 4);
			std::memcpy(buf + 8, char_table_4_digit_data + ffgg, 4);
			std::memcpy(buf + 12, char_table_4_digit_data + hhii, 4);
			return buf + 16 - tz;
		}

		consteval uint32_t numbits(uint32_t x) noexcept {
			return x < 2 ? x : 1 + numbits(x >> 1);
		}

		VN_FORCE_INLINE static int64_t abs(int64_t value) noexcept {
			const uint64_t temp = static_cast<uint64_t>(value >> 63);
			value ^= temp;
			value += temp & 1;
			return value;
		}

		template<float_types v_type> struct to_chars_impl<v_type> {
			VN_FORCE_INLINE static char* impl(char* buf, v_type val) noexcept {
				static_assert(std::numeric_limits<v_type>::is_iec559);
				static_assert(std::numeric_limits<v_type>::radix == 2);
				static_assert(std::is_same_v<float, v_type> || std::is_same_v<double, v_type>);
				static_assert(sizeof(float) == 4 && sizeof(double) == 8);
				using Raw					   = std::conditional_t<std::is_same_v<float, v_type>, uint32_t, uint64_t>;

				if (val == 0.0) {
					*buf = '-';
					buf += (std::bit_cast<Raw>(val) >> (sizeof(v_type) * 8 - 1));
					*buf = '0';
					return buf + 1;
				}

				using Conversion						 = jkj::dragonbox::default_float_bit_carrier_conversion_traits<v_type>;
				using FormatTraits						 = jkj::dragonbox::ieee754_binary_traits<typename Conversion::format, typename Conversion::carrier_uint>;
				static constexpr uint32_t exp_bits_count = numbits(std::numeric_limits<v_type>::max_exponent - std::numeric_limits<v_type>::min_exponent + 1);
				const auto float_bits					 = jkj::dragonbox::make_float_bits<v_type, Conversion, FormatTraits>(val);
				const auto exp_bits						 = float_bits.extract_exponent_bits();
				const auto s							 = float_bits.remove_exponent_bits();

				if (exp_bits == (uint32_t(1) << exp_bits_count) - 1) [[unlikely]] {
					std::memcpy(buf, "null", 4);
					return buf + 4;
				}

				*buf							 = '-';
				static constexpr auto zero_local = v_type(0.0);
				buf += (val < zero_local);

				const auto v = jkj::dragonbox::to_decimal_ex(s, exp_bits, jkj::dragonbox::policy::sign::ignore, jkj::dragonbox::policy::trailing_zero::ignore);

				uint64_t sig_dec = v.significand;
				int32_t exp_dec	 = v.exponent;

				int32_t sig_len = 17;
				sig_len -= (sig_dec < 100000000ull * 100000000ull);
				sig_len -= (sig_dec < 100000000ull * 10000000ull);
				int32_t dot_pos = sig_len + exp_dec;

				if (-6 < dot_pos && dot_pos <= 21) {
					if (dot_pos <= 0) {
						auto num_hdr = buf + (2 - dot_pos);
						auto num_end = write_64_len_15_to_17_trim(num_hdr, sig_dec);
						buf[0]		 = '0';
						buf[1]		 = '.';
						buf += 2;
						std::memset(buf, '0', size_t(num_hdr - buf));
						return num_end;
					} else {
						std::memset(buf, '0', 24);
						auto num_hdr = buf + 1;
						auto num_end = write_64_len_15_to_17_trim(num_hdr, sig_dec);
						std::memmove(buf, buf + 1, size_t(dot_pos));
						buf[dot_pos] = '.';
						return ((num_end - num_hdr) <= dot_pos) ? buf + dot_pos : num_end;
					}
				} else {
					auto end = write_64_len_15_to_17_trim(buf + 1, sig_dec);
					end -= (end == buf + 2);
					exp_dec += sig_len - 1;
					buf[0] = buf[1];
					buf[1] = '.';
					end[0] = 'e';
					buf	   = end + 1;
					buf[0] = '-';
					buf += exp_dec < 0;
					exp_dec = static_cast<int32_t>(abs(exp_dec));
					if (exp_dec < 10) {
						buf[0] = char_table_1_digit_data[exp_dec];
						return buf + 1;
					} else if (exp_dec < 100) {
						std::memcpy(buf, char_table_2_digit_data + exp_dec, 2);
						return buf + 2;
					} else {
						const uint32_t hi = (uint32_t(exp_dec) * 656) >> 16;
						const uint32_t lo = uint32_t(exp_dec) - hi * 100;
						buf[0]			  = uint8_t(hi) + '0';
						std::memcpy(&buf[1], char_table_2_digit_data + lo, 2);
						return buf + 3;
					}
				}
			}
		};

	}

	template<detail::float_types v_type> VN_FORCE_INLINE std::to_chars_result to_chars(char* first, char* last, v_type value, int base = 10) noexcept {
		if (base == 10) {
			char* end = detail::to_chars_impl<v_type>::impl(first, value);
			return { end, std::errc{} };
		} else {
			auto r = std::to_chars(first, last, value);
			return { r.ptr, r.ec };
		}
	}

}
