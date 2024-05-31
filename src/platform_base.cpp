#include "platform_base.h"

namespace uikit {

class platform_base::impl final
{
public:
  /* There used to be code here for OpenAL, but was removed.
   * Leaving it incase I decide to add audio support again with a different library. */

  impl() {}

  ~impl() {}

private:
};

platform_base::platform_base()
  : m_impl(new platform_base::impl())
{
}

platform_base::~platform_base()
{
  delete m_impl;
}

} // namespace uikit
