include_guard()

include(FetchContent)

FetchContent_Declare(pybind11 URL "https://github.com/pybind/pybind11/archive/refs/tags/v2.12.0.zip")
FetchContent_MakeAvailable(pybind11)
