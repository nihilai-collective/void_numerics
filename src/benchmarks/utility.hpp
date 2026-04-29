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

	std::string get_winner(const std::vector<bnch_swt::performance_metrics<bnch_swt::benchmark_types::cpu>>& libraries) {
		if (libraries.empty())
			return "None";

		double max_mbps		= -1.0;
		size_t winner_index = 0;
		bool tie			= false;

		for (size_t i = 0; i < libraries.size(); ++i) {
			double current_mbps = libraries[i].throughput_mb_per_sec;

			if (current_mbps > max_mbps) {
				max_mbps	 = current_mbps;
				winner_index = i;
				tie			 = false;
			} else if (std::abs(current_mbps - max_mbps) < 1e-7 && max_mbps > 0) {
				tie = true;
			}
		}

		return tie ? "Tie" : libraries[winner_index].name;
	}

	struct library_win_count {
		uint64_t win_count{};
		std::string name{};
		VN_FORCE_INLINE bool operator>(const library_win_count& other) const {
			return win_count > other.win_count;
		}
	};

	void print_total_win_counts(const std::vector<bnch_swt::performance_metrics<bnch_swt::benchmark_types::cpu>>& results) {
		if (results.empty())
			return;

		std::map<std::string, std::vector<bnch_swt::performance_metrics<bnch_swt::benchmark_types::cpu>>> benchmark_stages;
		for (const auto& metric: results) {
			benchmark_stages[metric.stage_name].push_back(metric);
		}

		std::map<std::string, uint64_t> library_tally;
		for (const auto& [stage_name, metrics]: benchmark_stages) {
			std::string winner = get_winner(metrics);
			library_tally[winner]++;
		}

		std::vector<library_win_count> libraries{};
		for (const auto& [library, win_count]: library_tally) {
			libraries.emplace_back(library_win_count{ win_count, library });
		}

		std::sort(libraries.begin(), libraries.end(), std::greater<library_win_count>{});

		std::cout << "=== TOTAL WINS BY LIBRARY ===\n";
		for (const auto& library: libraries) {
			std::cout << library.name << ": " << library.win_count << (library.win_count == 1 ? " win" : " wins") << "\n";
		}
	}

	template<bnch_swt::string_literal test_name_new, typename test_type_new> struct test_holder {
		static constexpr bnch_swt::string_literal test_name{ test_name_new };
		using test_type = test_type_new;
	};

	template<typename bench, typename test_type, typename test_data_type, typename output_buffer_type> auto execute_test(uint64_t test_size, uint64_t sub_iters,
		test_data_type& test_data, output_buffer_type& output_buffer, bnch_swt::internal::cache_clearer<bnch_swt::benchmark_types::cpu>& cclearer) {
		uint64_t current_index{};
		cclearer.evict_caches();
		bench::template run_benchmark<test_type::test_name, typename test_type::test_type>(test_size, sub_iters, test_data, output_buffer, current_index);
	}

	template<typename bench, typename test_data_type, typename output_buffer_type, typename... test_types>
	auto execute_tests(uint64_t test_size, uint64_t sub_iters, test_data_type& test_data, output_buffer_type& output_buffer) {
		bnch_swt::internal::cache_clearer<bnch_swt::benchmark_types::cpu> cclearer{};
		(execute_test<bench, test_types>(test_size, sub_iters, test_data, output_buffer, cclearer), ...);
	}

	template<bnch_swt::string_literal test_name_new, uint64_t test_size, uint64_t total_iters, uint64_t sub_iters, uint64_t measured_iters, typename correctness_verifier,
		typename test_data_type, typename output_buffer_type, typename... test_types>
	auto run_one_test(test_data_type& test_data, output_buffer_type& output_buffer) {
		static constexpr bnch_swt::string_literal test_name{ test_name_new };
		using bench = bnch_swt::benchmark_stage<test_name, total_iters, measured_iters, bnch_swt::benchmark_types::cpu>;
		bnch_swt::internal::cache_clearer<bnch_swt::benchmark_types::cpu> cclearer{};
		execute_tests<bench, test_data_type, output_buffer_type, test_types...>(test_size, sub_iters, test_data, output_buffer);
		correctness_verifier::impl(test_data, test_name.data());
		bench::print_results(true, false);
		return bench::get_results();
	}

	template<bnch_swt::string_literal test_name, uint64_t test_size, uint64_t total_iters, uint64_t sub_iters, uint64_t measured_iters, vn::detail::integer_types v_type,
		typename correctness_verifier, template<uint64_t, uint64_t, uint64_t, typename, uint64_t, uint64_t, bool> typename digit_generator_type, bool negative,
		typename result_data_type, typename... test_types>
	static void mixed_digit_count(result_data_type& result_data) {
		static constexpr bnch_swt::string_literal name{ test_name + "-mixed-lengths" + "-test-size[" + bnch_swt::internal::to_string_literal<test_size>() + "]" };
		auto test_data = digit_generator_type<test_size, sub_iters, total_iters, v_type, 1, max_digits_v<v_type>, negative>::impl();
		using output_buffer_type = typename digit_generator_type<test_size, sub_iters, total_iters, v_type, 1, max_digits_v<v_type>, negative>::output_data_type;
		static constexpr uint64_t type_size{ digit_generator_type<test_size, sub_iters, total_iters, v_type, 1, max_digits_v<v_type>, negative>::type_size };
		output_buffer_type output_buffer(total_iters * test_size * sub_iters * type_size, 0);
		using test_data_type	 = decltype(test_data);
		result_data += run_one_test<name, test_size, total_iters, sub_iters, measured_iters, correctness_verifier, test_data_type, output_buffer_type, test_types...>(test_data,
			output_buffer);
	}

	template<bnch_swt::string_literal test_name, uint64_t test_size, uint64_t total_iters, uint64_t sub_iters, uint64_t measured_iters, vn::detail::integer_types v_type,
		typename correctness_verifier, template<uint64_t, uint64_t, uint64_t, typename, uint64_t, uint64_t, bool> typename digit_generator_type, typename... test_types>
	struct digit_iterator {
		static auto impl() {
			using bench			   = bnch_swt::benchmark_stage<test_name, total_iters, measured_iters, bnch_swt::benchmark_types::cpu>;
			using result_data_type = decltype(bench::get_results());
			result_data_type result_data{};
			mixed_digit_count<test_name, test_size, total_iters, sub_iters, measured_iters, v_type, correctness_verifier, digit_generator_type, false, result_data_type,
				test_types...>(result_data);
			return result_data;
		}
	};

	template<bnch_swt::string_literal test_name, uint64_t test_size, uint64_t total_iters, uint64_t sub_iters, uint64_t measured_iters, vn::detail::int_types v_type,
		typename correctness_verifier, template<uint64_t, uint64_t, uint64_t, typename, uint64_t, uint64_t, bool> typename digit_generator_type, typename... test_types>
	struct digit_iterator<test_name, test_size, total_iters, sub_iters, measured_iters, v_type, correctness_verifier, digit_generator_type, test_types...> {
		static auto impl() {
			using bench			   = bnch_swt::benchmark_stage<test_name, total_iters, measured_iters, bnch_swt::benchmark_types::cpu>;
			using result_data_type = decltype(bench::get_results());
			result_data_type result_data{};
			mixed_digit_count<test_name + "-negative", test_size, total_iters, sub_iters, measured_iters, v_type, correctness_verifier, digit_generator_type, true,
				result_data_type, test_types...>(result_data);
			mixed_digit_count<test_name + "-positive", test_size, total_iters, sub_iters, measured_iters, v_type, correctness_verifier, digit_generator_type, false,
				result_data_type, test_types...>(result_data);
			return result_data;
		}
	};

	template<bnch_swt::string_literal stage_name, uint64_t test_size, uint64_t total_iters, uint64_t sub_iters, uint64_t measured_iters, typename correctness_verifier,
		template<uint64_t, uint64_t, uint64_t, typename, uint64_t, uint64_t, bool> typename digit_generator_type, typename... test_types>
	auto test_function_impl() {
		auto result_data =
			digit_iterator<stage_name + "-int8", test_size, total_iters, sub_iters, measured_iters, int8_t, correctness_verifier, digit_generator_type, test_types...>::impl();
		auto result_data_final = result_data;
		print_total_win_counts(result_data);
		result_data =
			digit_iterator<stage_name + "-uint8", test_size, total_iters, sub_iters, measured_iters, uint8_t, correctness_verifier, digit_generator_type, test_types...>::impl();
		print_total_win_counts(result_data);
		result_data_final += result_data;
		result_data =
			digit_iterator<stage_name + "-int16", test_size, total_iters, sub_iters, measured_iters, int16_t, correctness_verifier, digit_generator_type, test_types...>::impl();
		print_total_win_counts(result_data);
		result_data_final += result_data;
		result_data =
			digit_iterator<stage_name + "-uint16", test_size, total_iters, sub_iters, measured_iters, uint16_t, correctness_verifier, digit_generator_type, test_types...>::impl();
		print_total_win_counts(result_data);
		result_data_final += result_data;
		result_data =
			digit_iterator<stage_name + "-int32", test_size, total_iters, sub_iters, measured_iters, int32_t, correctness_verifier, digit_generator_type, test_types...>::impl();
		print_total_win_counts(result_data);
		result_data_final += result_data;
		result_data =
			digit_iterator<stage_name + "-uint32", test_size, total_iters, sub_iters, measured_iters, uint32_t, correctness_verifier, digit_generator_type, test_types...>::impl();
		print_total_win_counts(result_data);
		result_data_final += result_data;
		result_data =
			digit_iterator<stage_name + "-int64", test_size, total_iters, sub_iters, measured_iters, int64_t, correctness_verifier, digit_generator_type, test_types...>::impl();
		print_total_win_counts(result_data);
		result_data_final += result_data;
		 result_data =
			digit_iterator<stage_name + "-uint64", test_size, total_iters, sub_iters, measured_iters, uint64_t, correctness_verifier, digit_generator_type, test_types...>::impl();
		result_data_final += result_data;
		print_total_win_counts(result_data);
		print_total_win_counts(result_data_final);
		return result_data_final;
	}

	template<bnch_swt::string_literal stage_name, vn::detail::conversion_classes conversion_class, uint64_t total_iters, uint64_t measured_iters, typename correctness_verifier,
		template<uint64_t, uint64_t, uint64_t, typename, uint64_t, uint64_t, bool> typename digit_generator_type, typename... test_types>
	struct tests {
		static void impl() {
			//auto result_data = test_function_impl<stage_name, 100, total_iters, 10, measured_iters, correctness_verifier, digit_generator_type, test_types...>();
			//result_data += test_function_impl<stage_name, 1000, total_iters, 10, measured_iters, correctness_verifier, digit_generator_type, test_types...>();
			//result_data += test_function_impl<stage_name, 10000, total_iters, 10, measured_iters, correctness_verifier, digit_generator_type, test_types...>();
			//result_data += test_function_impl<stage_name, 100000, total_iters, 10, measured_iters, correctness_verifier, digit_generator_type, test_types...>();
			auto result_data = test_function_impl<stage_name, 1000000, total_iters, 10, measured_iters, correctness_verifier, digit_generator_type, test_types...>();
			print_total_win_counts(result_data);
		}
	};

}
