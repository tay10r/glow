include_guard()

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)

FetchContent_Declare(pybind11 URL "${PYBIND11_URL}")
FetchContent_MakeAvailable(pybind11)
