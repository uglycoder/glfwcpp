// myapp.cpp
// Creation date: 2015-03-26 11:25:09 +0000
// Copyright 2015 Simon Parmenter. All rights reserved.

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

