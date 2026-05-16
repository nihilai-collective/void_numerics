// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp

#pragma once

#include "utility.hpp"

namespace i_to_str_tests {

	template<vn::detail::uint16_types v_type> VN_FORCE_INLINE char* jeaiii_to_text_checked(char* buf, char* end, v_type value) noexcept {
		if (value < 10000U) {
			if (value < 1000U) {
				if (value < 100U) {
					if (value < 10U) {
						if (end - buf < 1) {
							return end;
						}
						return jeaiii::to_text(buf, value);
					}
					if (end - buf < 2)
						return end;
					return jeaiii::to_text(buf, value);
				}
				if (end - buf < 3)
					return end;
				return jeaiii::to_text(buf, value);
			}
			if (end - buf < 4)
				return end;
			return jeaiii::to_text(buf, value);
		}
		if (end - buf < 5)
			return end;
		return jeaiii::to_text(buf, value);
	}

	template<vn::detail::int16_types v_type> VN_FORCE_INLINE char* jeaiii_to_text_checked(char* buf, char* end, v_type value) noexcept {
		*buf		  = '-';
		uint16_t uval = (static_cast<uint16_t>(value) ^ static_cast<uint16_t>(value >> 15)) - static_cast<uint16_t>(value >> 15);
		return jeaiii_to_text_checked(buf + (value < 0), end, uval);
	}

	template<vn::detail::uint32_types v_type> VN_FORCE_INLINE char* jeaiii_to_text_checked(char* buf, char* end, v_type value) noexcept {
		if (value < 1000000000U) {
			if (value < 100000U) {
				if (value < 10000U) {
					if (value < 1000U) {
						if (value < 100U) {
							if (value < 10U) {
								if (end - buf < 1) {
									return end;
								}
								return jeaiii::to_text(buf, value);
							}
							if (end - buf < 2)
								return end;
							return jeaiii::to_text(buf, value);
						}
						if (end - buf < 3)
							return end;
						return jeaiii::to_text(buf, value);
					}
					if (end - buf < 4)
						return end;
					return jeaiii::to_text(buf, value);
				}
				if (end - buf < 5)
					return end;
				return jeaiii::to_text(buf, value);
			}
			if (value < 10000000U) {
				if (value < 1000000U) {
					if (end - buf < 6) {
						return end;
					}
					return jeaiii::to_text(buf, value);
				}
				if (end - buf < 7) {
					return end;
				}
				return jeaiii::to_text(buf, value);
			}
			if (value < 100000000U) {
				if (end - buf < 8) {
					return end;
				}
				return jeaiii::to_text(buf, value);
			}
			if (end - buf < 9) {
				return end;
			}
			return jeaiii::to_text(buf, value);
		}
		if (end - buf < 10) {
			return end;
		}
		return jeaiii::to_text(buf, value);
	}

	template<vn::detail::int32_types v_type> VN_FORCE_INLINE char* jeaiii_to_text_checked(char* buf, char* end, v_type value) noexcept {
		*buf		  = '-';
		uint32_t uval = (static_cast<uint32_t>(value) ^ static_cast<uint32_t>(value >> 31)) - static_cast<uint32_t>(value >> 31);
		return jeaiii_to_text_checked(buf + (value < 0), end, uval);
	}

	template<vn::detail::uint64_types v_type> VN_FORCE_INLINE char* jeaiii_to_text_checked(char* buf, char* end, v_type value) noexcept {
		if (value < 10000000000ULL) {
			if (value < 100000ULL) {
				if (value < 1000ULL) {
					if (value < 100ULL) {
						if (value < 10ULL) {
							if (end - buf < 1)
								return end;
							return jeaiii::to_text(buf, value);
						}
						if (end - buf < 2)
							return end;
						return jeaiii::to_text(buf, value);
					}
					if (end - buf < 3)
						return end;
					return jeaiii::to_text(buf, value);
				}
				if (value < 10000ULL) {
					if (end - buf < 4)
						return end;
					return jeaiii::to_text(buf, value);
				}
				if (end - buf < 5)
					return end;
				return jeaiii::to_text(buf, value);
			}
			if (value < 10000000ULL) {
				if (value < 1000000ULL) {
					if (end - buf < 6)
						return end;
					return jeaiii::to_text(buf, value);
				}
				if (end - buf < 7)
					return end;
				return jeaiii::to_text(buf, value);
			}
			if (value < 100000000ULL) {
				if (end - buf < 8)
					return end;
				return jeaiii::to_text(buf, value);
			}
			if (value < 1000000000ULL) {
				if (end - buf < 9)
					return end;
				return jeaiii::to_text(buf, value);
			}
			if (end - buf < 10)
				return end;
			return jeaiii::to_text(buf, value);
		} else {
			if (value < 1000000000000000ULL) {
				if (value < 1000000000000ULL) {
					if (value < 100000000000ULL) {
						if (end - buf < 11)
							return end;
						return jeaiii::to_text(buf, value);
					}
					if (end - buf < 12)
						return end;
					return jeaiii::to_text(buf, value);
				}
				if (value < 10000000000000ULL) {
					if (end - buf < 13)
						return end;
					return jeaiii::to_text(buf, value);
				}
				if (value < 100000000000000ULL) {
					if (end - buf < 14)
						return end;
					return jeaiii::to_text(buf, value);
				}
				if (end - buf < 15)
					return end;
				return jeaiii::to_text(buf, value);
			}
			if (value < 100000000000000000ULL) {
				if (value < 10000000000000000ULL) {
					if (end - buf < 16)
						return end;
					return jeaiii::to_text(buf, value);
				}
				if (end - buf < 17)
					return end;
				return jeaiii::to_text(buf, value);
			}
			if (value < 1000000000000000000ULL) {
				if (end - buf < 18)
					return end;
				return jeaiii::to_text(buf, value);
			}
			if (value < 10000000000000000000ULL) {
				if (end - buf < 19)
					return end;
				return jeaiii::to_text(buf, value);
			}
			if (end - buf < 20)
				return end;
			return jeaiii::to_text(buf, value);
		}
	}

	template<vn::detail::int64_types v_type> VN_FORCE_INLINE char* jeaiii_to_text_checked(char* buf, char* end, v_type value) noexcept {
		*buf		  = '-';
		uint64_t uval = (static_cast<uint64_t>(value) ^ static_cast<uint64_t>(value >> 63)) - static_cast<uint64_t>(value >> 63);
		return jeaiii_to_text_checked(buf + (value < 0), end, uval);
	}

	template<vn::detail::uint8_types v_type> VN_FORCE_INLINE char* jeaiii_to_text_checked(char* buf, char* end, v_type value) noexcept {
		if (value < 100U) {
			if (value < 10U) {
				if (end - buf < 1)
					return end;
				return jeaiii::to_text(buf, value);
			}
			if (end - buf < 2)
				return end;
			return jeaiii::to_text(buf, value);
		}
		if (end - buf < 3)
			return end;
		return jeaiii::to_text(buf, value);
	}

	template<vn::detail::int8_types v_type> VN_FORCE_INLINE char* jeaiii_to_text_checked(char* buf, char* end, v_type value) noexcept {
		*buf		 = '-';
		uint8_t uval = (static_cast<uint8_t>(value) ^ static_cast<uint8_t>(value >> 7)) - static_cast<uint8_t>(value >> 7);
		return jeaiii_to_text_checked(buf + (value < 0), end, uval);
	}

	template<uint64_t test_size, uint64_t sub_iters, uint64_t total_iters, typename int_type, uint64_t min_length, uint64_t max_length, bool negative> struct digit_generator {
		using test_data_type = std::vector<int_type>;
		using output_data_type = std::vector<char>;
		static constexpr uint64_t type_size{ benchmarks::max_digits_v<int_type> };
		static test_data_type impl() {
			test_data_type data{};
			data.resize(test_size * total_iters * sub_iters);
			for (uint64_t x = 0; x < data.size(); ++x) {
				int_type value = benchmarks::random_digits<int_type, min_length, max_length>::impl();
				if constexpr (vn::detail::int_types<int_type>) {
					value *= negative ? -1 : 1;
				}
				data[x] = value;
			}
			return data;
		}
	};

	struct vn_op {
		template<typename v_type> VN_FORCE_INLINE static char* convert(v_type v, char* buf) noexcept {
			return const_cast<char*>(vn::to_chars(buf, buf + 32, v).ptr);
		}
	};

	struct std_op {
		template<typename v_type> VN_FORCE_INLINE static char* convert(v_type v, char* buf) noexcept {
			return std::to_chars(buf, buf + 32, v).ptr;
		}
	};

	struct fmt_format_to_op {
		template<typename v_type> VN_FORCE_INLINE static char* convert(v_type v, char* buf) noexcept {
			return fmt::format_to(buf, FMT_COMPILE("{}"), v);
		}
	};

	struct jeaiii_op {
		template<typename v_type> VN_FORCE_INLINE static char* convert(v_type v, char* buf) noexcept {
			return jeaiii_to_text_checked(buf, buf + 32, v);
		}
	};

	template<typename Op> struct conversion_benchmark {
		template<typename size_type_01, typename size_type_02, typename vector_type, typename output_buffer_type, typename int_type> VN_FORCE_INLINE static uint64_t impl(
			size_type_01 test_size, size_type_02 sub_iters, const vector_type& test_data, output_buffer_type& output_buffer, int_type& current_index) {
			uint64_t total_bytes{};
			const auto& values = test_data.data() + current_index * test_size * sub_iters;
			char* const begin  = output_buffer.data() + current_index * test_size * sub_iters;
			char* p			   = begin;
			for (uint64_t y = 0; y < sub_iters; ++y) {
				uint64_t base_index = y * test_size;
				for (uint64_t x = 0; x < test_size; ++x) {
					p = Op::convert(values[base_index + x], p);
				}
			}
			++current_index;
			bnch_swt::do_not_optimize_away(p);
			total_bytes += static_cast<uint64_t>(p - begin);
			return total_bytes;
		}
	};

	struct verify_correctness {
		template<typename int_type> static void impl(const std::vector<int_type>& test_data, const char* test_label) {
			uint64_t vn_correct{}, vn_incorrect{};
			uint64_t jeaiii_correct{}, jeaiii_incorrect{};
			uint64_t fmt_format_to_correct{}, fmt_format_to_incorrect{}, total_incorrect{};
			int_type first_bad_value{};
			bool found_bad{ false };
			for (uint64_t x = 0; x < test_data.size(); ++x) {
				auto& v = test_data[x];
				char buf_std[32]{};
				char buf_vn[32]{};
				char buf_jeaiii[32]{};
				char buf_fmt_format_to[32]{};
				char* std_end			= std_op::convert(v, buf_std);
				char* vn_end			= vn_op::convert(v, buf_vn);
				char* jeaiii_end		= jeaiii_op::convert(v, buf_jeaiii);
				char* fmt_format_to_end = fmt_format_to_op::convert(v, buf_fmt_format_to);
				const uint64_t std_len	= static_cast<uint64_t>(std_end - buf_std);
				auto same				= [&](char* end, char* buf) {
					  return static_cast<uint64_t>(end - buf) == std_len && std::memcmp(buf, buf_std, std_len) == 0;
				};
				if (same(vn_end, buf_vn)) {
					++vn_correct;
				} else {
					++vn_incorrect;
					++total_incorrect;
					if (!found_bad) {
						first_bad_value = v;
						found_bad		= true;
					}
				}
				if (same(jeaiii_end, buf_jeaiii)) {
					++jeaiii_correct;
				} else {
					++jeaiii_incorrect;
					++total_incorrect;
				}
				if (same(fmt_format_to_end, buf_fmt_format_to)) {
					++fmt_format_to_correct;
				} else {
					++fmt_format_to_incorrect;
					++total_incorrect;
				}
			}
			if (total_incorrect > 0) {
				std::cout << "[" << test_label << "] vn correct: " << vn_correct << " | incorrect: " << vn_incorrect << " | jeaiii incorrect: " << jeaiii_incorrect
						  << " | fmt format_to incorrect: " << fmt_format_to_incorrect << std::endl;
			}
			if (vn_incorrect > 0) {
				std::cout << "  FIRST BAD vn VALUE: " << static_cast<int64_t>(first_bad_value) << std::endl;
			}
		}
	};

}