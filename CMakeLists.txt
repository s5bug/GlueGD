cmake_minimum_required(VERSION 3.15)

project(GlueGD)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(OpenGL REQUIRED)

if(NOT MSVC)
    message(FATAL_ERROR "GlueGD must be built using MSVC.")
endif()

add_compile_options(/W4 /WX)

# The library is named XINPUT9_1_0.dll as to be automatically loaded by GD
add_library(XINPUT9_1_0 SHARED GlueGDLoader/main.def GlueGDLoader/main.cpp)
target_compile_definitions(XINPUT9_1_0 PRIVATE -DUNICODE -D_UNICODE)
target_link_libraries(XINPUT9_1_0 ImageHlp PathCch)

add_library(GlueGD SHARED
    GlueGD/main.cpp

    GlueGD/imgui/imgui.cpp
    GlueGD/imgui/imgui_demo.cpp
    GlueGD/imgui/imgui_draw.cpp
    GlueGD/imgui/imgui_tables.cpp
    GlueGD/imgui/imgui_widgets.cpp
    GlueGD/imgui/backends/imgui_impl_win32.cpp
    GlueGD/imgui/backends/imgui_impl_opengl3.cpp
)
target_compile_definitions(GlueGD PRIVATE -DUNICODE -D_UNICODE -DIMGUI_IMPL_OPENGL_ES2)
target_include_directories(GlueGD PRIVATE external/include)
target_link_directories(GlueGD PRIVATE external/lib/win32)
target_link_libraries(GlueGD OpenGL::GL cocos2dx cocos2dext glew32)
