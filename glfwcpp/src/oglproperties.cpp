// oglproperties.cpp

#include "precompiled-header.hpp"

#include  "../interface/oglproperties.hpp"

#include <oglLimitsAndSettings/interface/ogllimitsandsettings.hpp>

namespace OGLimit = OGLLimitsAndSettings_ns;
using limitsMap_t = OGLimit::svLimitsMap_t;
using limitsVal_t = OGLimit::limitsValue_t;
namespace
{
  [[nodiscard]] OGLimit::OGL_SingleValueLimits GetSingleValueLimits() noexcept;
  OGLimit::OGL_SingleValueLimits const svLimitsMap{ GetSingleValueLimits() };
}

limitsVal_t GLFWPP_ns::GetSVLimit(int ogldefine) noexcept
{
  assert(svLimitsMap.errMsg.empty() && "There shouldbe no error message");
  try
  {
    return svLimitsMap.svLimitsMap.at(ogldefine);
  }
  catch(std::exception const & e)
  {
    using namespace std::literals;
    return{0, "", "Error: "s + e.what() + "; Error: " + svLimitsMap.errMsg + "; function " __FUNCTION__};
  }
}


namespace
{
  OGLimit::OGL_SingleValueLimits GetSingleValueLimits() noexcept
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
          return svMap;
        }
        return { limitsMap_t{}, svMap.errMsg };
      };      
      return { limitsMap_t{}, "GetProcAddress() failed" };
    }
    return { limitsMap_t{}, "LoadLibrary() failed"};
  }
}