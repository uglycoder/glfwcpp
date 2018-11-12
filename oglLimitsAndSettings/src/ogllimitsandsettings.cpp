// ogllimitsandsettings.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "../interface/ogllimitsandsettings.hpp"

#include "../interface/glheader.h"

std::vector<int> const OGLLimitsAndSettings_ns::GetSingleValueLimits() noexcept
{
  std::vector<int> limits(static_cast<int>(OGL_SINGLE_VALUE_LIMITS::COUNT, -1));

  return limits;
}


