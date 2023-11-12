cmake_minimum_required(VERSION 3.14.7)

add_library(glad_gles2 src/glad.c)
target_include_directories(glad_gles2 PUBLIC include)
add_library(glad::gles2 ALIAS glad_gles2)
add_library(glad::glad ALIAS glad_gles2)
