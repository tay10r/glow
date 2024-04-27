cmake_minimum_required(VERSION 3.14.7)

find_package(Python COMPONENTS Interpreter Development)

if(UIKIT_BUILD_PYBIND11)
  include(../cmake/pybind11.cmake)
else()
  find_package(pybind11 CONFIG)
endif()

pybind11_add_module(libs
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

target_link_libraries(libs PUBLIC uikit::uikit)

target_compile_features(libs PRIVATE cxx_std_17)

target_compile_definitions(libs PRIVATE PYBIND11_DETAILED_ERROR_MESSAGES=1)

if(UIKIT_INSTALL)
  install(TARGETS libs
    ARCHIVE DESTINATION uikitpy
    LIBRARY DESTINATION uikitpy
    RUNTIME DESTINATION uikitpy)
  install(FILES __init__.py
    DESTINATION uikitpy)
endif()
