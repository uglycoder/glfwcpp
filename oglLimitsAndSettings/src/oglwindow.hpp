// oglwindow.hpp

#pragma once

namespace oglwindow_ns
{
  struct OpenGL_Window_Context;

  using OGL_ctxt_ptr = std::unique_ptr<OpenGL_Window_Context>;

  struct OGL_Window_result_t
  {
    bool succeeded{};
    std::string errStr;
    OGL_ctxt_ptr ctxt_ptr;
  };

  OGL_Window_result_t CreateTemporaryOGL_Window(int pixelFormat = 1) noexcept;
}
