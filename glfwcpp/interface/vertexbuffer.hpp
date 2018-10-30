//  vertexbuffer.hpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once

#include "glbuffer.hpp"

namespace GLFWPP_ns
{


class VertexBuffer final : public GLBuffer
{
public:
  explicit VertexBuffer(std::string const & label);
  ~VertexBuffer() = default;

  VertexBuffer(VertexBuffer && rhs)             = default;
  VertexBuffer & operator=(VertexBuffer && rhs) = default;

  VertexBuffer()                                 = delete;
  VertexBuffer(VertexBuffer const &)             = delete;
  VertexBuffer & operator=(VertexBuffer const &) = delete;
};

}
