// glfwppvertexarrayobject.hpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once


#include "../interface/glheader.h"

#include <string>
#include <vector>



namespace GLFWPP_ns
{

class VertexBuffer;

class VAO final
{
public:
  static ::GLuint MaxVertexAttribBindings() noexcept;
  static ::GLuint MaxVertexAttribs() noexcept;

  using listVAO_t = std::vector<VAO>;
  static void CreateListofVAOs(std::size_t num, listVAO_t & list);
  static listVAO_t CreateListofVAOs(std::size_t num);

  static void BreakBinding() noexcept;

  explicit VAO(std::string label);

  VAO() = default;
  ~VAO();

  VAO(VAO const &)             = delete;
  VAO & operator=(VAO const &) = delete;

  VAO(VAO && rhs)             = default;
  VAO & operator=(VAO && rhs) = default;

  ::GLuint    name()  const noexcept;
  std::string label() const noexcept;

  void label(std::string labelStr) noexcept;

  void bind() const noexcept;

  void bind(VertexBuffer const & vbo, ::GLuint bindingIndex, ::GLintptr offset, ::GLsizei stride) const noexcept;

  void setAttributeFormat(::GLuint location, ::GLint numValuesPerVertex, ::GLenum typeOfData, bool isNormalised, ::GLuint firstElementOffset) const noexcept;

  void setAttributeBinding(::GLuint location, ::GLuint bindingindex) const noexcept;

  void enableAttrib(::GLuint location) const noexcept;
  void disableAttrib(::GLuint location) const noexcept;

private:
  ::GLuint    m_name{};
  std::string m_label;
};

}

