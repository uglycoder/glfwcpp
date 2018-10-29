// glfwppglslprogram.hpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once

#include <iosfwd>
#include <map>
#include <string>
#include <vector>

#include <glm/fwd.hpp>

#include "glheader.h"




namespace GLFWPP_ns
{

using shaderList_t = std::vector<::GLuint>;

enum class GLSLShaderType
{
  VERTEX          = GL_VERTEX_SHADER,
  FRAGMENT        = GL_FRAGMENT_SHADER,
  GEOMETRY        = GL_GEOMETRY_SHADER,
  TESS_CONTROL    = GL_TESS_CONTROL_SHADER,
  TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
  COMPUTE         = GL_COMPUTE_SHADER
};

class GLSLProgram
{
public:
  explicit GLSLProgram(std::string const & name = "");
  ~GLSLProgram();

  GLSLProgram(GLSLProgram const &)             = delete;
  GLSLProgram & operator=(GLSLProgram const &) = delete;
  GLSLProgram(GLSLProgram&&)                   = delete;
  GLSLProgram & operator=(GLSLProgram&&)       = delete;

  void compileShader( std::string const & fileName );
  void compileShader( std::string const & fileName, GLSLShaderType type );
  void compileShader( GLSLShaderType type, std::string const & source, std::string const & fileName = std::string());

  void link();
  void validate() const;
  void use() const;

  ::GLuint getHandle() const noexcept;
  bool   isLinked()  const noexcept;

  void bindAttribLocation( ::GLuint location, std::string const & name) const noexcept;
  void bindFragDataLocation( ::GLuint location, std::string const & name ) const noexcept;

  void setUniform( std::string const & name, float x, float y, float z);
  void setUniform( std::string const & name, glm::vec2 const & v);
  void setUniform( std::string const & name, glm::vec3 const & v);
  void setUniform( std::string const & name, glm::vec4 const & v);

  void setUniform( std::string const & name, glm::mat4 const & m);
  void setUniform( std::string const & name, std::vector<glm::mat4> const & m);

  void setUniform( std::string const & name, glm::mat3 const & m);
  void setUniform( std::string const & name, float val );
  void setUniform( std::string const & name, int val );
  void setUniform( std::string const & name, bool val );
  void setUniform( std::string const & name, ::GLuint val );

  void printActiveUniforms(std::ostream & os)      const;
  void printActiveUniformBlocks(std::ostream & os) const;
  void printActiveAttribs(std::ostream & os)       const;

private:
  using uniformLocationMap_t = std::map<std::string, ::GLint>;
  ::GLuint m_program{};
  bool     m_linked{};

  std::string m_name;

  uniformLocationMap_t m_uniformLocations;

  ::GLint getUniformLocation(std::string const & name);

  shaderList_t getAttachedShaders() const;

  void createProgramObject();

  static std::string GetCompilationLog(::GLuint shader);
  static std::string GetLinkLog(::GLuint program);
  static bool ValidateProgram(::GLuint program) noexcept;
  static void PrintActiveUniforms(::GLuint program, std::ostream & os);
  static void PrintActiveUniformBlocks(::GLuint program, std::ostream & os);
  static void PrintActiveAttribs(::GLuint program, std::ostream & os);
};

} // namespace GLFWPP_ns


