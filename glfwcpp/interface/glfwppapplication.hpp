// glfwppapplication.hpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once

namespace GLFWPP_ns
{

class Application
{
public:
  explicit Application();
  virtual ~Application();


  Application(Application const &)             = delete;
  Application & operator=(Application const &) = delete;
  Application(Application&&)                   = delete;
  Application & operator=(Application&&)       = delete;


  virtual bool init() = 0;
  virtual void render(double currentTime) = 0;
  virtual void run() = 0;
  virtual void shutdown() = 0;
};

}
