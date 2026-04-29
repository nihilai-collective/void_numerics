# MIT License
# 
# Copyright (c) 2026 Nihilai Collective Corp
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
include(FetchContent)

option(VN_ASAN "Enable AddressSanitizer" OFF)
option(VN_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)

set(VN_CLANG_COMPILE_OPTIONS
    $<$<CONFIG:Debug>:-O0>
    $<$<CONFIG:Debug>:-g>
    -Weverything
    -Werror
    -Wno-undef
    -Wno-unsafe-buffer-usage
    -Wno-poison-system-directories
    -Wno-c++20-compat
    -Wno-padded
    -Wno-zero-as-null-pointer-constant
    -Wno-c++98-compat-pedantic
    -Wno-c++98-compat
)

set(VN_GNU_COMPILE_OPTIONS
    $<$<CONFIG:Debug>:-O0>
    $<$<CONFIG:Debug>:-g>
    -Wpedantic
    -Wextra
    -Werror
    -Wall
    $<$<NOT:$<CONFIG:Debug>>:-fwhole-program>
)

set(VN_MSVC_COMPILE_OPTIONS
    /W4
    /WX
)

set(VN_COMPILE_OPTIONS
    $<$<CXX_COMPILER_ID:AppleClang>:${VN_CLANG_COMPILE_OPTIONS}>
    $<$<CXX_COMPILER_ID:Clang>:${VN_CLANG_COMPILE_OPTIONS}>
    $<$<CXX_COMPILER_ID:MSVC>:${VN_MSVC_COMPILE_OPTIONS}>
    $<$<CXX_COMPILER_ID:GNU>:${VN_GNU_COMPILE_OPTIONS}>
)

FetchContent_Declare(
  rt-ut
  GIT_REPOSITORY https://github.com/realtimechris/rt-ut.git
  GIT_TAG main
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(rt-ut)

add_executable(vn_unit_tests "void_numerics.cpp")

target_link_libraries(vn_unit_tests PUBLIC void-numerics::void-numerics rt-ut::rt-ut)

target_compile_options(vn_unit_tests PUBLIC ${VN_COMPILE_OPTIONS})

if(VN_ASAN)
    target_compile_definitions(vn_unit_tests PUBLIC VN_ASAN_ENABLED)
endif()

if(VN_UBSAN)
    target_compile_definitions(vn_unit_tests PUBLIC VN_UBSAN_ENABLED)
endif()

target_compile_options(vn_unit_tests PUBLIC
    $<$<CXX_COMPILER_ID:MSVC>:
        $<$<BOOL:${VN_ASAN}>:/fsanitize=address>
    >
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
        $<$<BOOL:${VN_ASAN}>:
            -fsanitize=address
            -fsanitize-address-use-after-scope
            -U_FORTIFY_SOURCE
            -D_FORTIFY_SOURCE=0
        >
        $<$<BOOL:${VN_UBSAN}>:
            -fsanitize=undefined
            -fno-sanitize-recover=all
        >
    >
    $<$<AND:$<CXX_COMPILER_ID:GNU>,$<NOT:$<PLATFORM_ID:Darwin>>>:
        $<$<BOOL:${VN_ASAN}>:
            -fsanitize=address
            -fsanitize-address-use-after-scope
            -U_FORTIFY_SOURCE
            -D_FORTIFY_SOURCE=0
        >
        $<$<BOOL:${VN_UBSAN}>:
            -fsanitize=undefined
            -fno-sanitize-recover=all
        >
    >
)

target_link_options(vn_unit_tests PUBLIC
    $<$<CXX_COMPILER_ID:MSVC>:
        $<$<BOOL:${VN_ASAN}>:/INFERASANLIBS>
    >
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
        $<$<BOOL:${VN_ASAN}>:-fsanitize=address>
        $<$<BOOL:${VN_UBSAN}>:-fsanitize=undefined>
    >
    $<$<AND:$<CXX_COMPILER_ID:GNU>,$<NOT:$<PLATFORM_ID:Darwin>>>:
        $<$<BOOL:${VN_ASAN}>:
            -fsanitize=address
            -static-libasan
        >
        $<$<BOOL:${VN_UBSAN}>:-fsanitize=undefined>
    >
)

install(
    FILES
        $<TARGET_FILE:vn_unit_tests>
    DESTINATION bin
    OPTIONAL
)