cmake_minimum_required(VERSION 3.27)
project(fractal)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconstexpr-steps=1000000000")

add_subdirectory(benchmark)

find_package(glm CONFIG REQUIRED)

include_directories(include)

add_library(fractal
        include/fractal/fractal.hpp
        include/fractal/gradient.hpp
        src/gradient.cpp
        benchmark/benchmark.cpp
        include/fractal/chunk.hpp)

target_link_libraries(fractal PRIVATE glm::glm)
