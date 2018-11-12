// ogllimitsandsettings.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "../interface/ogllimitsandsettings.hpp"

std::vector<int> OGLLimitsAndSettings_ns::GetSingleValueLimits() noexcept
{
  std::vector<int> limits(static_cast<int>(OGL_SINGLE_VALUE_LIMITS::COUNT));

  return limits;
}


