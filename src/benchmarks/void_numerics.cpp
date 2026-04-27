// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp

#include "i_to_str.hpp"
#include "str_to_i.hpp"

static constexpr uint64_t total_iterations{ 50 };
static constexpr uint64_t measured_iterations{ 10 };

using namespace vn;

int main() {
	str_to_i_tests::from_chars_tests<conversion_classes::str_to_i, total_iterations, measured_iterations>::impl();
	i_to_str_tests::to_chars_tests<conversion_classes::i_to_str, total_iterations, measured_iterations>::impl();
	return 0;
}