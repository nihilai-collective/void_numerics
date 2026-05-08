// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp
// vn-incl/d_to_str.hpp

#pragma once

#include <vn-incl/tables.hpp>
#include <vn-incl/dragonbox.hpp>

namespace vn {

	namespace detail {

			template<typename typeName> struct fiwb {
			VN_ALIGN(64ULL)
			inline static constexpr char charTable01[]{ 0x30, 0x30, 0x30, 0x31, 0x30, 0x32, 0x30, 0x33, 0x30, 0x34, 0x30, 0x35, 0x30, 0x36, 0x30, 0x37, 0x30, 0x38, 0x30, 0x39,
				0x31, 0x30, 0x31, 0x31, 0x31, 0x32, 0x31, 0x33, 0x31, 0x34, 0x31, 0x35, 0x31, 0x36, 0x31, 0x37, 0x31, 0x38, 0x31, 0x39, 0x32, 0x30, 0x32, 0x31, 0x32, 0x32, 0x32,
				0x33, 0x32, 0x34, 0x32, 0x35, 0x32, 0x36, 0x32, 0x37, 0x32, 0x38, 0x32, 0x39, 0x33, 0x30, 0x33, 0x31, 0x33, 0x32, 0x33, 0x33, 0x33, 0x34, 0x33, 0x35, 0x33, 0x36,
				0x33, 0x37, 0x33, 0x38, 0x33, 0x39, 0x34, 0x30, 0x34, 0x31, 0x34, 0x32, 0x34, 0x33, 0x34, 0x34, 0x34, 0x35, 0x34, 0x36, 0x34, 0x37, 0x34, 0x38, 0x34, 0x39, 0x35,
				0x30, 0x35, 0x31, 0x35, 0x32, 0x35, 0x33, 0x35, 0x34, 0x35, 0x35, 0x35, 0x36, 0x35, 0x37, 0x35, 0x38, 0x35, 0x39, 0x36, 0x30, 0x36, 0x31, 0x36, 0x32, 0x36, 0x33,
				0x36, 0x34, 0x36, 0x35, 0x36, 0x36, 0x36, 0x37, 0x36, 0x38, 0x36, 0x39, 0x37, 0x30, 0x37, 0x31, 0x37, 0x32, 0x37, 0x33, 0x37, 0x34, 0x37, 0x35, 0x37, 0x36, 0x37,
				0x37, 0x37, 0x38, 0x37, 0x39, 0x38, 0x30, 0x38, 0x31, 0x38, 0x32, 0x38, 0x33, 0x38, 0x34, 0x38, 0x35, 0x38, 0x36, 0x38, 0x37, 0x38, 0x38, 0x38, 0x39, 0x39, 0x30,
				0x39, 0x31, 0x39, 0x32, 0x39, 0x33, 0x39, 0x34, 0x39, 0x35, 0x39, 0x36, 0x39, 0x37, 0x39, 0x38, 0x39, 0x39 };
			VN_ALIGN(64ULL)
			inline static constexpr uint16_t charTable02[]{ 0x3030, 0x3130, 0x3230, 0x3330, 0x3430, 0x3530, 0x3630, 0x3730, 0x3830, 0x3930, 0x3031, 0x3131, 0x3231, 0x3331, 0x3431,
				0x3531, 0x3631, 0x3731, 0x3831, 0x3931, 0x3032, 0x3132, 0x3232, 0x3332, 0x3432, 0x3532, 0x3632, 0x3732, 0x3832, 0x3932, 0x3033, 0x3133, 0x3233, 0x3333, 0x3433,
				0x3533, 0x3633, 0x3733, 0x3833, 0x3933, 0x3034, 0x3134, 0x3234, 0x3334, 0x3434, 0x3534, 0x3634, 0x3734, 0x3834, 0x3934, 0x3035, 0x3135, 0x3235, 0x3335, 0x3435,
				0x3535, 0x3635, 0x3735, 0x3835, 0x3935, 0x3036, 0x3136, 0x3236, 0x3336, 0x3436, 0x3536, 0x3636, 0x3736, 0x3836, 0x3936, 0x3037, 0x3137, 0x3237, 0x3337, 0x3437,
				0x3537, 0x3637, 0x3737, 0x3837, 0x3937, 0x3038, 0x3138, 0x3238, 0x3338, 0x3438, 0x3538, 0x3638, 0x3738, 0x3838, 0x3938, 0x3039, 0x3139, 0x3239, 0x3339, 0x3439,
				0x3539, 0x3639, 0x3739, 0x3839, 0x3939 };
			VN_ALIGN(64ULL)
			inline static constexpr auto charTable04{ [] {
				std::array<uint32_t, 10000> return_values{};
				for (uint32_t i = 0; i < 10000; ++i) {
					return_values[i] = (0x30 + (i / 1000)) | ((0x30 + ((i / 100) % 10)) << 8) | ((0x30 + ((i / 10) % 10)) << 16) | ((0x30 + (i % 10)) << 24);
				}
				return return_values;
			}() };
		};

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

		inline constexpr uint8_t decTrailingZeroTable[] = { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
			0, 0, 0, 0 };

		VN_FORCE_INLINE auto* writeu64Len15To17Trim(auto* buf, uint64_t sig) noexcept {
			uint32_t tz1, tz2, tz;
			const uint64_t abbccddee = multiply_and_shift<uint64_t, 100000000>::impl(sig);
			const uint64_t ffgghhii	 = sig - abbccddee * 100000000;
			uint32_t abbcc			 = multiply_and_shift<uint64_t, 10000>::impl(abbccddee);
			uint32_t ddee			 = abbccddee - abbcc * 10000;
			uint32_t abb			 = uint32_t((uint64_t(abbcc) * 167773) >> 24);
			uint32_t a				 = (abb * 41) >> 12;
			uint32_t bb				 = abb - a * 100;
			uint32_t cc				 = abbcc - abb * 100;
			buf[0]					 = uint8_t(a + '0');
			buf += a > 0;
			bool lz = bb < 10 && a == 0;
			std::memcpy(buf, fiwb<void>::charTable01 + (bb * 2 + lz), 2);
			buf -= lz;
			std::memcpy(buf + 2, fiwb<void>::charTable02 + cc, 2);

			if (ffgghhii) {
				uint32_t ffgg = uint32_t((uint64_t(ffgghhii) * 109951163) >> 40);
				uint32_t hhii = ffgghhii - ffgg * 10000;
				uint32_t ff	  = (ffgg * 5243) >> 19;
				uint32_t gg	  = ffgg - ff * 100;
				std::memcpy(buf + 4, fiwb<void>::charTable04.data() + ddee, 4);
				std::memcpy(buf + 8, fiwb<void>::charTable04.data() + ffgg, 4);
				std::memcpy(buf + 10, fiwb<void>::charTable02 + gg, 2);
				if (hhii) {
					uint32_t hh = (hhii * 5243) >> 19;
					uint32_t ii = hhii - hh * 100;
					std::memcpy(buf + 12, fiwb<void>::charTable04.data() + hhii, 4);
					tz1 = decTrailingZeroTable[hh];
					tz2 = decTrailingZeroTable[ii];
					tz	= hhii ? tz2 : (tz1 + 2);
					buf += 16 - tz;
					return buf;
				} else {
					tz1 = decTrailingZeroTable[ff];
					tz2 = decTrailingZeroTable[gg];
					tz	= gg ? tz2 : (tz1 + 2);
					buf += 12 - tz;
					return buf;
				}
			} else {
				if (ddee) {
					uint32_t dd = (ddee * 5243) >> 19;
					uint32_t ee = ddee - dd * 100;
					std::memcpy(buf + 4, fiwb<void>::charTable04.data() + ddee, 4);
					tz1 = decTrailingZeroTable[dd];
					tz2 = decTrailingZeroTable[ee];
					tz	= ee ? tz2 : (tz1 + 2);
					buf += 8 - tz;
					return buf;
				} else {
					tz1 = decTrailingZeroTable[bb];
					tz2 = decTrailingZeroTable[cc];
					tz	= cc ? tz2 : (tz1 + tz2);
					buf += 4 - tz;
					return buf;
				}
			}
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
				constexpr bool is_float = std::is_same_v<float, v_type>;
				using Raw				= std::conditional_t<std::is_same_v<float, v_type>, uint32_t, uint64_t>;

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

				*buf				= '-';
				constexpr auto zero = v_type(0.0);
				buf += (val < zero);

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
						auto num_end = writeu64Len15To17Trim(num_hdr, sig_dec);
						buf[0]		 = '0';
						buf[1]		 = '.';
						buf += 2;
						std::memset(buf, '0', size_t(num_hdr - buf));
						return num_end;
					} else {
						std::memset(buf, '0', 24);
						auto num_hdr = buf + 1;
						auto num_end = writeu64Len15To17Trim(num_hdr, sig_dec);
						std::memmove(buf, buf + 1, size_t(dot_pos));
						buf[dot_pos] = '.';
						return ((num_end - num_hdr) <= dot_pos) ? buf + dot_pos : num_end;
					}
				} else {
					auto end = writeu64Len15To17Trim(buf + 1, sig_dec);
					end -= (end == buf + 2);
					exp_dec += sig_len - 1;
					buf[0] = buf[1];
					buf[1] = '.';
					end[0] = 'E';
					buf	   = end + 1;
					buf[0] = '-';
					buf += exp_dec < 0;
					exp_dec = abs(exp_dec);
					if (exp_dec < 100) {
						uint32_t lz = exp_dec < 10;
						std::memcpy(buf, fiwb<void>::charTable01 + (exp_dec * 2 + lz), 2);
						return buf + 2 - lz;
					} else {
						const uint32_t hi = (uint32_t(exp_dec) * 656) >> 16;
						const uint32_t lo = uint32_t(exp_dec) - hi * 100;
						buf[0]			  = uint8_t(hi) + '0';
						std::memcpy(&buf[1], fiwb<void>::charTable01 + (lo * 2), 2);
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
