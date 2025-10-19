#pragma once

#include <glow/main.hpp>

namespace glow {

class platform_base : public platform
{
public:
  class impl;

  platform_base();

  platform_base(const platform_base&) = delete;

  platform_base(platform_base&&) = delete;

  auto operator=(const platform_base&) -> platform_base& = delete;

  auto operator=(platform_base&&) -> platform_base& = delete;

  ~platform_base() override;

private:
  impl* m_impl{ nullptr };
};

} // namespace glow
