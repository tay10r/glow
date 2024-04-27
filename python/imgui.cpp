#include "imgui.hpp"

#include <pybind11/stl.h>

#include <imgui.h>

#include <cstdint>

namespace uikit::python {

namespace {

namespace py = pybind11;

using u32 = std::uint32_t;

auto
begin(const char* label, ImGuiWindowFlags flags)
{
  return ImGui::Begin(label, nullptr, flags);
}

enum class WindowFlags : u32
{
  None = ImGuiWindowFlags_None,
  NoTitleBar = ImGuiWindowFlags_NoTitleBar,
  NoResize = ImGuiWindowFlags_NoResize,
  NoMove = ImGuiWindowFlags_NoMove,
  NoScrollbar = ImGuiWindowFlags_NoScrollbar,
  NoScrollWithMouse = ImGuiWindowFlags_NoScrollWithMouse,
  NoCollapse = ImGuiWindowFlags_NoCollapse,
  AlwaysAutoResize = ImGuiWindowFlags_AlwaysAutoResize,
  NoBackground = ImGuiWindowFlags_NoBackground,
  NoSavedSettings = ImGuiWindowFlags_NoSavedSettings,
  NoMouseInputs = ImGuiWindowFlags_NoMouseInputs,
  MenuBar = ImGuiWindowFlags_MenuBar,
  HorizontalScrollbar = ImGuiWindowFlags_HorizontalScrollbar,
  NoFocusOnAppearing = ImGuiWindowFlags_NoFocusOnAppearing,
  NoBringToFrontOnFocus = ImGuiWindowFlags_NoBringToFrontOnFocus,
  AlwaysVerticalScrollbar = ImGuiWindowFlags_AlwaysVerticalScrollbar,
  AlwaysHorizontalScrollbar = ImGuiWindowFlags_AlwaysHorizontalScrollbar,
  NoNavInputs = ImGuiWindowFlags_NoNavInputs,
  NoNavFocus = ImGuiWindowFlags_NoNavFocus,
  UnsavedDocument = ImGuiWindowFlags_UnsavedDocument,
  NoNav = ImGuiWindowFlags_NoNav,
  NoDecoration = ImGuiWindowFlags_NoDecoration,
  NoInputs = ImGuiWindowFlags_NoInputs
};

enum class SelectableFlags : u32
{
  None = ImGuiSelectableFlags_None,
  DontClosePopups = ImGuiSelectableFlags_DontClosePopups,
  SpanAllColumns = ImGuiSelectableFlags_SpanAllColumns,
  AllowDoubleClick = ImGuiSelectableFlags_AllowDoubleClick,
  Disabled = ImGuiSelectableFlags_Disabled,
  AllowOverlap = ImGuiSelectableFlags_AllowOverlap
};

auto
selectable(const char* label, bool selected, const SelectableFlags flags, const ImVec2& size) -> py::tuple
{
  const auto changed = ImGui::Selectable(label, &selected, static_cast<ImGuiSelectableFlags>(flags), size);
  return py::make_tuple(changed, selected);
}

auto
checkbox(const char* label, bool value) -> py::tuple
{
  auto changed = ImGui::Checkbox(label, &value);
  return py::make_tuple(changed, value);
}

enum class SliderFlags : u32
{
  None = ImGuiSliderFlags_None,
  AlwaysClamp = ImGuiSliderFlags_AlwaysClamp,
  Logarithmic = ImGuiSliderFlags_Logarithmic,
  NoRoundToFormat = ImGuiSliderFlags_NoRoundToFormat,
  NoInput = ImGuiSliderFlags_NoInput
};

auto
slider_int(const char* label, int value, const int v_min, const int v_max, const char* format, const SliderFlags flags)
  -> py::tuple
{
  const auto changed = ImGui::SliderInt(label, &value, v_min, v_max, format, static_cast<ImGuiSliderFlags>(flags));
  return py::make_tuple(changed, value);
}

auto
slider_float(const char* label,
             float value,
             const float v_min,
             const float v_max,
             const char* format,
             const SliderFlags flags) -> py::tuple
{
  const auto changed = ImGui::SliderFloat(label, &value, v_min, v_max, format, static_cast<ImGuiSliderFlags>(flags));
  return py::make_tuple(changed, value);
}

auto
slider_angle(const char* label,
             float value,
             const float v_min,
             const float v_max,
             const char* format,
             const SliderFlags flags) -> py::tuple
{
  const auto changed = ImGui::SliderAngle(label, &value, v_min, v_max, format, static_cast<ImGuiSliderFlags>(flags));
  return py::make_tuple(changed, value);
}

auto
menu_item(const char* label, const std::optional<std::string>& shortcut, const bool selected, const bool enabled)
  -> bool
{
  const char* shortcut_ptr = shortcut.has_value() ? shortcut->c_str() : nullptr;
  return ImGui::MenuItem(label, shortcut_ptr, selected, enabled);
}

enum class TabBarFlags : u32
{
  None = ImGuiTabBarFlags_None,
  Reorderable = ImGuiTabBarFlags_Reorderable,
  AutoSelectNewTabs = ImGuiTabBarFlags_AutoSelectNewTabs,
  TabListPopupButton = ImGuiTabBarFlags_TabListPopupButton,
  NoCloseWithMiddleMouseButton = ImGuiTabBarFlags_NoCloseWithMiddleMouseButton,
  NoTabListScrollingButtons = ImGuiTabBarFlags_NoTabListScrollingButtons,
  NoTooltip = ImGuiTabBarFlags_NoTooltip,
  FittingPolicyResizeDown = ImGuiTabBarFlags_FittingPolicyResizeDown,
  FittingPolicyScroll = ImGuiTabBarFlags_FittingPolicyScroll
};

auto
begin_tab_bar(const char* label, const TabBarFlags flags) -> bool
{
  return ImGui::BeginTabBar(label, static_cast<ImGuiTabBarFlags>(flags));
}

enum class TabItemFlags : u32
{
  None = ImGuiTabItemFlags_None,
  UnsavedDocument = ImGuiTabItemFlags_UnsavedDocument,
  SetSelected = ImGuiTabItemFlags_SetSelected,
  NoCloseWithMiddleMouseButton = ImGuiTabItemFlags_NoCloseWithMiddleMouseButton,
  NoPushID = ImGuiTabItemFlags_NoPushId,
  NoTooltip = ImGuiTabItemFlags_NoTooltip,
  NoReorder = ImGuiTabItemFlags_NoReorder,
  Leading = ImGuiTabItemFlags_Leading,
  Trailing = ImGuiTabItemFlags_Trailing
};

auto
begin_tab_item(const char* label, const TabItemFlags flags) -> bool
{
  return ImGui::BeginTabItem(label, nullptr, static_cast<ImGuiTabItemFlags>(flags));
}

} // namespace

void
def_imgui_module(pybind11::module& m)
{
  py::class_<ImVec2>(m, "Vec2")
    .def(py::init<float, float>(), py::arg("x"), py::arg("y"))
    .def_readwrite("x", &ImVec2::x)
    .def_readwrite("y", &ImVec2::y);

  py::class_<ImVec4>(m, "Vec4")
    .def(py::init<float, float, float, float>(), py::arg("x"), py::arg("y"), py::arg("z"), py::arg("w"))
    .def_readwrite("x", &ImVec4::x)
    .def_readwrite("y", &ImVec4::y)
    .def_readwrite("z", &ImVec4::z)
    .def_readwrite("w", &ImVec4::w);

  py::enum_<WindowFlags>(m, "WindowFlags")
    .value("NONE", WindowFlags::None)
    .value("NO_TITLE_BAR", WindowFlags::NoTitleBar)
    .value("NO_RESIZE", WindowFlags::NoResize)
    .value("NO_COLLAPSE", WindowFlags::NoCollapse)
    .value("ALWAYS_AUTO_RESIZE", WindowFlags::AlwaysAutoResize)
    .value("NO_BACKGROUND", WindowFlags::NoBackground)
    .value("NO_SAVED_SETTINGS", WindowFlags::NoSavedSettings)
    .value("NO_MOUSE_INPUTS", WindowFlags::NoMouseInputs)
    .value("MENU_BAR", WindowFlags::MenuBar)
    .value("HORIZONTAL_SCROLLBAR", WindowFlags::HorizontalScrollbar)
    .value("NO_FOCUS_ON_APPEARING", WindowFlags::NoFocusOnAppearing)
    .value("NO_BRING_TO_FRONT_ON_FOCUS", WindowFlags::NoBringToFrontOnFocus)
    .value("ALWAYS_VERTICAL_SCROLLBAR", WindowFlags::AlwaysVerticalScrollbar)
    .value("ALWAYS_HORIZONTAL_SCROLLBAR", WindowFlags::AlwaysHorizontalScrollbar)
    .value("NO_NAV_INPUTS", WindowFlags::NoNavInputs)
    .value("NO_NAV_FOCUS", WindowFlags::NoNavFocus)
    .value("UNSAVED_DOCUMENT", WindowFlags::UnsavedDocument)
    .value("NO_NAV", WindowFlags::NoNav)
    .value("NO_DECORATION", WindowFlags::NoDecoration)
    .value("NO_INPUTS", WindowFlags::NoInputs)
    .def("__or__", [](const WindowFlags a, const WindowFlags b) -> WindowFlags {
      return static_cast<WindowFlags>(static_cast<u32>(a) | static_cast<u32>(b));
    });

  m.def("begin", &begin, "Begins rendering to a new widget.", py::arg("label"), py::arg("flags") = WindowFlags::None);
  m.def("end", &ImGui::End, "Completes the rendering of a widget.");

  m.def("begin_main_menu_bar", &ImGui::BeginMainMenuBar);
  m.def("end_menu_bar", &ImGui::EndMenuBar);

  m.def("begin_menu", &ImGui::BeginMenu, py::arg("label"), py::arg("enabled") = true);
  m.def("end_menu", &ImGui::EndMenu);
  m.def("menu_item",
        &menu_item,
        py::arg("label"),
        py::arg("shortcut") = std::optional<std::string>{},
        py::arg("selected") = false,
        py::arg("enabled") = true);

  py::enum_<SelectableFlags>(m, "SelectableFlags")
    .value("NONE", SelectableFlags::None)
    .value("DONT_CLOSE_POPUPS", SelectableFlags::DontClosePopups)
    .value("SPAN_ALL_COLUMNS", SelectableFlags::SpanAllColumns)
    .value("ALLOW_DOUBLE_CLICK", SelectableFlags::AllowDoubleClick)
    .value("DISABLED", SelectableFlags::Disabled)
    .value("ALLOW_OVERLAP", SelectableFlags::AllowOverlap)
    .def("__or__", [](const SelectableFlags left, const SelectableFlags right) -> SelectableFlags {
      return static_cast<SelectableFlags>(static_cast<u32>(left) | static_cast<u32>(right));
    });
  m.def("selectable",
        &selectable,
        py::arg("label"),
        py::arg("selected") = false,
        py::arg("flags") = SelectableFlags::None,
        py::arg("size") = ImVec2(0, 0));

  m.def("button", &ImGui::Button, py::arg("label"), py::arg("size") = ImVec2(0, 0));
  m.def("separator", &ImGui::Separator);
  m.def("same_line", &ImGui::SameLine);
  m.def("checkbox", &checkbox, py::arg("label"), py::arg("value"));

  py::enum_<SliderFlags>(m, "SliderFlags")
    .value("NONE", SliderFlags::None)
    .value("ALWAYS_CLAMP", SliderFlags::AlwaysClamp)
    .value("LOGARITHMIC", SliderFlags::Logarithmic)
    .value("NO_ROUND_TO_FORMAT", SliderFlags::NoRoundToFormat)
    .value("NO_INPUT", SliderFlags::NoInput)
    .def("__or__", [](const SliderFlags left, const SliderFlags right) -> SliderFlags {
      return static_cast<SliderFlags>(static_cast<u32>(left) | static_cast<u32>(right));
    });
  m.def("slider_int",
        &slider_int,
        py::arg("label"),
        py::arg("value"),
        py::arg("v_min"),
        py::arg("v_max"),
        py::arg("format") = "%d",
        py::arg("flags") = SliderFlags::None);
  m.def("slider_float",
        &slider_float,
        py::arg("label"),
        py::arg("value"),
        py::arg("v_min"),
        py::arg("v_max"),
        py::arg("format") = "%.3f",
        py::arg("flags") = SliderFlags::None);
  m.def("slider_angle",
        &slider_angle,
        py::arg("label"),
        py::arg("value"),
        py::arg("v_min") = -360.0f,
        py::arg("v_max") = 360.0f,
        py::arg("format") = "%.0f",
        py::arg("flags") = SliderFlags::None);

  py::enum_<TabBarFlags>(m, "TabBarFlags")
    .value("NONE", TabBarFlags::None)
    .value("REORDERABLE", TabBarFlags::Reorderable)
    .value("AUTO_SELECT_NEW_TABS", TabBarFlags::AutoSelectNewTabs)
    .value("TAB_LIST_POPUP_BUTTON", TabBarFlags::TabListPopupButton)
    .value("NO_CLOSE_WITH_MIDDLE_MOUSE_BUTTON", TabBarFlags::NoCloseWithMiddleMouseButton)
    .value("NO_TAB_LIST_SCROLLING_BUTTONS", TabBarFlags::NoTabListScrollingButtons)
    .value("NO_TOOLTIP", TabBarFlags::NoTooltip)
    .value("FITTING_POLICY_RESIZE_DOWN", TabBarFlags::FittingPolicyResizeDown)
    .value("FITTING_POLICY_SCROLL", TabBarFlags::FittingPolicyScroll);
  m.def("begin_tab_bar", &begin_tab_bar, py::arg("label"), py::arg("flags") = TabBarFlags::None);
  m.def("end_tab_bar", &ImGui::EndTabBar);

  py::enum_<TabItemFlags>(m, "TabItemFlags")
    .value("NONE", TabItemFlags::None)
    .value("UNSAVED_DOCUMENT", TabItemFlags::UnsavedDocument)
    .value("SET_SELECTED", TabItemFlags::SetSelected)
    .value("NO_CLOSE_WITH_MIDDLE_MOUSE_BUTTON", TabItemFlags::NoCloseWithMiddleMouseButton)
    .value("NO_PUSH_ID", TabItemFlags::NoPushID)
    .value("NO_TOOLTIP", TabItemFlags::NoTooltip)
    .value("NO_REORDER", TabItemFlags::NoReorder)
    .value("LEADING", TabItemFlags::Leading)
    .value("TRAILING", TabItemFlags::Trailing);

  m.def("begin_tab_item", &begin_tab_item, py::arg("label"), py::arg("flags") = TabItemFlags::None);
  m.def("end_tab_item", &ImGui::EndTabItem);
}

} // namespace uikit::python
