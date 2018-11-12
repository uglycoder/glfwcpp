// oglwindow.cpp

#include "stdafx.h"

#include "oglwindow.hpp"

#include "windows_utils.hpp"

  /// <summary>
  /// OpenGL_Window_Context: Simple struct for OpenGL contexts.
  /// </summary>
  /// <remarks>
  /// Client code setups the data members as required.
  /// </remarks>
struct oglwindow_ns::OpenGL_Window_Context final
{
  ::HWND        wnd{};
  ::HDC         hdc{};
  ::HGLRC       hglrc{};
  ::HINSTANCE   hinstance{ ::GetModuleHandle(nullptr) };
  std::wstring  className;

  OpenGL_Window_Context() = default;
  ~OpenGL_Window_Context() noexcept
  {
    if(wnd)
    {
      deleteContext();
      ::ReleaseDC(wnd, hdc);
      ::DestroyWindow(wnd);
      ::UnregisterClass(className.c_str(), hinstance);
    }
  }

  OpenGL_Window_Context(OpenGL_Window_Context const &) = delete;
  OpenGL_Window_Context & operator=(OpenGL_Window_Context const &) = delete;

  OpenGL_Window_Context(OpenGL_Window_Context &&) = delete;
  OpenGL_Window_Context & operator=(OpenGL_Window_Context &&) = delete;

  /// <summary>
  /// deleteContext(): Deletes the OpenGL context associated with a set of valid wnd, hdc, hInstance
  /// data members
  /// </summary>
  void deleteContext() & noexcept
  {
    assert(wnd && hdc && hinstance);

    if(hglrc)
    {
      ::wglDeleteContext(hglrc);
      hglrc = nullptr;
    }
  }
};

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