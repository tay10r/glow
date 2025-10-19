cmake_minimum_required(VERSION 3.14.7)

find_package(Python COMPONENTS Interpreter Development)

if(GLOW_BUILD_PYBIND11)
  include(../cmake/pybind11.cmake)
else()
  find_package(pybind11 CONFIG)
endif()

pybind11_add_module(glowpy
  python.cpp
  gl.hpp
  gl.cpp
  glfw.hpp
  glfw.cpp
  imgui.hpp
  imgui.cpp
  implot.hpp
  implot.cpp
  pfd.hpp
  pfd.cpp)

target_link_libraries(glowpy PUBLIC glow::glow)

target_compile_features(glowpy PRIVATE cxx_std_17)

target_compile_definitions(glowpy PRIVATE PYBIND11_DETAILED_ERROR_MESSAGES=1)

if(GLOW_INSTALL)
  install(TARGETS glowpy
    ARCHIVE DESTINATION .
    LIBRARY DESTINATION .
    RUNTIME DESTINATION .)
endif()
