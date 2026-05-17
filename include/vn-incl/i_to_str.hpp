// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp
// vn-incl/i_to_str.hpp

#pragma once

#include <vn-incl/utility.hpp>

namespace vn {

	namespace detail {

		template<typename v_type, uint64_t digit_length> struct to_chars_internal;

		template<typename v_type> struct to_chars_impl;

		template<uint_types v_type> struct to_chars_internal<v_type, 1ULL> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, const v_type value) noexcept {
				*buf = char_table_1_digit_data[value];
				return buf + 1;
			}
		};

		template<uint_types v_type> struct to_chars_internal<v_type, 2ULL> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, const v_type value) noexcept {
				std::memcpy(buf, char_table_2_digit_data + value, 2ULL);
				return buf + 2;
			}
		};

		template<uint_types v_type> struct to_chars_internal<v_type, 3ULL> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, const v_type value) noexcept {
				std::memcpy(buf, char_table_3_digit_data + value, 3ULL);
				return buf + 3;
			}
		};

		template<uint_types v_type> struct to_chars_internal<v_type, 4ULL> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, const v_type value) noexcept {
				std::memcpy(buf, char_table_4_digit_data + value, 4ULL);
				return buf + 4;
			}
		};

		template<uint_types v_type> struct to_chars_internal<v_type, 5ULL> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, const v_type value) noexcept {
				const uint64_t a = multiply_and_shift<uint64_t, 10000ULL>::impl(value);
				*buf			 = char_table_1_digit_data[a];
				std::memcpy(buf + 1, char_table_4_digit_data + value - a * 10000, 4ULL);
				return buf + 5;
			}
		};

		template<uint_types v_type> struct to_chars_internal<v_type, 6ULL> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, const v_type value) noexcept {
				const uint64_t ab = multiply_and_shift<uint64_t, 10000ULL>::impl(value);
				std::memcpy(buf, char_table_2_digit_data + ab, 2ULL);
				std::memcpy(buf + 2, char_table_4_digit_data + value - (ab * 10000U), 4ULL);
				return buf + 6;
			}
		};

		template<uint_types v_type> struct to_chars_internal<v_type, 7ULL> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, const v_type value) noexcept {
				const uint64_t abc = multiply_and_shift<uint64_t, 10000U>::impl(value);
				std::memcpy(buf, char_table_3_digit_data + abc, 3ULL);
				std::memcpy(buf + 3, char_table_4_digit_data + value - (abc * 10000U), 4ULL);
				return buf + 7;
			}
		};

		template<uint_types v_type> struct to_chars_internal<v_type, 8ULL> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, const v_type value) noexcept {
				const uint64_t abcd = multiply_and_shift<uint64_t, 10000U>::impl(value);
				std::memcpy(buf, char_table_4_digit_data + abcd, 4ULL);
				std::memcpy(buf + 4, char_table_4_digit_data + value - (abcd * 10000U), 4ULL);
				return buf + 8;
			}
		};

		template<uint_types v_type> struct to_chars_internal<v_type, 9ULL> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, const v_type value) noexcept {
				const uint64_t a		= multiply_and_shift<uint64_t, 100000000ULL>::impl(value);
				const uint64_t bcdefghi = value - a * 100000000ULL;
				const uint64_t bcde		= multiply_and_shift<uint64_t, 10000U>::impl(bcdefghi);
				const uint64_t fghi		= bcdefghi - (bcde * 10000U);
				*buf					= char_table_1_digit_data[a];
				std::memcpy(buf + 1, char_table_4_digit_data + bcde, 4ULL);
				std::memcpy(buf + 5, char_table_4_digit_data + fghi, 4ULL);
				return buf + 9;
			}
		};

		template<uint_types v_type> struct to_chars_internal<v_type, 10ULL> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, const v_type value) noexcept {
				const uint64_t ab		= multiply_and_shift<uint64_t, 100000000ULL>::impl(value);
				const uint64_t cdefghij = value - ab * 100000000ULL;
				const uint64_t cdef		= multiply_and_shift<uint64_t, 10000U>::impl(cdefghij);
				const uint64_t ghij		= cdefghij - (cdef * 10000U);
				std::memcpy(buf, char_table_2_digit_data + ab, 2ULL);
				std::memcpy(buf + 2, char_table_4_digit_data + cdef, 4ULL);
				std::memcpy(buf + 6, char_table_4_digit_data + ghij, 4ULL);
				return buf + 10;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 11ULL> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, const v_type value) noexcept {
				const uint64_t abc		= multiply_and_shift<uint64_t, 100000000ULL>::impl(value);
				const uint64_t defghijk = value - abc * 100000000ULL;
				const uint64_t defg		= multiply_and_shift<uint64_t, 10000U>::impl(defghijk);
				const uint64_t hijk		= defghijk - (defg * 10000U);
				std::memcpy(buf, char_table_3_digit_data + abc, 3ULL);
				std::memcpy(buf + 3, char_table_4_digit_data + defg, 4ULL);
				std::memcpy(buf + 7, char_table_4_digit_data + hijk, 4ULL);
				return buf + 11;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 12ULL> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, const v_type value) noexcept {
				const uint64_t abcd		= multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const uint64_t efghijkl = value - abcd * 100000000ULL;
				const uint64_t efgh		= multiply_and_shift<uint64_t, 10000U>::impl(efghijkl);
				const uint64_t ijkl		= efghijkl - (efgh * 10000U);
				std::memcpy(buf, char_table_4_digit_data + abcd, 4ULL);
				std::memcpy(buf + 4, char_table_4_digit_data + efgh, 4ULL);
				std::memcpy(buf + 8, char_table_4_digit_data + ijkl, 4ULL);
				return buf + 12;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 13ULL> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, const v_type value) noexcept {
				const uint64_t abcde	= multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const uint64_t fghijklm = value - abcde * 100000000ULL;
				const uint64_t a		= multiply_and_shift<uint64_t, 10000U>::impl(abcde);
				const uint64_t bcde		= abcde - (a * 10000U);
				const uint64_t fghi		= multiply_and_shift<uint64_t, 10000U>::impl(fghijklm);
				const uint64_t klm		= fghijklm - (fghi * 10000U);
				*buf					= char_table_1_digit_data[a];
				std::memcpy(buf + 1, char_table_4_digit_data + bcde, 4ULL);
				std::memcpy(buf + 5, char_table_4_digit_data + fghi, 4ULL);
				std::memcpy(buf + 9, char_table_4_digit_data + klm, 4ULL);
				return buf + 13;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 14ULL> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, const v_type value) noexcept {
				const uint64_t abcdef	= multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const uint64_t ghijklmn = value - abcdef * 100000000ULL;
				const uint64_t ab		= multiply_and_shift<uint64_t, 10000U>::impl(abcdef);
				const uint64_t cdef		= abcdef - (ab * 10000U);
				const uint64_t ghij		= ghijklmn * 0xd1b71759U >> 45;
				const uint64_t klmn		= ghijklmn - (ghij * 10000U);
				std::memcpy(buf, char_table_2_digit_data + ab, 2ULL);
				std::memcpy(buf + 2, char_table_4_digit_data + cdef, 4ULL);
				std::memcpy(buf + 6, char_table_4_digit_data + ghij, 4ULL);
				std::memcpy(buf + 10, char_table_4_digit_data + klmn, 4ULL);
				return buf + 14;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 15ULL> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, const v_type value) noexcept {
				const uint64_t abcdefg	= multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const uint64_t hijklmno = value - abcdefg * 100000000ULL;
				const uint64_t abc		= multiply_and_shift<uint64_t, 10000U>::impl(abcdefg);
				const uint64_t defg		= abcdefg - (abc * 10000U);
				const uint64_t hijk		= multiply_and_shift<uint64_t, 10000U>::impl(hijklmno);
				const uint64_t lmno		= hijklmno - (hijk * 10000U);
				std::memcpy(buf, char_table_3_digit_data + abc, 3ULL);
				std::memcpy(buf + 3, char_table_4_digit_data + defg, 4ULL);
				std::memcpy(buf + 7, char_table_4_digit_data + hijk, 4ULL);
				std::memcpy(buf + 11, char_table_4_digit_data + lmno, 4ULL);
				return buf + 15;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 16ULL> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, const v_type value) noexcept {
				const uint64_t abcdefgh = multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const uint64_t ijklmnop = value - abcdefgh * 100000000ULL;
				const uint64_t abcd		= abcdefgh * 0xd1b71759U >> 45;
				const uint64_t efgh		= abcdefgh - (abcd * 10000U);
				const uint64_t ijkl		= ijklmnop * 0xd1b71759U >> 45;
				const uint64_t mnop		= ijklmnop - (ijkl * 10000U);
				std::memcpy(buf, char_table_4_digit_data + abcd, 4ULL);
				std::memcpy(buf + 4, char_table_4_digit_data + efgh, 4ULL);
				std::memcpy(buf + 8, char_table_4_digit_data + ijkl, 4ULL);
				std::memcpy(buf + 12, char_table_4_digit_data + mnop, 4ULL);
				return buf + 16;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 17ULL> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, const v_type value) noexcept {
				const v_type abcdefghi	= multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const uint64_t abcdefg	= value - abcdefghi * 100000000ULL;
				const uint64_t a		= multiply_and_shift<v_type, 100000000ULL>::impl(abcdefghi);
				const uint64_t bcdefghi = abcdefghi - a * 100000000ULL;
				const uint64_t bcde		= bcdefghi * 0xd1b71759U >> 45;
				const uint64_t fghi		= bcdefghi - (bcde * 10000U);
				const uint64_t klm		= abcdefg * 0xd1b71759U >> 45;
				const uint64_t nopq		= abcdefg - (klm * 10000U);
				*buf					= char_table_1_digit_data[a];
				std::memcpy(buf + 1, char_table_4_digit_data + bcde, 4ULL);
				std::memcpy(buf + 5, char_table_4_digit_data + fghi, 4ULL);
				std::memcpy(buf + 9, char_table_4_digit_data + klm, 4ULL);
				std::memcpy(buf + 13, char_table_4_digit_data + nopq, 4ULL);
				return buf + 17;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 18ULL> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, const v_type value) noexcept {
				const v_type abcdefghij = multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const uint64_t klmnopqr = value - abcdefghij * 100000000ULL;
				const uint64_t ab		= multiply_and_shift<v_type, 100000000ULL>::impl(abcdefghij);
				const uint64_t cdefghij = abcdefghij - ab * 100000000ULL;
				const uint64_t cdef		= cdefghij * 0xd1b71759U >> 45;
				const uint64_t ghij		= cdefghij - (cdef * 10000U);
				const uint64_t klmn		= klmnopqr * 0xd1b71759U >> 45;
				const uint64_t opqr		= klmnopqr - (klmn * 10000U);
				std::memcpy(buf, char_table_2_digit_data + ab, 2ULL);
				std::memcpy(buf + 2, char_table_4_digit_data + cdef, 4ULL);
				std::memcpy(buf + 6, char_table_4_digit_data + ghij, 4ULL);
				std::memcpy(buf + 10, char_table_4_digit_data + klmn, 4ULL);
				std::memcpy(buf + 14, char_table_4_digit_data + opqr, 4ULL);
				return buf + 18;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 19ULL> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, const v_type value) noexcept {
				const v_type abcdefghijk = multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const uint64_t lmnopqrs	 = value - abcdefghijk * 100000000ULL;
				const uint64_t abc		 = multiply_and_shift<v_type, 100000000ULL>::impl(abcdefghijk);
				const uint64_t defghijk	 = abcdefghijk - abc * 100000000ULL;
				const uint64_t defg		 = defghijk * 0xd1b71759U >> 45;
				const uint64_t hijk		 = defghijk - (defg * 10000U);
				const uint64_t lmno		 = lmnopqrs * 0xd1b71759U >> 45;
				const uint64_t pqrs		 = lmnopqrs - (lmno * 10000U);
				std::memcpy(buf, char_table_3_digit_data + abc, 3ULL);
				std::memcpy(buf + 3, char_table_4_digit_data + defg, 4ULL);
				std::memcpy(buf + 7, char_table_4_digit_data + hijk, 4ULL);
				std::memcpy(buf + 11, char_table_4_digit_data + lmno, 4ULL);
				std::memcpy(buf + 15, char_table_4_digit_data + pqrs, 4ULL);
				return buf + 19;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 20ULL> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, const v_type value) noexcept {
				const v_type abcdefghijkl = multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const uint64_t mnopqrst	  = value - abcdefghijkl * 100000000ULL;
				const uint64_t abcd		  = multiply_and_shift<v_type, 100000000ULL>::impl(abcdefghijkl);
				const uint64_t efghijkl	  = abcdefghijkl - abcd * 100000000ULL;
				const uint64_t efgh		  = efghijkl * 0xd1b71759U >> 45;
				const uint64_t ijkl		  = efghijkl - (efgh * 10000U);
				const uint64_t mnop		  = mnopqrst * 0xd1b71759U >> 45;
				const uint64_t qrst		  = mnopqrst - (mnop * 10000U);
				std::memcpy(buf, char_table_4_digit_data + abcd, 4ULL);
				std::memcpy(buf + 4, char_table_4_digit_data + efgh, 4ULL);
				std::memcpy(buf + 8, char_table_4_digit_data + ijkl, 4ULL);
				std::memcpy(buf + 12, char_table_4_digit_data + mnop, 4ULL);
				std::memcpy(buf + 16, char_table_4_digit_data + qrst, 4ULL);
				return buf + 20;
			}
		};

		template<uint64_types v_type> struct to_chars_impl<v_type> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, char* __restrict end, const v_type value) noexcept {
				if (value < 10000000000ULL) {
					if (value < 100000ULL) {
						if (value < 1000ULL) {
							if (value < 100U) {
								if (value < 10U) {
									return (end - buf >= 1) ? (*buf = char_table_1_digit_data[value], buf + 1) : end;
								}
								return (end - buf >= 2) ? (std::memcpy(buf, char_table_2_digit_data + value, 2ULL), buf + 2) : end;
							}
							return (end - buf >= 3) ? (std::memcpy(buf, char_table_3_digit_data + value, 3ULL), buf + 3) : end;
						}
						if (value < 10000ULL) {
							return (end - buf >= 4) ? (std::memcpy(buf, char_table_4_digit_data + value, 4ULL), buf + 4) : end;
						}
						return (end - buf >= 5) ? to_chars_internal<v_type, 5ULL>::impl(buf, value) : end;
					}
					if (value < 10000000ULL) {
						if (value < 1000000ULL) {
							return (end - buf >= 6) ? to_chars_internal<v_type, 6ULL>::impl(buf, value) : end;
						}
						return (end - buf >= 7) ? to_chars_internal<v_type, 7ULL>::impl(buf, value) : end;
					}
					if (value < 100000000ULL) {
						return (end - buf >= 8) ? to_chars_internal<v_type, 8ULL>::impl(buf, value) : end;
					}
					if (value < 1000000000ULL) {
						return (end - buf >= 9) ? to_chars_internal<v_type, 9ULL>::impl(buf, value) : end;
					}
					return (end - buf >= 10) ? to_chars_internal<v_type, 10ULL>::impl(buf, value) : end;
				} else {
					if (value < 1000000000000000ULL) {
						if (value < 1000000000000ULL) {
							if (value < 100000000000ULL) {
								return (end - buf >= 11) ? to_chars_internal<v_type, 11ULL>::impl(buf, value) : end;
							}
							return (end - buf >= 12) ? to_chars_internal<v_type, 12ULL>::impl(buf, value) : end;
						}
						if (value < 10000000000000ULL) {
							return (end - buf >= 13) ? to_chars_internal<v_type, 13ULL>::impl(buf, value) : end;
						}
						if (value < 100000000000000ULL) {
							return (end - buf >= 14) ? to_chars_internal<v_type, 14ULL>::impl(buf, value) : end;
						}
						return (end - buf >= 15) ? to_chars_internal<v_type, 15ULL>::impl(buf, value) : end;
					}
					if (value < 100000000000000000ULL) {
						if (value < 10000000000000000ULL) {
							return (end - buf >= 16) ? to_chars_internal<v_type, 16ULL>::impl(buf, value) : end;
						}
						return (end - buf >= 17) ? to_chars_internal<v_type, 17ULL>::impl(buf, value) : end;
					}
					if (value < 1000000000000000000ULL) {
						return (end - buf >= 18) ? to_chars_internal<v_type, 18ULL>::impl(buf, value) : end;
					}
					if (value < 10000000000000000000ULL) {
						return (end - buf >= 19) ? to_chars_internal<v_type, 19ULL>::impl(buf, value) : end;
					}
					return (end - buf >= 20) ? to_chars_internal<v_type, 20ULL>::impl(buf, value) : end;
				}
			}
		};

		template<uint32_types v_type> struct to_chars_impl<v_type> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, char* __restrict end, const v_type value) noexcept {
				if (value < 1000000000U) {
					if (value < 100000U) {
						if (value < 10000U) {
							if (value < 1000U) {
								if (value < 100U) {
									if (value < 10U) {
										return (end - buf >= 1) ? (*buf = char_table_1_digit_data[value], buf + 1) : end;
									}
									return (end - buf >= 2) ? (std::memcpy(buf, char_table_2_digit_data + value, 2ULL), buf + 2) : end;
								}
								return (end - buf >= 3) ? (std::memcpy(buf, char_table_3_digit_data + value, 3ULL), buf + 3) : end;
							}
							return (end - buf >= 4) ? (std::memcpy(buf, char_table_4_digit_data + value, 4ULL), buf + 4) : end;
						}
						return (end - buf >= 5) ? to_chars_internal<v_type, 5ULL>::impl(buf, value) : end;
					}
					if (value < 10000000U) {
						if (value < 1000000U) {
							return (end - buf >= 6) ? to_chars_internal<v_type, 6ULL>::impl(buf, value) : end;
						}
						return (end - buf >= 7) ? to_chars_internal<v_type, 7ULL>::impl(buf, value) : end;
					}
					if (value < 100000000U) {
						return (end - buf >= 8) ? to_chars_internal<v_type, 8ULL>::impl(buf, value) : end;
					}
					return (end - buf >= 9) ? to_chars_internal<v_type, 9ULL>::impl(buf, value) : end;
				}
				return (end - buf >= 10) ? to_chars_internal<v_type, 10ULL>::impl(buf, value) : end;
			}
		};

		template<uint16_types v_type> struct to_chars_impl<v_type> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, char* __restrict end, const v_type value) noexcept {
				if (value < 10U) {
					return (end - buf >= 1) ? (*buf = char_table_1_digit_data[value], buf + 1) : end;
				}
				if (value < 100U) {
					return (end - buf >= 2) ? (std::memcpy(buf, char_table_2_digit_data + value, 2ULL), buf + 2) : end;
				}
				if (value < 1000U) {
					return (end - buf >= 3) ? (std::memcpy(buf, char_table_3_digit_data + value, 3ULL), buf + 3) : end;
				}
				if (value < 10000U) {
					return (end - buf >= 4) ? (std::memcpy(buf, char_table_4_digit_data + value, 4ULL), buf + 4) : end;
				}
				return (end - buf >= 5) ? to_chars_internal<v_type, 5ULL>::impl(buf, value) : end;
			}
		};

		template<uint8_types v_type> struct to_chars_impl<v_type> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, char* __restrict end, const v_type value) noexcept {
				if (value < 10U) {
					return (end - buf >= 1) ? (*buf = char_table_1_digit_data[value], buf + 1) : end;
				}
				if (value < 100U) {
					return (end - buf >= 2) ? (std::memcpy(buf, char_table_2_digit_data + value, 2ULL), buf + 2) : end;
				}
				return (end - buf >= 3) ? (std::memcpy(buf, char_table_3_digit_data + value, 3ULL), buf + 3) : end;
			}
		};

		template<int_types v_type> struct to_chars_impl<v_type> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, char* __restrict end, const v_type value) noexcept {
				using unsigned_type					 = std::make_unsigned_t<v_type>;
				constexpr unsigned_type shift_amount = static_cast<unsigned_type>(sizeof(v_type) * 8ULL - 1ULL);
				*buf								 = '-';
				return to_chars_impl<unsigned_type>::impl(buf + (value < 0), end,
					(static_cast<unsigned_type>(value) ^ static_cast<unsigned_type>(value >> shift_amount)) - static_cast<unsigned_type>(value >> shift_amount));
			}
		};
	}

	template<detail::integer_types v_type> VN_FORCE_INLINE std::to_chars_result to_chars(char* first, char* last, v_type value, int32_t base = 10) noexcept {
		if (base == 10) {
			char* end = detail::to_chars_impl<v_type>::impl(first, last, value);
			return { end, std::errc{} };
		} else {
			auto r = std::to_chars(first, last, value, base);
			return { r.ptr, r.ec };
		}
	}

}
