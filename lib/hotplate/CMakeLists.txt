cmake_minimum_required(VERSION 3.27)
project(hotplate)

set(CMAKE_CXX_STANDARD 23)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)

include_directories(include)

add_library(hotplate
        src/shader.cpp
        include/hotplate/shader.hpp
        src/shapes.cpp
        include/hotplate/shapes.hpp
        src/camera.cpp
        include/hotplate/camera.hpp
        src/texture.cpp
        include/hotplate/texture.hpp
        src/material.cpp
        include/hotplate/material.hpp
        src/light.cpp
        include/hotplate/light.hpp)

target_link_libraries(hotplate PRIVATE glad::glad)
target_link_libraries(hotplate PRIVATE glfw)
target_link_libraries(hotplate PRIVATE glm::glm)
target_link_libraries(hotplate PRIVATE fractal)

target_include_directories(hotplate PRIVATE ${Stb_INCUDE_DIR})

add_custom_command(
        TARGET hotplate POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/shader/main_shader.frag
        ${PROJECT_BINARY_DIR}/main_shader.frag)
add_custom_command(
        TARGET hotplate POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}//shader/light_shader.frag
        ${PROJECT_BINARY_DIR}/light_shader.frag)
add_custom_command(
        TARGET hotplate POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}//shader/main_shader.vert
        ${PROJECT_BINARY_DIR}/main_shader.vert)
add_custom_command(
        TARGET hotplate POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}//shader/instanced_shader.vert
        ${PROJECT_BINARY_DIR}/instanced_shader.vert)

target_include_directories(hotplate PRIVATE ${Stb_INCUDE_DIR})