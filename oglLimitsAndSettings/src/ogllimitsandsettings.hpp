// oglLimitsAndSettings.hpp



#pragma once

#include <map>
#include <string>
#include <utility>


namespace OGLLimitsAndSettings_ns
{
 
  struct OGL_SingleValueLimits
  {
    std::map<int, std::pair<int,char const * const>> svLimitsMap;
    std::string errMsg;
  };


  OGL_SingleValueLimits const GetSingleValueLimits() noexcept;

  using Ordinal1 = decltype(GetSingleValueLimits);
}
