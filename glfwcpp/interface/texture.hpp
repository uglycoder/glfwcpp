//  texture.cpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once

namespace GLFWPP_ns
{
  class Texture
  {
  public:
    Texture()                            = delete;
    Texture(Texture const &)             = delete;
    Texture & operator=(Texture const &) = delete;

    ::GLuint    name() const noexcept;
    std::string label() const noexcept;

    void label(std::string label) noexcept;

    void bind() const noexcept;

    void allocateImmutableStorage(
      ::GLsizeiptr numBytes
      , ::GLvoid const * srcData
      , ::GLbitfield flags = 0
    ) const noexcept;

    void unBind() const noexcept;

  protected:
    explicit Texture(::GLenum type, std::string labelstr);

    ~Texture();

    Texture(Texture && rhs)             = default;
    Texture & operator=(Texture && rhs) = default;

  private:
    ::GLuint m_name{};
    ::GLenum m_target{};
    std::string m_label;
  };

}

