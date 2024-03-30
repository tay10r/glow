#include <uikit/main.hpp>
#include <uikit/viewport.hpp>

#include <AL/al.h>
#include <AL/alc.h>

#include <imgui.h>

#include <implot.h>

#include <algorithm>
#include <limits>

#include <cmath>
#include <cstdint>

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

#include <cassert>

namespace {

class app_impl final : public uikit::app
{
public:
  void setup(uikit::platform& plt) override
  {
    plt.set_app_name("uikit_demo_app");

    alcGetIntegerv(plt.get_audio_device(), AL_FREQUENCY, 1, &m_frequency);

    alGenSources(1, &m_audio_source);
    alGenBuffers(1, &m_audio_buffer);

    alSourcef(m_audio_source, AL_PITCH, 1);
    alSourcef(m_audio_source, AL_GAIN, 1);
    alSource3f(m_audio_source, AL_POSITION, 0, 0, -1);
    alSource3f(m_audio_source, AL_VELOCITY, 0, 0, 0);
    alSourcei(m_audio_source, AL_LOOPING, AL_FALSE);

    generate_audio_data();

    alSourceQueueBuffers(m_audio_source, 1, &m_audio_buffer);

    assert(alGetError() == AL_NO_ERROR);
  }

  void teardown(uikit::platform&) override
  {
    alDeleteSources(1, &m_audio_source);
    alDeleteBuffers(1, &m_audio_buffer);
  }

  void loop(uikit::platform& plt) override
  {
    if (plt.exit_requested()) {
      plt.queue_exit();
    }

    ImGui::Begin("Window");

    if (ImGui::BeginTabBar("Tabs")) {

      if (ImGui::BeginTabItem("Plot")) {

        render_plots();

        ImGui::EndTabItem();
      }

      if (ImGui::BeginTabItem("File Dialogs")) {

        render_file_dialog_buttons(plt);

        ImGui::EndTabItem();
      }

      if (ImGui::BeginTabItem("Audio")) {

        render_audio_buttons(plt);

        ImGui::EndTabItem();
      }

      ImGui::EndTabBar();
    }

    ImGui::End();
  }

protected:
  template<typename Scalar>
  static auto clamp(Scalar x, Scalar min, Scalar max) -> Scalar
  {
    return std::max(std::min(x, max), min);
  }

  void generate_audio_data()
  {
    const int num_samples{ m_frequency };

    std::vector<std::int16_t> samples;

    samples.resize(num_samples);

    const float dt{ 1.0f / static_cast<float>(m_frequency) };

    constexpr float tone_freq{ 440.0f };

    constexpr float amplitude{ 0.9f };

    for (int i = 0; i < num_samples; i++) {

      const float x = static_cast<float>(i) * dt * (M_PI * 2.0f);

      const float value = amplitude * std::sin(tone_freq * x);

      constexpr auto min16 = static_cast<int>(std::numeric_limits<std::int16_t>::min());
      constexpr auto max16 = static_cast<int>(std::numeric_limits<std::int16_t>::max());

      const int value16 = clamp(static_cast<int>(value * 0x7fff), min16, max16);

      samples[i] = static_cast<std::int16_t>(value16);
    }

    alBufferData(m_audio_buffer, AL_FORMAT_MONO16, samples.data(), samples.size(), m_frequency);
  }

  void render_plots()
  {
    if (ImPlot::BeginPlot("Example Plot")) {
      ImPlot::EndPlot();
    }
  }

  void render_file_dialog_buttons(uikit::platform& plt)
  {
    if (ImGui::Button("Open File")) {
      plt.open_file_dialog("Example File Dialog", {}, nullptr, nullptr);
    }

    if (ImGui::Button("Open Directory")) {
      plt.open_directory_dialog("Example Directory Dialog", nullptr, nullptr);
    }
  }

  void render_audio_buttons(uikit::platform& plt)
  {
    ALint source_state{};

    alGetSourcei(m_audio_source, AL_SOURCE_STATE, &source_state);

    if (source_state == AL_PLAYING) {
      ImGui::TextUnformatted("Audio State: Playing");
    } else {
      ImGui::TextUnformatted("Audio State: Stopped");
    }

    if (ImGui::Button("Play Audio")) {
      alSourcePlay(m_audio_source);
      assert(alGetError() == AL_NO_ERROR);
    }
  }

private:
  std::unique_ptr<uikit::viewport> m_viewport;

  ALuint m_audio_source{};

  ALuint m_audio_buffer{};

  ALint m_frequency{ 44100 };
};

} // namespace

namespace uikit {

auto
app::create() -> std::unique_ptr<app>
{
  return std::unique_ptr<app>(new app_impl());
}

} // namespace uikit
