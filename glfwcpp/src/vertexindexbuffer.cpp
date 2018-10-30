//  vertexindexbuffer.cpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#include "precompiled-header.hpp"

#include "../interface/vertexindexbuffer.hpp"

#include <iostream>

GLFWPP_ns::VertexIndexBuffer::VertexIndexBuffer(const std::string & label)
  :
  GLBuffer(GL_ELEMENT_ARRAY_BUFFER, label)
{
  std::cout << "constructor: " << __func__ << std::endl;
}
