// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp
// vn-incl/str_to_i.hpp

#pragma once

#include <vn-incl/tables.hpp>
#include <iostream>

namespace vn {

	template<integer_types v_type> VN_FORCE_INLINE bool is_digit(v_type value) {
		return static_cast<uint8_t>(value - '0') < 10;
	}

	template<typename v_type> struct from_chars_impl;

	template<integer_types v_type> struct digit_counts_per_bits;

	template<uint64_types v_type> struct digit_counts_per_bits<v_type> {
		static constexpr uint64_t value{ 20 };
	};

	template<uint32_types v_type> struct digit_counts_per_bits<v_type> {
		static constexpr uint64_t value{ 10 };
	};

	template<uint16_types v_type> struct digit_counts_per_bits<v_type> {
		static constexpr uint64_t value{ 5 };
	};

	template<uint8_types v_type> struct digit_counts_per_bits<v_type> {
		static constexpr uint64_t value{ 3 };
	};

	template<int64_types v_type> struct digit_counts_per_bits<v_type> {
		static constexpr uint64_t value{ 19 };
	};

	template<int32_types v_type> struct digit_counts_per_bits<v_type> {
		static constexpr uint64_t value{ 10 };
	};

	template<int16_types v_type> struct digit_counts_per_bits<v_type> {
		static constexpr uint64_t value{ 5 };
	};

	template<int8_types v_type> struct digit_counts_per_bits<v_type> {
		static constexpr uint64_t value{ 3 };
	};

	template<int32_types v_type> struct from_chars_impl<v_type> : public pow_tables<>, public exp_tables<> {
		constexpr from_chars_impl() noexcept = default;

		VN_FORCE_INLINE static uint32_t umul_64_generic(int32_t a, int32_t b, int32_t& hi) noexcept {
			const int64_t res = static_cast<int64_t>(a) * static_cast<int64_t>(b);
			hi				   = static_cast<int32_t>(res >> 32);
			return static_cast<int32_t>(res);
		}

		VN_FORCE_INLINE static bool multiply(int32_t& value, int32_t exp_value) noexcept {
			const int64_t res = static_cast<int64_t>(value) * static_cast<int64_t>(exp_value);
			value			   = static_cast<int32_t>(res);
			return res <= 0xFFFFFFFFU;
		}

		VN_FORCE_INLINE static bool divide(int32_t& value, int32_t exp_value) noexcept {
			const int32_t remainder = value % exp_value;
			value /= exp_value;
			return remainder == 0;
		}

		VN_FORCE_INLINE static const uint8_t* parse_fraction(v_type& value, const uint8_t* iter) noexcept {
			if VN_LIKELY (is_digit(*iter)) {
				v_type frac_value{ static_cast<v_type>(*iter - zero) };
				typename get_int_type<v_type>::type frac_digits{ 1 };
				++iter;
				while (is_digit(*iter)) {
					frac_value = frac_value * 10 + static_cast<v_type>(*iter - zero);
					++iter;
					++frac_digits;
				}
				if (exp_table[*iter]) {
					++iter;
					int8_t exp_sign = 1;
					if (*iter == minus) {
						exp_sign = -1;
						++iter;
					} else if (*iter == plus) {
						++iter;
					}
					return parse_exponent_post_frac(value, iter, exp_sign, frac_value, frac_digits);
				}
			}
			if VN_LIKELY (!exp_frac_table[*iter]) {
				return iter;
			} else {
				return nullptr;
			}
		}

		VN_FORCE_INLINE static const uint8_t* parse_exponent_post_frac(v_type& value, const uint8_t* iter, int8_t exp_sign, v_type frac_value,
			typename get_int_type<v_type>::type frac_digits) noexcept {
			if VN_LIKELY (is_digit(*iter)) {
				v_type exp_value{ static_cast<v_type>(*iter - zero) };
				++iter;
				while (is_digit(*iter)) {
					exp_value = exp_value * 10 + static_cast<v_type>(*iter - zero);
					++iter;
				}
				if VN_LIKELY (exp_value < 19) {
					const v_type power_exp = power_of_ten_int[exp_value];

					constexpr v_type double_max = std::numeric_limits<v_type>::max();
					constexpr v_type double_min = std::numeric_limits<v_type>::min();

					if (frac_digits + exp_value >= 0) {
						exp_value *= exp_sign;
						const auto fractional_correction =
							exp_value > frac_digits ? frac_value * power_of_ten_int[exp_value - frac_digits] : frac_value / power_of_ten_int[frac_digits - exp_value];
						return (exp_sign > 0) ? ((value <= (double_max / power_exp)) ? (multiply(value, power_exp), value += fractional_correction, iter) : nullptr)
											  : ((value / power_exp >= (double_min)) ? (divide(value, power_exp), value += fractional_correction, iter) : nullptr);
					} else {
						return (exp_sign > 0) ? ((value <= (double_max / power_exp)) ? (multiply(value, power_exp), iter) : nullptr)
											  : ((value / power_exp >= (double_min)) ? (divide(value, power_exp), iter) : nullptr);
					}
				}
				VN_ELSE_UNLIKELY(else) {
					return nullptr;
				}
			}
			VN_ELSE_UNLIKELY(else) {
				return nullptr;
			}
		}

		VN_FORCE_INLINE static const uint8_t* parse_exponent(v_type& value, const uint8_t* iter, int8_t exp_sign) noexcept {
			if VN_LIKELY (is_digit(*iter)) {
				v_type exp_value{ static_cast<v_type>(*iter - zero) };
				++iter;
				while (is_digit(*iter)) {
					exp_value = exp_value * 10 + static_cast<v_type>(*iter - zero);
					++iter;
				}
				if VN_LIKELY (exp_value < 19) {
					const v_type power_exp		= power_of_ten_int[exp_value];
					constexpr v_type double_max = std::numeric_limits<v_type>::max();
					constexpr v_type double_min = std::numeric_limits<v_type>::min();
					exp_value *= exp_sign;
					return (exp_sign > 0) ? ((value <= (double_max / power_exp)) ? (multiply(value, power_exp), iter) : nullptr)
										  : ((value / power_exp >= (double_min)) ? (divide(value, power_exp), iter) : nullptr);
				}
				VN_ELSE_UNLIKELY(else) {
					return nullptr;
				}
			}
			VN_ELSE_UNLIKELY(else) {
				return nullptr;
			}
		}

		static const uint8_t* finish_parse(v_type& value, const uint8_t* iter) noexcept {
			if VN_UNLIKELY (*iter == decimal) {
				++iter;
				return parse_fraction(value, iter);
			} else if (exp_table[*iter]) {
				++iter;
				int8_t exp_sign = 1;
				if (*iter == minus) {
					exp_sign = -1;
					++iter;
				} else if (*iter == plus) {
					++iter;
				}
				return parse_exponent(value, iter, exp_sign);
			}
			if VN_LIKELY (!exp_frac_table[*iter]) {
				return iter;
			} else {
				return nullptr;
			}
		}

		template<bool negative> VN_FORCE_INLINE static const uint8_t* parse_integer(v_type& value, const uint8_t* iter, const uint8_t* end) noexcept {
			uint8_t numTmp{ *iter };
			if VN_LIKELY (is_digit(numTmp)) {
				value = numTmp - zero;
				++iter;
				numTmp = *iter;
			} else [[unlikely]] {
				return nullptr;
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_UNLIKELY (iter[-2] == zero) {
				return nullptr;
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				if constexpr (negative) {
					if (static_cast<uint32_t>(value) > static_cast<uint32_t>(raw_comp_vals_neg<v_type>[numTmp])) {
						return nullptr;
					}
					value *= -1;
					value = static_cast<v_type>(static_cast<uint32_t>(value * 10 - static_cast<uint32_t>(numTmp - zero)));
				} else {
					if (static_cast<v_type>(value) > static_cast<v_type>(raw_comp_vals_pos<v_type>[numTmp])) {
						return nullptr;
					}
					value = static_cast<int32_t>(value * 10 + static_cast<uint32_t>(numTmp - zero));
				}
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				if constexpr (negative) {
					value = value * 10 - static_cast<v_type>(numTmp - zero);
				} else {
					value = value * 10 + static_cast<v_type>(numTmp - zero);
				}
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
			}
			return nullptr;
		}

		VN_FORCE_INLINE static bool impl(v_type& value, const char*& iter, const char* end) noexcept {
			if VN_LIKELY (iter < end) {
				if (*iter == minus) {
					++iter;
					const uint8_t* result_ptr = parse_integer<true>(value, std::bit_cast<const uint8_t*>(iter), std::bit_cast<const uint8_t*>(end));
					if VN_LIKELY (result_ptr) {
						iter += result_ptr - std::bit_cast<const uint8_t*>(iter);
						return true;
					} else {
						value = 0;
						return false;
					}
				} else {
					const uint8_t* result_ptr = parse_integer<false>(value, std::bit_cast<const uint8_t*>(iter), std::bit_cast<const uint8_t*>(end));
					if VN_LIKELY (result_ptr) {
						iter += result_ptr - std::bit_cast<const uint8_t*>(iter);
						return true;
					} else {
						value = 0;
						return false;
					}
				}
			} else {
				return false;
			}
		}
	};

	template<int64_types v_type> struct from_chars_impl<v_type> : public pow_tables<>, public exp_tables<> {
		constexpr from_chars_impl() noexcept = default;

		template<bool, typename integer_sequence> struct loop_unroller;

		template<bool negative, uint64_t... indices> struct loop_unroller<negative, std::integer_sequence<uint64_t, indices...>> {
			template<uint64_t index> VN_FORCE_INLINE static bool impl(v_type& value, const uint8_t*& iter, const uint8_t* end, uint8_t& num_tmp) {
				if constexpr (index == sizeof...(indices) - 2) {
					if VN_LIKELY (is_digit(num_tmp)) {
						if constexpr (negative) {
							if (static_cast<std::make_unsigned_t<v_type>>(value) > static_cast<std::make_unsigned_t<v_type>>(raw_comp_vals_neg<v_type>[num_tmp])) {
								iter = nullptr;
								return false;
							}
							value *= -1;
							value = static_cast<v_type>(static_cast<std::make_unsigned_t<v_type>>(value * 10 - static_cast<std::make_unsigned_t<v_type>>(num_tmp - zero)));
						} else {
							if (static_cast<v_type>(value) > static_cast<v_type>(raw_comp_vals_pos<v_type>[num_tmp])) {
								iter = nullptr;
								return false;
							}
							value = static_cast<int64_t>(value * 10 + static_cast<std::make_unsigned_t<v_type>>(num_tmp - zero));
						}
						++iter;
						num_tmp = *iter;
					}
					VN_ELSE_UNLIKELY(else) {
						if VN_LIKELY (!exp_frac_table[num_tmp]) {
							if constexpr (negative) {
								value *= -1;
								return false;
							} else {
								return false;
							}
						}
						if constexpr (negative) {
							iter = (iter = finish_parse(value, iter), value *= -1, iter);
							return false;
						} else {
							iter = finish_parse(value, iter);
							return false;
						}
					}

					if VN_LIKELY (is_digit(num_tmp)) {
						if constexpr (negative) {
							value = value * 10 - static_cast<v_type>(num_tmp - zero);
						} else {
							value = value * 10 + static_cast<v_type>(num_tmp - zero);
						}
						++iter;
						num_tmp = *iter;
					}
					VN_ELSE_UNLIKELY(else) {
						if VN_LIKELY (!exp_frac_table[num_tmp]) {
							return false;
						}
					}
				} else {
					if VN_LIKELY (is_digit(num_tmp)) {
						value = value * 10 + static_cast<v_type>(num_tmp - zero);
						++iter;
						num_tmp = *iter;
					}
					VN_ELSE_UNLIKELY(else) {
						if VN_LIKELY (!exp_frac_table[num_tmp]) {
							if constexpr (negative) {
								value *= -1;
								return false;
							} else {
								return false;
							}
						}
						if constexpr (negative) {
							(iter = finish_parse(value, iter), value *= -1, iter);
							return false;
						} else {
							iter = finish_parse(value, iter);
							return false;
						}
					}
				}
				return true;
			}
			VN_FORCE_INLINE static bool impl(v_type& value, const uint8_t*& iter, const uint8_t* end, uint8_t& num_tmp) {
				return ((impl<indices>(value, iter, end, num_tmp)) && ...);
			}
		};

		VN_FORCE_INLINE static v_type mul_128_generic(v_type ab, v_type cd, v_type& hi) noexcept {
			v_type a_high = ab >> 32;
			v_type a_low  = ab & 0xFFFFFFFF;
			v_type b_high = cd >> 32;
			v_type b_low  = cd & 0xFFFFFFFF;
			v_type ad	  = a_high * b_low;
			v_type bd	  = a_high * b_low;
			v_type adbc	  = ad + a_low * b_high;
			v_type lo	  = bd + (adbc << 32);
			v_type carry  = (lo < bd);
			hi			  = a_high * b_high + (adbc >> 32) + carry;
			return lo;
		}

		VN_FORCE_INLINE static bool multiply(v_type& value, v_type exp_value) noexcept {
#if VN_COMPILER_CLANG || VN_COMPILER_GCC
			const __int128_t res = static_cast<__int128_t>(value) * static_cast<__int128_t>(exp_value);
			value				 = static_cast<v_type>(res);
			return res <= std::numeric_limits<v_type>::max();
#elif VN_COMPILER_MSVC
			int64_t values;
			value = _mul128(value, exp_value, &values);
			return values == 0;
#else
			v_type values;
			value = mul_128_generic(value, exp_value, &values);
			return values == 0;
#endif
		}

		VN_FORCE_INLINE static bool divide(v_type& value, v_type exp_value) noexcept {
#if VN_COMPILER_CLANG || VN_COMPILER_GCC
			const __int128_t dividend = static_cast<__int128_t>(value);
			value					  = static_cast<v_type>(dividend / static_cast<__int128_t>(exp_value));
			return (dividend % static_cast<__int128_t>(exp_value)) == 0;
#elif VN_COMPILER_MSVC
			int64_t values;
			value = _div128(0, value, exp_value, &values);
			return values == 0;
#else
			v_type values;
			values = value % exp_value;
			value  = value / exp_value;
			return values == 0;
#endif
		}

		VN_FORCE_INLINE static const uint8_t* parse_fraction(v_type& value, const uint8_t* iter) noexcept {
			if VN_LIKELY (is_digit(*iter)) {
				v_type frac_value{ static_cast<v_type>(*iter - zero) };
				typename get_int_type<v_type>::type frac_digits{ 1 };
				++iter;
				while (is_digit(*iter)) {
					frac_value = frac_value * 10 + static_cast<v_type>(*iter - zero);
					++iter;
					++frac_digits;
				}
				if (exp_table[*iter]) {
					++iter;
					int8_t exp_sign = 1;
					if (*iter == minus) {
						exp_sign = -1;
						++iter;
					} else if (*iter == plus) {
						++iter;
					}
					return parse_exponent_post_frac(value, iter, exp_sign, frac_value, frac_digits);
				}
			}
			if VN_LIKELY (!exp_frac_table[*iter]) {
				return iter;
			} else {
				return nullptr;
			}
		}

		VN_FORCE_INLINE static const uint8_t* parse_exponent_post_frac(v_type& value, const uint8_t* iter, int8_t exp_sign, v_type frac_value,
			typename get_int_type<v_type>::type frac_digits) noexcept {
			if VN_LIKELY (is_digit(*iter)) {
				v_type exp_value{ static_cast<v_type>(*iter - zero) };
				++iter;
				while (is_digit(*iter)) {
					exp_value = exp_value * 10 + static_cast<v_type>(*iter - zero);
					++iter;
				}
				if VN_LIKELY (exp_value < 19) {
					const v_type power_exp = power_of_ten_int[exp_value];

					constexpr v_type double_max = std::numeric_limits<v_type>::max();
					constexpr v_type double_min = std::numeric_limits<v_type>::min();

					if (frac_digits + exp_value >= 0) {
						exp_value *= exp_sign;
						const auto fractional_correction =
							exp_value > frac_digits ? frac_value * power_of_ten_int[exp_value - frac_digits] : frac_value / power_of_ten_int[frac_digits - exp_value];
						return (exp_sign > 0) ? ((value <= (double_max / power_exp)) ? (multiply(value, power_exp), value += fractional_correction, iter) : nullptr)
											  : ((value / power_exp >= (double_min)) ? (divide(value, power_exp), value += fractional_correction, iter) : nullptr);
					} else {
						return (exp_sign > 0) ? ((value <= (double_max / power_exp)) ? (multiply(value, power_exp), iter) : nullptr)
											  : ((value / power_exp >= (double_min)) ? (divide(value, power_exp), iter) : nullptr);
					}
				}
				VN_ELSE_UNLIKELY(else) {
					return nullptr;
				}
			}
			VN_ELSE_UNLIKELY(else) {
				return nullptr;
			}
		}

		VN_FORCE_INLINE static const uint8_t* parse_exponent(v_type& value, const uint8_t* iter, int8_t exp_sign) noexcept {
			if VN_LIKELY (is_digit(*iter)) {
				v_type exp_value{ static_cast<v_type>(*iter - zero) };
				++iter;
				while (is_digit(*iter)) {
					exp_value = exp_value * 10 + static_cast<v_type>(*iter - zero);
					++iter;
				}
				if VN_LIKELY (exp_value < 19) {
					const v_type power_exp		= power_of_ten_int[exp_value];
					constexpr v_type double_max = std::numeric_limits<v_type>::max();
					constexpr v_type double_min = std::numeric_limits<v_type>::min();
					exp_value *= exp_sign;
					return (exp_sign > 0) ? ((value <= (double_max / power_exp)) ? (multiply(value, power_exp), iter) : nullptr)
										  : ((value / power_exp >= (double_min)) ? (divide(value, power_exp), iter) : nullptr);
				}
				VN_ELSE_UNLIKELY(else) {
					return nullptr;
				}
			}
			VN_ELSE_UNLIKELY(else) {
				return nullptr;
			}
		}

		static const uint8_t* finish_parse(v_type& value, const uint8_t* iter) noexcept {
			if VN_UNLIKELY (*iter == decimal) {
				++iter;
				return parse_fraction(value, iter);
			} else if (exp_table[*iter]) {
				++iter;
				int8_t exp_sign = 1;
				if (*iter == minus) {
					exp_sign = -1;
					++iter;
				} else if (*iter == plus) {
					++iter;
				}
				return parse_exponent(value, iter, exp_sign);
			}
			if VN_LIKELY (!exp_frac_table[*iter]) {
				return iter;
			} else {
				return nullptr;
			}
		}

		template<bool negative> VN_FORCE_INLINE static const uint8_t* parse_integer(v_type& value, const uint8_t* iter, const uint8_t* end) noexcept {
			uint8_t numTmp{ *iter };
			if VN_LIKELY (is_digit(numTmp)) {
				value = numTmp - zero;
				++iter;
				numTmp = *iter;
			} else [[unlikely]] {
				return nullptr;
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_UNLIKELY (iter[-2] == zero) {
				return nullptr;
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				if constexpr (negative) {
					if (static_cast<uint64_t>(value) > static_cast<uint64_t>(raw_comp_vals_neg<v_type>[numTmp])) {
						return nullptr;
					}
					value *= -1;
					value = static_cast<v_type>(static_cast<uint64_t>(value * 10 - static_cast<uint64_t>(numTmp - zero)));
				} else {
					if (static_cast<v_type>(value) > static_cast<v_type>(raw_comp_vals_pos<v_type>[numTmp])) {
						return nullptr;
					}
					value = static_cast<int64_t>(value * 10 + static_cast<uint64_t>(numTmp - zero));
				}
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					if constexpr (negative) {
						value *= -1;
						return iter;
					} else {
						return iter;
					}
				}
				if constexpr (negative) {
					return (iter = finish_parse(value, iter), value *= -1, iter);
				} else {
					return finish_parse(value, iter);
				}
			}

			if VN_LIKELY (is_digit(numTmp)) {
				if constexpr (negative) {
					value = value * 10 - static_cast<v_type>(numTmp - zero);
				} else {
					value = value * 10 + static_cast<v_type>(numTmp - zero);
				}
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
			}
			return nullptr;
		}

		VN_FORCE_INLINE static bool impl(v_type& value, const char*& iter, const char* end) noexcept {
			if VN_LIKELY (iter < end) {
				if (*iter == minus) {
					++iter;
					const uint8_t* result_ptr = parse_integer<true>(value, std::bit_cast<const uint8_t*>(iter), std::bit_cast<const uint8_t*>(end));
					if VN_LIKELY (result_ptr) {
						iter += result_ptr - std::bit_cast<const uint8_t*>(iter);
						return true;
					} else {
						value = 0;
						return false;
					}
				} else {
					const uint8_t* result_ptr = parse_integer<false>(value, std::bit_cast<const uint8_t*>(iter), std::bit_cast<const uint8_t*>(end));
					if VN_LIKELY (result_ptr) {
						iter += result_ptr - std::bit_cast<const uint8_t*>(iter);
						return true;
					} else {
						value = 0;
						return false;
					}
				}
			} else {
				return false;
			}
		}
	};

	template<uint32_types v_type> struct from_chars_impl<v_type> : public pow_tables<>, public exp_tables<> {
		constexpr from_chars_impl() noexcept = default;

		VN_FORCE_INLINE static uint32_t umul_64_generic(uint32_t a, uint32_t b, uint32_t& hi) noexcept {
			const uint64_t res = static_cast<uint64_t>(a) * static_cast<uint64_t>(b);
			hi				   = static_cast<uint32_t>(res >> 32);
			return static_cast<uint32_t>(res);
		}

		VN_FORCE_INLINE static bool multiply(uint32_t& value, uint32_t exp_value) noexcept {
			const uint64_t res = static_cast<uint64_t>(value) * static_cast<uint64_t>(exp_value);
			value			   = static_cast<uint32_t>(res);
			return res <= 0xFFFFFFFFU;
		}

		VN_FORCE_INLINE static bool divide(uint32_t& value, uint32_t exp_value) noexcept {
			const uint32_t remainder = value % exp_value;
			value /= exp_value;
			return remainder == 0;
		}

		VN_FORCE_INLINE static const uint8_t* parse_fraction(v_type& value, const uint8_t* iter) noexcept {
			if VN_LIKELY (is_digit(*iter)) {
				v_type frac_value{ static_cast<v_type>(*iter - zero) };
				typename get_int_type<v_type>::type frac_digits{ 1 };
				++iter;
				while (is_digit(*iter)) {
					frac_value = frac_value * 10 + static_cast<v_type>(*iter - zero);
					++iter;
					++frac_digits;
				}
				if (exp_table[*iter]) {
					++iter;
					int8_t exp_sign = 1;
					if (*iter == minus) {
						exp_sign = -1;
						++iter;
					} else if (*iter == plus) {
						++iter;
					}
					return parse_exponent_post_frac(value, iter, exp_sign, frac_value, frac_digits);
				}
			}
			if VN_LIKELY (!exp_frac_table[*iter]) {
				return iter;
			} else {
				return nullptr;
			}
		}

		VN_FORCE_INLINE static const uint8_t* parse_exponent_post_frac(v_type& value, const uint8_t* iter, int8_t exp_sign, v_type frac_value,
			typename get_int_type<v_type>::type frac_digits) noexcept {
			if VN_LIKELY (is_digit(*iter)) {
				int64_t exp_value{ *iter - zero };
				++iter;
				while (is_digit(*iter)) {
					exp_value = exp_value * 10 + *iter - zero;
					++iter;
				}
				if VN_LIKELY (exp_value <= 19) {
					const v_type power_exp = power_of_ten_uint[exp_value];

					constexpr v_type double_max = std::numeric_limits<v_type>::max();
					constexpr v_type double_min = std::numeric_limits<v_type>::min();

					if (frac_digits + exp_value >= 0) {
						exp_value *= exp_sign;
						const auto fractional_correction =
							exp_value > frac_digits ? frac_value * power_of_ten_uint[exp_value - frac_digits] : frac_value / power_of_ten_uint[frac_digits - exp_value];
						return (exp_sign > 0) ? ((value <= (double_max / power_exp)) ? (multiply(value, power_exp), value += fractional_correction, iter) : nullptr)
											  : ((value / power_exp >= (double_min)) ? (divide(value, power_exp), value += fractional_correction, iter) : nullptr);
					} else {
						return (exp_sign > 0) ? ((value <= (double_max / power_exp)) ? (multiply(value, power_exp), iter) : nullptr)
											  : ((value / power_exp >= (double_min)) ? (divide(value, power_exp), iter) : nullptr);
					}
				}
				VN_ELSE_UNLIKELY(else) {
					return nullptr;
				}
			}
			VN_ELSE_UNLIKELY(else) {
				return nullptr;
			}
		}

		VN_FORCE_INLINE static const uint8_t* parse_exponent(v_type& value, const uint8_t* iter, int8_t exp_sign) noexcept {
			if VN_LIKELY (is_digit(*iter)) {
				v_type exp_value{ static_cast<v_type>(*iter - zero) };
				++iter;
				while (is_digit(*iter)) {
					exp_value = exp_value * 10 + static_cast<v_type>(*iter - zero);
					++iter;
				}
				if VN_LIKELY (exp_value <= 19) {
					const v_type power_exp		= power_of_ten_uint[exp_value];
					constexpr v_type double_max = std::numeric_limits<v_type>::max();
					constexpr v_type double_min = std::numeric_limits<v_type>::min();
					exp_value *= exp_sign;
					return (exp_sign > 0) ? ((value <= (double_max / power_exp)) ? (multiply(value, power_exp), iter) : nullptr)
										  : ((value / power_exp >= (double_min)) ? (divide(value, power_exp), iter) : nullptr);
				}
				VN_ELSE_UNLIKELY(else) {
					return nullptr;
				}
			}
			VN_ELSE_UNLIKELY(else) {
				return nullptr;
			}
		}

		static const uint8_t* finish_parse(v_type& value, const uint8_t* iter) noexcept {
			if VN_UNLIKELY (*iter == decimal) {
				++iter;
				return parse_fraction(value, iter);
			} else if (exp_table[*iter]) {
				++iter;
				int8_t exp_sign = 1;
				if (*iter == minus) {
					exp_sign = -1;
					++iter;
				} else if (*iter == plus) {
					++iter;
				}
				return parse_exponent(value, iter, exp_sign);
			}
			if VN_LIKELY (!exp_frac_table[*iter]) {
				return iter;
			} else {
				return nullptr;
			}
		}

		VN_FORCE_INLINE static const uint8_t* parse_integer(v_type& value, const uint8_t* iter, const uint8_t* end) noexcept {
			uint8_t numTmp{ *iter };
			if VN_LIKELY (is_digit(numTmp)) {
				value = static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			} else [[unlikely]] {
				return nullptr;
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_UNLIKELY (iter[-2] == zero) {
				return nullptr;
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				if (value > raw_comp_vals_pos<v_type>[numTmp]) {
					return nullptr;
				}
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
			}
			return nullptr;
		}

		VN_FORCE_INLINE static bool impl(v_type& value, const char*& iter, const char* end) noexcept {
			if VN_LIKELY (iter < end) {
				const uint8_t* result_ptr = parse_integer(value, std::bit_cast<const uint8_t*>(iter), std::bit_cast<const uint8_t*>(end));
				if VN_LIKELY (result_ptr) {
					iter += result_ptr - std::bit_cast<const uint8_t*>(iter);
					return true;
				} else {
					value = 0;
					return false;
				}
			} else {
				return false;
			}
		}
	};

	template<uint64_types v_type> struct from_chars_impl<v_type> : public pow_tables<>, public exp_tables<> {
		constexpr from_chars_impl() noexcept = default;

		template<typename integer_sequence> struct loop_unroller;

		template<uint64_t... indices> struct loop_unroller<std::integer_sequence<uint64_t, indices...>> {
			template<uint64_t index> VN_FORCE_INLINE static bool impl(v_type& value, const uint8_t*& iter, const uint8_t* end, uint8_t& num_tmp) {
				if constexpr (index == sizeof...(indices) - 2) {
					if VN_LIKELY (is_digit(num_tmp)) {
						if (value > raw_comp_vals_pos<v_type>[num_tmp]) {
							iter = nullptr;
							return false;
						}
						value = value * 10 + static_cast<v_type>(num_tmp - zero);
						++iter;
						num_tmp = *iter;
					}
					VN_ELSE_UNLIKELY(else) {
						if VN_LIKELY (!exp_frac_table[num_tmp]) {
							return false;
						}
						iter = finish_parse(value, iter);
						return false;
					}

					if VN_LIKELY (is_digit(num_tmp)) {
						value = value * 10 + static_cast<v_type>(num_tmp - zero);
						++iter;
						num_tmp = *iter;
					}
					VN_ELSE_UNLIKELY(else) {
						if VN_LIKELY (!exp_frac_table[num_tmp]) {
							return false;
						}
					}
				} else {
					if VN_LIKELY (is_digit(num_tmp)) {
						value = value * 10 + static_cast<v_type>(num_tmp - zero);
						++iter;
						num_tmp = *iter;
					}
					VN_ELSE_UNLIKELY(else) {
						if VN_LIKELY (!exp_frac_table[num_tmp]) {
							return false;
						}
						iter = finish_parse(value, iter);
						return false;
					}
				}
				return true;
			}
			VN_FORCE_INLINE static bool impl(v_type& value, const uint8_t*& iter, const uint8_t* end, uint8_t& num_tmp) {
				return ((impl<indices>(value, iter, end, num_tmp)) && ...);
			}
		};

		VN_FORCE_INLINE static v_type umul_128_generic(v_type ab, v_type cd, v_type& hi) noexcept {
			v_type a_high = ab >> 32;
			v_type a_low  = ab & 0xFFFFFFFF;
			v_type b_high = cd >> 32;
			v_type b_low  = cd & 0xFFFFFFFF;
			v_type ad	  = a_high * b_low;
			v_type bd	  = a_high * b_low;
			v_type adbc	  = ad + a_low * b_high;
			v_type lo	  = bd + (adbc << 32);
			v_type carry  = (lo < bd);
			hi			  = a_high * b_high + (adbc >> 32) + carry;
			return lo;
		}

		VN_FORCE_INLINE static bool multiply(v_type& value, v_type exp_value) noexcept {
#if VN_COMPILER_CLANG || VN_COMPILER_GCC
			const __uint128_t res = static_cast<__uint128_t>(value) * static_cast<__uint128_t>(exp_value);
			value				  = static_cast<v_type>(res);
			return res <= std::numeric_limits<v_type>::max();
#elif VN_COMPILER_MSVC
			uint64_t values;
			value = _umul128(value, exp_value, &values);
			return values == 0;
#else
			v_type values;
			value = umul_128_generic(value, exp_value, &values);
			return values == 0;
#endif
		}

		VN_FORCE_INLINE static bool divide(v_type& value, v_type exp_value) noexcept {
#if VN_COMPILER_CLANG || VN_COMPILER_GCC
			const __uint128_t dividend = static_cast<__uint128_t>(value);
			value					   = static_cast<v_type>(dividend / static_cast<__uint128_t>(exp_value));
			return (dividend % static_cast<__uint128_t>(exp_value)) == 0;
#elif VN_COMPILER_MSVC
			uint64_t values;
			value = _udiv128(0, value, exp_value, &values);
			return values == 0;
#else
			v_type values;
			values = value % exp_value;
			value  = value / exp_value;
			return values == 0;
#endif
		}

		VN_FORCE_INLINE static const uint8_t* parse_fraction(v_type& value, const uint8_t* iter) noexcept {
			if VN_LIKELY (is_digit(*iter)) {
				v_type frac_value{ static_cast<v_type>(*iter - zero) };
				typename get_int_type<v_type>::type frac_digits{ 1 };
				++iter;
				while (is_digit(*iter)) {
					frac_value = frac_value * 10 + static_cast<v_type>(*iter - zero);
					++iter;
					++frac_digits;
				}
				if (exp_table[*iter]) {
					++iter;
					int8_t exp_sign = 1;
					if (*iter == minus) {
						exp_sign = -1;
						++iter;
					} else if (*iter == plus) {
						++iter;
					}
					return parse_exponent_post_frac(value, iter, exp_sign, frac_value, frac_digits);
				}
			}
			if VN_LIKELY (!exp_frac_table[*iter]) {
				return iter;
			} else {
				return nullptr;
			}
		}

		VN_FORCE_INLINE static const uint8_t* parse_exponent_post_frac(v_type& value, const uint8_t* iter, int8_t exp_sign, v_type frac_value,
			typename get_int_type<v_type>::type frac_digits) noexcept {
			if VN_LIKELY (is_digit(*iter)) {
				int64_t exp_value{ *iter - zero };
				++iter;
				while (is_digit(*iter)) {
					exp_value = exp_value * 10 + *iter - zero;
					++iter;
				}
				if VN_LIKELY (exp_value <= 19) {
					const v_type power_exp = power_of_ten_uint[exp_value];

					constexpr v_type double_max = std::numeric_limits<v_type>::max();
					constexpr v_type double_min = std::numeric_limits<v_type>::min();

					if (frac_digits + exp_value >= 0) {
						exp_value *= exp_sign;
						const auto fractional_correction =
							exp_value > frac_digits ? frac_value * power_of_ten_uint[exp_value - frac_digits] : frac_value / power_of_ten_uint[frac_digits - exp_value];
						return (exp_sign > 0) ? ((value <= (double_max / power_exp)) ? (multiply(value, power_exp), value += fractional_correction, iter) : nullptr)
											  : ((value / power_exp >= (double_min)) ? (divide(value, power_exp), value += fractional_correction, iter) : nullptr);
					} else {
						return (exp_sign > 0) ? ((value <= (double_max / power_exp)) ? (multiply(value, power_exp), iter) : nullptr)
											  : ((value / power_exp >= (double_min)) ? (divide(value, power_exp), iter) : nullptr);
					}
				}
				VN_ELSE_UNLIKELY(else) {
					return nullptr;
				}
			}
			VN_ELSE_UNLIKELY(else) {
				return nullptr;
			}
		}

		VN_FORCE_INLINE static const uint8_t* parse_exponent(v_type& value, const uint8_t* iter, int8_t exp_sign) noexcept {
			if VN_LIKELY (is_digit(*iter)) {
				v_type exp_value{ static_cast<v_type>(*iter - zero) };
				++iter;
				while (is_digit(*iter)) {
					exp_value = exp_value * 10 + static_cast<v_type>(*iter - zero);
					++iter;
				}
				if VN_LIKELY (exp_value <= 19) {
					const v_type power_exp		= power_of_ten_uint[exp_value];
					constexpr v_type double_max = std::numeric_limits<v_type>::max();
					constexpr v_type double_min = std::numeric_limits<v_type>::min();
					exp_value *= exp_sign;
					return (exp_sign > 0) ? ((value <= (double_max / power_exp)) ? (multiply(value, power_exp), iter) : nullptr)
										  : ((value / power_exp >= (double_min)) ? (divide(value, power_exp), iter) : nullptr);
				}
				VN_ELSE_UNLIKELY(else) {
					return nullptr;
				}
			}
			VN_ELSE_UNLIKELY(else) {
				return nullptr;
			}
		}

		static const uint8_t* finish_parse(v_type& value, const uint8_t* iter) noexcept {
			if VN_UNLIKELY (*iter == decimal) {
				++iter;
				return parse_fraction(value, iter);
			} else if (exp_table[*iter]) {
				++iter;
				int8_t exp_sign = 1;
				if (*iter == minus) {
					exp_sign = -1;
					++iter;
				} else if (*iter == plus) {
					++iter;
				}
				return parse_exponent(value, iter, exp_sign);
			}
			if VN_LIKELY (!exp_frac_table[*iter]) {
				return iter;
			} else {
				return nullptr;
			}
		}

		VN_FORCE_INLINE static const uint8_t* parse_integer(v_type& value, const uint8_t* iter, const uint8_t* end) noexcept {
			uint8_t numTmp{ *iter };
			if VN_LIKELY (is_digit(numTmp)) {
				value = static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			} else [[unlikely]] {
				return nullptr;
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_UNLIKELY (iter[-2] == zero) {
				return nullptr;
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				if (value > raw_comp_vals_pos<v_type>[numTmp]) {
					return nullptr;
				}
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
				return finish_parse(value, iter);
			}

			if VN_LIKELY (is_digit(numTmp)) {
				value = value * 10 + static_cast<v_type>(numTmp - zero);
				++iter;
				numTmp = *iter;
			}
			VN_ELSE_UNLIKELY(else) {
				if VN_LIKELY (!exp_frac_table[numTmp]) {
					return iter;
				}
			}
			return nullptr;
		}

		VN_FORCE_INLINE static bool impl(v_type& value, const char*& iter, const char* end) noexcept {
			if VN_LIKELY (iter < end) {
				const uint8_t* result_ptr = parse_integer(value, std::bit_cast<const uint8_t*>(iter), std::bit_cast<const uint8_t*>(end));
				if VN_LIKELY (result_ptr) {
					iter += result_ptr - std::bit_cast<const uint8_t*>(iter);
					return true;
				} else {
					value = 0;
					return false;
				}
			} else {
				return false;
			}
		}
	};

	enum class errc {
		success		 = 0,
		failed_parse = 1,
	};

	struct from_chars_result {
		const char* ptr;
		errc ec;
	};

	template<integer_types v_type> bool from_chars(const char*& iter, const char* end, v_type& value) noexcept {
		return from_chars_impl<v_type>::impl(value, iter, end);
	}

}