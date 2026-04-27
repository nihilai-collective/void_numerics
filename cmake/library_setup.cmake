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
        >
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
            $<$<CONFIG:Release>:-O3 -march=native -flto -fno-exceptions -ffast-math -finline-functions -fomit-frame-pointer -funroll-loops -fno-rtti>
            $<$<CONFIG:Debug>:-O0 -g -fno-omit-frame-pointer>
        >
)

target_link_options(${PROJECT_NAME}
    INTERFACE
        $<$<CXX_COMPILER_ID:MSVC>:
            $<$<CONFIG:Release>:/LTCG /OPT:REF /OPT:ICF>
        >
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
            $<$<CONFIG:Release>:-flto -s>
        >
)

target_compile_definitions(${PROJECT_NAME}
    INTERFACE
        $<$<CONFIG:Release>:NDEBUG>
        $<$<CONFIG:Debug>:DEBUG _DEBUG>
)