cmake_minimum_required(VERSION 3.10)
project(LinearCPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Release)

include_directories(MatrixLibrary/include)

include_directories(externalEigen)

find_package(benchmark REQUIRED)
find_package(Threads REQUIRED)

add_executable(matrix_bench MatrixLibrary/benchmarks/bench_matrix.cpp)

target_link_libraries(matrix_bench
    PRIVATE
    benchmark::benchmark
    Threads::Threads
)