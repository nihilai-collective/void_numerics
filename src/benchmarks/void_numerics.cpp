// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp

#include "i_to_str.hpp"
#include "d_to_str.hpp"
#include "str_to_i.hpp"

#if defined(NDEBUG)
static constexpr uint64_t total_iterations{ 30 };
static constexpr uint64_t measured_iterations{ 10 };
#else
static constexpr uint64_t total_iterations{ 2 };
static constexpr uint64_t measured_iterations{ 1 };
#endif

int32_t main() {
	//benchmarks::double_tests<"floating-point-to-str", vn::detail::conversion_classes::d_to_str, total_iterations, measured_iterations, d_to_str_tests::verify_double_correctness,
	//d_to_str_tests::double_generator, benchmarks::test_holder<"std::to_chars", i_to_str_tests::conversion_benchmark<d_to_str_tests::std_double_op>>,
	//		benchmarks::test_holder<"vn::to_chars", i_to_str_tests::conversion_benchmark<d_to_str_tests::vn_double_op>>,
	//benchmarks::test_holder<"zmij::write", i_to_str_tests::conversion_benchmark<d_to_str_tests::zmij_double_op>>,
	//benchmarks::test_holder<"fmt::format_to", i_to_str_tests::conversion_benchmark<d_to_str_tests::fmt_double_op>>>::impl();
	benchmarks::tests<"int-to-str", vn::detail::conversion_classes::i_to_str, total_iterations, measured_iterations, i_to_str_tests::verify_correctness,
		i_to_str_tests::digit_generator, benchmarks::test_holder<"std::to_chars", i_to_str_tests::conversion_benchmark<i_to_str_tests::std_op>>,
		benchmarks::test_holder<"jeaiii::to_text", i_to_str_tests::conversion_benchmark<i_to_str_tests::jeaiii_op>>,
		benchmarks::test_holder<"fmt::format_to", i_to_str_tests::conversion_benchmark<i_to_str_tests::fmt_format_to_op>>,
		benchmarks::test_holder<"vn::to_chars", i_to_str_tests::conversion_benchmark<i_to_str_tests::vn_op>>>::impl();
	benchmarks::tests<"str-to-int-leading-zeros", vn::detail::conversion_classes::str_to_i, total_iterations, measured_iterations, str_to_i_tests::verify_correctness_leading_zeros,
		str_to_i_tests::leading_zero_string_generator, benchmarks::test_holder<"std::from_chars", str_to_i_tests::from_chars_benchmark<str_to_i_tests::std_from_op>>,
		benchmarks::test_holder<"absl::SimpleAtoi", str_to_i_tests::from_chars_benchmark<str_to_i_tests::absl_from_op>>,
		benchmarks::test_holder<"strtoll/strtoull", str_to_i_tests::from_chars_benchmark<str_to_i_tests::strto_op>>,
		benchmarks::test_holder<"vn::from_chars", str_to_i_tests::from_chars_benchmark<str_to_i_tests::vn_from_op>>>::impl();
	benchmarks::tests<"str-to-int", vn::detail::conversion_classes::str_to_i, total_iterations, measured_iterations, str_to_i_tests::verify_correctness,
		str_to_i_tests::string_generator, benchmarks::test_holder<"std::from_chars", str_to_i_tests::from_chars_benchmark<str_to_i_tests::std_from_op>>,
		benchmarks::test_holder<"absl::SimpleAtoi", str_to_i_tests::from_chars_benchmark<str_to_i_tests::absl_from_op>>,
		benchmarks::test_holder<"strtoll/strtoull", str_to_i_tests::from_chars_benchmark<str_to_i_tests::strto_op>>,
		benchmarks::test_holder<"vn::from_chars", str_to_i_tests::from_chars_benchmark<str_to_i_tests::vn_from_op>>>::impl();
	return 0;
}