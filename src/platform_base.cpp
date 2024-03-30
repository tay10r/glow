#include "platform_base.h"

#include <AL/al.h>

#include <AL/alc.h>

namespace uikit {

class platform_base::impl final
{
public:
  impl()
    : m_audio_device(alcOpenDevice(nullptr))
    , m_audio_context(alcCreateContext(m_audio_device, nullptr))
  {
    alcMakeContextCurrent(m_audio_context);

    const ALfloat listener_orientation[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

    alListener3f(AL_POSITION, 0, 0, 0);
    alListener3f(AL_VELOCITY, 0, 0, 0);
    alListenerfv(AL_ORIENTATION, listener_orientation);
  }

  ~impl()
  {
    alcDestroyContext(m_audio_context);

    alcCloseDevice(m_audio_device);
  }

  auto get_audio_device() -> ALCdevice* { return m_audio_device; }

  auto get_audio_context() -> ALCcontext* { return m_audio_context; }

private:
  ALCdevice* m_audio_device{ nullptr };

  ALCcontext* m_audio_context{ nullptr };
};

platform_base::platform_base()
  : m_impl(new platform_base::impl())
{
}

platform_base::~platform_base()
{
  delete m_impl;
}

auto
platform_base::get_audio_device() -> ALCdevice*
{
  return m_impl->get_audio_device();
}

auto
platform_base::get_audio_context() -> ALCcontext*
{
  return m_impl->get_audio_context();
}

} // namespace uikit
