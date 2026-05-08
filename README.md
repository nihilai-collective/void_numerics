# void_numerics

**SWAR-optimized integer conversion for C++20.**

`vn::to_chars` and `vn::from_chars` — drop-in replacements for the standard library equivalents, engineered for performance. Header-only, zero-allocation, no exceptions, no RTTI.

---

## Why

The C++ standard library's `<charconv>` is already fast. `void_numerics` is faster — substantially so on hot integer-conversion paths — without sacrificing correctness, portability, or API compatibility.

Every conversion is exhaustively unit-tested against `std::to_chars` / `std::from_chars` (including the LLVM libc++ test suite) and benchmarked against `std`, `jeaiii`, `fmt`, and `strtoll`/`strtoull` across 8-, 16-, 32-, and 64-bit signed and uint32_t integer types.

---

## Features

- **API-compatible** with `std::to_chars` / `std::from_chars` — returns `std::to_chars_result` / `std::from_chars_result`
- **All integer types**: `int8_t`, `uint8_t`, `int16_t`, `uint16_t`, `int32_t`, `uint32_t`, `int64_t`, `uint64_t`
- **Header-only** — single include, no build step required to consume
- **Zero allocation, zero exceptions, zero RTTI**
- **C++20** — leverages concepts for type-correct dispatch
- **Compile-time tables** for digit conversion and overflow bounds
- **Multiply-and-shift division replacement** for hot paths
- **Force-inlined helpers** with manual ladder dispatch sized to digit count
- **Cross-platform CI**: Ubuntu Clang-20, Ubuntu GCC-14, macOS Clang, macOS GCC-15, Windows MSVC
- **AddressSanitizer / UndefinedBehaviorSanitizer** support out of the box

---

## Quick Start

### Integer → string

```cpp
#include <vn-incl/i_to_str.hpp>

char buffer[32];
int64_t value = -9223372036854775807LL;

auto result = vn::to_chars(buffer, buffer + sizeof(buffer), value);
if (result.ec == std::errc{}) {
    // result.ptr points one past the last written character
    std::string_view text(buffer, result.ptr - buffer);
    // text == "-9223372036854775807"
}
```

### string → integer

```cpp
#include <vn-incl/str_to_i.hpp>

const char* input = "42abc";
uint32_t value{};

auto result = vn::from_chars(input, input + 5, value);
// result.ptr points to 'a' (first non-digit)
// result.ec == std::errc{}
// value == 42
```

### Base support

`vn::to_chars` and `vn::from_chars` accept an optional `base` parameter (defaulting to 10). Non-base-10 conversions transparently dispatch to `std::to_chars` / `std::from_chars` for full standard-library compatibility:

```cpp
char buf[32];
auto r = vn::to_chars(buf, buf + 32, 0xDEADBEEF, 16);
// r.ptr - buf == 8, buf == "deadbeef"
```

---

## API

### `vn::to_chars`

```cpp
template<integer_types v_type>
std::to_chars_result to_chars(char* first, char* last, v_type value, int32_t base = 10) noexcept;
```

Writes the textual representation of `value` to `[first, last)`. Returns `{ptr, std::errc{}}` on success, where `ptr` is one past the last character written. Returns `{last, std::errc::value_too_large}` if the buffer is too small.

### `vn::from_chars`

```cpp
template<integer_types v_type>
std::from_chars_result from_chars(const char* first, const char* last, v_type& value, int32_t base = 10) noexcept;
```

Parses an integer from `[first, last)` into `value`. Returns `{ptr, std::errc{}}` on success, where `ptr` points to the first character not consumed. Returns `{first, std::errc::invalid_argument}` if no characters could be parsed.

---

## Building

`void_numerics` is header-only — copy `include/vn-incl/` into your project, or consume via CMake.

### CMake

```cmake
add_subdirectory(void-numerics)
target_link_libraries(your_target PRIVATE void-numerics::void-numerics)
```

### CMake options

| Option | Default | Description |
|---|---|---|
| `VN_TESTS` | `OFF` | Build the unit test suite |
| `VN_BENCHMARKS` | `OFF` | Build the benchmark harness |
| `VN_ASAN` | `OFF` | Enable AddressSanitizer |
| `VN_UBSAN` | `OFF` | Enable UndefinedBehaviorSanitizer |

### Requirements

- C++20-capable compiler (Clang ≥ 20, GCC ≥ 14, MSVC latest, AppleClang)
- CMake ≥ 3.28

---

## Testing

The test suite covers:

- Exhaustive value enumeration for `int8`/`uint8`/`int16`/`uint16`
- Digit-length sweeps (1 through max digits) for all types
- Powers of 2, powers of 10, all-1s, all-9s patterns
- Limits: `min`, `min+1`, `min/2`, `max`, `max-1`, `max/2`, `max/3`, `max/7`
- Round-trip verification (`to_chars` → `from_chars` → equality)
- Leading-zero handling
- Stop-at-non-digit semantics (whitespace, alpha, dot, sign)
- Lone-minus and empty-input edge cases
- The full LLVM libc++ `<charconv>` test suite (`to_chars`, `from_chars`, integral-pass, roundtrip)

```bash
cmake -S . -B build -DVN_TESTS=TRUE -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/bin/vn_unit_tests
```

---

## Benchmarks

The benchmark suite compares `vn::to_chars` against `std::to_chars`, `jeaiii::to_text`, and `fmt::format_to`, and `vn::from_chars` against `std::from_chars` and `strtoll`/`strtoull` — across all integer types and digit lengths, with separate runs for negative and positive signed values, and a dedicated leading-zero test.

```bash
cmake -S . -B build -DVN_BENCHMARKS=TRUE -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/bin/vn_benchmarks
```

Results report MB/s throughput per library, per benchmark stage, with win-tallies aggregated across the full test matrix.

---

## Memory Footprint

`vn::to_chars` uses a precomputed lookup table of approximately 40KB to enable 
branch-free digit pair extraction. This is well-suited to any CPU with an L2 
cache of 256KB or larger (all modern desktop, server, and laptop CPUs) — the 
table comfortably resides in L2 while streaming integer data flows through L1, 
with no contention between the two. Benchmarks demonstrate this behavior holds 
even under explicit cache-eviction pressure between runs.

`vn::from_chars` carries no significant lookup tables and is suitable for any 
target.

`vn::to_chars` is **not recommended for**:

- Microcontrollers without L2 cache (Cortex-M0/M3/M4/M7, AVR, MSP430) — the 
  table cannot fit in available cache or SRAM
- Embedded targets where total flash/SRAM is smaller than the lookup table itself
- Any environment where the 40KB constant data footprint is unacceptable for 
  binary size reasons

For these targets, `std::to_chars` will be more appropriate.

---

## License

MIT. See [LICENSE](LICENSE).

Copyright © 2026 Nihilai Collective Corp.

---
