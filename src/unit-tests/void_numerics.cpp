// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp

#include <charconv>
#include <system_error>
#include <rt-ut-incl/index.hpp>
#include "i_to_str.hpp"
#include "str_to_i.hpp"
#include "test_macros.hpp"
#include "charconv_test_helpers.hpp"
#include "llvm_to_chars.hpp"
#include "llvm_roundtrip.hpp"
#include "llvm_integral_pass.hpp"

int main(int, char**) {
	std::cout << "ZEROS: " << vn::detail::count_trailing_decimal_zeros(1000) << std::endl;
	std::cout << "ZEROS: " << vn::detail::count_trailing_decimal_zeros(100000) << std::endl;
	std::cout << "ZEROS: " << vn::detail::count_trailing_decimal_zeros(1000000000000000) << std::endl;
	std::cout << "ZEROS: " << vn::detail::count_trailing_decimal_zeros(10000000000000000000ULL) << std::endl;
	std::cout << "ZEROS: " << vn::detail::count_trailing_decimal_zeros(1000000000000000000) << std::endl;
	std::cout << "ZEROS: " << vn::detail::count_trailing_decimal_zeros(100000000000000000) << std::endl;
	std::cout << "ZEROS: " << vn::detail::count_trailing_decimal_zeros(10000000000000000) << std::endl;
	for (uint64_t x = 0; x < 4; ++x) {
		std::cout << "{" << std::hex << vn::detail::fast_div_table<uint16_t>::values[x].inv_odd << ", " << std::dec << vn::detail::fast_div_table<uint16_t>::values[x].max_quotient
				  << "," << vn::detail::fast_div_table<uint16_t>::values[x].even_mask << "}," << std::endl;
	}
	unit_tests::tests<vn::detail::conversion_classes::i_to_str>::impl();
	unit_tests::tests<vn::detail::conversion_classes::str_to_i>::impl();
	integral_pass_llvm_tests::test();
	roundtrip_llvm_tests::test();
	to_chars_llvm_tests::test();
	return 0;
}
