//  glslprogram.cpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#include "precompiled-header.hpp"

#include "../interface/glslprogram.hpp"

#include "../interface/utils.hpp"
#include "../interface/exception.hpp"

#include "../../include/macros.hpp"
#include "../interface/globalutils.hpp"

namespace
{
using GLFWPP_ns::GLSLShaderType;

std::map<std::string const, GLSLShaderType> const ShaderExtensionsMap =
{
  {".vs",   GLSLShaderType::VERTEX},
  {".vert", GLSLShaderType::VERTEX},            // from https://www.khronos.org/opengles/sdk/tools/Reference-Compiler/
  {".tcs",  GLSLShaderType::TESS_CONTROL},
  {".tesc", GLSLShaderType::TESS_CONTROL},      // from https://www.khronos.org/opengles/sdk/tools/Reference-Compiler/
  {".tes",  GLSLShaderType::TESS_EVALUATION},
  {".tese", GLSLShaderType::TESS_EVALUATION},   // from https://www.khronos.org/opengles/sdk/tools/Reference-Compiler/
  {".gs",   GLSLShaderType::GEOMETRY},
  {".geom", GLSLShaderType::GEOMETRY},          // from https://www.khronos.org/opengles/sdk/tools/Reference-Compiler/
  {".fs",   GLSLShaderType::FRAGMENT},
  {".frag", GLSLShaderType::FRAGMENT},          // from https://www.khronos.org/opengles/sdk/tools/Reference-Compiler/
  {".cs",   GLSLShaderType::COMPUTE},
  {".comp", GLSLShaderType::COMPUTE}            // from https://www.khronos.org/opengles/sdk/tools/Reference-Compiler/
};
}

GLFWPP_ns::GLSLProgram::GLSLProgram(std::string const & name /*= ""*/) : m_name(name)
{
}

GLFWPP_ns::GLSLProgram::~GLSLProgram()
{
  if(m_program == 0)
  {
    return;
  }

  shaderList_t const & shaders = getAttachedShaders();

  for(shaderList_t::value_type s : shaders)
  {
    ::glDeleteShader(s);
  }

  ::glDeleteProgram(m_program);
}
////////////////////////////////////////////////////////////////////////////////

void GLFWPP_ns::GLSLProgram::compileShader( std::string const & fileName )
{
  std::filesystem::path const p{fileName};
  auto const & ext = p.extension().string();
  GLSLShaderType type;

  try
  {
    type = ShaderExtensionsMap.at(ext);
  }
  catch(std::out_of_range & )
  {
    auto const & msg = Exception::formatMsg(__FILE__, STRINGIZE(__LINE__), "Unrecognized shader extension: " + ext);
    throw Exception{msg};
  }
  compileShader( fileName, type );
}

void GLFWPP_ns::GLSLProgram::compileShader( std::string const & fileName, GLSLShaderType type )
{
  if(!std::filesystem::exists(fileName))
  {
    auto const & msg = Exception::formatMsg(__FILE__, STRINGIZE(__LINE__), std::string("Shader: ") + fileName + " not found.");
    throw Exception{msg};
  }

  std::ifstream inFile( fileName, std::ios::in );
  if( !inFile )
  {
    auto const & msg = Exception::formatMsg(__FILE__, STRINGIZE(__LINE__), "Unable to open: " + fileName);
    throw Exception{msg};
  }

  // Get file contents
  std::ostringstream code;
  code << inFile.rdbuf();
  inFile.close();

  compileShader(type, code.str(), fileName);
}

void GLFWPP_ns::GLSLProgram::compileShader( GLSLShaderType type, std::string const & source, std::string const & fileName)
{
  auto const shaderHandle = ::glCreateShader(static_cast<GLenum>(type));
  if(shaderHandle == 0)
  {
    auto const & msg = Exception::formatMsg(__FILE__, STRINGIZE(__LINE__), "Unable to create shader object.");
    throw Exception{msg};
  }

  auto const * c_code = source.c_str();
  ::glShaderSource( shaderHandle, 1, &c_code, nullptr );

  // Compile the shader
  ::glCompileShader(shaderHandle);

  // Check for errors
  int result{};
  ::glGetShaderiv( shaderHandle, GL_COMPILE_STATUS, &result );
  if(result == GL_TRUE)
  {
    createProgramObject();

    ::glAttachShader(m_program, shaderHandle);
  }
  else
  {
    // Compile failed, get log
    std::string const & msg = !fileName.empty() ? fileName + ": shader compilation failed\n": "Shader compilation failed.\n";

    auto const & message = Exception::formatMsg(__FILE__, STRINGIZE(__LINE__), msg + GetCompilationLog(shaderHandle));
    throw Exception{message};
  }
}

void GLFWPP_ns::GLSLProgram::link()
{
  if(m_linked)
  {
    return;
  }

  if(m_program == 0)
  {
    auto const & msg = Exception::formatMsg(
          __FILE__, STRINGIZE(__LINE__),
          "No shader has been compiled, no program object been created.");

    throw Exception{msg};
  }

  ::glLinkProgram(m_program);

  int status{};
  ::glGetProgramiv(m_program, GL_LINK_STATUS, &status);
  if(status == GL_TRUE)
  {
    m_uniformLocations.clear();
    m_linked = true;
  }
  else
  {
    // link failed, get log
    auto const & message = Exception::formatMsg(
          __FILE__, STRINGIZE(__LINE__),
          "Program failed to link:\n" + GetLinkLog(m_program));

    throw Exception{message};
  }
}

void GLFWPP_ns::GLSLProgram::validate() const
{
  assert(::glIsProgram(m_program) == GL_TRUE);

  if(m_linked)
  {
    if(ValidateProgram(m_program))
    {
      return;
    }
    else
    {
      auto const & message = Exception::formatMsg(
            __FILE__, STRINGIZE(__LINE__),
            "Program failed to validate:\n" + GetLinkLog(m_program));

      throw Exception{message};
    }
  }
  else
  {
    auto const & message = Exception::formatMsg(__FILE__, STRINGIZE(__LINE__), "Shader program is not linked.");
    throw Exception{message};
  }
}

void GLFWPP_ns::GLSLProgram::use() const
{
  if(m_program > 0 && m_linked)
  {
    ::glUseProgram(m_program);
  }
  else
  {
    auto const & message = Exception::formatMsg(__FILE__, STRINGIZE(__LINE__), "Shader program is not linked.");
    throw Exception{message};
  }
}

::GLuint GLFWPP_ns::GLSLProgram::getHandle() const noexcept
{
  return m_program;
}

bool GLFWPP_ns::GLSLProgram::isLinked() const noexcept
{
  return m_linked;
}

void GLFWPP_ns::GLSLProgram::bindAttribLocation( ::GLuint location, std::string const & name) const noexcept
{
  ::glBindAttribLocation(m_program, location, name.c_str());
}

void GLFWPP_ns::GLSLProgram::bindFragDataLocation( ::GLuint location, std::string const & name ) const noexcept
{
  ::glBindFragDataLocation(m_program, location, name.c_str());
}

void GLFWPP_ns::GLSLProgram::setUniform( std::string const & name, float x, float y, float z)
{
  ::glProgramUniform3f(m_program,  getUniformLocation(name), x, y, z);
}

void GLFWPP_ns::GLSLProgram::setUniform( std::string const & name, glm::vec2 const & v)
{
  ::glProgramUniform2f(m_program, getUniformLocation(name),v.x,v.y);
}

void GLFWPP_ns::GLSLProgram::setUniform( std::string const & name, glm::vec3 const & v)
{
  setUniform(name,v.x,v.y,v.z);
}

void GLFWPP_ns::GLSLProgram::setUniform( std::string const & name, glm::vec4 const & v)
{
  ::glProgramUniform4f(m_program, getUniformLocation(name),v.x,v.y,v.z,v.w);
}

void GLFWPP_ns::GLSLProgram::setUniform( std::string const & name, glm::mat4 const & m)
{
  ::glProgramUniformMatrix4fv(m_program, getUniformLocation(name), 1, GL_FALSE, &m[0][0]);
}

void GLFWPP_ns::GLSLProgram::setUniform(std::string const & name, std::vector<glm::mat4> const & m)
{
  ::glProgramUniformMatrix4fv(m_program, getUniformLocation(name), static_cast<GLsizei>(m.size()), GL_FALSE, reinterpret_cast<const float*>(m.data()));
}

void GLFWPP_ns::GLSLProgram::setUniform( std::string const & name, glm::mat3 const & m)
{
  ::glProgramUniformMatrix3fv(m_program, getUniformLocation(name), 1, GL_FALSE, &m[0][0]);
}

void GLFWPP_ns::GLSLProgram::setUniform( std::string const & name, float val )
{
  ::glProgramUniform1f(m_program, getUniformLocation(name), val);
}

void GLFWPP_ns::GLSLProgram::setUniform( std::string const & name, int val )
{
  ::glProgramUniform1i(m_program, getUniformLocation(name), val);
}

void GLFWPP_ns::GLSLProgram::setUniform( std::string const & name, bool val )
{
  ::glProgramUniform1i(m_program, getUniformLocation(name), val);
}

void GLFWPP_ns::GLSLProgram::setUniform( std::string const & name, ::GLuint val )
{
  ::glProgramUniform1ui(m_program, getUniformLocation(name), val);
}
////////////////////////////////////////////////////////////////////////////////


void GLFWPP_ns::GLSLProgram::printActiveUniforms(std::ostream & os) const
{
  std::string const & shaderName = !m_name.empty() ? m_name : "no name given";
  os << "Active uniforms for shader program '" << shaderName << '\'' << '\n';
  if(m_linked)
  {
    PrintActiveUniforms(m_program, os);
  }
  else
  {
    os << "    Shader program not linked." << std::endl;
  }
}

void GLFWPP_ns::GLSLProgram::printActiveUniformBlocks(std::ostream & os) const
{
  std::string const & shaderName = !m_name.empty() ? m_name : "no name given";
  os << "Active uniform blocks for shader program '" << shaderName << '\'' << '\n';
  if(m_linked)
  {
    PrintActiveUniformBlocks(m_program, os);
  }
  else
  {
    os << "    Shader program not linked." << std::endl;
  }
}

void GLFWPP_ns::GLSLProgram::printActiveAttribs(std::ostream & os) const
{
  std::string const & shaderName = !m_name.empty() ? m_name : "no name given";
  os << "Active attributes for shader program '" << shaderName << '\'' << '\n';
  if(m_linked)
  {
    PrintActiveAttribs(m_program, os);
  }
  else
  {
    os << "    Shader program not linked." << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////

// Private
::GLint GLFWPP_ns::GLSLProgram::getUniformLocation(std::string const & name)
{
  if( m_uniformLocations.find(name) == m_uniformLocations.end() )
  {
    m_uniformLocations[name] = ::glGetUniformLocation(m_program, name.c_str());
  }

  return m_uniformLocations[name];
}

GLFWPP_ns::shaderList_t GLFWPP_ns::GLSLProgram::getAttachedShaders() const
{
  if(m_program == 0)
  {
    return shaderList_t{};
  }

  // Query the number of attached shaders
  ::GLint numShaders{};
  ::glGetProgramiv(m_program, GL_ATTACHED_SHADERS, &numShaders);

  shaderList_t shaders(static_cast<shaderList_t::value_type>(numShaders));

  ::glGetAttachedShaders(m_program, numShaders, nullptr, shaders.data());

  return shaders;
}

void GLFWPP_ns::GLSLProgram::createProgramObject()
{
  // Have we a program object?
  if(m_program == 0)
  {
    // No
    m_program = ::glCreateProgram();
    if(m_program == 0)
    {
      auto const & msg = Exception::formatMsg(__FILE__, STRINGIZE(__LINE__), "Unable to create shader program object.");
      throw Exception{msg};
    }

    if(!m_name.empty())
    {
      ::glObjectLabel(GL_PROGRAM, m_program, -1, m_name.c_str());
    }
    else
    {
      ::glObjectLabel(GL_PROGRAM, m_program, 0, nullptr);
    }
  }
}

// static
std::string GLFWPP_ns::GLSLProgram::GetCompilationLog(::GLuint shader)
{
  assert(::glIsShader(shader) == GL_TRUE);

  int length{};
  ::glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length );
  if( length > 0 )
  {
    auto const & log = std::make_unique<char[]>(static_cast<std::size_t>(length));
    int written{};
    ::glGetShaderInfoLog(shader, length, &written, log.get());
    return log.get();
  }
  return {};
}

// static
std::string GLFWPP_ns::GLSLProgram::GetLinkLog(::GLuint program)
{
  assert(::glIsProgram(program) == GL_TRUE);

  int length{};
  ::glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length );
  if(length > 0)
  {
    auto const & log = std::make_unique<char[]>(static_cast<std::size_t>(length));
    int written{};
    ::glGetProgramInfoLog(program, length, &written, log.get());
    return log.get();
  }
  return {};
}

// static
bool GLFWPP_ns::GLSLProgram::ValidateProgram(::GLuint program) noexcept
{
  assert(::glIsProgram(program) == GL_TRUE);

  GLint status{};
  ::glValidateProgram( program );
  ::glGetProgramiv( program, GL_VALIDATE_STATUS, &status );
  return status == GL_TRUE;
}

// static
void GLFWPP_ns::GLSLProgram::PrintActiveUniforms(::GLuint program, std::ostream & os)
{
  assert(::glIsProgram(program) == GL_TRUE);

  using GlobalUtils_ns::ArraySize;

  ::GLint numUniforms{};
  ::glGetProgramInterfaceiv( program, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);
  if(numUniforms < 1)
  {
    os << "    None\n";
  }

  ::GLenum properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX};
  constexpr auto const numProperties = ArraySize(properties);

  for( int i = 0; i < numUniforms; ++i )
  {
    ::GLint results[numProperties];
    ::glGetProgramResourceiv(program, GL_UNIFORM, static_cast<::GLuint>(i), numProperties, properties, numProperties, nullptr, results);

    if( results[3] != -1 )
    {
      continue;  // Skip uniforms in blocks
    }
    ::GLint const & nameBufSize = results[0] + 1;
    auto const & name = std::make_unique<char[]>(static_cast<std::size_t>(nameBufSize));
    ::glGetProgramResourceName(program, GL_UNIFORM, static_cast<::GLuint>(i), nameBufSize, nullptr, name.get());
    os.width(5);
    os << results[2] << ' ' << name.get() << '(' << GetTypeString(static_cast<::GLenum>(results[1])) << ')' << std::endl;
  }
}

// static
void GLFWPP_ns::GLSLProgram::PrintActiveUniformBlocks(::GLuint program, std::ostream & os)
{
  assert(::glIsProgram(program) == GL_TRUE);

  using GlobalUtils_ns::ArraySize;

  ::GLint numBlocks{};

  ::glGetProgramInterfaceiv(program, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numBlocks);
  if(numBlocks < 1)
  {
    os << "    None\n";
  }

  ::GLenum blockProps[]{GL_NUM_ACTIVE_VARIABLES, GL_NAME_LENGTH};
  constexpr auto const numBlockProps{ArraySize(blockProps)};

  ::GLenum blockIndex[]{GL_ACTIVE_VARIABLES};

  ::GLenum props[]{GL_NAME_LENGTH, GL_TYPE, GL_BLOCK_INDEX};
  constexpr auto const numProps{ArraySize(props)};

  for(int block{}; block < numBlocks; ++block)
  {
    ::GLint blockInfo[numBlockProps];
    ::glGetProgramResourceiv(program, GL_UNIFORM_BLOCK, static_cast<::GLuint>(block), numBlockProps, blockProps, numBlockProps, nullptr, blockInfo);
    auto const & numActiveUniforms = blockInfo[0];

    auto const & nameLength{blockInfo[1]+1};
    auto const & blockName{std::make_unique<char[]>(static_cast<std::size_t>(nameLength))};
    ::glGetProgramResourceName(program, GL_UNIFORM_BLOCK, static_cast<::GLuint>(block), nameLength, nullptr, blockName.get());
    os << R"(Uniform block ")" << blockName.get() << R"(":)" << '\n';

    auto const unifIndexes{std::make_unique<GLint[]>(static_cast<std::size_t>(numActiveUniforms))};
    ::glGetProgramResourceiv(program, GL_UNIFORM_BLOCK, static_cast<::GLuint>(block), 1, blockIndex, numActiveUniforms, nullptr, unifIndexes.get());

    for( int unif{}; unif < numActiveUniforms; ++unif )
    {
      auto const & uniIndex{unifIndexes[static_cast<std::size_t>(unif)]};
      ::GLint results[3];
      ::glGetProgramResourceiv(program, GL_UNIFORM, static_cast<::GLuint>(uniIndex), numProps, props, numProps, nullptr, results);

      auto const & nameBufSize{results[0] + 1};
      auto const & name = std::make_unique<char[]>(static_cast<std::size_t>(nameBufSize));
      ::glGetProgramResourceName(program, GL_UNIFORM, static_cast<::GLuint>(uniIndex), nameBufSize, nullptr, name.get());
      os << "    " << name.get() << " (" << GetTypeString(static_cast<::GLenum>(results[1])) << ")\n";
    }
  }
}

// static
void GLFWPP_ns::GLSLProgram::PrintActiveAttribs(::GLuint program, std::ostream & os)
{
  assert(::glIsProgram(program) == GL_TRUE);

  using GlobalUtils_ns::ArraySize;

  ::GLint numAttribs;
  ::glGetProgramInterfaceiv( program, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttribs);
  if(numAttribs < 1)
  {
    os << "    None\n";
  }

  ::GLenum properties[]{GL_NAME_LENGTH, GL_TYPE, GL_LOCATION};
  constexpr auto const numProps{ArraySize(properties)};

  for( ::GLint i{}; i < numAttribs; ++i )
  {
    ::GLint results[3];
    ::glGetProgramResourceiv(program, GL_PROGRAM_INPUT, static_cast<::GLuint>(i), numProps, properties, numProps, nullptr, results);

    auto const & nameBufSize{results[0] + 1};
    auto const & name = std::make_unique<char[]>(static_cast<std::size_t>(nameBufSize));
    ::glGetProgramResourceName(program, GL_PROGRAM_INPUT, static_cast<::GLuint>(i), nameBufSize, nullptr, name.get());
    os.width(5);
    os << results[2] << ' ' << name.get() << '(' << GetTypeString(static_cast<::GLenum>(results[1])) << ")\n";
  }
}

