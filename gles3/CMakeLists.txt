cmake_minimum_required(VERSION 3.14.7)

add_library(glow_gles3
  src/glad.c
)

target_include_directories(glow_gles3
  PUBLIC
    include
)

add_library(glow::gles3 ALIAS glow_gles3)
