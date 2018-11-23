

#include "pch.h"

#include "printOpenGLlimits/gllmits.hpp"

#include <Windows.h>

#include <oglLimitsAndSettings/interface/ogllimitsandsettings.hpp>

namespace OGLimit = OGLLimitsAndSettings_ns;
using limitsMap_t = OGLimit::svLimitsMap_t;
using limitsVal_t = OGLimit::limitsValue_t;

namespace
{
  OGLimit::OGL_SingleValueLimits GetSingleValueLimits() noexcept;
}

OGLimit::OGL_SingleValueLimits const svLimitsMap{ GetSingleValueLimits() };


namespace
{
  OGLimit::OGL_SingleValueLimits GetSingleValueLimits() noexcept
  {
    using funcPtr = OGLLimitsAndSettings_ns::func1ptr;

    if(auto library{ ::LoadLibraryW(L"oglLimitsAndSettings") }; library)
    {
      struct scopeGuard
      {
        ::HMODULE hnd;
        ~scopeGuard()
        {
          ::FreeLibrary(hnd);
        }
      }SG{ library };

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
    return { limitsMap_t{}, "LoadLibrary() failed" };
  }
}