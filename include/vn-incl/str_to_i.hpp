// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp
// vn-incl/str_to_i.hpp

#pragma once

#include <vn-incl/utility.hpp>
#include <iostream>

namespace vn {

	namespace detail {

		template<uint64_t byte_count> struct first_non_zero_byte;

		template<uint_types v_type> VN_FORCE_INLINE v_type count_zeros(v_type value) noexcept {
			if constexpr (std::endian::native == std::endian::little) {
				return tzcnt(value);
			} else {
				return lzcnt(value);
			}
		}

		template<> struct first_non_zero_byte<1> {
			VN_FORCE_INLINE static uint64_t impl(const char* __restrict str) {
				return *str == '0';
			}
		};

		template<> struct first_non_zero_byte<2> {
			VN_FORCE_INLINE static uint64_t impl(const char* __restrict str) {
				uint16_t chunk;
				std::memcpy(&chunk, str, 2);
				uint16_t diff = chunk ^ repeat_bytes_v<static_cast<uint8_t>(0x30), uint16_t>;
				return (diff == 0) ? 2 : count_zeros(diff) >> 3;
			}
		};

		template<> struct first_non_zero_byte<3> {
			VN_FORCE_INLINE static uint64_t impl(const char* __restrict str) {
				uint32_t chunk;
				std::memcpy(&chunk, str, 3);
				uint32_t diff = chunk ^ repeat_bytes_v<static_cast<uint8_t>(0x30), uint32_t>;

				return (diff == 0) ? 3 : count_zeros(diff) >> 3;
			}
		};

		template<> struct first_non_zero_byte<4> {
			VN_FORCE_INLINE static uint64_t impl(const char* __restrict str) {
				uint32_t chunk;
				std::memcpy(&chunk, str, 4);
				uint32_t diff = chunk ^ repeat_bytes_v<static_cast<uint8_t>(0x30), uint32_t>;
				return (diff == 0) ? 4 : count_zeros(diff) >> 3;
			}
		};

		template<> struct first_non_zero_byte<5> {
			VN_FORCE_INLINE static uint64_t impl(const char* __restrict str) {
				uint64_t chunk;
				std::memcpy(&chunk, str, 5);
				uint64_t diff = chunk ^ repeat_bytes_v<static_cast<uint8_t>(0x30), uint64_t>;
				return (diff == 0) ? 5 : count_zeros(diff) >> 3;
			}
		};

		template<> struct first_non_zero_byte<6> {
			VN_FORCE_INLINE static uint64_t impl(const char* __restrict str) {
				uint64_t chunk;
				std::memcpy(&chunk, str, 6);
				uint64_t diff = chunk ^ repeat_bytes_v<static_cast<uint8_t>(0x30), uint64_t>;
				return (diff == 0) ? 6 : count_zeros(diff) >> 3;
			}
		};

		template<> struct first_non_zero_byte<7> {
			VN_FORCE_INLINE static uint64_t impl(const char* __restrict str) {
				uint64_t chunk;
				std::memcpy(&chunk, str, 7);
				uint64_t diff = chunk ^ repeat_bytes_v<static_cast<uint8_t>(0x30), uint64_t>;
				return (diff == 0) ? 7 : count_zeros(diff) >> 3;
			}
		};

		template<> struct first_non_zero_byte<8> {
			VN_FORCE_INLINE static uint64_t impl(const char* __restrict str) {
				uint64_t chunk;
				std::memcpy(&chunk, str, 8);
				uint64_t diff = chunk ^ repeat_bytes_v<static_cast<uint8_t>(0x30), uint64_t>;
				return (diff == 0) ? 8 : count_zeros(diff) >> 3;
			}
		};

		template<typename v_type> struct from_chars_impl;

		template<uint_types v_type> VN_FORCE_INLINE bool vn_is_digit(v_type value) {
			return ((static_cast<uint8_t>(value - '0')) < 10);
		}

		VN_FORCE_INLINE static const char* trim_leading_zeros(const char* __restrict iter, const char* __restrict end) noexcept {
			uint64_t length{ static_cast<uint64_t>(end - iter) };
			switch (length) {
				case 1: {
					iter += first_non_zero_byte<1>::impl(iter);
					return iter;
				}
				case 2: {
					iter += first_non_zero_byte<2>::impl(iter);
					return iter;
				}
				case 3: {
					iter += first_non_zero_byte<3>::impl(iter);
					return iter;
				}
				case 4: {
					iter += first_non_zero_byte<4>::impl(iter);
					return iter;
				}
				case 5: {
					iter += first_non_zero_byte<5>::impl(iter);
					return iter;
				}
				case 6: {
					iter += first_non_zero_byte<6>::impl(iter);
					return iter;
				}
				case 7: {
					iter += first_non_zero_byte<7>::impl(iter);
					return iter;
				}
				case 8: {
					iter += first_non_zero_byte<8>::impl(iter);
					return iter;
				}
				default: {
					while (iter + 8 < end && *iter == zero) {
						iter += first_non_zero_byte<8>::impl(iter);
					}
					if (iter + 4 < end && *iter == zero) {
						iter += first_non_zero_byte<4>::impl(iter);
					}
					if (iter + 2 < end && *iter == zero) {
						iter += first_non_zero_byte<2>::impl(iter);
					}
					if (iter < end && *iter == zero) {
						++iter;
					}
				}
			}
			return iter;
		}

		template<bool negative, integer_types v_type>
		VN_FORCE_INLINE static const char* __restrict parse_integer(v_type& value_new, const char* __restrict iter, const char* __restrict end) noexcept {
			using v_type_local					  = std::make_unsigned_t<v_type>;
			VN_ALIGN(64) static constexpr v_type_local zero_val{ 0 };

			if (iter >= end) [[unlikely]] {
				return iter;
			}

			v_type_local value;
			uint8_t c = static_cast<uint8_t>(*iter);

			if (c == zero) {
				iter = trim_leading_zeros(iter, end);
				if (iter < end && vn_is_digit(static_cast<uint8_t>(*iter))) {
					value = static_cast<v_type_local>(*iter - zero);
					++iter;
				} else {
					value_new = 0;
					return iter;
				}
			} else if VN_LIKELY (vn_is_digit(c)) {
				value = static_cast<v_type_local>(c - zero);
				++iter;
			} else {
				return iter;
			}

			if VN_LIKELY (iter < end) {
				c = static_cast<uint8_t>(*iter);
				if VN_LIKELY (vn_is_digit(c)) {
					value = static_cast<v_type_local>(value * 10 + (c - zero));
					++iter;
				} else {
					if constexpr (negative) {
						value_new = static_cast<v_type>(zero_val - value);
					} else {
						value_new = static_cast<v_type>(value);
					}
					return iter;
				}
			} else {
				if constexpr (negative) {
					value_new = static_cast<v_type>(zero_val - value);
				} else {
					value_new = static_cast<v_type>(value);
				}
				return iter;
			}

			if constexpr (sizeof(v_type) > 1) {
				if VN_LIKELY (iter < end) {
					c = static_cast<uint8_t>(*iter);
					if VN_LIKELY (vn_is_digit(c)) {
						value = static_cast<v_type_local>(value * 10 + (c - zero));
						++iter;
					} else {
						return iter;
					}
				} else {
					if constexpr (negative) {
						value_new = static_cast<v_type>(zero_val - value);
					} else {
						value_new = static_cast<v_type>(value);
					}
					return iter;
				}

				if VN_LIKELY (iter < end) {
					c = static_cast<uint8_t>(*iter);
					if VN_LIKELY (vn_is_digit(c)) {
						value = static_cast<v_type_local>(value * 10 + (c - zero));
						++iter;
					} else {
						return iter;
					}
				} else {
					if constexpr (negative) {
						value_new = static_cast<v_type>(zero_val - value);
					} else {
						value_new = static_cast<v_type>(value);
					}
					return iter;
				}

				if constexpr (sizeof(v_type) > 2) {
					if VN_LIKELY (iter < end) {
						c = static_cast<uint8_t>(*iter);
						if VN_LIKELY (vn_is_digit(c)) {
							value = static_cast<v_type_local>(value * 10 + (c - zero));
							++iter;
						} else {
							return iter;
						}
					} else {
						if constexpr (negative) {
							value_new = static_cast<v_type>(zero_val - value);
						} else {
							value_new = static_cast<v_type>(value);
						}
						return iter;
					}

					if VN_LIKELY (iter < end) {
						c = static_cast<uint8_t>(*iter);
						if VN_LIKELY (vn_is_digit(c)) {
							value = static_cast<v_type_local>(value * 10 + (c - zero));
							++iter;
						} else {
							return iter;
						}
					} else {
						if constexpr (negative) {
							value_new = static_cast<v_type>(zero_val - value);
						} else {
							value_new = static_cast<v_type>(value);
						}
						return iter;
					}

					if VN_LIKELY (iter < end) {
						c = static_cast<uint8_t>(*iter);
						if VN_LIKELY (vn_is_digit(c)) {
							value = static_cast<v_type_local>(value * 10 + (c - zero));
							++iter;
						} else {
							return iter;
						}
					} else {
						if constexpr (negative) {
							value_new = static_cast<v_type>(zero_val - value);
						} else {
							value_new = static_cast<v_type>(value);
						}
						return iter;
					}

					if VN_LIKELY (iter < end) {
						c = static_cast<uint8_t>(*iter);
						if VN_LIKELY (vn_is_digit(c)) {
							value = static_cast<v_type_local>(value * 10 + (c - zero));
							++iter;
						} else {
							return iter;
						}
					} else {
						if constexpr (negative) {
							value_new = static_cast<v_type>(zero_val - value);
						} else {
							value_new = static_cast<v_type>(value);
						}
						return iter;
					}

					if VN_LIKELY (iter < end) {
						c = static_cast<uint8_t>(*iter);
						if VN_LIKELY (vn_is_digit(c)) {
							value = static_cast<v_type_local>(value * 10 + (c - zero));
							++iter;
						} else {
							return iter;
						}
					} else {
						if constexpr (negative) {
							value_new = static_cast<v_type>(zero_val - value);
						} else {
							value_new = static_cast<v_type>(value);
						}
						return iter;
					}

					if constexpr (sizeof(v_type) > 4) {
						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if VN_LIKELY (iter < end) {
							c = static_cast<uint8_t>(*iter);
							if VN_LIKELY (vn_is_digit(c)) {
								value = static_cast<v_type_local>(value * 10 + (c - zero));
								++iter;
							} else {
								return iter;
							}
						} else {
							if constexpr (negative) {
								value_new = static_cast<v_type>(zero_val - value);
							} else {
								value_new = static_cast<v_type>(value);
							}
							return iter;
						}

						if constexpr (uint_types<v_type>) {
							if VN_LIKELY (iter < end) {
								c = static_cast<uint8_t>(*iter);
								if VN_LIKELY (vn_is_digit(c)) {
									value = static_cast<v_type_local>(value * 10 + (c - zero));
									++iter;
								} else {
									return iter;
								}
							} else {
								if constexpr (negative) {
									value_new = static_cast<v_type>(zero_val - value);
								} else {
									value_new = static_cast<v_type>(value);
								}
								return iter;
							}
						}
					}
				}
			}

			if VN_LIKELY (iter < end) {
				c = static_cast<uint8_t>(*iter);
				if VN_LIKELY (vn_is_digit(c)) {
					if constexpr (negative) {
						if VN_UNLIKELY (static_cast<uint64_t>(value) > static_cast<uint64_t>(raw_comp_vals_neg<v_type>[c])) {
							while (++iter < end && vn_is_digit(static_cast<uint8_t>(*iter))) {
							}
							return iter;
						}
					} else {
						if VN_UNLIKELY (static_cast<uint64_t>(value) > static_cast<uint64_t>(raw_comp_vals_pos<v_type>[c])) {
							while (++iter < end && vn_is_digit(static_cast<uint8_t>(*iter))) {
							}
							return iter;
						}
					}
					value = static_cast<v_type_local>(value * 10 + (c - zero));
					++iter;
				} else {
					if constexpr (negative) {
						value_new = static_cast<v_type>(zero_val - value);
					} else {
						value_new = static_cast<v_type>(value);
					}
					return iter;
				}
			} else {
				if constexpr (negative) {
					value_new = static_cast<v_type>(zero_val - value);
				} else {
					value_new = static_cast<v_type>(value);
				}
				return iter;
			}

			if (iter < end && vn_is_digit(static_cast<uint8_t>(*iter))) {
				while (++iter < end && vn_is_digit(static_cast<uint8_t>(*iter))) {
				}
				return iter;
			}

			if constexpr (negative) {
				value_new = static_cast<v_type>(zero_val - value);
			} else {
				value_new = static_cast<v_type>(value);
			}
			return iter;
		}

		template<int_types v_type> struct from_chars_impl<v_type> {
			constexpr from_chars_impl() noexcept = default;

			VN_FORCE_INLINE static const char* impl(v_type& value, const char* __restrict iter, const char* __restrict end) noexcept {
				return (iter >= end) ? end
					: (*iter == '-') ? ((iter + 1 >= end || !vn_is_digit(static_cast<uint8_t>(*(iter + 1)))) ? iter : parse_integer<true>(value, iter + 1, end))
									 : parse_integer<false>(value, iter, end);
			}
		};

		template<uint_types v_type> struct from_chars_impl<v_type> {
			constexpr from_chars_impl() noexcept = default;

			VN_FORCE_INLINE static const char* impl(v_type& value, const char* __restrict iter, const char* __restrict end) noexcept {
				return (iter < end) ? (*iter != '-') ? (parse_integer<false>(value, iter, end)) : iter : end;
			}
		};

	}

	template<detail::integer_types v_type>
	VN_FORCE_INLINE std::from_chars_result from_chars(const char* __restrict first, const char* __restrict last, v_type& value, int32_t base = 10) noexcept {
		if (base == 10) {
			const char* end = detail::from_chars_impl<v_type>::impl(value, first, last);

			if (end == first) {
				return { first, std::errc::invalid_argument };
			}
			return { end, std::errc{} };
		} else {
			auto r = std::from_chars(first, last, value, base);
			return { r.ptr, r.ec };
		}
	}

}
