// windows_utils.hpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once


#include <Windows.h>

#include <string>
#include <utility>


namespace windows_ns
{
  std::tuple<::HWND, std::wstring, ::HINSTANCE> CreateTemporaryWindow() noexcept;

  std::string GetWGLErrorString(int wglErrCode) noexcept;

  std::string GetLastError(DWORD errCode) noexcept;

  std::string GetString(unsigned int ids) noexcept;
 }



