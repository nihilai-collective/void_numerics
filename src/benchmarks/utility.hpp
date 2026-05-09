// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp

#pragma once

#include <void-numerics>
#include <bnch_swt/index.hpp>
#include <fmt/format.h>
#include <to_text_from_integer.h>
#include <fmt/compile.h>
#include <charconv>
#include <cstring>
#include <limits>
#include <vector>
#include <memory>
#include <map>
#include <iostream>

namespace benchmarks {

	template<typename v_type> VN_FORCE_INLINE constexpr v_type pow10(uint32_t n) {
		uint64_t r = 1;
		for (uint32_t i = 0; i < n; ++i)
			r *= 10;
		return static_cast<v_type>(r);
	}

	template<typename v_type, uint64_t min_length, uint64_t max_length> struct random_digits;

	template<vn::detail::uint_types v_type, uint64_t min_length, uint64_t max_length>
		requires(max_length > 0 && min_length > 0)
	struct random_digits<v_type, min_length, max_length> {
	  private:
		static constexpr v_type min_u = pow10<v_type>(min_length - 1);
		static constexpr v_type max_u = pow10<v_type>(max_length) - 1;

	  public:
		VN_FORCE_INLINE static v_type impl() {
			bnch_swt::random_generator<v_type> rg{};
			auto val = rg.impl(min_u, max_u);
			return static_cast<v_type>(val);
		}
	};

	template<vn::detail::int_types v_type, uint64_t min_length, uint64_t max_length>
		requires(max_length > 0 && min_length > 0)
	struct random_digits<v_type, min_length, max_length> {
	  private:
		static constexpr v_type min_u = pow10<v_type>(min_length - 1);
		static constexpr v_type max_u = pow10<v_type>(max_length) - 1;

	  public:
		VN_FORCE_INLINE static v_type impl() {
			bnch_swt::random_generator<v_type> rg{};
			auto val = rg.impl(min_u, max_u);
			return static_cast<v_type>(val);
		}
	};

	template<vn::detail::integer_types v_type> struct max_digits {
	  private:
		using limits = std::numeric_limits<v_type>;
		using U		 = std::make_unsigned_t<v_type>;

		static consteval uint64_t count_digits(U value) {
			uint64_t digits = 0;
			do {
				value /= 10;
				++digits;
			} while (value != 0);
			return digits;
		}

	  public:
		static constexpr uint64_t value = count_digits(static_cast<U>(limits::max()));
	};

	template<vn::detail::integer_types v_type> static constexpr uint64_t max_digits_v = max_digits<v_type>::value;

	template<bnch_swt::string_literal library_name_new, typename test_type_new> struct test_holder {
		static constexpr bnch_swt::string_literal library_name{ library_name_new };
		using test_type = test_type_new;
	};

	template<typename benchmark_stage_type, bnch_swt::string_literal test_name_new, typename test_type, typename test_data_type, typename output_buffer_type> auto execute_test(uint64_t test_size, uint64_t sub_iters,
		test_data_type& test_data, output_buffer_type& output_buffer, bnch_swt::internal::cache_clearer<bnch_swt::benchmark_types::cpu>& cclearer) {
		static constexpr bnch_swt::string_literal test_name{ test_name_new };
		uint64_t current_index{};
		cclearer.evict_caches();
		benchmark_stage_type::template run_benchmark<test_name, test_type::library_name, typename test_type::test_type>(test_size, sub_iters, test_data, output_buffer,
			current_index);
	}

	template<typename benchmark_stage_type, bnch_swt::string_literal test_name_new, typename test_data_type, typename output_buffer_type, typename... test_types>
	auto execute_tests(uint64_t test_size, uint64_t sub_iters, test_data_type& test_data, output_buffer_type& output_buffer) {
		bnch_swt::internal::cache_clearer<bnch_swt::benchmark_types::cpu> cclearer{};
		(execute_test<benchmark_stage_type, test_name_new, test_types>(test_size, sub_iters, test_data, output_buffer, cclearer), ...);
	}

	template<typename benchmark_stage_type, bnch_swt::string_literal test_name_new, uint64_t test_size, uint64_t total_iters, uint64_t sub_iters, uint64_t measured_iters,
		typename correctness_verifier, typename test_data_type, typename output_buffer_type, typename... test_types>
	auto run_one_test(test_data_type& test_data, output_buffer_type& output_buffer) {
		static constexpr bnch_swt::string_literal test_name{ test_name_new };
		bnch_swt::internal::cache_clearer<bnch_swt::benchmark_types::cpu> cclearer{};
		execute_tests<benchmark_stage_type, test_name, test_data_type, output_buffer_type, test_types...>(test_size, sub_iters, test_data, output_buffer);
		correctness_verifier::impl(test_data, test_name.data());
	}

	struct library_win_count {
		uint64_t win_count{};
		std::string name{};

		VN_FORCE_INLINE bool operator>(const library_win_count& other) const {
			return win_count > other.win_count;
		}
	};

	template<typename benchmark_stage_type, bnch_swt::string_literal test_name, uint64_t test_size, uint64_t total_iters, uint64_t sub_iters, uint64_t measured_iters,
		vn::detail::integer_types v_type, typename correctness_verifier, template<uint64_t, uint64_t, uint64_t, typename, uint64_t, uint64_t, bool> typename digit_generator_type,
		bool negative, typename... test_types>
	static void mixed_digit_count() {
		static constexpr bnch_swt::string_literal name{ test_name + "-mixed-lengths" + "-test-size[" + bnch_swt::internal::to_string_literal<test_size>() + "]" };
		auto test_data			 = digit_generator_type<test_size, sub_iters, total_iters, v_type, 1, max_digits_v<v_type>, negative>::impl();
		using output_buffer_type = typename digit_generator_type<test_size, sub_iters, total_iters, v_type, 1, max_digits_v<v_type>, negative>::output_data_type;
		static constexpr uint64_t type_size{ digit_generator_type<test_size, sub_iters, total_iters, v_type, 1, max_digits_v<v_type>, negative>::type_size };
		output_buffer_type output_buffer(total_iters * test_size * sub_iters * type_size, 0);
		using test_data_type = decltype(test_data);
		run_one_test<benchmark_stage_type, name, test_size, total_iters, sub_iters, measured_iters, correctness_verifier, test_data_type, output_buffer_type, test_types...>(
			test_data, output_buffer);
	}

	template<typename benchmark_stage_type, bnch_swt::string_literal test_name, uint64_t test_size, uint64_t total_iters, uint64_t sub_iters, uint64_t measured_iters,
		vn::detail::integer_types v_type, typename correctness_verifier, template<uint64_t, uint64_t, uint64_t, typename, uint64_t, uint64_t, bool> typename digit_generator_type,
		typename... test_types>
	struct digit_iterator {
		static auto impl() {
			if constexpr (vn::detail::int_types<v_type>) {
				mixed_digit_count<benchmark_stage_type, test_name + "-negative", test_size, total_iters, sub_iters, measured_iters, v_type, correctness_verifier,
					digit_generator_type, true, test_types...>();
			}
			mixed_digit_count<benchmark_stage_type, test_name + "-positive", test_size, total_iters, sub_iters, measured_iters, v_type, correctness_verifier, digit_generator_type,
				false, test_types...>();
		}
	};

	template<typename benchmark_stage_type, bnch_swt::string_literal stage_name, uint64_t test_size, uint64_t total_iters, uint64_t sub_iters, uint64_t measured_iters,
		typename correctness_verifier, template<uint64_t, uint64_t, uint64_t, typename, uint64_t, uint64_t, bool> typename digit_generator_type, typename... test_types>
	auto test_function_impl() {
		digit_iterator<benchmark_stage_type, "int8", test_size, total_iters, sub_iters, measured_iters, int8_t, correctness_verifier,
				  digit_generator_type, test_types...>::impl();
		digit_iterator<benchmark_stage_type, "uint8", test_size, total_iters, sub_iters, measured_iters, uint8_t, correctness_verifier,
			digit_generator_type, test_types...>::impl();
		digit_iterator<benchmark_stage_type, "int16", test_size, total_iters, sub_iters, measured_iters, int16_t, correctness_verifier,
			digit_generator_type, test_types...>::impl();
		digit_iterator<benchmark_stage_type, "uint16", test_size, total_iters, sub_iters, measured_iters, uint16_t, correctness_verifier,
			digit_generator_type, test_types...>::impl();
		digit_iterator<benchmark_stage_type, "int32", test_size, total_iters, sub_iters, measured_iters, int32_t, correctness_verifier,
			digit_generator_type, test_types...>::impl();
		digit_iterator<benchmark_stage_type, "uint32", test_size, total_iters, sub_iters, measured_iters, uint32_t, correctness_verifier,
			digit_generator_type, test_types...>::impl();
		digit_iterator<benchmark_stage_type, "int64", test_size, total_iters, sub_iters, measured_iters, int64_t, correctness_verifier,
			digit_generator_type, test_types...>::impl();
		digit_iterator<benchmark_stage_type, "uint64_t", test_size, total_iters, sub_iters, measured_iters, uint64_t, correctness_verifier,
			digit_generator_type, test_types...>::impl();
	}

	template<bnch_swt::string_literal stage_name, vn::detail::conversion_classes conversion_class, uint64_t total_iters, uint64_t measured_iters, typename correctness_verifier,
		template<uint64_t, uint64_t, uint64_t, typename, uint64_t, uint64_t, bool> typename digit_generator_type, typename... test_types>
	struct tests {
		static void impl() {
			using bench_stage_type = bnch_swt::benchmark_stage<stage_name, total_iters, measured_iters, bnch_swt::benchmark_types::cpu>;
			test_function_impl<bench_stage_type, stage_name, 100, total_iters, 10, measured_iters, correctness_verifier, digit_generator_type, test_types...>();
			test_function_impl<bench_stage_type, stage_name, 1000, total_iters, 10, measured_iters, correctness_verifier, digit_generator_type, test_types...>();
			test_function_impl<bench_stage_type, stage_name, 10000, total_iters, 10, measured_iters, correctness_verifier, digit_generator_type, test_types...>();
			test_function_impl<bench_stage_type, stage_name, 100000, total_iters, 10, measured_iters, correctness_verifier, digit_generator_type, test_types...>();
			test_function_impl<bench_stage_type, stage_name, 1000000, total_iters, 10, measured_iters, correctness_verifier, digit_generator_type, test_types...>();
			bench_stage_type::print_results();
		}
	};

	template<typename v_type>
		requires(std::is_floating_point_v<v_type>)
	struct max_exponent {
		static constexpr int64_t max_value = static_cast<int64_t>(std::numeric_limits<v_type>::max_exponent10);
		static constexpr int64_t min_value = static_cast<int64_t>(std::numeric_limits<v_type>::min_exponent10);
	};

	template<typename v_type> static constexpr int64_t max_exponent_v = max_exponent<v_type>::max_value;
	template<typename v_type> static constexpr int64_t min_exponent_v = max_exponent<v_type>::min_value;

	template<typename v_type>
		requires(std::is_floating_point_v<v_type>)
	static constexpr uint64_t max_chars_v = 32;

	template<typename v_type, int64_t min_exp, int64_t max_exp>
		requires(std::is_floating_point_v<v_type> && max_exp > min_exp)
	struct random_double_value {
		VN_FORCE_INLINE static v_type impl() {
			thread_local std::mt19937_64 rng{ std::random_device{}() };
			std::uniform_int_distribution<int64_t> exp_dist{ min_exp, max_exp - 1 };
			std::uniform_real_distribution<v_type> mantissa_dist{ static_cast<v_type>(1.0), static_cast<v_type>(10.0) };
			return static_cast<v_type>(mantissa_dist(rng) * std::pow(static_cast<v_type>(10.0), static_cast<v_type>(exp_dist(rng))));
		}
	};

	template<uint64_t test_size, uint64_t sub_iters, uint64_t total_iters, typename float_type, int64_t min_exp, int64_t max_exp, bool negative>
		requires(std::is_floating_point_v<float_type>)
	struct double_generator {
		using test_data_type   = std::vector<float_type>;
		using output_data_type = std::vector<char>;
		static constexpr uint64_t type_size{ max_chars_v<float_type> };
		static test_data_type impl() {
			test_data_type data{};
			data.resize(test_size * total_iters * sub_iters);
			for (uint64_t x = 0; x < data.size(); ++x) {
				float_type value = random_double_value<float_type, min_exp, max_exp>::impl();
				data[x]			 = negative ? -value : value;
			}
			return data;
		}
	};

	template<typename benchmark_stage_type, bnch_swt::string_literal test_name_new, uint64_t test_size, uint64_t total_iters, uint64_t sub_iters, uint64_t measured_iters,
		typename float_type,
		typename correctness_verifier, template<uint64_t, uint64_t, uint64_t, typename, int64_t, int64_t, bool> typename double_generator_type, bool negative,
		typename... test_types>
		requires(std::is_floating_point_v<float_type>)
	static void mixed_exponent_range() {
		static constexpr int64_t min_exp = min_exponent_v<float_type>;
		static constexpr int64_t max_exp = max_exponent_v<float_type>;
		static constexpr auto test_name{ test_name_new + "-mixed-exponents" + "-test-size[" + bnch_swt::internal::to_string_literal<test_size>() + "]" };
		auto test_data			 = double_generator_type<test_size, sub_iters, total_iters, float_type, min_exp, max_exp, negative>::impl();
		using output_buffer_type = typename double_generator_type<test_size, sub_iters, total_iters, float_type, min_exp, max_exp, negative>::output_data_type;
		static constexpr uint64_t type_size{ double_generator_type<test_size, sub_iters, total_iters, float_type, min_exp, max_exp, negative>::type_size };
		output_buffer_type output_buffer(total_iters * test_size * sub_iters * type_size, 0);
		using test_data_type = decltype(test_data);
		run_one_test<benchmark_stage_type, test_name, test_size, total_iters, sub_iters, measured_iters, correctness_verifier, test_data_type, output_buffer_type, test_types...>(
			test_data, output_buffer);
	}

	template<typename benchmark_stage_type, bnch_swt::string_literal test_name, uint64_t test_size, uint64_t total_iters, uint64_t sub_iters, uint64_t measured_iters,
		typename float_type, typename correctness_verifier, template<uint64_t, uint64_t, uint64_t, typename, int64_t, int64_t, bool> typename double_generator_type,
		typename... test_types>
		requires(std::is_floating_point_v<float_type>)
	struct double_iterator {
		static auto impl() {
			mixed_exponent_range<benchmark_stage_type, test_name + "-negative", test_size, total_iters, sub_iters, measured_iters, float_type, correctness_verifier,
				double_generator_type, true,
				test_types...>();
			mixed_exponent_range<benchmark_stage_type, test_name + "-positive", test_size, total_iters, sub_iters, measured_iters, float_type, correctness_verifier,
				double_generator_type, false,
				test_types...>();
		}
	};

	template<typename benchmark_stage_type, uint64_t test_size, uint64_t total_iters, uint64_t sub_iters, uint64_t measured_iters, typename correctness_verifier,
		template<uint64_t, uint64_t, uint64_t, typename, int64_t, int64_t, bool> typename double_generator_type, typename... test_types>
	auto double_test_function_impl() {
		double_iterator<benchmark_stage_type, "f-to-str", test_size, total_iters, sub_iters, measured_iters, float, correctness_verifier, double_generator_type,
			test_types...>::impl();
		double_iterator<benchmark_stage_type, "d-to-str", test_size, total_iters, sub_iters, measured_iters, double, correctness_verifier, double_generator_type,
			test_types...>::impl();
	}

	template<bnch_swt::string_literal stage_name, vn::detail::conversion_classes conversion_class, uint64_t total_iters, uint64_t measured_iters, typename correctness_verifier,
		template<uint64_t, uint64_t, uint64_t, typename, int64_t, int64_t, bool> typename double_generator_type, typename... test_types>
	struct double_tests {
		static void impl() {
			using bench_stage_type			   = bnch_swt::benchmark_stage<stage_name, total_iters, measured_iters, bnch_swt::benchmark_types::cpu>;
			double_test_function_impl<bench_stage_type, 100, total_iters, 10, measured_iters, correctness_verifier, double_generator_type, test_types...>();
			double_test_function_impl<bench_stage_type, 1000, total_iters, 10, measured_iters, correctness_verifier, double_generator_type, test_types...>();
			double_test_function_impl<bench_stage_type, 10000, total_iters, 10, measured_iters, correctness_verifier, double_generator_type, test_types...>();
			double_test_function_impl<bench_stage_type, 100000, total_iters, 10, measured_iters, correctness_verifier, double_generator_type, test_types...>();
			double_test_function_impl<bench_stage_type, 1000000, total_iters, 10, measured_iters, correctness_verifier, double_generator_type, test_types...>();
			bench_stage_type::print_results();
		}
	};

}
