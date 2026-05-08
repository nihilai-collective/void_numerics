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

option(VN_ASAN "Enable AddressSanitizer" OFF)
option(VN_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)

if(APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(VN_ASAN)
        message(WARNING "VN_ASAN is not supported with GCC on macOS -- disabling.")
        set(VN_ASAN OFF CACHE BOOL "" FORCE)
    endif()
    if(VN_UBSAN)
        message(WARNING "VN_UBSAN is not supported with GCC on macOS -- disabling.")
        set(VN_UBSAN OFF CACHE BOOL "" FORCE)
    endif()
endif()

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_options(${PROJECT_NAME}
    INTERFACE
        $<$<CXX_COMPILER_ID:MSVC>:
            $<$<CONFIG:Release>:/O2 /Ob2 /GL /arch:AVX2 /fp:fast /GS- /Gy>
            $<$<CONFIG:Debug>:/Od /Zi /RTC1>
            $<$<BOOL:${VN_ASAN}>:/fsanitize=address>
            /arch:AVX2
        >
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
            $<$<AND:$<CONFIG:Release>,$<NOT:$<BOOL:${VN_ASAN}>>>:
                -O3
                -march=native
                -flto
                -ffast-math
                -finline-functions
                -fomit-frame-pointer
                -funroll-loops
                -fno-rtti
            >
            $<$<AND:$<CONFIG:Debug>,$<BOOL:${VN_ASAN}>>:
                -fsanitize=address
                -fno-omit-frame-pointer
                -fno-optimize-sibling-calls
                -fsanitize-address-use-after-scope
                -U_FORTIFY_SOURCE
                -D_FORTIFY_SOURCE=0
            >
            $<$<BOOL:${VN_UBSAN}>:
                -fsanitize=undefined
                -fno-sanitize-recover=all
            >
            $<$<AND:$<CONFIG:Release>,$<BOOL:${VN_ASAN}>>:
                -O1
                -ffast-math
                -fno-rtti
            >
            $<$<CONFIG:Debug>:
                -O0
                -g
                -fno-omit-frame-pointer
            >
        >
)

target_link_options(${PROJECT_NAME}
    INTERFACE
        $<$<CXX_COMPILER_ID:MSVC>:
            $<$<CONFIG:Release>:/LTCG /OPT:REF /OPT:ICF>
            $<$<BOOL:${VN_ASAN}>:/INFERASANLIBS>
        >
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
            $<$<AND:$<CONFIG:Release>,$<NOT:$<BOOL:${VN_ASAN}>>>:
                -flto
                $<$<PLATFORM_ID:Darwin>:-Wl,-x>
                $<$<NOT:$<PLATFORM_ID:Darwin>>:-s>
            >
            $<$<AND:$<PLATFORM_ID:Darwin>,$<CXX_COMPILER_ID:GNU>>:
                -L/opt/homebrew/lib/gcc/15
                -Wl,-rpath,/opt/homebrew/lib/gcc/15
            >
            $<$<BOOL:${VN_ASAN}>:
                -fsanitize=address
                $<$<AND:$<PLATFORM_ID:Linux>,$<CXX_COMPILER_ID:GNU>>:-static-libasan>
            >
            $<$<BOOL:${VN_UBSAN}>:
                -fsanitize=undefined
            >
        >
)

target_compile_definitions(${PROJECT_NAME}
    INTERFACE
        $<$<CONFIG:Release>:NDEBUG>
        VN_ARCH_ARM64=$<IF:$<OR:$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},aarch64>,$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},ARM64>,$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},arm64>>,1,0>
        VN_ARCH_X64=$<IF:$<OR:$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},x86_64>,$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},AMD64>>,1,0>
        $<$<CONFIG:Debug>:DEBUG _DEBUG>
        $<$<BOOL:${VN_ASAN}>:VN_ASAN_ENABLED>
        $<$<BOOL:${VN_UBSAN}>:VN_UBSAN_ENABLED>
        $<$<CONFIG:Debug>:VN_DEBUG=1>
        $<$<NOT:$<CONFIG:Debug>>:VN_DEBUG=0>
)