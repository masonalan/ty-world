cmake_minimum_required(VERSION 3.27)
project(fractal-benchmark)

set(CMAKE_CXX_STANDARD 23)

find_package(glm CONFIG REQUIRED)
find_package(benchmark CONFIG REQUIRED)

include_directories(../include/)

add_executable(fractal-benchmark
        benchmark.cpp)

target_link_libraries(fractal-benchmark PRIVATE glm::glm)
target_link_libraries(fractal-benchmark PRIVATE benchmark::benchmark)
target_link_libraries(fractal-benchmark PRIVATE fractal)
