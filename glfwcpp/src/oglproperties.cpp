// oglproperties.cpp

#include "precompiled-header.hpp"

#include  "../interface/oglproperties.hpp"


namespace
{
  OGLLimitsAndSettings_ns::svLimitsMap_t GetSingleValueLimits() noexcept;
}

OGLLimitsAndSettings_ns::svLimitsMap_t const GLFWPP_ns::svLimitsMap{GetSingleValueLimits()};


namespace
{
  OGLLimitsAndSettings_ns::svLimitsMap_t GetSingleValueLimits() noexcept
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
        return {std::make_pair(-1, std::make_pair(-1, svMap.errMsg))};
      }
      return { std::make_pair(-1, std::make_pair(-1, "GetProcAddress() failed"))};
    }
    return { std::make_pair(-1, std::make_pair(-1, "LoadLibrary() failed")) };
  }
}