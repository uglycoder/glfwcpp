// oglproperties.cpp

#include "precompiled-header.hpp"

#include  "../interface/oglproperties.hpp"

#include <oglLimitsAndSettings/interface/ogllimitsandsettings.hpp>

namespace OGLimit = OGLLimitsAndSettings_ns;
using limitsMap_t = OGLimit::svLimitsMap_t;
using limitsVal_t = OGLimit::limitsValue_t;
namespace
{
  limitsMap_t GetSingleValueLimits() noexcept;
  limitsMap_t const svLimitsMap{ GetSingleValueLimits() };
}

OGLimit::limitsValue_t GLFWPP_ns::GetSVLimit(int ogldefine) noexcept
{
  assert(!svLimitsMap.empty() && "svLimitsMap should not be empty");
  try
  {
    return svLimitsMap.at(ogldefine);
  }
  catch(std::exception const & e)
  {
    using namespace std::literals;
    return{0, "", "Error: "s + e.what() + ": " __FUNCTION__};
  }
}


namespace
{
  limitsMap_t GetSingleValueLimits() noexcept
  {
    using funcPtr = OGLLimitsAndSettings_ns::func1ptr;

    if(auto library{::LoadLibraryW(L"oglLimitsAndSettings") }; library)
    {
      struct scopeGuard
      {
        ::HMODULE hnd;
        ~scopeGuard() 
        { 
          ::FreeLibrary(hnd); 
        }
      }SG{library};

      if(auto proc{ (funcPtr)::GetProcAddress(library, (LPCSTR)1) }; proc)
      {
        auto svMap{ proc() };
        if(svMap.errMsg.empty())
        {
          return svMap.svLimitsMap;
        }
        return { {0, { 0, "", svMap.errMsg }} };
      }
      return { {0, {0, "", "GetProcAddress() failed"}} };
    }
    return { {0,{0, "", "LoadLibrary() failed"}} };
  }
}