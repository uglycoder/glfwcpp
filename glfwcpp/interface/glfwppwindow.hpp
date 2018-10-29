// glfwppwindow.hpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once

#include <functional>
#include <string>

#include "glfwpp.hpp"


struct GLFWwindow;
struct GLFWmonitor;

namespace GLFWPP_ns
{
using SetWindowHintsFn = std::function<void()>;

struct rectWH
{
  int width;
  int height;
};



class Window
{
public:
  inline static SetWindowHintsFn SetWindowHints;
  
  explicit Window(
      int width, int height
      ,std::string const & title
      ,SetWindowHintsFn hints);

  explicit Window(
      int width, int height
      ,std::string const & title
      ,SetWindowHintsFn hints
      ,bool makeCntxtCurrent);

  explicit Window(
      int width, int height
      ,std::string const & title
      ,SetWindowHintsFn hints
      ,bool makeCntxtCurrent
      ,GLFWmonitor * monitor
      ,GLFWwindow * share);
  
  virtual ~Window();

  Window()                           = default;
  Window(Window const &)             = delete;
  Window & operator=(Window const &) = delete;
  Window(Window&&)                   = default;
  Window & operator=(Window&&)       = default;


  void createGLFWwindow(
      int width, int height
      ,std::string const & title
      ,SetWindowHintsFn hints
      ,bool makeCntxtCurrent = true
      ,GLFWmonitor * monitor = nullptr, GLFWwindow * share = nullptr);

  GLFWwindow * glfwHandle() const noexcept;

  bool shouldClose() const noexcept;

  // If an error occurs, all size arguments will be set to zero.
  rectWH framebufferSize() const  noexcept;

  void swapBuffers() const noexcept;

  void makeContextCurrent() const noexcept;
  bool isContextCurrent()   const noexcept;
  contextAttributes getContext() const noexcept;

  void loadOGLFunctions();
  bool openGLFunctionsLoaded() const noexcept;

  virtual bool init(){ return true;}
  virtual void render(double /*currentTime*/){}
  virtual void shutdown(){}

  virtual void registerCallbacks() const noexcept {}

  // Key input
  virtual void keyHandler(int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/) noexcept {}
  static void  KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods) noexcept;

  // Frame buffer
  virtual void frameBufferSizeHandler(int /*width*/, int /*height*/) noexcept {}  // pixels
  static void FrameBufferSizeCallback(GLFWwindow * window, int width, int height) noexcept;


protected:
  GLFWwindow * m_hWnd{};

  bool m_OGLFunctionsLoaded{};

};

Window * GetWindow(GLFWwindow * window) noexcept;

} // namespace GLFWPP_ns

