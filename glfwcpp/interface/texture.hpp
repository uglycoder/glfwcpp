//  texture.cpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once

#include <string>
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

  
  struct textureParameteri
  {
    ::GLenum pName;
    ::GLint  param;
  };

  constexpr textureParameteri TEXTUREBASELEVEL(::GLint val) noexcept
  {
    return {GL_TEXTURE_BASE_LEVEL, val };
  }

  constexpr textureParameteri TEXTUREMAXLEVEL(::GLint val) noexcept
  {
    return { GL_TEXTURE_MAX_LEVEL, val };
  }

  constexpr textureParameteri TEXCOMPFUNC_LEQUAL   = { GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL};
  constexpr textureParameteri TEXCOMPFUNC_GEQUAL   = { GL_TEXTURE_COMPARE_FUNC, GL_GEQUAL};
  constexpr textureParameteri TEXCOMPFUNC_LESS     = { GL_TEXTURE_COMPARE_FUNC, GL_LESS};
  constexpr textureParameteri TEXCOMPFUNC_GREATER  = { GL_TEXTURE_COMPARE_FUNC, GL_GREATER};
  constexpr textureParameteri TEXCOMPFUNC_EQUAL    = { GL_TEXTURE_COMPARE_FUNC, GL_EQUAL};
  constexpr textureParameteri TEXCOMPFUNC_NOTEQUAL = { GL_TEXTURE_COMPARE_FUNC, GL_NOTEQUAL};
  constexpr textureParameteri TEXCOMPFUNC_ALWAYS   = { GL_TEXTURE_COMPARE_FUNC, GL_ALWAYS};
  constexpr textureParameteri TEXCOMPFUNC_NEVER    = { GL_TEXTURE_COMPARE_FUNC, GL_NEVER };

  constexpr textureParameteri TEXMINFIL_NEAREST    = { GL_TEXTURE_MIN_FILTER, GL_NEAREST};
  constexpr textureParameteri TEXMINFIL_LINEAR     = { GL_TEXTURE_MIN_FILTER, GL_LINEAR };
  constexpr textureParameteri TEXMINFIL_NEARMPNEAR = { GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST };
  constexpr textureParameteri TEXMINFIL_LINMPLNEAR = { GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST };
  constexpr textureParameteri TEXMINFIL_NEARMPLIN  = { GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR };
  constexpr textureParameteri TEXMINFIL_LINMPLIN   = { GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR };

  constexpr textureParameteri TEXMAGFIL_NEAREST   = { GL_TEXTURE_MAG_FILTER, GL_NEAREST};
  constexpr textureParameteri TEXMAGFIL_LINEAR    = { GL_TEXTURE_MAG_FILTER, GL_LINEAR };

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
    void setParameter(T const & v) const noexcept
    {
      if constexpr(std::is_same_v<std::decay_t<decltype(v)>, textureParameteri>)
      {
        ::glTextureParameteri(m_name, v.pName, v.param);
      }
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

  [[nodiscard]] textureVariant LoadTexture(
    std::filesystem::path const & filename
    , std::string const & objLabel
  );

  template<OGL_TEXTURE_TARGETS texTarget>
  [[nodiscard]] auto LoadTexture(
    std::filesystem::path const & filename
    , std::string const & objLabel)
  {
    return std::get<Texture<texTarget>>(LoadTexture(filename, objLabel));
  }
}

