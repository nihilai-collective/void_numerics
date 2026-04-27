// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp
// vn-incl/utility.hpp

#pragma once

#include <vn-incl/concepts.hpp>
#include <limits>

namespace vn {

	namespace detail {

		template<typename v_type> struct get_int_type {
			using type = std::conditional_t<std::is_unsigned_v<v_type>, uint8_t, int8_t>;
		};

		template<auto value_new> struct make_static {
			static constexpr auto value{ value_new };
		};

		template<typename v_type> struct get_next_higher_int;

		template<uint_types v_type> struct get_next_higher_int<v_type> {
			using type = std::conditional_t<std::same_as<uint8_t, base_t<v_type>>, uint16_t, std::conditional_t<std::same_as<uint16_t, base_t<v_type>>, uint32_t, uint64_t>>;
		};

		template<int_types v_type> struct get_next_higher_int<v_type> {
			using type = std::conditional_t<std::same_as<int8_t, base_t<v_type>>, int16_t, std::conditional_t<std::same_as<int16_t, base_t<v_type>>, int32_t, int64_t>>;
		};

		template<integer_types v_type> using get_next_higher_int_t = typename get_next_higher_int<v_type>::type;

		template<uint8_types auto repeat, uint_types return_type> static consteval return_type repeat_byte() noexcept {
			return static_cast<return_type>(0x0101010101010101ull) * static_cast<return_type>(repeat);
		}

		template<uint8_types auto repeat, uint64_t repetition_count> struct repeat_bytes {
			VN_FORCE_INLINE static consteval auto impl() {
				if constexpr (repetition_count == 1) {
					return static_cast<uint8_t>(0x01) * static_cast<uint8_t>(repeat);
				} else if constexpr (repetition_count == 2) {
					return static_cast<uint16_t>(0x0101) * static_cast<uint16_t>(repeat);
				} else if constexpr (repetition_count == 3) {
					return static_cast<uint32_t>(0x010101) * static_cast<uint32_t>(repeat);
				} else if constexpr (repetition_count == 4) {
					return static_cast<uint32_t>(0x01010101) * static_cast<uint32_t>(repeat);
				} else if constexpr (repetition_count == 5) {
					return static_cast<uint64_t>(0x0101010101) * static_cast<uint64_t>(repeat);
				} else if constexpr (repetition_count == 6) {
					return static_cast<uint64_t>(0x010101010101) * static_cast<uint64_t>(repeat);
				} else if constexpr (repetition_count == 7) {
					return static_cast<uint64_t>(0x01010101010101) * static_cast<uint64_t>(repeat);
				} else {
					return static_cast<uint64_t>(0x0101010101010101) * static_cast<uint64_t>(repeat);
				}
			}
		};

		template<uint8_types auto repeat, uint64_t repitition_count> static constexpr uint64_t repeat_bytes_v = repeat_bytes<repeat, repitition_count>::impl();

		template<uint_types v_type> VN_FORCE_INLINE constexpr v_type tzcnt_constexpr(const v_type value) noexcept {
			if (value == 0)
				return device_cast<v_type>(sizeof(v_type) * 8);

			v_type count = 0;
			while ((value & 1) == 0) {
				value >>= 1;
				++count;
			}
			return count;
		}

		template<uint8_types v_type> VN_FORCE_INLINE constexpr v_type tzcnt_constexpr(const v_type value) noexcept {
			return tzcnt_constexpr(value);
		}

		template<uint16_types v_type> VN_FORCE_INLINE constexpr v_type tzcnt(const v_type value) noexcept {
#if VN_COMPILER_MSVC
	#if VN_ARCH_ARM64
			unsigned long trailing_zero = 0;
			if (_BitScanForward(&trailing_zero, static_cast<unsigned long>(value))) {
				return static_cast<v_type>(trailing_zero);
			} else {
				return 16;
			}
	#else
			return static_cast<v_type>(_tzcnt_u16(value));
	#endif
#elif VN_COMPILER_CLANG || VN_COMPILER_GNU
			return (value == 0) ? 16 : static_cast<v_type>(__builtin_ctz(static_cast<unsigned int>(value)));
#else
			return tzcnt_constexpr(value);
#endif
		}

		template<uint32_types v_type> VN_FORCE_INLINE constexpr v_type tzcnt(const v_type value) noexcept {
#if VN_COMPILER_MSVC
	#if VN_ARCH_ARM64
			unsigned long trailing_zero = 0;
			if (_BitScanForward(&trailing_zero, static_cast<unsigned long>(value))) {
				return static_cast<v_type>(trailing_zero);
			} else {
				return 32;
			}
	#else
			return static_cast<v_type>(_tzcnt_u32(value));
	#endif
#elif VN_COMPILER_CLANG || VN_COMPILER_GNU
			return (value == 0) ? 32 : static_cast<v_type>(__builtin_ctz(static_cast<unsigned int>(value)));
#else
			return tzcnt_constexpr(value);
#endif
		}

		template<uint64_types v_type> VN_FORCE_INLINE constexpr v_type tzcnt(const v_type value) noexcept {
#if VN_COMPILER_MSVC
	#if VN_ARCH_ARM64
			unsigned long trailing_zero = 0;
			if (_BitScanForward64(&trailing_zero, static_cast<unsigned __int64>(value))) {
				return static_cast<v_type>(trailing_zero);
			} else {
				return 64;
			}
	#else
			return static_cast<v_type>(_tzcnt_u64(value));
	#endif
#elif VN_COMPILER_CLANG || VN_COMPILER_GNU
			return (value == 0) ? 64 : static_cast<v_type>(__builtin_ctzll(static_cast<uint64_t>(value)));
#else
			return tzcnt_constexpr(value);
#endif
		}

	}

}
