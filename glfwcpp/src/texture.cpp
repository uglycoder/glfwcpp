//  texture.cpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter


#include "precompiled-header.hpp"
#include "../interface/texture.hpp"
#include "../interface/utils.hpp"



GLFWPP_ns::TextureBase::TextureBase(
  OGL_TEXTURE_TARGETS target
  , std::string labelstr)
{
  ::glCreateTextures(static_cast<::GLenum>(target), 1, &m_name);
  label(labelstr);

  std::cout << "constructor: " << __func__ << std::endl;
}


GLFWPP_ns::TextureBase::~TextureBase()
{
  assert(::glIsTexture(m_name));

  ::glDeleteTextures(1, &m_name);
}

GLuint GLFWPP_ns::TextureBase::name() const noexcept
{
  return m_name;
}

std::string GLFWPP_ns::TextureBase::label() const noexcept
{
  return m_label;
}

void GLFWPP_ns::TextureBase::label(std::string label) noexcept
{
  assert(::glIsTexture(m_name));

  m_label = std::move(label);
  LabelObject(OGL_LABEL_NAMESPACE::TEXTURE, m_name, m_label);
}
