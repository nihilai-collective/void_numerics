
#pragma once

#include <vn-incl/utility.hpp>

namespace vn {

	namespace detail {

		template<typename v_type, uint64_t digit_length> struct to_chars_internal;

		template<typename v_type> struct to_chars_impl;

		template<uint16_types v_type> struct to_chars_internal<v_type, 5ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_1_digit_data = int_tables<1>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				const v_type a																 = static_cast<next_higher_int_t<v_type>>(value) * 53688 >> 29;
				*buf																		 = char_table_1_digit_data[a];
				std::memcpy(buf + 1, char_table_4_digit_data + value - a * 10000, 4ULL);
				return buf + 5;
			}
		};

		template<uint32_types v_type> struct to_chars_internal<v_type, 5ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_1_digit_data = int_tables<1>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				const v_type a																 = static_cast<next_higher_int_t<v_type>>(value) * 3518437209U >> 45;
				*buf																		 = char_table_1_digit_data[a];
				std::memcpy(buf + 1, char_table_4_digit_data + value - a * 10000U, 4ULL);
				return buf + 5;
			}
		};

		template<uint32_types v_type> struct to_chars_internal<v_type, 6ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_2_digit_data = int_tables<2>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				const v_type ab																 = static_cast<next_higher_int_t<v_type>>(value) * 3518437209U >> 45;
				std::memcpy(buf, char_table_2_digit_data + ab, 2ULL);
				std::memcpy(buf + 2, char_table_4_digit_data + value - (ab * 10000U), 4ULL);
				return buf + 6;
			}
		};

		template<uint32_types v_type> struct to_chars_internal<v_type, 7ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_3_digit_data = int_tables<3>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				const v_type abc															 = static_cast<next_higher_int_t<v_type>>(value) * 3518437209U >> 45;
				std::memcpy(buf, char_table_3_digit_data + abc, 3ULL);
				std::memcpy(buf + 3, char_table_4_digit_data + value - (abc * 10000U), 4ULL);
				return buf + 7;
			}
		};

		template<uint32_types v_type> struct to_chars_internal<v_type, 8ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				const v_type abcd															 = static_cast<next_higher_int_t<v_type>>(value) * 3518437209U >> 45;
				std::memcpy(buf, char_table_4_digit_data + abcd, 4ULL);
				std::memcpy(buf + 4, char_table_4_digit_data + value - (abcd * 10000U), 4ULL);
				return buf + 8;
			}
		};

		template<uint32_types v_type> struct to_chars_internal<v_type, 9ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_1_digit_data = int_tables<1>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				const v_type a																 = static_cast<next_higher_int_t<v_type>>(value) * 2882303762U >> 58;
				const v_type bcdefghi														 = value - a * 100000000ULL;
				const v_type bcde															 = static_cast<next_higher_int_t<v_type>>(bcdefghi) * 3518437209U >> 45;
				const v_type fghi															 = bcdefghi - (bcde * 10000U);
				*buf																		 = char_table_1_digit_data[a];
				std::memcpy(buf + 1, char_table_4_digit_data + bcde, 4ULL);
				std::memcpy(buf + 5, char_table_4_digit_data + fghi, 4ULL);
				return buf + 9;
			}
		};

		template<uint32_types v_type> struct to_chars_internal<v_type, 10ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_2_digit_data = int_tables<2>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				const v_type ab																 = static_cast<next_higher_int_t<v_type>>(value) * 2882303762U >> 58;
				const v_type cdefghij														 = value - ab * 100000000ULL;
				const v_type cdef															 = static_cast<next_higher_int_t<v_type>>(cdefghij) * 3518437209U >> 45;
				const v_type ghij															 = cdefghij - (cdef * 10000U);
				std::memcpy(buf, char_table_2_digit_data + ab, 2ULL);
				std::memcpy(buf + 2, char_table_4_digit_data + cdef, 4ULL);
				std::memcpy(buf + 6, char_table_4_digit_data + ghij, 4ULL);
				return buf + 10;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 5ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_1_digit_data = int_tables<1>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				const v_type a																 = value * 0xd1b71759U >> 45;
				*buf																		 = char_table_1_digit_data[a];
				std::memcpy(buf + 1, char_table_4_digit_data + value - a * 10000, 4ULL);
				return buf + 5;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 6ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_2_digit_data = int_tables<2>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				const v_type ab																 = value * 0xd1b71759U >> 45;
				std::memcpy(buf, char_table_2_digit_data + ab, 2ULL);
				std::memcpy(buf + 2, char_table_4_digit_data + value - (ab * 10000U), 4ULL);
				return buf + 6;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 7ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_3_digit_data = int_tables<3>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				const v_type abc															 = value * 0xd1b71759U >> 45;
				std::memcpy(buf, char_table_3_digit_data + abc, 3ULL);
				std::memcpy(buf + 3, char_table_4_digit_data + value - (abc * 10000U), 4ULL);
				return buf + 7;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 8ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				const v_type abcd															 = value * 0xd1b71759U >> 45;
				std::memcpy(buf, char_table_4_digit_data + abcd, 4ULL);
				std::memcpy(buf + 4, char_table_4_digit_data + value - (abcd * 10000U), 4ULL);
				return buf + 8;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 9ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_1_digit_data = int_tables<1>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				const v_type a																 = multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const v_type bcdefghi														 = value - a * 100000000ULL;
				const v_type bcde															 = bcdefghi * 0xd1b71759U >> 45;
				const v_type fghi															 = bcdefghi - (bcde * 10000U);
				*buf																		 = char_table_1_digit_data[a];
				std::memcpy(buf + 1, char_table_4_digit_data + bcde, 4ULL);
				std::memcpy(buf + 5, char_table_4_digit_data + fghi, 4ULL);
				return buf + 9;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 10ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_2_digit_data = int_tables<2>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				const v_type ab																 = multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const v_type cdefghij														 = value - ab * 100000000ULL;
				const v_type cdef															 = cdefghij * 0xd1b71759U >> 45;
				const v_type ghij															 = cdefghij - (cdef * 10000U);
				std::memcpy(buf, char_table_2_digit_data + ab, 2ULL);
				std::memcpy(buf + 2, char_table_4_digit_data + cdef, 4ULL);
				std::memcpy(buf + 6, char_table_4_digit_data + ghij, 4ULL);
				return buf + 10;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 11ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_3_digit_data = int_tables<3>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				const v_type abc															 = multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const v_type defghijk														 = value - abc * 100000000ULL;
				const v_type defg															 = defghijk * 0xd1b71759U >> 45;
				const v_type hijk															 = defghijk - (defg * 10000U);
				std::memcpy(buf, char_table_3_digit_data + abc, 3ULL);
				std::memcpy(buf + 3, char_table_4_digit_data + defg, 4ULL);
				std::memcpy(buf + 7, char_table_4_digit_data + hijk, 4ULL);
				return buf + 11;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 12ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				const v_type abcd															 = multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const v_type efghijkl														 = value - abcd * 100000000ULL;
				const v_type efgh															 = efghijkl * 0xd1b71759U >> 45;
				const v_type ijkl															 = efghijkl - (efgh * 10000U);
				std::memcpy(buf, char_table_4_digit_data + abcd, 4ULL);
				std::memcpy(buf + 4, char_table_4_digit_data + efgh, 4ULL);
				std::memcpy(buf + 8, char_table_4_digit_data + ijkl, 4ULL);
				return buf + 12;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 13ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_1_digit_data = int_tables<1>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				const v_type abcde															 = multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const v_type fghijklm														 = value - abcde * 100000000ULL;
				const v_type a																 = abcde * 0xd1b71759U >> 45;
				const v_type bcde															 = abcde - (a * 10000U);
				const v_type fghi															 = fghijklm * 0xd1b71759U >> 45;
				const v_type jklm															 = fghijklm - (fghi * 10000U);
				*buf																		 = char_table_1_digit_data[a];
				std::memcpy(buf + 1, char_table_4_digit_data + bcde, 4ULL);
				std::memcpy(buf + 5, char_table_4_digit_data + fghi, 4ULL);
				std::memcpy(buf + 9, char_table_4_digit_data + jklm, 4ULL);
				return buf + 13;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 14ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_2_digit_data = int_tables<2>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				const v_type abcdef															 = multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const v_type ghijklmn														 = value - abcdef * 100000000ULL;
				const v_type ab																 = abcdef * 0xd1b71759U >> 45;
				const v_type cdef															 = abcdef - (ab * 10000U);
				const v_type ghij															 = ghijklmn * 0xd1b71759U >> 45;
				const v_type klmn															 = ghijklmn - (ghij * 10000U);
				std::memcpy(buf, char_table_2_digit_data + ab, 2ULL);
				std::memcpy(buf + 2, char_table_4_digit_data + cdef, 4ULL);
				std::memcpy(buf + 6, char_table_4_digit_data + ghij, 4ULL);
				std::memcpy(buf + 10, char_table_4_digit_data + klmn, 4ULL);
				return buf + 14;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 15ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_3_digit_data = int_tables<3>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				const v_type abcdefg														 = multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const v_type hijklmno														 = value - abcdefg * 100000000ULL;
				const v_type abc															 = abcdefg * 0xd1b71759U >> 45;
				const v_type defg															 = abcdefg - (abc * 10000U);
				const v_type hijk															 = hijklmno * 0xd1b71759U >> 45;
				const v_type lmno															 = hijklmno - (hijk * 10000U);
				std::memcpy(buf, char_table_3_digit_data + abc, 3ULL);
				std::memcpy(buf + 3, char_table_4_digit_data + defg, 4ULL);
				std::memcpy(buf + 7, char_table_4_digit_data + hijk, 4ULL);
				std::memcpy(buf + 11, char_table_4_digit_data + lmno, 4ULL);
				return buf + 15;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 16ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				const v_type abcdefgh														 = multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const v_type ijklmnop														 = value - abcdefgh * 100000000ULL;
				const v_type abcd															 = abcdefgh * 0xd1b71759U >> 45;
				const v_type efgh															 = abcdefgh - (abcd * 10000U);
				const v_type ijkl															 = ijklmnop * 0xd1b71759U >> 45;
				const v_type mnop															 = ijklmnop - (ijkl * 10000U);
				std::memcpy(buf, char_table_4_digit_data + abcd, 4ULL);
				std::memcpy(buf + 4, char_table_4_digit_data + efgh, 4ULL);
				std::memcpy(buf + 8, char_table_4_digit_data + ijkl, 4ULL);
				std::memcpy(buf + 12, char_table_4_digit_data + mnop, 4ULL);
				return buf + 16;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 17ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_1_digit_data = int_tables<1>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				const v_type abcdefghi														 = multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const v_type jklmnopq														 = value - abcdefghi * 100000000ULL;
				const v_type a																 = multiply_and_shift<v_type, 100000000ULL>::impl(abcdefghi);
				const v_type bcdefghi														 = abcdefghi - a * 100000000ULL;
				const v_type bcde															 = bcdefghi * 0xd1b71759U >> 45;
				const v_type fghi															 = bcdefghi - (bcde * 10000U);
				const v_type jklm															 = jklmnopq * 0xd1b71759U >> 45;
				const v_type nopq															 = jklmnopq - (jklm * 10000U);
				*buf																		 = char_table_1_digit_data[a];
				std::memcpy(buf + 1, char_table_4_digit_data + bcde, 4ULL);
				std::memcpy(buf + 5, char_table_4_digit_data + fghi, 4ULL);
				std::memcpy(buf + 9, char_table_4_digit_data + jklm, 4ULL);
				std::memcpy(buf + 13, char_table_4_digit_data + nopq, 4ULL);
				return buf + 17;
			}
		};

		template<uint64_types v_type> struct to_chars_internal<v_type, 18ULL> {
			inline static char* impl(char* __restrict buf, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_2_digit_data = int_tables<2>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				const v_type abcdefghij														 = multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const v_type klmnopqr														 = value - abcdefghij * 100000000ULL;
				const v_type ab																 = multiply_and_shift<v_type, 100000000ULL>::impl(abcdefghij);
				const v_type cdefghij														 = abcdefghij - ab * 100000000ULL;
				const v_type cdef															 = cdefghij * 0xd1b71759U >> 45;
				const v_type ghij															 = cdefghij - (cdef * 10000U);
				const v_type klmn															 = klmnopqr * 0xd1b71759U >> 45;
				const v_type opqr															 = klmnopqr - (klmn * 10000U);
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
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_3_digit_data = int_tables<3>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				const v_type abcdefghijk													 = multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const v_type lmnopqrs														 = value - abcdefghijk * 100000000ULL;
				const v_type abc															 = multiply_and_shift<v_type, 100000000ULL>::impl(abcdefghijk);
				const v_type defghijk														 = abcdefghijk - abc * 100000000ULL;
				const v_type defg															 = defghijk * 0xd1b71759U >> 45;
				const v_type hijk															 = defghijk - (defg * 10000U);
				const v_type lmno															 = lmnopqrs * 0xd1b71759U >> 45;
				const v_type pqrs															 = lmnopqrs - (lmno * 10000U);
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
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				const v_type abcdefghijkl													 = multiply_and_shift<v_type, 100000000ULL>::impl(value);
				const v_type mnopqrst														 = value - abcdefghijkl * 100000000ULL;
				const v_type abcd															 = multiply_and_shift<v_type, 100000000ULL>::impl(abcdefghijkl);
				const v_type efghijkl														 = abcdefghijkl - abcd * 100000000ULL;
				const v_type efgh															 = efghijkl * 0xd1b71759U >> 45;
				const v_type ijkl															 = efghijkl - (efgh * 10000U);
				const v_type mnop															 = mnopqrst * 0xd1b71759U >> 45;
				const v_type qrst															 = mnopqrst - (mnop * 10000U);
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
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_1_digit_data = int_tables<1>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_2_digit_data = int_tables<2>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_3_digit_data = int_tables<3>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				if VN_LIKELY (end - buf >= 20) {
					if (value < 10000000000ULL) {
						if (value < 100000ULL) {
							if (value < 1000ULL) {
								if (value < 100U) {
									if (value < 10U) {
										return (*buf = char_table_1_digit_data[value], buf + 1);
									}
									return (std::memcpy(buf, char_table_2_digit_data + value, 2ULL), buf + 2);
								}
								return (std::memcpy(buf, char_table_3_digit_data + value, 3ULL), buf + 3);
							}
							if (value < 10000ULL) {
								return (std::memcpy(buf, char_table_4_digit_data + value, 4ULL), buf + 4);
							}
							return to_chars_internal<v_type, 5ULL>::impl(buf, value);
						}
						if (value < 10000000ULL) {
							if (value < 1000000ULL) {
								return to_chars_internal<v_type, 6ULL>::impl(buf, value);
							}
							return to_chars_internal<v_type, 7ULL>::impl(buf, value);
						}
						if (value < 100000000ULL) {
							return to_chars_internal<v_type, 8ULL>::impl(buf, value);
						}
						if (value < 1000000000ULL) {
							return to_chars_internal<v_type, 9ULL>::impl(buf, value);
						}
						return to_chars_internal<v_type, 10ULL>::impl(buf, value);
					} else {
						if (value < 1000000000000000ULL) {
							if (value < 1000000000000ULL) {
								if (value < 100000000000ULL) {
									return to_chars_internal<v_type, 11ULL>::impl(buf, value);
								}
								return to_chars_internal<v_type, 12ULL>::impl(buf, value);
							}
							if (value < 10000000000000ULL) {
								return to_chars_internal<v_type, 13ULL>::impl(buf, value);
							}
							if (value < 100000000000000ULL) {
								return to_chars_internal<v_type, 14ULL>::impl(buf, value);
							}
							return to_chars_internal<v_type, 15ULL>::impl(buf, value);
						}
						if (value < 100000000000000000ULL) {
							if (value < 10000000000000000ULL) {
								return to_chars_internal<v_type, 16ULL>::impl(buf, value);
							}
							return to_chars_internal<v_type, 17ULL>::impl(buf, value);
						}
						if (value < 1000000000000000000ULL) {
							return to_chars_internal<v_type, 18ULL>::impl(buf, value);
						}
						if (value < 10000000000000000000ULL) {
							return to_chars_internal<v_type, 19ULL>::impl(buf, value);
						}
						return to_chars_internal<v_type, 20ULL>::impl(buf, value);
					}
				}
				return end;
			}
		};

		template<uint32_types v_type> struct to_chars_impl<v_type> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, char* __restrict end, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_1_digit_data = int_tables<1>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_2_digit_data = int_tables<2>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_3_digit_data = int_tables<3>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				if VN_LIKELY (end - buf >= 10) {
					if (value < 1000000000U) {
						if (value < 100000U) {
							if (value < 10000U) {
								if (value < 1000U) {
									if (value < 100U) {
										if (value < 10U) {
											return (*buf = char_table_1_digit_data[value], buf + 1);
										}
										return (std::memcpy(buf, char_table_2_digit_data + value, 2ULL), buf + 2);
									}
									return (std::memcpy(buf, char_table_3_digit_data + value, 3ULL), buf + 3);
								}
								return (std::memcpy(buf, char_table_4_digit_data + value, 4ULL), buf + 4);
							}
							return to_chars_internal<v_type, 5ULL>::impl(buf, value);
						}
						if (value < 10000000U) {
							if (value < 1000000U) {
								return to_chars_internal<v_type, 6ULL>::impl(buf, value);
							}
							return to_chars_internal<v_type, 7ULL>::impl(buf, value);
						}
						if (value < 100000000U) {
							return to_chars_internal<v_type, 8ULL>::impl(buf, value);
						}
						return to_chars_internal<v_type, 9ULL>::impl(buf, value);
					}
					return to_chars_internal<v_type, 10ULL>::impl(buf, value);
				}
				return end;
			}
		};

		template<uint16_types v_type> struct to_chars_impl<v_type> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, char* __restrict end, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_1_digit_data = int_tables<1>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_2_digit_data = int_tables<2>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_3_digit_data = int_tables<3>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_4_digit_data = int_tables<4>::values;
				if VN_LIKELY (end - buf >= 5) {
					if (value < 10000U) {
						if (value < 1000U) {
							if (value < 100U) {
								if (value < 10U) {
									return (*buf = char_table_1_digit_data[value], buf + 1);
								}
								return (std::memcpy(buf, char_table_2_digit_data + value, 2ULL), buf + 2);
							}
							return (std::memcpy(buf, char_table_3_digit_data + value, 3ULL), buf + 3);
						}
						return (std::memcpy(buf, char_table_4_digit_data + value, 4ULL), buf + 4);
					}
					return to_chars_internal<v_type, 5ULL>::impl(buf, value);
				}
				return end;
			}
		};

		template<uint8_types v_type> struct to_chars_impl<v_type> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, char* __restrict end, const v_type value) noexcept {
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_1_digit_data = int_tables<1>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_2_digit_data = int_tables<2>::values;
				VN_ALIGN(64) static constexpr const auto* __restrict char_table_3_digit_data = int_tables<3>::values;
				if VN_LIKELY (end - buf >= 3) {
					if (value < 100U) {
						if (value < 10U) {
							return (*buf = char_table_1_digit_data[value], buf + 1);
						}
						return (std::memcpy(buf, char_table_2_digit_data + value, 2ULL), buf + 2);
					}
					return (std::memcpy(buf, char_table_3_digit_data + value, 3ULL), buf + 3);
				}
				return end;
			}
		};

		template<int_types v_type> struct to_chars_impl<v_type> {
			VN_FORCE_INLINE static char* impl(char* __restrict buf, char* __restrict end, const v_type value) noexcept {
				using unsigned_type = std::make_unsigned_t<v_type>;
				return (value < 0) ? (static_cast<void>(*buf = '-'), ++buf, to_chars_impl<unsigned_type>::impl(buf, end, static_cast<unsigned_type>(value * -1)))
								   : to_chars_impl<unsigned_type>::impl(buf, end, static_cast<unsigned_type>(value));
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
