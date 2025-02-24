cmake_minimum_required(VERSION 3.27)
project(lighting)

set(CMAKE_CXX_STANDARD 23)

find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Stb REQUIRED)

add_subdirectory(lib)

include_directories(lib/fractal/include)
include_directories(lib/hotplate/include)

add_executable(lighting main.cpp
        graph.hpp
        graph.cpp
        ChunkEntity.cpp
        ChunkEntity.hpp
        quad.hpp
        benchmark.hpp
        benchmark.cpp
        render.hpp
        render.cpp
        direction.hpp
        quad.cpp
        chunk_entity.cpp
        chunk_entity.hpp)

target_link_libraries(lighting glad::glad)
target_link_libraries(lighting glfw)
target_link_libraries(lighting glm::glm)
target_link_libraries(lighting fractal)
target_link_libraries(lighting hotplate)
