// glfwpputils.hpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once

#include <iosfwd>
#include <string>

#include "glheader.h"

namespace GLFWPP_ns
{
  enum class OGL_LABEL_NAMESPACE
  {
    BUFFER              = GL_BUFFER
    ,SHADER             = GL_SHADER
    ,PROGRAM            = GL_PROGRAM
    ,VERTEX_ARRAY       = GL_VERTEX_ARRAY
    ,QUERY              = GL_QUERY
    ,PROGRAM_PIPELINE   = GL_PROGRAM_PIPELINE
    ,TRANSFORM_FEEDBACK = GL_TRANSFORM_FEEDBACK
    ,SAMPLER            = GL_SAMPLER
    ,TEXTURE            = GL_TEXTURE
    ,RENDERBUFFER       = GL_RENDERBUFFER
    ,FRAMEBUFFER        = GL_FRAMEBUFFER
  };

  void LabelObject(
    OGL_LABEL_NAMESPACE ns
    ,::GLuint name
    ,std::string const & label
  ) noexcept;

  void PrintGLVersionInfo(std::ostream & os, bool printExtensions = false);

  // Returns true if error occurred
  [[nodiscard]] bool CheckForOpenGLError(
    std::ostream & os
    ,std::string const & filename
    ,std::string const & line
  );

  void APIENTRY DefaultDebugCallback(
    ::GLenum source
    ,::GLenum type
    ,::GLuint id
    ,::GLenum severity
    ,::GLsizei length
    ,::GLchar const * msg
    ,void const * param
  );

  void SetDefaultDebugCallback(std::ostream & os) noexcept;

  [[nodiscard]] std::string GetTypeString( ::GLenum type );
}

