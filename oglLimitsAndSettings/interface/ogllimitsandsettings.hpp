// ogllimitsAndSettings.hpp



#pragma once

#include <cstdint>
#include <map>
#include <string>



namespace OGLLimitsAndSettings_ns
{
  struct limitsValue_t
  {
    int64_t value{};
    std::string keyStr;
    std::string errMsg;
  };

  using svLimitsMap_t = std::map<unsigned int, limitsValue_t>;
 
  struct OGL_SingleValueLimits
  {
    svLimitsMap_t svLimitsMap;
    std::string errMsg;
  };


  [[nodiscard]] OGL_SingleValueLimits GetSingleValueLimits() noexcept;

  using func1ptr = decltype(&GetSingleValueLimits);
}
