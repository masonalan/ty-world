cmake_minimum_required(VERSION 3.27)
project(lighting)

set(CMAKE_CXX_STANDARD 23)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)

add_executable(lighting main.cpp
        shader.cpp
        shader.hpp
        shapes.cpp
        shapes.hpp
        camera.cpp
        camera.hpp
        texture.cpp
        texture.hpp
        material.cpp
        material.hpp
        light.cpp
        light.hpp)

target_link_libraries(lighting PRIVATE glad::glad)
target_link_libraries(lighting PRIVATE glfw)
target_link_libraries(lighting PRIVATE glm::glm)

target_include_directories(lighting PRIVATE ${Stb_INCUDE_DIR})

add_custom_command(
        TARGET lighting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/shaders/main_shader.frag
        ${PROJECT_BINARY_DIR}/main_shader.frag)
add_custom_command(
        TARGET lighting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/shaders/light_shader.frag
        ${PROJECT_BINARY_DIR}/light_shader.frag)
add_custom_command(
        TARGET lighting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/shaders/main_shader.vert
        ${PROJECT_BINARY_DIR}/main_shader.vert)
add_custom_command(
        TARGET lighting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/shaders/instanced_shader.vert
        ${PROJECT_BINARY_DIR}/instanced_shader.vert)

add_custom_command(
        TARGET lighting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/resources/gradient-logo.png
        ${PROJECT_BINARY_DIR}/gradient-logo.png)
add_custom_command(
        TARGET lighting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/resources/large-logo.png
        ${PROJECT_BINARY_DIR}/large-logo.png)
add_custom_command(
        TARGET lighting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/resources/palette.png
        ${PROJECT_BINARY_DIR}/palette.png)
add_custom_command(
        TARGET lighting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/resources/nav-play-inverse.png
        ${PROJECT_BINARY_DIR}/nav-play-inverse.png)
add_custom_command(
        TARGET lighting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/resources/nav-play.png
        ${PROJECT_BINARY_DIR}/nav-play.png)
add_custom_command(
        TARGET lighting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/resources/logo.png
        ${PROJECT_BINARY_DIR}/logo.png)
add_custom_command(
        TARGET lighting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/resources/circle-logo.png
        ${PROJECT_BINARY_DIR}/circle-logo.png)
add_custom_command(
        TARGET lighting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/resources/btn-play.png
        ${PROJECT_BINARY_DIR}/btn-play.png)
add_custom_command(
        TARGET lighting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/resources/face.png
        ${PROJECT_BINARY_DIR}/face.png)
add_custom_command(
        TARGET lighting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/resources/face5.png
        ${PROJECT_BINARY_DIR}/face5.png)
add_custom_command(
        TARGET lighting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/resources/win-bg.png
        ${PROJECT_BINARY_DIR}/win-bg.png)