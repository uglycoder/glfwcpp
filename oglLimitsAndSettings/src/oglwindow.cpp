// oglwindow.cpp

//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter


#include "stdafx.h"

#include "oglwindow.hpp"

#include "windows_utils.hpp"



oglwindow_ns::OGL_Window_result_t oglwindow_ns::CreateTemporaryOGL_Window(int pixelFormat /*= 1*/) noexcept
{
  static constexpr ::PIXELFORMATDESCRIPTOR DEFAULT_PIXELFORMATDESCRIPTOR{ sizeof ::PIXELFORMATDESCRIPTOR, 1U };

  auto * const glcxt{ new (std::nothrow) OpenGL_Window_Context };
  if(glcxt)
  {
    std::tie(glcxt->wnd, std::ignore, glcxt->hinstance) = windows_ns::CreateTemporaryWindow();

    if(glcxt->wnd)
    {
      glcxt->hdc = ::GetDC(glcxt->wnd);
      if(glcxt->hdc)
      {
        if(::SetPixelFormat(glcxt->hdc, pixelFormat, &DEFAULT_PIXELFORMATDESCRIPTOR))
        {
          // Create OGL context and make it current
          glcxt->hglrc = ::wglCreateContext(glcxt->hdc);
          if(glcxt->hglrc)
          {
            if(::wglMakeCurrent(glcxt->hdc, glcxt->hglrc))
            {
              return { true, "", OGL_ctxt_ptr(glcxt) };
            }
          }
          else
          {
            return { false, "opengl context creation failed" };
          }
        }
        else
        {
          auto const & errStr{ windows_ns::GetLastError(::GetLastError()) };
          auto const & msg{ "SetPixelFormat failed + : " + errStr };
          return { false, msg };
        }
      }
    }
  }
  return{};
}