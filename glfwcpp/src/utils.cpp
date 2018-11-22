// utils.cpp
// MIT License
// See LICENSE.txt file in root of project
// Copyright(c) 2018 Simon Parmenter

#include "precompiled-header.hpp"

#include "../interface/utils.hpp"
#include "../interface/oglproperties.hpp"


void GLFWPP_ns::LabelObject(OGL_LABEL_NAMESPACE ns, ::GLuint name, std::string const & label) noexcept
{
  assert(static_cast<::GLint>(label.size()) < GetSVLimit(GL_MAX_LABEL_LENGTH).value);
  auto const len{ GetSVLimit(GL_MAX_LABEL_LENGTH)};

  ::GLsizei const size{(!label.empty() ? -1 : 0)};
  char const * const strPtr{size ? label.c_str() : nullptr};
  ::glObjectLabel(static_cast<::GLenum>(ns), name, size, strPtr);
}


void GLFWPP_ns::PrintGLVersionInfo(std::ostream & os, bool printExtensions /*= false*/)
{
  auto const * const renderer    = ::glGetString(GL_RENDERER);
  auto const * const vendor      = ::glGetString(GL_VENDOR);
  auto const * const version     = ::glGetString(GL_VERSION);
  auto const * const glslVersion = ::glGetString(GL_SHADING_LANGUAGE_VERSION);

  ::GLint major;
  ::GLint minor;
  ::glGetIntegerv(GL_MAJOR_VERSION, &major);
  ::glGetIntegerv(GL_MINOR_VERSION, &minor);

  os << "GL Vendor            : " << vendor   << '\n'
     << "GL Renderer          : " << renderer << '\n'
     << "GL Version (string)  : " << version  << '\n'
     << "GL Version (integer) : " << major    << '.' << minor << '\n'
     << "GLSL Version         : " << glslVersion;

  if(printExtensions)
  {
    ::GLint nExtensions;
    ::glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);

    os << "\n\nOpenGL Extensions:\n";

    for(int i{}; i < nExtensions; ++i)
    {
      os << ::glGetStringi(GL_EXTENSIONS, i) << '\n';
    }
  }
  os << std::endl;
}

bool GLFWPP_ns::CheckForOpenGLError(
  std::ostream & os
  ,std::string const & filename
  ,std::string const & line)
{
  auto retCode = false;

  auto glErr = ::glGetError();
  while(glErr != GL_NO_ERROR)
  {
    std::string message;
    switch( glErr )
    {
    case GL_INVALID_ENUM:
      message = "Invalid enum";
      break;
    case GL_INVALID_VALUE:
      message = "Invalid value";
      break;
    case GL_INVALID_OPERATION:
      message = "Invalid operation";
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      message = "Invalid framebuffer operation";
      break;
    case GL_OUT_OF_MEMORY:
      message = "Out of memory";
      break;
    default:
      message = "Unknown error";
    }

    os << "glError in file " << filename << ": line " << line << ": " << message << '\n';
    retCode = true;
    glErr = ::glGetError();
  }
  return retCode;
}

void APIENTRY GLFWPP_ns::DefaultDebugCallback(
  ::GLenum source
  ,::GLenum type
  ,::GLuint id
  ,::GLenum severity
  ,::GLsizei /*length*/
  ,::GLchar const * msg
  ,void const * param
)
{
  std::ostream & os = const_cast<std::ostream &>(*reinterpret_cast<std::ostream const*>(param));
  std::string sourceStr;
  switch(source)
  {
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    sourceStr = "WindowSys";
    break;
  case GL_DEBUG_SOURCE_APPLICATION:
    sourceStr = "App";
    break;
  case GL_DEBUG_SOURCE_API:
    sourceStr = "OpenGL API call";
    break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    sourceStr = "ShaderCompiler";
    break;
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    sourceStr = "3rdParty";
    break;
  case GL_DEBUG_SOURCE_OTHER:
    sourceStr = "Other";
    break;
  default:
    sourceStr = "Unknown";
  }

  std::string typeStr;
  switch(type)
  {
  case GL_DEBUG_TYPE_ERROR:
    typeStr = "Error";
    break;
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    typeStr = "Deprecated";
    break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    typeStr = "Undefined";
    break;
  case GL_DEBUG_TYPE_PORTABILITY:
    typeStr = "Portability";
    break;
  case GL_DEBUG_TYPE_PERFORMANCE:
    typeStr = "Performance";
    break;
  case GL_DEBUG_TYPE_MARKER:
    typeStr = "Marker";
    break;
  case GL_DEBUG_TYPE_PUSH_GROUP:
    typeStr = "PushGrp";
    break;
  case GL_DEBUG_TYPE_POP_GROUP:
    typeStr = "PopGrp";
    break;
  case GL_DEBUG_TYPE_OTHER:
    typeStr = "Other";
    break;
  default:
    typeStr = "Unknown";
  }

  std::string sevStr;
  switch(severity)
  {
  case GL_DEBUG_SEVERITY_HIGH:
    sevStr = "HIGH";
    break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    sevStr = "MED";
    break;
  case GL_DEBUG_SEVERITY_LOW:
    sevStr = "LOW";
    break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    sevStr = "NOTIFY";
    break;
  default:
    sevStr = "UNKNOWN";
  }

  os << sourceStr << ':' << typeStr << '[' << sevStr << "] (" << id << ' ' << msg << std::endl;
}

void GLFWPP_ns::SetDefaultDebugCallback(std::ostream & os) noexcept
{
  ::glDebugMessageCallback(DefaultDebugCallback, &os);

  // Enable all messages, all sources, all levels, and all IDs
  ::glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
}

std::string GLFWPP_ns::GetTypeString( ::GLenum type )
{
  // There are many more types than are covered here, but
  // these are the most common in these examples.
  switch(type)
  {
  case GL_FLOAT:
    return "float";
  case GL_FLOAT_VEC2:
    return "vec2";
  case GL_FLOAT_VEC3:
    return "vec3";
  case GL_FLOAT_VEC4:
    return "vec4";
  case GL_DOUBLE:
    return "double";
  case GL_INT:
    return "int";
  case GL_UNSIGNED_INT:
    return "unsigned int";
  case GL_BOOL:
    return "bool";
  case GL_FLOAT_MAT2:
    return "mat2";
  case GL_FLOAT_MAT3:
    return "mat3";
  case GL_FLOAT_MAT4:
    return "mat4";
  default:
    return "?";
  }
}

