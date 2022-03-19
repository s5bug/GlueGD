cmake_minimum_required(VERSION 3.15)

project(GlueGD)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include_directories(external/include)

find_package(OpenGL REQUIRED)
add_subdirectory(external/subproject/imgui)
add_subdirectory(external/subproject/lua543)
add_subdirectory(external/subproject/ztd)

if(NOT MSVC)
    message(FATAL_ERROR "GlueGD must be built using MSVC.")
endif()

add_compile_options(/W4 /WX)

# The library is named XINPUT9_1_0.dll as to be automatically loaded by GD
add_library(XINPUT9_1_0 SHARED GlueGDLoader/main.def GlueGDLoader/main.cpp)
target_compile_definitions(XINPUT9_1_0 PRIVATE -DUNICODE -D_UNICODE)
target_include_directories(XINPUT9_1_0 PRIVATE external/subproject/lua543)
target_link_libraries(XINPUT9_1_0 ImageHlp PathCch lua54 ztd::text)

add_library(GlueGD SHARED
    GlueGD/glue.cpp
    GlueGD/main.cpp
)
target_compile_definitions(GlueGD PRIVATE -DUNICODE -D_UNICODE -DIMGUI_IMPL_OPENGL_ES2)
add_dependencies(GlueGD ztd::text)
target_include_directories(GlueGD PRIVATE external/subproject/imgui external/subproject/lua543)
target_link_directories(GlueGD PRIVATE external/lib/win32)
target_link_libraries(GlueGD OpenGL::GL cocos2dx cocos2dext ImGui lua54)
