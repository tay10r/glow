cmake_minimum_required(VERSION 3.14.7)

add_library(glad_gles3 src/glad.c)
target_include_directories(glad_gles3 PUBLIC include)
add_library(glad::gles3 ALIAS glad_gles3)
add_library(glad::glad ALIAS glad_gles3)
