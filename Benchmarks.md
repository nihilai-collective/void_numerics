# void-numerics (vn) Benchmark Results

**Library:** `void-numerics` (`vn::to_chars` / `vn::from_chars`)
**CPU:** Intel Core i9-14900KF
**Competitors:** `jeaiii::to_text`, `std::to_chars` / `std::from_chars`, `fmt::format_to`, `strtoll/strtoull`
**Methodology:** 30 iterations per test, 10 measured after stabilization, throughput in MB/s

---

## Summary — Total Wins

| Platform / Compiler | Stage | vn wins | Runner-up wins |
|---|---|---|---|
| Linux / Clang 23 | int-to-str | **23** | std: 1 |
| Linux / Clang 23 | str-to-int-leading-zeros | **21** | std: 3 |
| Linux / Clang 23 | str-to-int | **23** | — (1 tie each) |
| Linux / GCC 16 | int-to-str | **12** | std: 7, jeaiii: 2 (3 ties each) |
| Linux / GCC 16 | str-to-int-leading-zeros | **24** | — |
| Linux / GCC 16 | str-to-int | **23** | std: 1 |
| Windows / MSVC 19.44 | int-to-str | **23** | jeaiii: 1 |
| Windows / MSVC 19.44 | str-to-int-leading-zeros | **23** | std: 1 |
| Windows / MSVC 19.44 | str-to-int | **24** | — |
| macOS / GCC 15 (Apple M1) | str-to-int | **24** | — |
| macOS / Clang 22 (Apple M1) | int-to-str | **16** | jeaiii: 6 (2 ties each) |
| macOS / Clang 22 (Apple M1) | str-to-int-leading-zeros | **24** | — |
| macOS / Clang 22 (Apple M1) | str-to-int | **24** | — |

---

## int-to-str

### Linux / Clang 23.0.0

| Test | 1st | 2nd | 3rd | 4th |
|---|---|---|---|---|
| int8 neg [1K] | **vn** 3449 MB/s | jeaiii 2841 | std 2570 | fmt 1689 |
| int8 neg [10K] | **vn** 3567 MB/s | jeaiii 3214 | std 2690 | fmt 1948 |
| int8 pos [1K] | **vn** 2301 MB/s | jeaiii 1600 | std 1335 | fmt 972 |
| int8 pos [10K] | **vn** 2189 MB/s | jeaiii 1615 | std 1254 | fmt 972 |
| uint8 pos [1K] | **vn** 908 MB/s | jeaiii 883 | std 788 | fmt 783 |
| uint8 pos [10K] | **vn** 966 MB/s | jeaiii 919 | std 801 | fmt 791 |
| int16 neg [1K] | **vn** 2902 MB/s | std 2663 | jeaiii 2506 | fmt 1939 |
| int16 neg [10K] | **vn** 3035 MB/s | jeaiii 2699 | std 2499 | fmt 1964 |
| int16 pos [1K] | **vn** 1755 MB/s | std 1332 | jeaiii 1252 | fmt 1061 |
| int16 pos [10K] | **vn** 1776 MB/s | jeaiii 1346 | std 1253 | fmt 1032 |
| uint16 pos [1K] | **vn** 2291 MB/s | jeaiii 1909 | std 1502 | fmt 1490 |
| uint16 pos [10K] | **vn** 2310 MB/s | jeaiii 2006 | std 1508 | fmt 1466 |
| int32 neg [1K] | **vn** 3127 MB/s | jeaiii 2518 | fmt 1778 | std 1669 |
| int32 neg [10K] | **vn** 2910 MB/s | jeaiii 2528 | fmt 1907 | std 1681 |
| int32 pos [1K] | **vn** 3005 MB/s | jeaiii 2317 | fmt 1695 | std 1511 |
| int32 pos [10K] | **vn** 2982 MB/s | jeaiii 2308 | fmt 1714 | std 1505 |
| uint32 pos [1K] | **vn** 3055 MB/s | jeaiii 2455 | fmt 1728 | std 1558 |
| uint32 pos [10K] | **vn** 3103 MB/s | jeaiii 2470 | fmt 1915 | std 1533 |
| int64 neg [1K] | std 2215 MB/s | **vn** 2159 | jeaiii 2004 | fmt 1971 |
| int64 neg [10K] | **vn** 2292 MB/s | std 2180 | jeaiii 2072 | fmt 1957 |
| int64 pos [1K] | **vn** 1280 MB/s | std 1176 | jeaiii 1043 | fmt 914 |
| int64 pos [10K] | **vn** 1299 MB/s | std 1149 | jeaiii 974 | fmt 916 |
| uint64 pos [1K] | **vn** 7263 MB/s | jeaiii 4672 | fmt 2697 | std 2161 |
| uint64 pos [10K] | **vn** 7447 MB/s | jeaiii 4668 | fmt 2809 | std 2129 |

### Linux / GCC 16.0.1

| Test | 1st | 2nd | 3rd | 4th |
|---|---|---|---|---|
| int8 neg [1K] | **vn** 4102 MB/s | std 3770 | jeaiii 3489 | fmt 2139 |
| int8 neg [10K] | **vn** 4173 MB/s | std 3892 | jeaiii 3564 | fmt 2196 |
| int8 pos [1K] | **vn/std TIE** ~2325 MB/s | jeaiii 1769 | fmt 1045 | — |
| int8 pos [10K] | **vn/std TIE** ~2322 MB/s | jeaiii 1798 | fmt 1203 | — |
| uint8 pos [1K] | jeaiii 947 MB/s | **vn** 905 | std 783 | fmt 587 |
| uint8 pos [10K] | jeaiii 934 MB/s | **vn** 915 | fmt 834 | std 770 |
| int16 neg [1K] | std 3951 MB/s | **vn** 3519 | jeaiii 3280 | fmt 2193 |
| int16 neg [10K] | std 4274 MB/s | **vn** 3595 | jeaiii 3324 | fmt 1043 |
| int16 pos [1K] | std 2583 MB/s | **vn** 1916 | jeaiii 1639 | fmt 1327 |
| int16 pos [10K] | std 2467 MB/s | **vn** 1963 | jeaiii 1658 | fmt 1345 |
| uint16 pos [1K] | **vn** 2223 MB/s | jeaiii 2166 | fmt 1504 | std 1320 |
| uint16 pos [10K] | **vn** 2229 MB/s | jeaiii 2163 | fmt 1446 | std 1326 |
| int32 neg [1K] | **vn** 3466 MB/s | jeaiii 2867 | fmt 1947 | std 1572 |
| int32 neg [10K] | **vn** 3413 MB/s | jeaiii 2860 | fmt 1918 | std 1552 |
| int32 pos [1K] | **vn** 2815 MB/s | jeaiii 2558 | fmt 1726 | std 1406 |
| int32 pos [10K] | **vn** 3032 MB/s | jeaiii 2557 | fmt 1774 | std 1389 |
| uint32 pos [1K] | **vn** 3239 MB/s | jeaiii 2994 | fmt 1709 | std 1304 |
| uint32 pos [10K] | **vn** 3203 MB/s | jeaiii 2964 | fmt 1895 | std 1417 |
| int64 neg [1K] | **vn/std TIE** ~2533 MB/s | jeaiii 2088 | fmt 1841 | — |
| int64 neg [10K] | std 3379 MB/s | **vn** 2693 | jeaiii 2223 | fmt 1872 |
| int64 pos [1K] | std 1675 MB/s | **vn** 1332 | jeaiii 1100 | fmt 936 |
| int64 pos [10K] | std 1724 MB/s | **vn** 1335 | jeaiii 1111 | fmt 967 |
| uint64 pos [1K] | **vn** 6597 MB/s | jeaiii 4322 | fmt 2721 | std 2087 |
| uint64 pos [10K] | **vn** 6471 MB/s | jeaiii 4250 | fmt 2771 | std 2105 |

### Windows / MSVC 19.44.35227.0

| Test | 1st | 2nd | 3rd | 4th |
|---|---|---|---|---|
| int8 neg [1K] | **vn** 3853 MB/s | jeaiii 2868 | fmt 1336 | std 1186 |
| int8 neg [10K] | **vn** 4017 MB/s | jeaiii 2948 | fmt 1353 | std 1208 |
| int8 pos [1K] | **vn** 2326 MB/s (+250% vs std) | std 664 | jeaiii 547 | fmt 201 |
| int8 pos [10K] | **vn** 2329 MB/s | jeaiii 1467 | fmt 667 | std 622 |
| uint8 pos [1K] | **vn** 1941 MB/s | jeaiii 1881 | fmt 1351 | std 628 |
| uint8 pos [10K] | jeaiii 2068 MB/s | **vn** 1922 | fmt 1372 | std 626 |
| int16 neg [1K] | **vn** 3086 MB/s | jeaiii 1792 | fmt 1304 | std 1187 |
| int16 neg [10K] | **vn** 3137 MB/s | jeaiii 1807 | fmt 1354 | std 1203 |
| int16 pos [1K] | **vn** 1766 MB/s | jeaiii 894 | std/fmt TIE ~672 | — |
| int16 pos [10K] | **vn** 1679 MB/s | jeaiii 895 | std/fmt TIE ~677 | — |
| uint16 pos [1K] | **vn** 4262 MB/s | jeaiii 2685 | fmt 1932 | std 924 |
| uint16 pos [10K] | **vn** 4380 MB/s | jeaiii 2729 | fmt 1946 | std 936 |
| int32 neg [1K] | **vn** 5493 MB/s | jeaiii 2648 | fmt 2336 | std 1310 |
| int32 neg [10K] | **vn** 5717 MB/s | jeaiii 2751 | fmt 2537 | std 1240 |
| int32 pos [1K] | **vn** 5432 MB/s | jeaiii 2403 | fmt 2326 | std 1160 |
| int32 pos [10K] | **vn** 5338 MB/s | jeaiii 2518 | fmt 2342 | std 1239 |
| uint32 pos [1K] | **vn** 4972 MB/s | jeaiii 3587 | fmt 2331 | std 1142 |
| uint32 pos [10K] | **vn** 6052 MB/s | jeaiii 4093 | fmt 2576 | std 1208 |
| int64 neg [1K] | **vn** 2507 MB/s | jeaiii/std TIE ~1191 | fmt 1177 | — |
| int64 neg [10K] | **vn** 2658 MB/s | jeaiii 1303 | std 1199 | fmt 1104 |
| int64 pos [1K] | **vn** 1357 MB/s | std/jeaiii TIE ~664 | fmt 589 | — |
| int64 pos [10K] | **vn** 1388 MB/s | std 672 | jeaiii 598 | fmt 590 |
| uint64 pos [1K] | **vn** 7623 MB/s | jeaiii 4856 | fmt 2184 | std 1486 |
| uint64 pos [10K] | **vn** 7531 MB/s | jeaiii 4864 | fmt 2182 | std 1476 |

---

## str-to-int (no leading zeros)

### Linux / Clang 23.0.0

| Test | vn MB/s | std MB/s | vn vs std |
|---|---|---|---|
| int8 neg [1K] | 1131 | 562 | **+101%** |
| int8 neg [10K] | 1140 | 539 | **+111%** |
| int8 pos [1K] | 1451 | 539 | **+168%** |
| int8 pos [10K] | 1488 | 535 | **+178%** |
| uint8 pos [1K] | 242 | 243 | TIE |
| uint8 pos [10K] | **240** | 237 | +1% |
| int16 neg [1K] | **1810** | 1067 | **+69%** |
| int16 neg [10K] | **1811** | 1040 | **+74%** |
| int16 pos [1K] | **2257** | 1170 | **+92%** |
| int16 pos [10K] | **2433** | 1025 | **+137%** |
| uint16 pos [1K] | **498** | 452 | +10% |
| uint16 pos [10K] | **480** | 449 | +7% |
| int32 neg [1K] | **674** | 523 | +28% |
| int32 neg [10K] | **671** | 521 | +28% |
| int32 pos [1K] | **626** | 523 | +19% |
| int32 pos [10K] | **674** | 506 | +33% |
| uint32 pos [1K] | **691** | 586 | +17% |
| uint32 pos [10K] | **666** | 589 | +13% |
| int64 neg [1K] | **3339** | 3234 | +3% |
| int64 neg [10K] | **3964** | 3559 | +11% |
| int64 pos [1K] | **4041** | 3339 | +21% |
| int64 pos [10K] | **4117** | 3215 | +28% |
| uint64 pos [1K] | **1017** | 763 | +33% |
| uint64 pos [10K] | **991** | 739 | +34% |

### Linux / GCC 16.0.1

| Test | vn MB/s | std MB/s | vn vs std |
|---|---|---|---|
| int8 neg [1K] | **1255** | 759 | **+65%** |
| int8 neg [10K] | **1278** | 823 | **+55%** |
| int8 pos [1K] | **1389** | 819 | **+69%** |
| int8 pos [10K] | **1503** | 833 | **+80%** |
| uint8 pos [1K] | **259** | 240 | +7% |
| uint8 pos [10K] | **260** | 237 | +9% |
| int16 neg [1K] | **1909** | 1549 | **+23%** |
| int16 neg [10K] | **1777** | 1460 | **+21%** |
| int16 pos [1K] | **2557** | 1695 | **+50%** |
| int16 pos [10K] | **2019** | 1540 | **+31%** |
| uint16 pos [1K] | **548** | 510 | +7% |
| uint16 pos [10K] | **530** | 509 | +4% |
| int32 neg [1K] | **735** | 633 | +16% |
| int32 neg [10K] | **712** | 619 | +14% |
| int32 pos [1K] | **772** | 637 | +21% |
| int32 pos [10K] | **760** | 619 | +22% |
| uint32 pos [1K] | **797** | 673 | +18% |
| uint32 pos [10K] | **775** | 655 | +18% |
| int64 neg [1K] | **4058** | 3681 | +10% |
| int64 neg [10K] | **3950** | 3236 | +22% |
| int64 pos [1K] | **3857** | 2719 | **+41%** |
| int64 pos [10K] | **4412** | 3730 | +18% |
| uint64 pos [1K] | 578 | **993** | std wins |
| uint64 pos [10K] | **548** | 513 | +6% |

### Windows / MSVC 19.44.35227.0

| Test | vn MB/s | std MB/s | vn vs std |
|---|---|---|---|
| int8 neg [1K] | **993** | 762 | **+30%** |
| int8 neg [10K] | **1169** | 693 | **+68%** |
| int8 pos [1K] | **1263** | 819 | **+54%** |
| int8 pos [10K] | **1272** | 807 | **+57%** |
| uint8 pos [1K] | **473** | 401 | +17% |
| uint8 pos [10K] | **459** | 405 | +13% |
| int16 neg [1K] | **1675** | 1511 | +10% |
| int16 neg [10K] | **1683** | 1496 | +12% |
| int16 pos [1K] | **1751** | 1600 | +9% |
| int16 pos [10K] | **2080** | 1590 | **+30%** |
| uint16 pos [1K] | **707** | 629 | +12% |
| uint16 pos [10K] | **699** | 643 | +8% |
| int32 neg [1K] | **791** | 683 | +15% |
| int32 neg [10K] | **803** | 684 | +17% |
| int32 pos [1K] | **869** | 679 | +27% |
| int32 pos [10K] | **860** | 674 | +27% |
| uint32 pos [1K] | **894** | 775 | +15% |
| uint32 pos [10K] | **859** | 747 | +15% |
| int64 neg [1K] | **4179** | 3810 | +9% |
| int64 neg [10K] | **4037** | 3580 | +12% |
| int64 pos [1K] | **4297** | 4092 | +5% |
| int64 pos [10K] | **4828** | 4085 | +18% |
| uint64 pos [1K] | **1109** | 716 | **+54%** |
| uint64 pos [10K] | **1108** | 803 **| +38%** |

### macOS Apple M1 / Clang 22.1.4

| Test | vn MB/s | std MB/s | vn vs std |
|---|---|---|---|
| int8 neg [1K] | **704** | 353 | **+99%** |
| int8 neg [10K] | **687** | 350 | **+95%** |
| int8 pos [1K] | **925** | 341 | **+170%** |
| int8 pos [10K] | **952** | 353 | **+169%** |
| uint8 pos [1K] | **185** | 143 | **+29%** |
| uint8 pos [10K] | **183** | 150 | +22% |
| int16 neg [1K] | **1402** | 696 | **+101%** |
| int16 neg [10K] | **1472** | 695 | **+111%** |
| int16 pos [1K] | **1923** | 673 | **+185%** |
| int16 pos [10K] | **1830** | 656 | **+178%** |
| uint16 pos [1K] | **384** | 268 | **+43%** |
| uint16 pos [10K] | **378** | 268 | **+41%** |
| int32 neg [1K] | **549** | 284 | **+93%** |
| int32 neg [10K] | **335** | 257 | **+30%** |
| int32 pos [1K] | **497** | 277 | **+79%** |
| int32 pos [10K] | **515** | 212 | **+143%** |
| uint32 pos [1K] | **531** | 315 | **+68%** |
| uint32 pos [10K] | **527** | 242 | **+117%** |
| int64 neg [1K] | **3614** | 2382 | **+51%** |
| int64 neg [10K] | **3192** | 2233 | **+42%** |
| int64 pos [1K] | **6071** | 2227 | **+172%** |
| int64 pos [10K] | **5858** | 2240 | **+161%** |
| uint64 pos [1K] | **690** | 384 | **+79%** |
| uint64 pos [10K] | **682** | 381 | **+79%** |

---

## str-to-int (leading zeros)

### Linux / Clang 23.0.0 — vn wins 21/24, std wins 3

| Test | vn MB/s | std MB/s | vn vs std |
|---|---|---|---|
| int8 neg [1K] | **82** | 70 | +16% |
| int8 neg [10K] | **84** | 69 | +21% |
| int8 pos [1K] | **88** | 70 | +25% |
| int8 pos [10K] | **89** | 71 | +24% |
| uint8 pos [1K] | **70** | 70 | +1% |
| uint8 pos [10K] | 69 | **70** | std +1% |
| int16 neg [1K] | **166** | 147 | +12% |
| int16 neg [10K] | **165** | 147 | +12% |
| int16 pos [1K] | **169** | 146 | +15% |
| int16 pos [10K] | **169** | 140 | +21% |
| uint16 pos [1K] | **150** | 123 | +21% |
| uint16 pos [10K] | **150** | 131 | +14% |
| int32 neg [1K] | **314** | 291 | +8% |
| int32 neg [10K] | **327** | 298 | +9% |
| int32 pos [1K] | 259 | **285** | std +9% |
| int32 pos [10K] | **356** | 294 | +21% |
| uint32 pos [1K] | **296** | 233 | +27% |
| uint32 pos [10K] | **299** | 237 | +26% |
| int64 neg [1K] | **626** | 593 | +5% |
| int64 neg [10K] | 626 | **645** | std +2% |
| int64 pos [1K] | **679** | 605 | +12% |
| int64 pos [10K] | **696** | 649 | +7% |
| uint64 pos [1K] | **513** | 366 | **+40%** |
| uint64 pos [10K] | **514** | 360 | **+42%** |

### Linux / GCC 16.0.1 — vn wins 24/24

| Test | vn MB/s | std MB/s | vn vs std |
|---|---|---|---|
| int8 neg [1K] | **82** | 67 | +22% |
| int8 neg [10K] | **82** | 67 | +23% |
| int8 pos [1K] | **86** | 62 | **+37%** |
| int8 pos [10K] | **88** | 66 | +33% |
| uint8 pos [1K] | **76** | 70 | +7% |
| uint8 pos [10K] | **79** | 66 | +19% |
| int16 neg [1K] | **168** | 132 | **+26%** |
| int16 neg [10K] | **163** | 131 | +24% |
| int16 pos [1K] | **159** | 132 | +20% |
| int16 pos [10K] | **175** | 130 | **+34%** |
| uint16 pos [1K] | **185** | 127 | **+44%** |
| uint16 pos [10K] | **191** | 125 | **+52%** |
| int32 neg [1K] | **317** | 282 | +12% |
| int32 neg [10K] | **318** | 273 | +16% |
| int32 pos [1K] | **340** | 279 | +21% |
| int32 pos [10K] | **341** | 279 | +22% |
| uint32 pos [1K] | **357** | 218 | **+63%** |
| uint32 pos [10K] | **357** | 215 | **+65%** |
| int64 neg [1K] | **600** | 580 | +3% |
| int64 neg [10K] | **617** | 568 | +8% |
| int64 pos [1K] | **663** | 545 | +21% |
| int64 pos [10K] | **668** | 581 | +14% |
| uint64 pos [1K] | **626** | 382 | **+63%** |
| uint64 pos [10K] | **623** | 385 | **+61%** |

### Windows / MSVC 19.44.35227.0 — vn wins 23/24, std wins 1

| Test | vn MB/s | std MB/s | vn vs std |
|---|---|---|---|
| int8 neg [1K] | **137** | 97 | **+40%** |
| int8 neg [10K] | **144** | 98 | **+46%** |
| int8 pos [1K] | **153** | 99 | **+54%** |
| int8 pos [10K] | **143** | 95 | **+50%** |
| uint8 pos [1K] | **102** | 86 | +18% |
| uint8 pos [10K] | **104** | 88 | +17% |
| int16 neg [1K] | **291** | 229 | **+27%** |
| int16 neg [10K] | **277** | 225 | +22% |
| int16 pos [1K] | **303** | 229 | **+32%** |
| int16 pos [10K] | **286** | 217 | **+31%** |
| uint16 pos [1K] | **236** | 172 | **+37%** |
| uint16 pos [10K] | **237** | 172 | **+37%** |
| int32 neg [1K] | **562** | 421 | **+33%** |
| int32 neg [10K] | **579** | 421 | **+37%** |
| int32 pos [1K] | **551** | 323 | **+70%** |
| int32 pos [10K] | **559** | 405 | **+38%** |
| uint32 pos [1K] | **391** | 263 | **+48%** |
| uint32 pos [10K] | **391** | 252 | **+55%** |
| int64 neg [1K] | **977** | 860 | +13% |
| int64 neg [10K] | 804 | **864** | std +7% |
| int64 pos [1K] | **1137** | 839 | **+35%** |
| int64 pos [10K] | **1121** | 842 | **+33%** |
| uint64 pos [1K] | **643** | 390 | **+64%** |
| uint64 pos [10K] | **627** | 448 | **+39%** |

### macOS Apple M1 / Clang 22.1.4 — vn wins 24/24

| Test | vn MB/s | std MB/s | vn vs std |
|---|---|---|---|
| int8 neg [1K] | **71** | 54 | **+31%** |
| int8 neg [10K] | **69** | 53 | +29% |
| int8 pos [1K] | **72** | 54 | **+32%** |
| int8 pos [10K] | **61** | 49 | +23% |
| uint8 pos [1K] | **51** | 50 | +3% |
| uint8 pos [10K] | **54** | 51 | +6% |
| int16 neg [1K] | **137** | 111 | +23% |
| int16 neg [10K] | **141** | 113 | +24% |
| int16 pos [1K] | **146** | 114 | +28% |
| int16 pos [10K] | **147** | 101 | **+44%** |
| uint16 pos [1K] | **120** | 99 | +20% |
| uint16 pos [10K] | **118** | 99 | +19% |
| int32 neg [1K] | **284** | 216 | **+31%** |
| int32 neg [10K] | **287** | 214 | **+33%** |
| int32 pos [1K] | **289** | 214 | **+34%** |
| int32 pos [10K] | **294** | 224 | **+31%** |
| uint32 pos [1K] | **224** | 153 | **+46%** |
| uint32 pos [10K] | **226** | 151 | **+49%** |
| int64 neg [1K] | **575** | 387 | **+48%** |
| int64 neg [10K] | **571** | 424 | **+34%** |
| int64 pos [1K] | **583** | 436 | **+33%** |
| int64 pos [10K] | **572** | 422 | **+35%** |
| uint64 pos [1K] | **356** | 211 | **+69%** |
| uint64 pos [10K] | **351** | 212 | **+65%** |

---

## Notable Highlights

- **uint64 int-to-str:** `vn::to_chars` hits **7447 MB/s** on Linux/Clang and **7623 MB/s** on MSVC — 55–60% ahead of jeaiii, the prior fastest
- **int8 str-to-int (no leading zeros):** up to **+178%** over `std::from_chars` on Linux/Clang; **+170%** on Apple M1
- **int16 str-to-int:** up to **+137%** over std on Linux/Clang, **+185%** on Apple M1
- **int64 str-to-int:** peaks at **6071 MB/s** on Apple M1/Clang vs std at 2227 MB/s (**+172%**)
- **int64 str-to-int (Windows):** **4828 MB/s** vs std 4085 MB/s; consistent cross-platform leadership
- **str-to-int-leading-zeros:** near-total dominance — 24/24 on GCC, 24/24 on MSVC, 24/24 on Apple M1/Clang
- **Cross-platform consistency:** `vn` wins every str-to-int benchmark on Windows and Apple M1 with zero exceptions

---

*Benchmark suite: [BenchmarkSuite v1.0.2](https:github.com/realtimechris/benchmarksuite) | Compiler flags: strictest available per platform | ASAN/UBSAN clean*