// glfwppwindow.cpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#include "precompiled-header.hpp"

#include "../interface/glfwppwindow.hpp"

#include "../interface/glfwppexception.hpp"
#include "../interface/glheader.h"

#include "../include/macros.hpp"




////////////////////////////////////////////////////////////////////////////////

GLFWPP_ns::Window::Window(
    int width, int height
    ,std::string const & title
   ,SetWindowHintsFn hints)
  : Window{width, height, title, hints, true, nullptr, nullptr}
{  
}

GLFWPP_ns::Window::Window(
    int width, int height
    ,const std::string & title
    ,SetWindowHintsFn hints
    ,bool makeCntxtCurrent)
  : Window{width, height, title, hints, makeCntxtCurrent, nullptr, nullptr}
{
}

GLFWPP_ns::Window::Window(
    int width, int height
    ,std::string const & title
    ,SetWindowHintsFn hints
    ,bool makeCntxtCurrent
    ,GLFWmonitor* monitor
   ,GLFWwindow* share)
{
  createGLFWwindow(width, height, title, hints, makeCntxtCurrent, monitor, share);
}


GLFWPP_ns::Window::~Window()
{
  if(m_hWnd != nullptr)
  {
    // If the context of the specified window is current on the main thread, it is detached before being destroyed.
    // The context of the specified window must not be current on any other thread when this function is called.
    ::glfwMakeContextCurrent(m_hWnd);
    ::glfwMakeContextCurrent(nullptr);
    ::glfwDestroyWindow(m_hWnd);
  }
}

void GLFWPP_ns::Window::createGLFWwindow(
    int width, int height
    ,std::string const & title
    ,SetWindowHintsFn hints
    ,bool makeCntxtCurrent
    ,GLFWmonitor * monitor
    ,GLFWwindow * share)
{
  if(!::glfwInit())
  {
    throw GLFWPP_ns::Exception{GLFWPP_ns::Exception::formatMsg(__FILE__, STRINGIZE(__LINE__), "Failed to initialise GLFW library")};
  }

  hints ? hints() : ::glfwDefaultWindowHints();

  m_hWnd = ::glfwCreateWindow(width, height, title.c_str(), monitor, share);

  if(m_hWnd == nullptr)
  {
    throw Exception{GLFWPP_ns::Exception::formatMsg(__FILE__, STRINGIZE(__LINE__), "Failed to create glfw window.")};
  }

  ::glfwSetWindowUserPointer(m_hWnd, this);

  if(makeCntxtCurrent)
  {
    makeContextCurrent();
    loadOGLFunctions();
  }
}

GLFWwindow * GLFWPP_ns::Window::glfwHandle() const noexcept
{
  return m_hWnd;
}

bool GLFWPP_ns::Window::shouldClose() const noexcept
{
  return ::glfwWindowShouldClose(m_hWnd) > 0;
}

// If an error occurs, all size arguments will be set to zero.
GLFWPP_ns::rectWH GLFWPP_ns::Window::framebufferSize() const noexcept
{
  rectWH rect;
  ::glfwGetFramebufferSize(m_hWnd, &rect.width, &rect.height);
  return rect;
}

void GLFWPP_ns::Window::swapBuffers() const noexcept
{
  ::glfwSwapBuffers(m_hWnd);
}

void GLFWPP_ns::Window::makeContextCurrent() const noexcept
{
  ::glfwMakeContextCurrent(m_hWnd);
}

bool GLFWPP_ns::Window::isContextCurrent() const noexcept
{
  GLFWwindow * hWnd{::glfwGetCurrentContext()};
  return  hWnd != nullptr && hWnd == m_hWnd;
}

GLFWPP_ns::contextAttributes GLFWPP_ns::Window::getContext() const noexcept
{
  GL_PROFILE glProfile;
  auto const profile = ::glfwGetWindowAttrib(m_hWnd, GLFW_OPENGL_PROFILE);
  switch(profile)
  {
  case GLFW_OPENGL_CORE_PROFILE:
    glProfile = GL_PROFILE::CORE;
    break;
  case GLFW_OPENGL_COMPAT_PROFILE:
    glProfile = GL_PROFILE::COMPAT;
    break;
  default:
    glProfile = GL_PROFILE::ANY;
  }

  return {
    (::glfwGetWindowAttrib(m_hWnd, GLFW_CLIENT_API) == GLFW_OPENGL_API ? GL_API::OPENGL : GL_API::ES),
    {::glfwGetWindowAttrib(m_hWnd, GLFW_CONTEXT_VERSION_MAJOR),
    ::glfwGetWindowAttrib(m_hWnd, GLFW_CONTEXT_VERSION_MINOR),
    ::glfwGetWindowAttrib(m_hWnd, GLFW_CONTEXT_REVISION)},
    glProfile,
    ::glfwGetWindowAttrib(m_hWnd, GLFW_OPENGL_FORWARD_COMPAT) > 0,
    ::glfwGetWindowAttrib(m_hWnd, GLFW_OPENGL_DEBUG_CONTEXT) > 0
   };
}

void GLFWPP_ns::Window::loadOGLFunctions()
{
//  auto const loaded = ogl_LoadFunctions();
//  m_OGLFunctionsLoaded = !(loaded == ogl_LOAD_FAILED);
  m_OGLFunctionsLoaded = ::gladLoadGLLoader(reinterpret_cast<GLADloadproc>(::glfwGetProcAddress)) > 0;
  if(!m_OGLFunctionsLoaded)
  {
    throw Exception{GLFWPP_ns::Exception::formatMsg(__FILE__, STRINGIZE(__LINE__), "Failed to load OpenGL functions.")};
  }
}

bool GLFWPP_ns::Window::openGLFunctionsLoaded() const noexcept
{
  return m_OGLFunctionsLoaded;
}


// static
void GLFWPP_ns::Window::KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods) noexcept
{
  Window * const wnd = GetWindow(window);
  wnd->keyHandler(key, scancode, action, mods);
}

// static
void GLFWPP_ns::Window::FrameBufferSizeCallback(GLFWwindow * window, int width, int height) noexcept
{
  Window * const wnd = GetWindow(window);
  wnd->frameBufferSizeHandler(width, height);
}


GLFWPP_ns::Window * GLFWPP_ns::GetWindow(GLFWwindow * window) noexcept
{
  return reinterpret_cast<Window*>(::glfwGetWindowUserPointer(window));
}

