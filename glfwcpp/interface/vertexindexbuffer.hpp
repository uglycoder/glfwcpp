//  vertexindexbuffer.hpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once

#include "glbuffer.hpp"

#include <string>

namespace GLFWPP_ns
{

class VertexIndexBuffer final : public GLBuffer
{
public:
  explicit VertexIndexBuffer(std::string const & label);
  ~VertexIndexBuffer() = default;

  VertexIndexBuffer(VertexIndexBuffer && rhs)              = default;
  VertexIndexBuffer & operator=(VertexIndexBuffer && rhs)  = default;

  VertexIndexBuffer()                                      = delete;
  VertexIndexBuffer(VertexIndexBuffer const &)             = delete;
  VertexIndexBuffer & operator=(VertexIndexBuffer const &) = delete;
};

}
