//  texture.cpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter


#include "precompiled-header.hpp"
#include "../interface/texture.hpp"
#include "../interface/utils.hpp"


GLFWPP_ns::OGL_TEXTURE_TARGETS GLFWPP_ns::GetTargetOfTexture(::GLuint name) noexcept
{
  assert(::glIsTexture(name));

  ::GLenum target;
  ::glGetTextureParameterIuiv(name, GL_TEXTURE_TARGET, &target);

  return static_cast<OGL_TEXTURE_TARGETS>(target);
}

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

namespace
{
  struct ktxFileHeader
  {
    unsigned char       identifier[12];
    unsigned int        endianness;
    unsigned int        gltype;
    unsigned int        gltypesize;
    unsigned int        glformat;
    unsigned int        glinternalformat;
    unsigned int        glbaseinternalformat;
    unsigned int        pixelwidth;
    unsigned int        pixelheight;
    unsigned int        pixeldepth;
    unsigned int        arrayelements;
    unsigned int        faces;
    unsigned int        miplevels;
    unsigned int        keypairbytes;
  };

  
  std::istream & operator>>(std::istream & istrm, ktxFileHeader & hdr)
  {
    istrm.read(reinterpret_cast<char*>(&hdr), sizeof hdr);
    return istrm;
  }
}

::GLuint GLFWPP_ns::LoadTexture(std::filesystem::path const & filename, ::GLuint textureName)
{
  namespace fs = std::filesystem;

  if(fs::exists(filename))
  {
    std::ifstream ifs(filename, std::ios::binary);
   
    if(ktxFileHeader hdr; ifs >> hdr)
    {
      return {};
    }
  }

  return {};
}

