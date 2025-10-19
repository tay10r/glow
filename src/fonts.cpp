#include <glow/fonts.hpp>

#include <cmrc/cmrc.hpp>

#include <cstdlib>
#include <cstring>

CMRC_DECLARE(glow_font_data);

namespace glow {

ImFont*
open_font(const char* resource_path, float font_size, const ImFontConfig* config, const ImWchar* glyph_ranges)
{
  const auto fs = cmrc::glow_font_data::get_filesystem();

  const auto file = fs.open(resource_path);

  void* data = std::malloc(file.size());
  if (!data) {
    return nullptr;
  }

  std::memcpy(data, file.begin(), file.size());

  return ImGui::GetIO().Fonts->AddFontFromMemoryTTF(data, file.size(), font_size, config, glyph_ranges);
}

} // namespace glow
