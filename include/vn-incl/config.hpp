// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Nihilai Collective Corp
// vn-incl/config.hpp

#pragma once

#if defined(__clang__)
	#define VN_COMPILER_CLANG 1
	#define VN_COMPILER_GNU 0
	#define VN_COMPILER_MSVC 0
#elif defined(__GNUC__) || defined(__GNUG__)
	#define VN_COMPILER_CLANG 0
	#define VN_COMPILER_GNU 1
	#define VN_COMPILER_MSVC 0
#elif defined(_MSC_VER)
	#define VN_COMPILER_CLANG 0
	#define VN_COMPILER_GNU 0
	#define VN_COMPILER_MSVC 1
#else
	#define VN_COMPILER_CLANG 0
	#define VN_COMPILER_GNU 0
	#define VN_COMPILER_MSVC 0
#endif

#if defined(_WIN32) || defined(_WIN64)
	#define VN_PLATFORM_WINDOWS 1
	#define VN_PLATFORM_LINUX 0
	#define VN_PLATFORM_MACOS 0
	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
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

#if defined(_DEBUG) || !defined(NDEBUG)
	#define VN_DEBUG 1
#else
	#define VN_DEBUG 0
#endif

#define VN_ALIGN(x) alignas(x)

#if VN_DEBUG
	#if VN_COMPILER_MSVC
		#define VN_FORCE_INLINE [[msvc::noinline]]
	#elif VN_COMPILER_GNU || VN_COMPILER_CLANG
		#define VN_FORCE_INLINE __attribute__((noinline))
	#else
		#define VN_FORCE_INLINE
	#endif
#else
	#if VN_COMPILER_MSVC
		#define VN_FORCE_INLINE [[msvc::forceinline]]
	#elif VN_COMPILER_GNU || VN_COMPILER_CLANG
		#define VN_FORCE_INLINE inline __attribute__((always_inline))
	#else
		#define VN_FORCE_INLINE inline
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
#include <array>
#include <bit>

namespace vn {
	using true_type	 = std::integral_constant<bool, true>;
	using false_type = std::integral_constant<bool, false>;

	enum class conversion_classes {
		i_to_str,
		d_to_str,
		str_to_i,
		str_to_d,
	};

	VN_FORCE_INLINE static void vn_prefetch(const void* ptr) noexcept {
#if VN_PLATFORM_MACOS && defined(__arm64__)
		__builtin_prefetch(ptr, 0, 0);
#elif VN_COMPILER_MSVC || VN_COMPILER_GNU || VN_COMPILER_CLANG
		_mm_prefetch(static_cast<const char*>(ptr), _MM_HINT_T0);
#else
	#error "Compiler or architecture not supported for prefetching"
#endif
	}

	template<typename v_type> struct get_int_type {
		using type = std::conditional_t<std::is_unsigned_v<v_type>, uint8_t, int8_t>;
	};

}