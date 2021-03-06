// main.cpp : instancedCubes: This file contains the 'main' function. Program execution begins and ends there.
//
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter


#include "pch.h"
#include <iostream>
#include <stdexcept>

#include <hints.hpp>
#include <utils.hpp>

#include "src/myapp.hpp"


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
    MyApp app{new MyWindow{ 640, 480, "Instanced Cubes", SetWindowHints }};
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


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
