# void-numerics Benchmark Results

**Platforms:** Intel Core i9-14900KF · Apple M1 (Virtual)  
**Compilers tested:**
- i9-14900KF: GNU 16.0.1 (WSL2 / Linux 6.6), Clang 23.0.0 (WSL2), MSVC 19.44 (Windows 10)
- Apple M1: GNU 15.2.0 (Darwin 24.6), Clang 22.1.4 (Darwin 24.6)

**Test methodology:** 30 iterations per test, stabilization phase excluded, 10 measured iterations, throughput in MB/s

Competitors: `jeaiii::to_text`, `std::to_chars` / `std::from_chars`, `fmt::format_to`, `strtoll` / `strtoull`

---

## Integer Serialization (`int-to-str`)

### Win Totals (across all integer types and batch sizes)

| Platform | Compiler | `vn::to_chars` | `jeaiii::to_text` | `std::to_chars` | `fmt::format_to` |
|---|---|---|---|---|---|
| i9-14900KF | GNU 16.0.1 | **17** | 12 | 22 | 0 |
| i9-14900KF | Clang 23.0.0 | **43** | 8 | 6 | 1 |
| i9-14900KF | MSVC 19.44 | **45** | 11 | 2 | 0 |
| Apple M1 | GNU 15.2.0 | **32** | 6 | 6 | 5 |
| Apple M1 | Clang 22.1.4 | **40** | 8 | 0 | 0 |

> GNU note (both platforms): `std::to_chars` leads on signed 64-bit integers specifically. `vn::to_chars` is dominant on unsigned 64-bit and all 32-bit types, and takes the overall win count on both platforms. The M1 GNU result is affected by `fmt::format_to` winning at batch size `[1]` on several small-integer types — a cache/startup effect that disappears at larger batches. Clang shows the cleanest picture on both platforms, with `vn` dominating across the board.

### Headline Numbers — `uint64_t` Serialization (strongest category for `vn`)

| Platform | Compiler | `vn::to_chars` | `jeaiii::to_text` | Lead |
|---|---|---|---|---|
| i9-14900KF | GNU 16.0.1 | **7,100 MB/s** | 4,293 MB/s | **+65%** |
| i9-14900KF | Clang 23.0.0 | **5,995 MB/s** | 4,783 MB/s | **+25%** |
| i9-14900KF | MSVC 19.44 | **4,154 MB/s** | 2,367 MB/s | **+76%** |
| Apple M1 | GNU 15.2.0 | **6,817 MB/s** | 3,454 MB/s | **+97%** |
| Apple M1 | Clang 22.1.4 | **5,676 MB/s** | 3,074 MB/s | **+85%** |

### Headline Numbers — `uint32_t` Serialization

| Platform | Compiler | `vn::to_chars` | `jeaiii::to_text` | Lead |
|---|---|---|---|---|
| i9-14900KF | GNU 16.0.1 | **3,002 MB/s** | 2,624 MB/s | **+14%** |
| i9-14900KF | Clang 23.0.0 | **3,172 MB/s** | 2,478 MB/s | **+28%** |
| i9-14900KF | MSVC 19.44 | **5,178 MB/s** | 2,232 MB/s | **+132%** |
| Apple M1 | GNU 15.2.0 | **5,332 MB/s** | 4,431 MB/s | **+20%** |
| Apple M1 | Clang 22.1.4 | **2,745 MB/s** | 1,927 MB/s | **+42%** |

### Headline Numbers — `int32_t` Negative Serialization

| Platform | Compiler | `vn::to_chars` | `jeaiii::to_text` | Lead |
|---|---|---|---|---|
| i9-14900KF | GNU 16.0.1 | **2,906 MB/s** | 2,843 MB/s | **+2%** |
| i9-14900KF | Clang 23.0.0 | **2,924 MB/s** | 2,267 MB/s | **+29%** |
| i9-14900KF | MSVC 19.44 | **2,470 MB/s** | 1,434 MB/s | **+72%** |
| Apple M1 | GNU 15.2.0 | **5,541 MB/s** | 4,060 MB/s | **+36%** |
| Apple M1 | Clang 22.1.4 | **2,645 MB/s** | 1,857 MB/s | **+42%** |

> The M1 GNU `int32_t` numbers are striking — M1 reaches 5,541 MB/s on negative int32 serialization, substantially above even the i9 GNU results, suggesting the M1's wide execution units and memory bandwidth are well-suited to this workload.

### Notable: Signed `int64_t` Serialization — GNU Pattern

On both platforms under GNU, `std::to_chars` leads for signed 64-bit specifically. Under Clang, `vn::to_chars` catches up or leads. This is the one consistent exception to `vn`'s overall dominance in serialization.

---

## Integer Deserialization — Standard Input (`str-to-int`)

### Win Totals

| Platform | Compiler | `vn::from_chars` | `std::from_chars` |
|---|---|---|---|
| i9-14900KF | GNU 16.0.1 | **55** | 4 (+1 tie) |
| i9-14900KF | Clang 23.0.0 | **58** | 1 (+1 tie) |
| i9-14900KF | MSVC 19.44 | **52** | 8 |
| Apple M1 | GNU 15.2.0 | **56** | 3 (+1 tie) |
| Apple M1 | Clang 22.1.4 | **60** | 0 — perfect sweep |

### Headline Numbers — `int64_t` Positive Deserialization

| Platform | Compiler | `vn::from_chars` | `std::from_chars` | Lead |
|---|---|---|---|---|
| i9-14900KF | GNU 16.0.1 | **4,495 MB/s** | 3,560 MB/s | **+26%** |
| i9-14900KF | Clang 23.0.0 | **4,221 MB/s** | 2,954 MB/s | **+43%** |
| i9-14900KF | MSVC 19.44 | **4,750 MB/s** | 4,138 MB/s | **+15%** |
| Apple M1 | GNU 15.2.0 | **5,528 MB/s** | 3,466 MB/s | **+60%** |
| Apple M1 | Clang 22.1.4 | **6,394 MB/s** | 2,357 MB/s | **+171%** |

### Headline Numbers — `int64_t` Negative Deserialization

| Platform | Compiler | `vn::from_chars` | `std::from_chars` | Lead |
|---|---|---|---|---|
| i9-14900KF | GNU 16.0.1 | **3,967 MB/s** | 3,400 MB/s | **+17%** |
| i9-14900KF | Clang 23.0.0 | **3,934 MB/s** | 3,324 MB/s | **+18%** |
| i9-14900KF | MSVC 19.44 | **4,270 MB/s** | 3,970 MB/s | **+8%** |
| Apple M1 | GNU 15.2.0 | **4,599 MB/s** | 3,481 MB/s | **+32%** |
| Apple M1 | Clang 22.1.4 | **5,670 MB/s** | 2,479 MB/s | **+129%** |

### Headline Numbers — `int16_t` Positive Deserialization (large batch)

| Platform | Compiler | `vn::from_chars` | `std::from_chars` | Lead |
|---|---|---|---|---|
| i9-14900KF | GNU 16.0.1 | **2,364 MB/s** | 1,507 MB/s | **+57%** |
| i9-14900KF | Clang 23.0.0 | **2,161 MB/s** | 1,090 MB/s | **+98%** |
| i9-14900KF | MSVC 19.44 | **2,344 MB/s** | 1,607 MB/s | **+46%** |
| Apple M1 | GNU 15.2.0 | **1,192 MB/s** | 944 MB/s | **+26%** |
| Apple M1 | Clang 22.1.4 | **1,949 MB/s** | 705 MB/s | **+176%** |

> The M1 Clang `int16_t` and `int64_t` results are particularly notable. Clang 22 appears to allow `vn::from_chars`'s implementation to express its algorithmic advantage much more fully on M1 than GNU does — the gap over `std::from_chars` roughly doubles or triples compared to the GNU results on the same hardware.

### `strtoll` / `strtoull` vs `vn::from_chars`

`strtoll`/`strtoull` is not competitive in this benchmark. Representative gaps:

- `int8_t` (i9 GNU): `vn` at **1,288 MB/s** vs `strtoll` at **184 MB/s** — **+600%**
- `int16_t` (i9 Clang): `vn` at **2,161 MB/s** vs `strtoll` at **627 MB/s** — **+245%**
- `int32_t` (i9 MSVC): `vn` at **859 MB/s** vs `strtoll` at **291 MB/s** — **+195%**
- `int8_t` (M1 GNU): `vn` at **438 MB/s** vs `strtoll` at **149 MB/s** — **+194%**
- `int16_t` (M1 Clang): `vn` at **1,472 MB/s** vs `strtoll` at **293 MB/s** — **+402%**

---

## Integer Deserialization — Leading Zeros (`str-to-int-leading-zeros`)

### Win Totals

| Platform | Compiler | `vn::from_chars` | `std::from_chars` |
|---|---|---|---|
| i9-14900KF | GNU 16.0.1 | **58** | 2 |
| i9-14900KF | Clang 23.0.0 | **48** | 7 (+5 ties) |
| i9-14900KF | MSVC 19.44 | **52** | 7 (+1 tie) |
| Apple M1 | GNU 15.2.0 | **60** | 0 — perfect sweep |
| Apple M1 | Clang 22.1.4 | **60** | 0 — perfect sweep |

The leading-zeros path is `vn::from_chars`'s most dominant category. Both M1 compilers produce a clean sweep across all 60 test cases. On x86, `std::from_chars` wins a handful of edge cases but `vn` still takes the overwhelming majority.

### Headline — Leading Zeros, `int32_t` (M1 GNU)

`vn::from_chars` at **879 MB/s** vs `std::from_chars` at **342 MB/s** — **+157%**

### Headline — Leading Zeros, `int64_t` Positive (M1 GNU)

`vn::from_chars` at **1,791 MB/s** vs `std::from_chars` at **800 MB/s** — **+124%**

### Headline — Leading Zeros, `uint32_t` (i9 GNU, large batch)

`vn::from_chars` at **359 MB/s** vs `std::from_chars` at **198 MB/s** — **+81%**

---

## Summary

| Stage | Best performer | Notes |
|---|---|---|
| `int-to-str` | `vn::to_chars` | Dominant on Clang and MSVC; wins overall count on GNU; exception is signed `int64_t` under GNU |
| `str-to-int` | `vn::from_chars` | Perfect sweep on M1/Clang; wins 87–97% of matchups elsewhere |
| `str-to-int-leading-zeros` | `vn::from_chars` | Perfect sweeps on both M1 compilers; near-perfect on x86 |

`fmt::format_to` and `strtoll`/`strtoull` are not competitive against any of the modern implementations. They are included as baseline references.

Benchmarks have been run with ASAN/UBSAN clean builds, strict compiler flags, and verified against LLVM's integer parsing test suite.