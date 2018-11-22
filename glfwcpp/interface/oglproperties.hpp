// oglproperties.hpp

#pragma once

#include <oglLimitsAndSettings/interface/ogllimitsandsettings.hpp>

namespace GLFWPP_ns
{
  [[nodiscard]] OGLLimitsAndSettings_ns::limitsValue_t GetSVLimit(int ogldefine) noexcept;
}
