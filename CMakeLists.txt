cmake_minimum_required(VERSION 3.10)
project(umbra)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libs/glfw)
add_subdirectory(libs/openal)

include_directories(
    libs/glfw/include
    libs/glm
    libs/glad/include
    libs/imgui
    libs/stb
    libs/openal/include
    libs/json/single_include
)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

set(GLAD_FILES 
    libs/glad/src/glad.c
)

set(IMGUI_FILES
    libs/imgui/imgui.cpp
    libs/imgui/imgui_draw.cpp 
    libs/imgui/imgui_tables.cpp
    libs/imgui/imgui_widgets.cpp
    libs/imgui/backends/imgui_impl_glfw.cpp
    libs/imgui/backends/imgui_impl_opengl3.cpp
)

add_executable(umbra ${SRC_FILES} ${GLAD_FILES} ${IMGUI_FILES})

target_link_libraries(umbra PRIVATE glfw OpenAL::OpenAL)

file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
