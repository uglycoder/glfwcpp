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
  ::GLenum target
  ,std::string labelstr)
  :
  m_target{target}
{
  ::glCreateBuffers(1, &m_name);
  bind();
  label(labelstr);

  std::cout << "constructor: " << __func__ << std::endl;
}


GLFWPP_ns::GLBuffer::~GLBuffer()
{
  assert(::glIsBuffer(m_name));

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
  ::glBindBuffer(m_target, m_name);
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
  ::glBindBuffer(m_target, 0);
}
