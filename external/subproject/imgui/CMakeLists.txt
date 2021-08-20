cmake_minimum_required(VERSION 3.15)

project(ImGui)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ImGui STATIC
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_win32.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)
target_compile_definitions(ImGui PRIVATE -DIMGUI_IMPL_OPENGL_ES2)
target_link_libraries(ImGui glew32)
