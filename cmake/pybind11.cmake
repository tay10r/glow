include_guard()

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)

FetchContent_Declare(pybind11 URL "https://github.com/pybind/pybind11/archive/refs/tags/v2.12.0.zip")
FetchContent_MakeAvailable(pybind11)
