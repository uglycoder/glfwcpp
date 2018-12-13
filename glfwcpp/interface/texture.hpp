//  texture.cpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once

#include <utility>
#include <variant>

#include "../interface/glheader.h"

namespace GLFWPP_ns
{
  enum class[[nodiscard]] OGL_TEXTURE_TARGETS
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
    , UNKNOWN                 = -1
  };

  class TEXTURE_MIN_FILTER
  {
  public:
    enum class PARAM
    {
      NEAREST                  = GL_NEAREST
      , LINEAR                 = GL_LINEAR
      , NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST
      , LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST
      , NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR
      , LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR
    };

    constexpr explicit TEXTURE_MIN_FILTER(PARAM val) noexcept :  m_value{val} {}

    constexpr operator std::pair<::GLenum, ::GLint>() const noexcept
    {
      return std::make_pair(Pname, static_cast<::GLint>(m_value));
    }

  private:
    static constexpr ::GLenum Pname{GL_TEXTURE_MIN_FILTER};
    PARAM m_value;

  };

  OGL_TEXTURE_TARGETS GetTargetOfTexture(::GLuint name) noexcept;

  class TextureBase
  {
  public:  
    TextureBase()                                = delete;
    TextureBase(TextureBase const &)             = delete;
    TextureBase & operator=(TextureBase const &) = delete;

    [[nodiscard]] ::GLuint    name() const noexcept;
    [[nodiscard]] std::string label() const noexcept;

    void label(std::string label) noexcept;

    template<typename T>
    void setParameteri(T v) const noexcept
    {
      std::apply(::glTextureParameteri, std::make_tuple(m_name, v.first, v.second));
    }

  protected:
    explicit TextureBase(OGL_TEXTURE_TARGETS target, std::string labelstr);

    ~TextureBase();

    TextureBase(TextureBase && rhs) noexcept;
    TextureBase & operator=(TextureBase && rhs) noexcept;

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

      assert(::glIsTexture(name()));

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
        ::glTextureStorage3D(
          name()
          , std::forward<ARGS>(args)...
        );
      }
      else
      {
        static_assert(false, "Invalid number of arguments for texture target.");
      }
    }

    template<typename ... ARGS>
    void loadData(ARGS &&... args) noexcept
    {
      static_assert(sizeof...(args) > 5 && sizeof...(args) < 11, "Invalid number of arguments.");

      assert(::glIsTexture(name()));

      if constexpr(texTarget == OGL_TEXTURE_TARGETS::ONE_D && sizeof...(args) == 6)
      {
        ::glTextureSubImage1D(
          name()
          , std::forward<ARGS>(args)...
        );
      }
      else if constexpr(texTarget == OGL_TEXTURE_TARGETS::TWO_D && sizeof...(args) == 8)
      {
        ::glTextureSubImage2D(
          name()
          , std::forward<ARGS>(args)...
        );
      }
      else if constexpr(texTarget == OGL_TEXTURE_TARGETS::THREE_D && sizeof...(args) == 10)
      {
        ::glTextureSubImage3D(
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

  using textureVariant = std::variant<
    Texture<OGL_TEXTURE_TARGETS::ONE_D>
    ,Texture<OGL_TEXTURE_TARGETS::TWO_D>
    ,Texture<OGL_TEXTURE_TARGETS::THREE_D>
    ,Texture<OGL_TEXTURE_TARGETS::ONE_D_ARRAY>
    ,Texture<OGL_TEXTURE_TARGETS::TWO_D_ARRAY>
    ,Texture<OGL_TEXTURE_TARGETS::RECTANGLE>
    ,Texture<OGL_TEXTURE_TARGETS::CUBE_MAP>
    ,Texture<OGL_TEXTURE_TARGETS::CUBE_MAP_ARRAY>
    ,Texture<OGL_TEXTURE_TARGETS::BUFFER>
    ,Texture<OGL_TEXTURE_TARGETS::TWO_D_MULTISAMPLE>
    ,Texture<OGL_TEXTURE_TARGETS::TWO_D_MULTISAMPLE>
    ,std::string // error type indicator
  >;

  [[nodiscard]] textureVariant LoadTexture(std::filesystem::path const & filename);

  template<OGL_TEXTURE_TARGETS texTarget>
  [[nodiscard]] auto LoadTexture(std::filesystem::path const & filename)
  {
    return std::get<Texture<texTarget>>(LoadTexture(filename));
  }
}

