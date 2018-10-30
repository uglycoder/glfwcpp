// glfwppapplication.cpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#include "precompiled-header.hpp"

#include "../interface/application.hpp"

#include "../interface/glfwppexception.hpp"

#include "../include/macros.hpp"



GLFWPP_ns::Application::Application()
{
  if(!::glfwInit())
  {
    throw GLFWPP_ns::Exception{GLFWPP_ns::Exception::formatMsg(__FILE__, STRINGIZE(__LINE__), "Failed to initialise GLFW library")};
  }

  std::cout << "GLFW library initialised." << std::endl;
}

GLFWPP_ns::Application::~Application()
{
  ::glfwTerminate();
  std::cout << "GLFW library terminated." << std::endl;
}
