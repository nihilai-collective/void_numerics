#include <bnch_swt/index.hpp>
#include <void-numerics>
#include <charconv>
#include <cstring>
#include <cstdlib>
#include <limits>
#include <vector>
#include <array>
#include <iostream>

namespace str_to_i_tests {

	template<typename T> BNCH_SWT_HOST constexpr T pow10_v(uint32_t n) {
		T r = 1;
		for (uint32_t i = 0; i < n; ++i)
			r *= 10;
		return r;
	}

	template<typename T> BNCH_SWT_HOST constexpr uint32_t max_digits_v() {
		if constexpr (sizeof(T) == 4) {
			return 10U;
		} else {
			return std::is_signed_v<T> ? 19U : 20U;
		}
	}

	struct str_entry {
		std::array<char, 32> buf{};
		uint32_t len{};
	};

	using test_data_type = std::vector<std::vector<str_entry>>;

	template<typename int_type> str_entry make_entry(int_type v) {
		str_entry e{};
		auto [ptr, ec] = std::to_chars(e.buf.data(), e.buf.data() + 32, v);
		*ptr		   = '\0';
		e.len		   = static_cast<uint32_t>(ptr - e.buf.data());
		return e;
	}

	template<uint64_t test_size, uint64_t total_iters, typename int_type, auto...> struct uniform_random_generator {
		static test_data_type impl() {
			test_data_type data{};
			data.resize(total_iters);
			for (uint64_t x = 0; x < total_iters; ++x) {
				data[x].resize(test_size);
				for (uint64_t y = 0; y < test_size; ++y)
					data[x][y] = make_entry(bnch_swt::random_generator<int_type>::impl());
			}
			return data;
		}
	};

	template<uint64_t test_size, uint64_t total_iters, typename int_type, auto... extra> struct fixed_digit_generator {
		static_assert(sizeof...(extra) == 1);
		static constexpr auto extras		  = std::make_tuple(extra...);
		static constexpr uint32_t digit_count = static_cast<uint32_t>(std::get<0>(extras));
		static test_data_type impl() {
			test_data_type data{};
			data.resize(total_iters);
			using unsigned_t	= std::make_unsigned_t<int_type>;
			const unsigned_t lo = (digit_count == 1U) ? unsigned_t{ 0 } : pow10_v<unsigned_t>(digit_count - 1U);
			const unsigned_t hi =
				(digit_count >= max_digits_v<int_type>()) ? static_cast<unsigned_t>(std::numeric_limits<int_type>::max()) : pow10_v<unsigned_t>(digit_count) - unsigned_t{ 1 };
			for (uint64_t x = 0; x < total_iters; ++x) {
				data[x].resize(test_size);
				for (uint64_t y = 0; y < test_size; ++y) {
					unsigned_t mag = bnch_swt::random_generator<unsigned_t>::impl(lo, hi);
					if constexpr (std::is_signed_v<int_type>) {
						int_type signed_val = static_cast<int_type>(mag);
						if (bnch_swt::random_generator<uint32_t>::impl(0, 1) == 1U)
							signed_val = -signed_val;
						data[x][y] = make_entry(signed_val);
					} else {
						data[x][y] = make_entry(static_cast<int_type>(mag));
					}
				}
			}
			return data;
		}
	};

	template<uint64_t test_size, uint64_t total_iters, typename int_type, auto...> struct mixed_digit_generator {
		static test_data_type impl() {
			test_data_type data{};
			data.resize(total_iters);
			using unsigned_t		 = std::make_unsigned_t<int_type>;
			constexpr uint32_t max_d = max_digits_v<int_type>();
			for (uint64_t x = 0; x < total_iters; ++x) {
				data[x].resize(test_size);
				for (uint64_t y = 0; y < test_size; ++y) {
					uint32_t dc	   = bnch_swt::random_generator<uint32_t>::impl(1U, max_d);
					unsigned_t lo  = (dc == 1U) ? unsigned_t{ 0 } : pow10_v<unsigned_t>(dc - 1U);
					unsigned_t hi  = (dc >= max_d) ? static_cast<unsigned_t>(std::numeric_limits<int_type>::max()) : pow10_v<unsigned_t>(dc) - unsigned_t{ 1 };
					unsigned_t mag = bnch_swt::random_generator<unsigned_t>::impl(lo, hi);
					if constexpr (std::is_signed_v<int_type>) {
						int_type signed_val = static_cast<int_type>(mag);
						if (bnch_swt::random_generator<uint32_t>::impl(0, 1) == 1U)
							signed_val = -signed_val;
						data[x][y] = make_entry(signed_val);
					} else {
						data[x][y] = make_entry(static_cast<int_type>(mag));
					}
				}
			}
			return data;
		}
	};

	struct vn_from_bench {
		template<typename vector_type, typename int_type> BNCH_SWT_HOST static uint64_t impl(vector_type& test_data, int_type& current_index) {
			uint64_t total{};
			for (const auto& e: test_data[current_index]) {
				std::remove_cvref_t<int_type> v{};
				auto ptr = e.buf.data();
				vn::from_chars(ptr, e.buf.data() + e.len, v);
				bnch_swt::do_not_optimize_away(v);
				total += e.len;
			}
			++current_index;
			return total;
		}
	};

	struct std_from_bench {
		template<typename vector_type, typename int_type> BNCH_SWT_HOST static uint64_t impl(vector_type& test_data, int_type& current_index) {
			uint64_t total{};
			for (const auto& e: test_data[current_index]) {
				std::remove_cvref_t<int_type> v{};
				std::from_chars(e.buf.data(), e.buf.data() + e.len, v);
				bnch_swt::do_not_optimize_away(v);
				total += e.len;
			}
			++current_index;
			return total;
		}
	};

	struct strtox_bench {
		template<typename vector_type, typename int_type> BNCH_SWT_HOST static uint64_t impl(vector_type& test_data, int_type& current_index) {
			uint64_t total{};
			using value_type = std::remove_cvref_t<int_type>;
			for (const auto& e: test_data[current_index]) {
				value_type v{};
				if constexpr (std::is_same_v<value_type, uint32_t>)
					v = static_cast<uint32_t>(std::strtoul(e.buf.data(), nullptr, 10));
				else if constexpr (std::is_same_v<value_type, int32_t>)
					v = static_cast<int32_t>(std::strtol(e.buf.data(), nullptr, 10));
				else if constexpr (std::is_same_v<value_type, uint64_t>)
					v = static_cast<uint64_t>(std::strtoull(e.buf.data(), nullptr, 10));
				else
					v = static_cast<int64_t>(std::strtoll(e.buf.data(), nullptr, 10));
				bnch_swt::do_not_optimize_away(v);
				total += e.len;
			}
			++current_index;
			return total;
		}
	};

	template<typename int_type> void verify_correctness(const test_data_type& test_data, const char* label, uint64_t sample_iters) {
		uint64_t vn_correct{}, vn_incorrect{}, strtox_incorrect{};
		for (uint64_t x = 0; x < sample_iters && x < test_data.size(); ++x) {
			for (const auto& e: test_data[x]) {
				int_type ref{}, vn_r{};
				std::from_chars(e.buf.data(), e.buf.data() + e.len, ref);
				auto ptr = e.buf.data();
				vn::from_chars(ptr, e.buf.data() + e.len, vn_r);
				int_type strtox_r{};
				if constexpr (std::is_same_v<int_type, uint32_t>)
					strtox_r = static_cast<uint32_t>(std::strtoul(e.buf.data(), nullptr, 10));
				else if constexpr (std::is_same_v<int_type, int32_t>)
					strtox_r = static_cast<int32_t>(std::strtol(e.buf.data(), nullptr, 10));
				else if constexpr (std::is_same_v<int_type, uint64_t>)
					strtox_r = static_cast<uint64_t>(std::strtoull(e.buf.data(), nullptr, 10));
				else
					strtox_r = static_cast<int64_t>(std::strtoll(e.buf.data(), nullptr, 10));
				if (vn_r == ref)
					++vn_correct;
				else
					++vn_incorrect;
				if (strtox_r != ref)
					++strtox_incorrect;
			}
		}
		std::cout << "[" << label << "] vn correct: " << vn_correct << " | vn incorrect: " << vn_incorrect << " | strtox incorrect: " << strtox_incorrect << std::endl;
	}

	template<bnch_swt::string_literal test_name, uint64_t test_size, uint64_t total_iters, uint64_t measured_iters,
		template<uint64_t, uint64_t, typename, auto...> typename data_gen, typename int_type, auto... gen_extra>
	void run_one_test() {
		static constexpr bnch_swt::string_literal name{ test_name + "-" + bnch_swt::internal::to_string_literal<test_size>() };
		auto test_data = data_gen<test_size, total_iters, int_type, gen_extra...>::impl();
		using bench	   = bnch_swt::benchmark_stage<name, total_iters, measured_iters, bnch_swt::benchmark_types::cpu>;
		int_type current_index{};
		bench::template run_benchmark<"vn::from_chars", vn_from_bench>(test_data, current_index);
		current_index = 0;
		bench::template run_benchmark<"std::from_chars", std_from_bench>(test_data, current_index);
		current_index = 0;
		bench::template run_benchmark<"strtox", strtox_bench>(test_data, current_index);
		verify_correctness<int_type>(test_data, name.data(), total_iters);
		bench::print_results(true, true);
	}

	template<uint64_t test_size, uint64_t total_iterations, uint64_t measured_iterations> void test_uniform_random_all_types() {
		std::cout << "\n--- Uniform random uint32_t across full range ---" << std::endl;
		run_one_test<"u32-uniform-from-chars", test_size, total_iterations, measured_iterations, uniform_random_generator, uint32_t>();
		std::cout << "\n--- Uniform random int32_t across full range ---" << std::endl;
		run_one_test<"i32-uniform-from-chars", test_size, total_iterations, measured_iterations, uniform_random_generator, int32_t>();
		std::cout << "\n--- Uniform random uint64_t across full range ---" << std::endl;
		run_one_test<"u64-uniform-from-chars", test_size, total_iterations, measured_iterations, uniform_random_generator, uint64_t>();
		std::cout << "\n--- Uniform random int64_t across full range ---" << std::endl;
		run_one_test<"i64-uniform-from-chars", test_size, total_iterations, measured_iterations, uniform_random_generator, int64_t>();
	}

	template<uint64_t test_size, uint64_t total_iterations, uint64_t measured_iterations> void test_mixed_digit_all_types() {
		std::cout << "\n--- Mixed digit lengths uint32_t ---" << std::endl;
		run_one_test<"u32-mixed-digits-from-chars", test_size, total_iterations, measured_iterations, mixed_digit_generator, uint32_t>();
		std::cout << "\n--- Mixed digit lengths int32_t ---" << std::endl;
		run_one_test<"i32-mixed-digits-from-chars", test_size, total_iterations, measured_iterations, mixed_digit_generator, int32_t>();
		std::cout << "\n--- Mixed digit lengths uint64_t ---" << std::endl;
		run_one_test<"u64-mixed-digits-from-chars", test_size, total_iterations, measured_iterations, mixed_digit_generator, uint64_t>();
		std::cout << "\n--- Mixed digit lengths int64_t ---" << std::endl;
		run_one_test<"i64-mixed-digits-from-chars", test_size, total_iterations, measured_iterations, mixed_digit_generator, int64_t>();
	}

	template<uint64_t test_size, uint64_t total_iterations, uint64_t measured_iterations> void test_functions() {
		std::cout << "\n##### test_size = " << test_size << " #####" << std::endl;
		test_uniform_random_all_types<test_size, total_iterations, measured_iterations>();
		test_mixed_digit_all_types<test_size, total_iterations, measured_iterations>();
	}

	using namespace vn;

	template<conversion_classes, uint64_t total_iterations, uint64_t measured_iterations> struct from_chars_tests;

	template<uint64_t total_iterations, uint64_t measured_iterations> struct from_chars_tests<conversion_classes::str_to_i, total_iterations, measured_iterations> {
		static void impl() {
			std::cout << "\n=== void_numerics::from_chars Benchmark Suite ===" << std::endl;
			test_functions<10, total_iterations, measured_iterations>();
			test_functions<100, total_iterations, measured_iterations>();
			test_functions<1000, total_iterations, measured_iterations>();
			test_functions<10000, total_iterations, measured_iterations>();
			test_functions<100000, total_iterations, measured_iterations>();
			test_functions<1000000, total_iterations, measured_iterations>();
			std::cout << "=== Benchmark Complete ===" << std::endl;
		}
	};

}