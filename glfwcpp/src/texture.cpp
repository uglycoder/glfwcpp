//  texture.cpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter


#include "precompiled-header.hpp"
#include "../interface/texture.hpp"
#include "../interface/utils.hpp"



GLFWPP_ns::Texture::Texture(
  ::GLenum target
  , std::string labelstr)
  :
  m_target{target}
{
  ::glCreateTextures(target, 1, &m_name);
  bind();
  label(labelstr);

  std::cout << "constructor: " << __func__ << std::endl;
}


GLFWPP_ns::Texture::~Texture()
{

  ::glDeleteTextures(1, &m_name);
}

GLuint GLFWPP_ns::Texture::name() const noexcept
{
  return m_name;
}

std::string GLFWPP_ns::Texture::label() const noexcept
{
  return m_label;
}

void GLFWPP_ns::Texture::label(std::string label) noexcept
{
  assert(::glIsBuffer(m_name));

  m_label = std::move(label);
  LabelObject(OGL_LABEL_NAMESPACE::TEXTURE, m_name, m_label);
}
