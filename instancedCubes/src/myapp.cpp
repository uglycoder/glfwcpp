// myapp.cpp

//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#include "pch.h"

#pragma warning (push)
#pragma warning (disable:4201)
#include "myapp.hpp"
#pragma warning (pop)

MyApp::MyApp(GLFWPP_ns::Window * wnd) : m_wnd{wnd}
{
}

MyApp::~MyApp()
{
}

bool MyApp::init()
{

  m_wnd->registerCallbacks();

  return m_wnd->init();
}

void MyApp::render(double currentTime)
{
  m_wnd->render(currentTime);
}

void MyApp::run()
{
  while(!m_wnd->shouldClose())
  {
    glfwPollEvents();
    render(glfwGetTime());
  }
}

void MyApp::shutdown()
{

}

