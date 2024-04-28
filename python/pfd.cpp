#include "pfd.hpp"

#include <portable-file-dialogs.h>

#include <pybind11/stl.h>

namespace uikit::python {

namespace {

namespace py = pybind11;

} // namespace

void
def_pfd_module(pybind11::module_&& m)
{
  py::class_<pfd::open_file>(m, "OpenFileDialog")
    .def(py::init<std::string, std::string, std::vector<std::string>>(),
         py::arg("title"),
         py::arg("default_path") = "",
         py::arg("filters") = std::vector<std::string>{ "All Files", "*" })
    .def("result", &pfd::open_file::result)
    .def("ready", &pfd::open_file::ready, py::arg("timeout") = pfd::internal::default_wait_timeout)
    .def("kill", &pfd::open_file::kill);

  py::class_<pfd::save_file>(m, "SaveFileDialog")
    .def(py::init<std::string, std::string, std::vector<std::string>>(),
         py::arg("title"),
         py::arg("default_path") = "",
         py::arg("filters") = std::vector<std::string>{ "All Files", "*" })
    .def("result", &pfd::save_file::result)
    .def("ready", &pfd::save_file::ready, py::arg("timeout") = pfd::internal::default_wait_timeout)
    .def("kill", &pfd::save_file::kill);

  py::class_<pfd::select_folder>(m, "OpenFolderDialog")
    .def(py::init<std::string, std::string>(), py::arg("title"), py::arg("default_path"))
    .def("result", &pfd::select_folder::result)
    .def("ready", &pfd::select_folder::ready, py::arg("timeout") = pfd::internal::default_wait_timeout)
    .def("kill", &pfd::select_folder::kill);
}

} // namespace uikit::python
