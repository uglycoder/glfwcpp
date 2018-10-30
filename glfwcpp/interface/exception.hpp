// glfwppexception.hpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once

#include <stdexcept>
#include <string>


namespace GLFWPP_ns
{

class Exception : public std::runtime_error
{
  using std::runtime_error::runtime_error;

public:
  static std::string formatMsg(std::string const & fileName, std::string const & lineNumber, std::string const & msg);
};

} // namespace GLFWPP_ns

