//  exception.cpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#include "precompiled-header.hpp"

#include "../interface/exception.hpp"


// static
std::string GLFWPP_ns::Exception::formatMsg(
    std::string const & fileName, std::string const & lineNumber, std::string const & msg)
{
  return std::string{"Error: " + fileName + ":" + lineNumber + ": " + msg};
}

