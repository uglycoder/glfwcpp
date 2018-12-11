// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter


#include "pch.h"
#include <iostream>
#include <stdexcept>

#include <hints.hpp>
#include <utils.hpp>

#include "myapp.hpp"


static void error_callback(int error, const char* description)
{
  std::cerr << "Error# " << error << ": " << description << '\n';
}

void SetWindowHints()
{
  glfwDefaultWindowHints();
  GLFWPP_ns::SetContextHintOGLVersion(4, 6);
  GLFWPP_ns::SetContextHintOGLProfile(GLFWPP_ns::GL_PROFILE::CORE);
  GLFWPP_ns::SetContextHintOGLForwardCompatibility(true);
  GLFWPP_ns::SetContextHintOGLDebugContext(true);

  std::cout << "Requesting context: Version 4.6, Core, forward compatibility, debug on\n" << std::endl;
}

int main()
{
  ::glfwSetErrorCallback(error_callback);

  try
  {
    MyApp app{ new MyWindow{ 640, 480, "Rename me", SetWindowHints } };
    if(app.init())
    {
      GLFWPP_ns::PrintGLVersionInfo(std::cout);
      GLFWPP_ns::SetDefaultDebugCallback(std::cout);
      app.run();
    }
    else
    {
      return 1;
    }
  }
  catch(std::exception const & e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}

