// oglLimitsAndSettings.hpp



#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <utility>


namespace OGLLimitsAndSettings_ns
{

  using svLimitsMap_t = std::map<unsigned int, std::pair<int64_t, std::string>>;
 
  struct OGL_SingleValueLimits
  {
    svLimitsMap_t svLimitsMap;
    std::string errMsg;
  };


  [[nodiscard]] OGL_SingleValueLimits GetSingleValueLimits() noexcept;

  using func1ptr = decltype(&GetSingleValueLimits);
}
