#include <bnch_swt/index.hpp>
#include <void-numerics>
#include <fmt/format.h>
#include <to_text_from_integer.h>
#include <fmt/compile.h>
#include <charconv>
#include <cstring>
#include <limits>
#include <vector>
#include <memory>
#include <iostream>

namespace i_to_str_tests {

	template<typename T> BNCH_SWT_HOST constexpr T pow10_v(uint32_t n) {
		T r = 1;
		for (uint32_t i = 0; i < n; ++i)
			r *= 10;
		return r;
	}

	template<typename T> BNCH_SWT_HOST constexpr uint32_t max_digits_v() {
		if constexpr (sizeof(T) == 4) {
			return std::is_signed_v<T> ? 10U : 10U;
		} else {
			return std::is_signed_v<T> ? 19U : 20U;
		}
	}

	struct vn_op {
		template<typename T> BNCH_SWT_HOST static char* convert(T v, char* buf) noexcept {
			return const_cast<char*>(vn::to_chars(v, buf));
		}
	};

	struct std_op {
		template<typename T> BNCH_SWT_HOST static char* convert(T v, char* buf) noexcept {
			return std::to_chars(buf, buf + 32, v).ptr;
		}
	};

	struct fmt_format_to_op {
		template<typename T> BNCH_SWT_HOST static char* convert(T v, char* buf) noexcept {
			return fmt::format_to(buf, FMT_COMPILE("{}"), v);
		}
	};

	struct fmt_format_int_op {
		template<typename T> BNCH_SWT_HOST static char* convert(T v, char* buf) noexcept {
			fmt::format_int f{ v };
			std::memcpy(buf, f.data(), f.size());
			return buf + f.size();
		}
	};

	struct jeaiii_op {
		template<typename T> BNCH_SWT_HOST static char* convert(T v, char* buf) noexcept {
			return jeaiii::to_text_from_integer(buf, v);
		}
	};

	template<uint64_t test_size, uint64_t total_iters, typename int_type, auto... extra> struct uniform_random_generator {
		using test_data_type = std::vector<std::vector<int_type>>;
		static test_data_type impl() {
			test_data_type data{};
			data.resize(total_iters);
			for (uint64_t x = 0; x < total_iters; ++x) {
				data[x].resize(test_size);
				for (uint64_t y = 0; y < test_size; ++y) {
					data[x][y] = bnch_swt::random_generator<int_type>::impl();
				}
			}
			return data;
		}
	};

	template<uint64_t test_size, uint64_t total_iters, typename int_type, auto... extra> struct fixed_digit_generator {
		using test_data_type = std::vector<std::vector<int_type>>;
		static_assert(sizeof...(extra) == 1, "fixed_digit_generator requires exactly one extra arg: digit_count");
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
						if (bnch_swt::random_generator<uint32_t>::impl(0, 1) == 1U) {
							signed_val = -signed_val;
						}
						data[x][y] = signed_val;
					} else {
						data[x][y] = static_cast<int_type>(mag);
					}
				}
			}
			return data;
		}
	};

	template<uint64_t test_size, uint64_t total_iters, typename int_type, auto... extra> struct mixed_digit_generator {
		using test_data_type = std::vector<std::vector<int_type>>;
		static test_data_type impl() {
			test_data_type data{};
			data.resize(total_iters);
			using unsigned_t		 = std::make_unsigned_t<int_type>;
			constexpr uint32_t max_d = max_digits_v<int_type>();
			for (uint64_t x = 0; x < total_iters; ++x) {
				data[x].resize(test_size);
				for (uint64_t y = 0; y < test_size; ++y) {
					uint32_t digit_count = bnch_swt::random_generator<uint32_t>::impl(1U, max_d);
					const unsigned_t lo	 = (digit_count == 1U) ? unsigned_t{ 0 } : pow10_v<unsigned_t>(digit_count - 1U);
					const unsigned_t hi =
						(digit_count >= max_d) ? static_cast<unsigned_t>(std::numeric_limits<int_type>::max()) : pow10_v<unsigned_t>(digit_count) - unsigned_t{ 1 };
					unsigned_t mag = bnch_swt::random_generator<unsigned_t>::impl(lo, hi);
					if constexpr (std::is_signed_v<int_type>) {
						int_type signed_val = static_cast<int_type>(mag);
						if (bnch_swt::random_generator<uint32_t>::impl(0, 1) == 1U) {
							signed_val = -signed_val;
						}
						data[x][y] = signed_val;
					} else {
						data[x][y] = static_cast<int_type>(mag);
					}
				}
			}
			return data;
		}
	};

	template<typename Op> struct conversion_benchmark {
		template<typename vector_type, typename output_buffer_type, typename int_type>
		BNCH_SWT_HOST static uint64_t impl(const vector_type& test_data, output_buffer_type& output_buffer, int_type& current_index) {
			uint64_t total_bytes{};
			const auto& values = test_data[current_index];
			char* const begin  = output_buffer.data();
			char* p			   = begin;
			for (auto v: values) {
				p = Op::convert(v, p);
			}
			bnch_swt::do_not_optimize_away(p);
			total_bytes += static_cast<uint64_t>(p - begin);
			++current_index;
			return total_bytes;
		}
	};

	template<uint64_t total_iterations, typename int_type> void verify_correctness(const std::vector<std::vector<int_type>>& test_data, const char* test_label) {
		uint64_t vn_correct{}, vn_incorrect{};
		uint64_t jeaiii_correct{}, jeaiii_incorrect{};
		uint64_t amdn_correct{}, amdn_incorrect{};
		uint64_t fmt_correct{}, fmt_incorrect{};
		int_type first_bad_value{};
		bool found_bad{ false };
		const uint64_t sample_iterations = std::min<uint64_t>(total_iterations, test_data.size());
		for (uint64_t x = 0; x < sample_iterations; ++x) {
			for (auto v: test_data[x]) {
				char buf_std[32]{};
				char buf_vn[32]{};
				char buf_jeaiii[32]{};
				char buf_amdn[32]{};
				char buf_fmt[32]{};
				char* std_end		 = std_op::convert(v, buf_std);
				char* vn_end		 = vn_op::convert(v, buf_vn);
				char* jeaiii_end	 = jeaiii_op::convert(v, buf_jeaiii);
				char* fmt_end		 = fmt_format_to_op::convert(v, buf_fmt);
				const size_t std_len = static_cast<size_t>(std_end - buf_std);
				auto same			 = [&](char* end, char* buf) {
					   return static_cast<size_t>(end - buf) == std_len && std::memcmp(buf, buf_std, std_len) == 0;
				};
				if (same(vn_end, buf_vn))
					++vn_correct;
				else {
					++vn_incorrect;
					if (!found_bad) {
						first_bad_value = v;
						found_bad		= true;
					}
				}
				if (same(jeaiii_end, buf_jeaiii))
					++jeaiii_correct;
				else
					++jeaiii_incorrect;
				if (same(fmt_end, buf_fmt))
					++fmt_correct;
				else
					++fmt_incorrect;
			}
		}
		std::cout << "[" << test_label << "] vn correct: " << vn_correct << " | incorrect: " << vn_incorrect << " | jeaiii incorrect: " << jeaiii_incorrect
				  << " | amdn incorrect: " << amdn_incorrect << " | fmt incorrect: " << fmt_incorrect << std::endl;
		if (vn_incorrect > 0) {
			std::cout << "  FIRST BAD vn VALUE: " << static_cast<int64_t>(first_bad_value) << std::endl;
		}
	}

	template<bnch_swt::string_literal test_name, uint64_t test_size, uint64_t total_iters, uint64_t measured_iters,
		template<uint64_t, uint64_t, typename, auto...> typename data_gen, typename int_type, auto... gen_extra>
	void run_one_test() {
		static constexpr bnch_swt::string_literal name{ test_name + "-" + bnch_swt::internal::to_string_literal<test_size>() };
		auto test_data = data_gen<test_size, total_iters, int_type, gen_extra...>::impl();
		std::vector<char> output_buffer(test_size * 24ULL + 64ULL, '\0');
		using bench = bnch_swt::benchmark_stage<name, total_iters, measured_iters, bnch_swt::benchmark_types::cpu>;
		uint64_t current_index{};
		bench::template run_benchmark<"vn::to_chars", conversion_benchmark<vn_op>>(test_data, output_buffer, current_index);
		current_index = 0;
		bench::template run_benchmark<"std::to_chars", conversion_benchmark<std_op>>(test_data, output_buffer, current_index);
		current_index = 0;
		bench::template run_benchmark<"jeaiii", conversion_benchmark<jeaiii_op>>(test_data, output_buffer, current_index);
		current_index = 0;
		bench::template run_benchmark<"fmt::format_int", conversion_benchmark<fmt_format_int_op>>(test_data, output_buffer, current_index);
		current_index = 0;
		bench::template run_benchmark<"fmt::format_to", conversion_benchmark<fmt_format_to_op>>(test_data, output_buffer, current_index);
		verify_correctness<total_iters>(test_data, name.data());
		bench::print_results(true, true);
		return;
	}

	template<uint64_t test_size, uint64_t total_iterations, uint64_t measured_iterations> void test_uniform_random_all_types() {
		std::cout << "\n--- Uniform random uint32_t across full range ---" << std::endl;
		run_one_test<"u32-uniform", test_size, total_iterations, measured_iterations, uniform_random_generator, uint32_t>();

		std::cout << "\n--- Uniform random int32 across full range ---" << std::endl;
		run_one_test<"i32-uniform", test_size, total_iterations, measured_iterations, uniform_random_generator, int32_t>();

		std::cout << "\n--- Uniform random uint64_t across full range ---" << std::endl;
		run_one_test<"u64-uniform", test_size, total_iterations, measured_iterations, uniform_random_generator, uint64_t>();

		std::cout << "\n--- Uniform random int64 across full range ---" << std::endl;
		run_one_test<"i64-uniform", test_size, total_iterations, measured_iterations, uniform_random_generator, int64_t>();
	}

	template<uint64_t test_size, uint64_t total_iterations, uint64_t measured_iterations> void test_mixed_digit_all_types() {
		std::cout << "\n--- Mixed digit lengths uint32_t (1..10 digits, balanced) ---" << std::endl;
		run_one_test<"u32-mixed-digits", test_size, total_iterations, measured_iterations, mixed_digit_generator, uint32_t>();

		std::cout << "\n--- Mixed digit lengths int32 (signed, balanced) ---" << std::endl;
		run_one_test<"i32-mixed-digits", test_size, total_iterations, measured_iterations, mixed_digit_generator, int32_t>();

		std::cout << "\n--- Mixed digit lengths uint64_t (1..20 digits, balanced) ---" << std::endl;
		run_one_test<"u64-mixed-digits", test_size, total_iterations, measured_iterations, mixed_digit_generator, uint64_t>();

		std::cout << "\n--- Mixed digit lengths int64 (signed, balanced) ---" << std::endl;
		run_one_test<"i64-mixed-digits", test_size, total_iterations, measured_iterations, mixed_digit_generator, int64_t>();
	}

	template<uint64_t test_size, uint64_t total_iterations, uint64_t measured_iterations> void test_functions() {
		std::cout << "\n##### test_size = " << test_size << " #####" << std::endl;
		test_uniform_random_all_types<test_size, total_iterations, measured_iterations>();
		test_mixed_digit_all_types<test_size, total_iterations, measured_iterations>();
	}

	using namespace vn;

	template<conversion_classes, uint64_t total_iterations, uint64_t measured_iterations> struct to_chars_tests;

	template<uint64_t total_iterations, uint64_t measured_iterations> struct to_chars_tests<conversion_classes::i_to_str, total_iterations, measured_iterations> {
		static void impl() {
			std::cout << "\n=== void_numerics::to_chars Benchmark Suite ===" << std::endl;
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