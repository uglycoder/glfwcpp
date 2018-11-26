// oglwindow.hpp

#pragma once

namespace oglwindow_ns
{
  /// <summary>
 /// OpenGL_Window_Context: Simple struct for OpenGL contexts.
 /// </summary>
 /// <remarks>
 /// Client code setups the data members as required.
 /// </remarks>
  struct[[nodiscard]] OpenGL_Window_Context final
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

  using OGL_ctxt_ptr = std::unique_ptr<OpenGL_Window_Context>;

  struct[[nodiscard]] OGL_Window_result_t
  {
    bool succeeded{};
    std::string errStr;
    OGL_ctxt_ptr ctxt_ptr;
  };

  OGL_Window_result_t CreateTemporaryOGL_Window(int pixelFormat = 1) noexcept;
}
