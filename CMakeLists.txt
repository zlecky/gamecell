cmake_minimum_required(VERSION 3.2)
project(a-bit-helper)

# googletest and gbenchmark version
set(GTEST_VERSION 1.10.0 CACHE STRING "googletest version")
set(GBENCHMARK_VERSION 1.5.2 CACHE STRING "gbenchmark version")

set(BENCHMARK_ENABLE_TESTING on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -fPIC")

# libs path
set(INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(INSTALL_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(INSTALL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

# some modules cmake dirs
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(spdlog)
include(googletest)
include(gbenchmark)

# include dirs
include_directories(include)

# sub dirs
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(bench)
add_subdirectory(examples)
#add_subdirectory(3party)
