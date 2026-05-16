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

int32_t main([[maybe_unused]] int32_t argc, [[maybe_unused]] char** argv) {
	unit_tests::tests<vn::detail::conversion_classes::i_to_str>::impl();
	unit_tests::tests<vn::detail::conversion_classes::str_to_i>::impl();
	integral_pass_llvm_tests::test();
	roundtrip_llvm_tests::test();
	to_chars_llvm_tests::test();
	return 0;
}
