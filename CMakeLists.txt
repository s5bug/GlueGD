cmake_minimum_required(VERSION 3.15)

project(GlueGD)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

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
)
target_compile_definitions(GlueGD PRIVATE -DUNICODE -D_UNICODE)
target_include_directories(GlueGD PRIVATE)
target_link_libraries(GlueGD)
