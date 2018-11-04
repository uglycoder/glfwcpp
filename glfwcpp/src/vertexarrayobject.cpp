//  vertexarrayobject.cpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#include "precompiled-header.hpp"

#include "../interface/vertexarrayobject.hpp"

#include "../interface/vertexbuffer.hpp"
#include "../interface/utils.hpp"

#include <cassert>
#include <iostream>


//static
::GLuint GLFWPP_ns::VAO::MaxVertexAttribBindings() noexcept
{
  ::GLint num;
  ::glGetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, &num);
  return num;
}

//static
::GLuint GLFWPP_ns::VAO::MaxVertexAttribs() noexcept
{
  ::GLint num;
  ::glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &num);
  return num;
}

// static
void GLFWPP_ns::VAO::CreateListofVAOs(std::size_t num, listVAO_t & list)
{
  for(decltype(num) n{}; n < num; ++n)
  {
    list.emplace_back(VAO());
  }
}

// static
GLFWPP_ns::VAO::listVAO_t GLFWPP_ns::VAO::CreateListofVAOs(std::size_t num)
{
  return listVAO_t(num);
}

// static
void GLFWPP_ns::VAO::BreakBinding() noexcept
{
  ::glBindVertexArray(0);
}


////////////////////////////////////////////////////////////////////////////////

GLFWPP_ns::VAO::VAO(std::string labelStr)
{
  ::glCreateVertexArrays(1, &m_name);
  bind();
  label(labelStr);
  std::cout << "constructor: " << __func__ << std::endl;
}


GLFWPP_ns::VAO::~VAO()
{
  ::glDeleteVertexArrays(1, &m_name);
  std::cout << __func__ << std::endl;
}

::GLuint GLFWPP_ns::VAO::name() const noexcept
{
  return m_name;
}

std::string GLFWPP_ns::VAO::label() const noexcept
{
  return m_label;
}

void GLFWPP_ns::VAO::label(std::string labelStr) noexcept
{
  assert(::glIsVertexArray(m_name));

  m_label = std::move(labelStr);
  LabelObject(OGL_LABEL_NAMESPACE::VERTEX_ARRAY, m_name, m_label);
}

void GLFWPP_ns::VAO::bind() const noexcept
{
  ::glBindVertexArray(m_name);
}

void GLFWPP_ns::VAO::bind(VertexBuffer const & vbo, ::GLuint bindingIndex, ::GLintptr offset, ::GLsizei stride) const noexcept
{
  assert(::glIsVertexArray(m_name));
  assert(::glIsBuffer(vbo.name()));

  ::glVertexArrayVertexBuffer(m_name, bindingIndex, vbo.name(), offset, stride);
}

void GLFWPP_ns::VAO::setAttributeFormat(::GLuint location, ::GLint numValuesPerVertex, ::GLenum typeOfData, bool isNormalised, ::GLuint firstElementOffset) const noexcept
{
  ::glVertexArrayAttribFormat(m_name, location, numValuesPerVertex, typeOfData, isNormalised ? GL_TRUE : GL_FALSE, firstElementOffset);
}

void GLFWPP_ns::VAO::setAttributeBinding(::GLuint location, ::GLuint bindingindex) const noexcept
{
  ::glVertexArrayAttribBinding(m_name, location, bindingindex);
}

void GLFWPP_ns::VAO::enableAttrib(::GLuint location) const noexcept
{
  ::glEnableVertexArrayAttrib(m_name, location);
}

void GLFWPP_ns::VAO::disableAttrib(::GLuint location) const noexcept
{
  ::glDisableVertexArrayAttrib(m_name, location);
}
