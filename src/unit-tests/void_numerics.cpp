// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp

#include "i_to_str.hpp"
#include "str_to_i.hpp"

int main() {
	to_chars_tests<conversion_classes::i_to_str>::impl();
	from_chars_tests<conversion_classes::str_to_i>::impl();
	return 0;
}
