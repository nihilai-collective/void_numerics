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
#include "llvm_floating_point_pass.hpp"
#include "charconv.msvc/test.hpp"

int32_t main(int32_t argc, char**argv) {
	mt19937_64 mt64;
	initialize_randomness(mt64, argc, argv);
	all_integer_tests();
	//all_floating_tests(mt64);
	double val{ 3.333e307 };
	std::string string{};
	string.resize(128);
	vn::to_chars(string.data(), string.data() + string.size(), val);
	std::cout << "VALUE: " << string << std::endl;
	unit_tests::tests<vn::detail::conversion_classes::i_to_str>::impl();
	unit_tests::tests<vn::detail::conversion_classes::str_to_i>::impl();
	integral_pass_llvm_tests::test();
	floating_point_pass_llvm_tests::test();
	roundtrip_llvm_tests::test();
	to_chars_llvm_tests::test();
	return 0;
}
