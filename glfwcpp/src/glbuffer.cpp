//  glbuffer.cpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#include "precompiled-header.hpp"

#include "../interface/glbuffer.hpp"

#include "../interface/utils.hpp"

//std::cout << __func__ << std::endl;

////////////////////////////////////////////////////////////////////////////////

GLFWPP_ns::GLBuffer::GLBuffer(
  ::GLenum type
  ,std::string label)
  :
  m_type{type}
  ,m_label{std::move(label)}
{
  ::glGenBuffers(1, &m_name);
  bind();

  std::cout << "constructor: " << __func__ << std::endl;
}


GLFWPP_ns::GLBuffer::~GLBuffer()
{
  ::glDeleteBuffers(1, &m_name);

  std::cout << __func__ << std::endl;
}

GLuint GLFWPP_ns::GLBuffer::name() const noexcept
{
  return m_name;
}

std::string GLFWPP_ns::GLBuffer::label() const noexcept
{
  return m_label;
}

void GLFWPP_ns::GLBuffer::label(std::string label) noexcept
{
  assert(::glIsBuffer(m_name));

  m_label = std::move(label);
  LabelObject(OGL_LABEL_NAMESPACE::BUFFER, m_name, m_label);
}

void GLFWPP_ns::GLBuffer::bind() const noexcept
{
  ::glBindBuffer(m_type, m_name);
}

void GLFWPP_ns::GLBuffer::allocateImmutableStorage(
  ::GLsizeiptr numBytes
  ,::GLvoid const * srcData
  ,::GLbitfield flags /*= 0*/
) const noexcept
{
  assert(::glIsBuffer(m_name));

  ::glNamedBufferStorage(
        m_name
        ,numBytes
        ,srcData
        ,flags);
}

void GLFWPP_ns::GLBuffer::unBind() const noexcept
{
  ::glBindBuffer(m_type, 0);
}
