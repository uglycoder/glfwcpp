// oglproperties.hpp

//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once

#include <oglLimitsAndSettings/interface/ogllimitsandsettings.hpp>

namespace GLFWPP_ns
{
  [[nodiscard]] OGLLimitsAndSettings_ns::limitsValue_t GetSVLimit(int ogldefine) noexcept;
}
