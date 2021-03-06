//  glbuffer.hpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter


#pragma once

#include <string>

#include "glheader.h"

namespace GLFWPP_ns
{

class GLBuffer
{
public:
  GLBuffer()                             = delete;
  GLBuffer(GLBuffer const &)             = delete;
  GLBuffer & operator=(GLBuffer const &) = delete;

  [[nodiscard]] ::GLuint    name() const noexcept;
  [[nodiscard]] std::string label() const noexcept;

  void label(std::string label) noexcept;

  void bind() const noexcept;

  void allocateImmutableStorage(
    ::GLsizeiptr numBytes
    ,::GLvoid const * srcData
    ,::GLbitfield flags = 0
  ) const noexcept;

  void unBind() const noexcept;

protected:
  explicit GLBuffer(::GLenum target, std::string labelstr);

  ~GLBuffer();

  GLBuffer(GLBuffer && rhs)             = default;
  GLBuffer & operator=(GLBuffer && rhs) = default;

private:
  ::GLuint m_name{};
  ::GLenum m_target{};
  std::string m_label;
};


}
