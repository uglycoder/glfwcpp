//  texture.cpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once

#include "../interface/glheader.h"

namespace GLFWPP_ns
{
  enum class OGL_TEXTURE_TARGETS
  {
    ONE_D                     = GL_TEXTURE_1D
    , TWO_D                   = GL_TEXTURE_2D
    , THREE_D                 = GL_TEXTURE_3D
    , ONE_D_ARRAY             = GL_TEXTURE_1D_ARRAY
    , TWO_D_ARRAY             = GL_TEXTURE_2D_ARRAY
    , RECTANGLE               = GL_TEXTURE_RECTANGLE
    , CUBE_MAP                = GL_TEXTURE_CUBE_MAP
    , CUBE_MAP_ARRAY          = GL_TEXTURE_CUBE_MAP_ARRAY
    , BUFFER                  = GL_TEXTURE_BUFFER
    , TWO_D_MULTISAMPLE       = GL_TEXTURE_2D_MULTISAMPLE
    , TWO_D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
  };

  class TextureBase
  {
  public:  
    TextureBase()                            = delete;
    TextureBase(TextureBase const &)             = delete;
    TextureBase & operator=(TextureBase const &) = delete;

    ::GLuint    name() const noexcept;
    std::string label() const noexcept;

    void label(std::string label) noexcept;

  protected:
    explicit TextureBase(OGL_TEXTURE_TARGETS target, std::string labelstr);

    ~TextureBase();

    TextureBase(TextureBase && rhs) = default;
    TextureBase & operator=(TextureBase && rhs) = default;

  private:
    ::GLuint m_name{};
    std::string m_label;
  };

  
  template<OGL_TEXTURE_TARGETS texTarget>
  class Texture final : public TextureBase
  {
  public:
    Texture(std::string labelStr) : TextureBase(texTarget, labelStr) { bind(); }

    void bind() const noexcept
    {
      assert(::glIsTexture(name()));

      ::glBindTexture(static_cast<::GLenum>(texTarget), name());
    }

    static void unBind() noexcept
    {
      ::glBindTexture(static_cast<::GLenum>(texTarget), 0);
    }

    template<typename ... ARGS>
    void allocateImmutableStorage(ARGS &&... args) noexcept
    {
      static_assert(sizeof...(args) > 2 && sizeof...(args) < 6, "Invalid number of arguments.");

      if constexpr(texTarget == OGL_TEXTURE_TARGETS::ONE_D && sizeof...(args) == 3)
      {
        ::glTextureStorage1D(
          name()
          , std::forward<ARGS>(args)...
        );
      }
      else if constexpr(texTarget == OGL_TEXTURE_TARGETS::TWO_D && sizeof...(args) == 4)
      {
        ::glTextureStorage2D(
          name()
          , std::forward<ARGS>(args)...
        );
      }
      else if constexpr(texTarget == OGL_TEXTURE_TARGETS::THREE_D && sizeof...(args) == 5)
      {
        ::glTextureStorag3D(
          name()
          , std::forward<ARGS>(args)...
        );
      }
      else
      {
        static_assert(false, "Invalid number of arguments for texture target.");
      }
    }
  };

}

