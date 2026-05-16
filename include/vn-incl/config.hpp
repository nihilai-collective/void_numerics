// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp
// vn-incl/config.hpp

#pragma once

#if defined(__clang__)
	#define VN_COMPILER_CLANG 1
	#define VN_COMPILER_GNU 0
	#define VN_COMPILER_MSVC 0
	#define VN_LIFETIME_BOUND [[clang::lifetimebound]]
#elif defined(__GNUC__) || defined(__GNUG__)
	#define VN_COMPILER_CLANG 0
	#define VN_COMPILER_GNU 1
	#define VN_COMPILER_MSVC 0
	#define VN_LIFETIME_BOUND
#elif defined(_MSC_VER)
	#define VN_COMPILER_CLANG 0
	#define VN_COMPILER_GNU 0
	#define VN_COMPILER_MSVC 1
	#define VN_LIFETIME_BOUND [[msvc::lifetimebound]]
#else
	#define VN_COMPILER_CLANG 0
	#define VN_COMPILER_GNU 0
	#define VN_COMPILER_MSVC 0
	#define VN_LIFETIME_BOUND
#endif

#if defined(_WIN32) || defined(_WIN64)
	#define VN_PLATFORM_WINDOWS 1
	#define VN_PLATFORM_LINUX 0
	#define VN_PLATFORM_MACOS 0
#elif defined(__APPLE__) || defined(__MACH__)
	#if TARGET_OS_MAC
		#define VN_PLATFORM_WINDOWS 0
		#define VN_PLATFORM_LINUX 0
		#define VN_PLATFORM_MACOS 1
	#else
		#define VN_PLATFORM_WINDOWS 0
		#define VN_PLATFORM_LINUX 0
		#define VN_PLATFORM_MACOS 0
	#endif
#elif defined(__linux__)
	#define VN_PLATFORM_WINDOWS 0
	#define VN_PLATFORM_LINUX 1
	#define VN_PLATFORM_MACOS 0
#else
	#define VN_PLATFORM_WINDOWS 0
	#define VN_PLATFORM_LINUX 0
	#define VN_PLATFORM_MACOS 0
#endif

#define VN_ALIGN(x) alignas(x)

#if VN_COMPILER_MSVC
	#pragma warning(push)
	#pragma warning(disable : 4324)
#endif

#if VN_DEBUG
	#if VN_COMPILER_MSVC
		#define VN_FORCE_INLINE [[msvc::noinline]]
		#define VN_FORCE_NOINLINE [[msvc::noinline]]
	#elif VN_COMPILER_GNU || VN_COMPILER_CLANG
		#define VN_FORCE_INLINE __attribute__((noinline))
		#define VN_FORCE_NOINLINE __attribute__((noinline))
	#else
		#define VN_FORCE_INLINE
		#define VN_FORCE_NOINLINE
	#endif
#else
	#if VN_COMPILER_MSVC
		#define VN_FORCE_INLINE [[msvc::forceinline]]
		#define VN_FORCE_NOINLINE [[msvc::noinline]]
	#elif VN_COMPILER_GNU || VN_COMPILER_CLANG
		#define VN_FORCE_INLINE inline __attribute__((always_inline))
		#define VN_FORCE_NOINLINE __attribute__((noinline))
	#else
		#define VN_FORCE_INLINE inline
		#define VN_FORCE_NOINLINE
	#endif
#endif

#if !defined(VN_LIKELY)
	#define VN_LIKELY(...) (__VA_ARGS__) [[likely]]
#endif

#if !defined(VN_UNLIKELY)
	#define VN_UNLIKELY(...) (__VA_ARGS__) [[unlikely]]
#endif

#if !defined(VN_ELSE_UNLIKELY)
	#define VN_ELSE_UNLIKELY(...) __VA_ARGS__ [[unlikely]]
#endif

#if !defined VN_ALIGN
	#define VN_ALIGN(b) alignas(b)
#endif

#include <concepts>
#include <charconv>
#include <cstring>
#include <cstdint>
#include <limits>
#include <array>
#include <bit>

namespace vn {

	namespace detail {

		using true_type	 = std::integral_constant<bool, true>;
		using false_type = std::integral_constant<bool, false>;

		enum class conversion_classes {
			i_to_str,
			d_to_str,
			str_to_i,
			str_to_d,
		};

	}

}
