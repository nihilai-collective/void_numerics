
#pragma once

#include <vn-incl/utility.hpp>

namespace vn {

	namespace detail {

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

		template<typename v_type, uint64_t digit_length> struct to_chars_internal;

		template<typename v_type> struct to_chars_impl;

		template<uint_types v_type> struct to_chars_internal<v_type, 1ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				*buf = char_table_1_digit_data[value];
				return buf + 1;
			}
		};

		template<uint_types v_type> struct to_chars_internal<v_type, 2ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				std::memcpy(buf, char_table_2_digit_data + value, 2ULL);
				return buf + 2;
			}
		};

		template<uint_types v_type> struct to_chars_internal<v_type, 3ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				std::memcpy(buf, char_table_3_digit_data + value, 3ULL);
				return buf + 3;
			}
		};

		template<uint_types v_type> struct to_chars_internal<v_type, 4ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				std::memcpy(buf, char_table_4_digit_data + value, 4ULL);
				return buf + 4;
			}
		};

		template<uint_types v_type> struct to_chars_internal<v_type, 5ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				const v_type a = static_cast<v_type>(static_cast<uint64_t>(value) * 0xd1b71759U >> 45);
				*buf		   = char_table_1_digit_data[a];
				std::memcpy(buf + 1, char_table_4_digit_data + value - a * 10000, 4ULL);
				return buf + 5;
			}
		};

		template<uint_types v_type> struct to_chars_internal<v_type, 6ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				const uint32_t ab = static_cast<uint64_t>(value) * 0xd1b71759U >> 45;
				std::memcpy(buf, char_table_2_digit_data + ab, 2ULL);
				std::memcpy(buf + 2, char_table_4_digit_data + value - (ab * 10000U), 4ULL);
				return buf + 6;
			}
		};

		template<uint_types v_type> struct to_chars_internal<v_type, 7ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				const uint32_t abc = static_cast<uint64_t>(value) * 0xd1b71759U >> 45;
				std::memcpy(buf, char_table_3_digit_data + abc, 3ULL);
				std::memcpy(buf + 3, char_table_4_digit_data + value - (abc * 10000U), 4ULL);
				return buf + 7;
			}
		};

		template<uint_types v_type> struct to_chars_internal<v_type, 8ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				const uint32_t abcd = static_cast<uint64_t>(value) * 0xd1b71759U >> 45;
				std::memcpy(buf, char_table_4_digit_data + abcd, 4ULL);
				std::memcpy(buf + 4, char_table_4_digit_data + value - (abcd * 10000U), 4ULL);
				return buf + 8;
			}
		};

		template<uint_types v_type> struct to_chars_internal<v_type, 9ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				const uint32_t a		= multiply_and_shift<uint32_t, 100000000ULL>::impl(static_cast<uint32_t>(value));
				const uint32_t bcdefghi = static_cast<uint32_t>(value - a * 100000000ULL);
				const uint32_t bcde		= static_cast<uint64_t>(bcdefghi) * 0xd1b71759U >> 45;
				const uint32_t fghi		= bcdefghi - (bcde * 10000U);
				*buf					= char_table_1_digit_data[a];
				std::memcpy(buf + 1, char_table_4_digit_data + bcde, 4ULL);
				std::memcpy(buf + 5, char_table_4_digit_data + fghi, 4ULL);
				return buf + 9;
			}
		};

		template<uint_types v_type> struct to_chars_internal<v_type, 10ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				const uint32_t ab		= static_cast<uint32_t>(multiply_and_shift<v_type, 100000000ULL>::impl(value));
				const uint32_t cdefghij = static_cast<uint32_t>(value - ab * 100000000ULL);
				const uint32_t cdef		= static_cast<uint64_t>(cdefghij) * 0xd1b71759U >> 45;
				const uint32_t ghij		= cdefghij - (cdef * 10000U);
				std::memcpy(buf, char_table_2_digit_data + ab, 2ULL);
				std::memcpy(buf + 2, char_table_4_digit_data + cdef, 4ULL);
				std::memcpy(buf + 6, char_table_4_digit_data + ghij, 4ULL);
				return buf + 10;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 11ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				const uint32_t abc		= static_cast<uint32_t>(multiply_and_shift<v_type, 100000000ULL>::impl(value));
				const uint32_t defghijk = static_cast<uint32_t>(value - abc * 100000000ULL);
				const uint32_t defg		= static_cast<uint64_t>(defghijk) * 0xd1b71759U >> 45;
				const uint32_t hijk		= defghijk - (defg * 10000U);
				std::memcpy(buf, char_table_3_digit_data + abc, 3ULL);
				std::memcpy(buf + 3, char_table_4_digit_data + defg, 4ULL);
				std::memcpy(buf + 7, char_table_4_digit_data + hijk, 4ULL);
				return buf + 11;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 12ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				const uint32_t abcd		= static_cast<uint32_t>(multiply_and_shift<v_type, 100000000ULL>::impl(value));
				const uint32_t efghijkl = static_cast<uint32_t>(value - abcd * 100000000ULL);
				const uint32_t efgh		= static_cast<uint64_t>(efghijkl) * 0xd1b71759U >> 45;
				const uint32_t ijkl		= efghijkl - (efgh * 10000U);
				std::memcpy(buf, char_table_4_digit_data + abcd, 4ULL);
				std::memcpy(buf + 4, char_table_4_digit_data + efgh, 4ULL);
				std::memcpy(buf + 8, char_table_4_digit_data + ijkl, 4ULL);
				return buf + 12;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 13ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				const uint32_t abcde	= static_cast<uint32_t>(multiply_and_shift<v_type, 100000000ULL>::impl(value));
				const uint32_t fghijklm = static_cast<uint32_t>(value - abcde * 100000000ULL);
				const uint32_t a		= static_cast<uint64_t>(abcde) * 0xd1b71759U >> 45;
				const uint32_t bcde		= abcde - (a * 10000U);
				const uint32_t fghi		= static_cast<uint64_t>(fghijklm) * 0xd1b71759U >> 45;
				const uint32_t klm		= fghijklm - (fghi * 10000U);
				*buf					= char_table_1_digit_data[a];
				std::memcpy(buf + 1, char_table_4_digit_data + bcde, 4ULL);
				std::memcpy(buf + 5, char_table_4_digit_data + fghi, 4ULL);
				std::memcpy(buf + 9, char_table_4_digit_data + klm, 4ULL);
				return buf + 13;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 14ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				const uint32_t abcdef	= static_cast<uint32_t>(multiply_and_shift<v_type, 100000000ULL>::impl(value));
				const uint32_t ghijklmn = static_cast<uint32_t>(value - abcdef * 100000000ULL);
				const uint32_t ab		= static_cast<uint64_t>(abcdef) * 0xd1b71759U >> 45;
				const uint32_t cdef		= abcdef - (ab * 10000U);
				const uint32_t ghij		= static_cast<uint64_t>(ghijklmn) * 0xd1b71759U >> 45;
				const uint32_t klmn		= ghijklmn - (ghij * 10000U);
				std::memcpy(buf, char_table_2_digit_data + ab, 2ULL);
				std::memcpy(buf + 2, char_table_4_digit_data + cdef, 4ULL);
				std::memcpy(buf + 6, char_table_4_digit_data + ghij, 4ULL);
				std::memcpy(buf + 10, char_table_4_digit_data + klmn, 4ULL);
				return buf + 14;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 15ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				const uint32_t abcdefg	= static_cast<uint32_t>(multiply_and_shift<v_type, 100000000ULL>::impl(value));
				const uint32_t hijklmno = static_cast<uint32_t>(value - abcdefg * 100000000ULL);
				const uint32_t abc		= static_cast<uint64_t>(abcdefg) * 0xd1b71759U >> 45;
				const uint32_t defg		= abcdefg - (abc * 10000U);
				const uint32_t hijk		= static_cast<uint64_t>(hijklmno) * 0xd1b71759U >> 45;
				const uint32_t lmno		= hijklmno - (hijk * 10000U);
				std::memcpy(buf, char_table_3_digit_data + abc, 3ULL);
				std::memcpy(buf + 3, char_table_4_digit_data + defg, 4ULL);
				std::memcpy(buf + 7, char_table_4_digit_data + hijk, 4ULL);
				std::memcpy(buf + 11, char_table_4_digit_data + lmno, 4ULL);
				return buf + 15;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 16ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				const uint32_t abcdefgh = static_cast<uint32_t>(multiply_and_shift<v_type, 100000000ULL>::impl(value));
				const uint32_t ijklmnop = static_cast<uint32_t>(value - abcdefgh * 100000000ULL);
				const uint32_t abcd		= static_cast<uint64_t>(abcdefgh) * 0xd1b71759U >> 45;
				const uint32_t efgh		= abcdefgh - (abcd * 10000U);
				const uint32_t ijkl		= static_cast<uint64_t>(ijklmnop) * 0xd1b71759U >> 45;
				const uint32_t mnop		= ijklmnop - (ijkl * 10000U);
				std::memcpy(buf, char_table_4_digit_data + abcd, 4ULL);
				std::memcpy(buf + 4, char_table_4_digit_data + efgh, 4ULL);
				std::memcpy(buf + 8, char_table_4_digit_data + ijkl, 4ULL);
				std::memcpy(buf + 12, char_table_4_digit_data + mnop, 4ULL);
				return buf + 16;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 17ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				const v_type abcdefghi	= multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const uint32_t abcdefg	= static_cast<uint32_t>(value - abcdefghi * 100000000ULL);
				const uint32_t a		= static_cast<uint32_t>(multiply_and_shift<v_type, 100000000ULL>::impl(abcdefghi));
				const uint32_t bcdefghi = static_cast<uint32_t>(abcdefghi - a * 100000000ULL);
				const uint32_t bcde		= static_cast<uint64_t>(bcdefghi) * 0xd1b71759U >> 45;
				const uint32_t fghi		= bcdefghi - (bcde * 10000U);
				const uint32_t klm		= static_cast<uint64_t>(abcdefg) * 0xd1b71759U >> 45;
				const uint32_t nopq		= abcdefg - (klm * 10000U);
				*buf					= char_table_1_digit_data[a];
				std::memcpy(buf + 1, char_table_4_digit_data + bcde, 4ULL);
				std::memcpy(buf + 5, char_table_4_digit_data + fghi, 4ULL);
				std::memcpy(buf + 9, char_table_4_digit_data + klm, 4ULL);
				std::memcpy(buf + 13, char_table_4_digit_data + nopq, 4ULL);
				return buf + 17;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 18ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				const v_type abcdefghij = multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const uint32_t klmnopqr = static_cast<uint32_t>(value - abcdefghij * 100000000ULL);
				const uint32_t ab		= static_cast<uint32_t>(multiply_and_shift<v_type, 100000000ULL>::impl(abcdefghij));
				const uint32_t cdefghij = static_cast<uint32_t>(abcdefghij - ab * 100000000ULL);
				const uint32_t cdef		= static_cast<uint64_t>(cdefghij) * 0xd1b71759U >> 45;
				const uint32_t ghij		= cdefghij - (cdef * 10000U);
				const uint32_t klmn		= static_cast<uint64_t>(klmnopqr) * 0xd1b71759U >> 45;
				const uint32_t opqr		= klmnopqr - (klmn * 10000U);
				std::memcpy(buf, char_table_2_digit_data + ab, 2ULL);
				std::memcpy(buf + 2, char_table_4_digit_data + cdef, 4ULL);
				std::memcpy(buf + 6, char_table_4_digit_data + ghij, 4ULL);
				std::memcpy(buf + 10, char_table_4_digit_data + klmn, 4ULL);
				std::memcpy(buf + 14, char_table_4_digit_data + opqr, 4ULL);
				return buf + 18;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 19ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				const v_type abcdefghijk = multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const uint32_t lmnopqrs	 = static_cast<uint32_t>(value - abcdefghijk * 100000000ULL);
				const uint32_t abc		 = static_cast<uint32_t>(multiply_and_shift<v_type, 100000000ULL>::impl(abcdefghijk));
				const uint32_t defghijk	 = static_cast<uint32_t>(abcdefghijk - abc * 100000000ULL);
				const uint32_t defg		 = static_cast<uint64_t>(defghijk) * 0xd1b71759U >> 45;
				const uint32_t hijk		 = defghijk - (defg * 10000U);
				const uint32_t lmno		 = static_cast<uint64_t>(lmnopqrs) * 0xd1b71759U >> 45;
				const uint32_t pqrs		 = lmnopqrs - (lmno * 10000U);
				std::memcpy(buf, char_table_3_digit_data + abc, 3ULL);
				std::memcpy(buf + 3, char_table_4_digit_data + defg, 4ULL);
				std::memcpy(buf + 7, char_table_4_digit_data + hijk, 4ULL);
				std::memcpy(buf + 11, char_table_4_digit_data + lmno, 4ULL);
				std::memcpy(buf + 15, char_table_4_digit_data + pqrs, 4ULL);
				return buf + 19;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 20ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				const v_type abcdefghijkl = multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const uint32_t mnopqrst	  = static_cast<uint32_t>(value - abcdefghijkl * 100000000ULL);
				const uint32_t abcd		  = static_cast<uint32_t>(multiply_and_shift<v_type, 100000000ULL>::impl(abcdefghijkl));
				const uint32_t efghijkl	  = static_cast<uint32_t>(abcdefghijkl - abcd * 100000000ULL);
				const uint32_t efgh		  = static_cast<uint64_t>(efghijkl) * 0xd1b71759U >> 45;
				const uint32_t ijkl		  = efghijkl - (efgh * 10000U);
				const uint32_t mnop		  = static_cast<uint64_t>(mnopqrst) * 0xd1b71759U >> 45;
				const uint32_t qrst		  = mnopqrst - (mnop * 10000U);
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
		};

		template<uint8_types v_type> struct to_chars_impl<v_type> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, char* __restrict end, const v_type value) noexcept {
				if (value < 100U) {
					if (value < 10U) {
						return (end - buf >= 1) ? (*buf = char_table_1_digit_data[value], buf + 1) : end;
					}
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
