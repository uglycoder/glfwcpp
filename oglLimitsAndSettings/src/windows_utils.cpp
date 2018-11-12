// windows_utils.cpp
//
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter


#include "stdafx.h"

#include "windows_utils.hpp"

#include <tchar.h>
#include <Windows.h>
#include <gl/GL.h>
#include <rpc.h>

#include <atlstr.h>

#include <cassert>

#include <algorithm>
#include <string>
#include <utility>


namespace
{
  LRESULT CALLBACK WndProc(
    ::HWND	hWnd,                       // Handle For This Window
    unsigned int	uMsg,                 // Message For This Window
    WPARAM	wParam,                     // Additional Message Information
    LPARAM	lParam)	noexcept;           // Additional Message Information

  ::ATOM RegisterWndClass(::WNDCLASSEX const & wc) noexcept;

  std::wstring CreateWndClassName() noexcept;
}

//#########################################################################################
std::tuple<::HWND, std::wstring, ::HINSTANCE> windows_ns::CreateTemporaryWindow() noexcept
{
  ::HINSTANCE const hInstance{ ::GetModuleHandle(nullptr) };
  auto const & className{ CreateWndClassName() };

  // setup window class
  ::WNDCLASSEX wc
  {
    sizeof ::WNDCLASSEX   // cbSize
    , CS_OWNDC            // set style for this class unique DC
    , (::WNDPROC)WndProc  // lpfnWndProc
    , 0                   // cbClsExtra: Extra class memory
    , 0                   // cbWndExtra: Extra window memory
    , hInstance           // Use this module for the module handle
    , nullptr             // hIcon
    , nullptr             // hCursor
    , nullptr             // hbrBackground
    , nullptr             // lpszMenuName
    , className.data()    // lpszClassName
    , nullptr             // hIconSm
  };


  RegisterWndClass(wc);

  DWORD dwExtStyle{ WS_EX_APPWINDOW | WS_EX_WINDOWEDGE };
  DWORD dwWindStyle{ WS_OVERLAPPEDWINDOW };

  // Create window
  auto const hWnd = ::CreateWindowEx(dwExtStyle,     // Extended style
    className.data(),        // class name
    nullptr,            // window name
    dwWindStyle |
    WS_CLIPSIBLINGS |
    WS_CLIPCHILDREN,    // window stlye
    0,                  // window position, x
    0,                  // window position, y
    0,                  // height
    0,                  // width
    HWND_MESSAGE,       // message only window
    nullptr,            // menu
    hInstance,          // instance
    nullptr);           // pass this to WM_CREATE

  return {hWnd, className, hInstance};
}

/////

std::string windows_ns::GetWGLErrorString(int wglErrCode) noexcept
{
  constexpr std::pair<int, char const * const> wglErrCodes[]
  {
    //  WGL_ARB_create_context, WGL_ARB_create_context_profile
     { 0x2095,                      "Invalid OpenGL version number requested"}
    ,{ 0x2096,                      "Invalid OpenGL profile requested" }
    ,{ ERROR_INVALID_OPERATION,     "Invalid share context handle given" }
    ,{ ERROR_DC_NOT_FOUND,          "Invalid hDC device context handle given" }
    ,{ ERROR_INVALID_PIXEL_FORMAT,  "Invalid pixel format given" }
    ,{ ERROR_NO_SYSTEM_RESOURCES,   "Insufficent system resources for new context" }
    ,{ ERROR_INVALID_PARAMETER,     "Unrecognised name in attribute list" }
  };

  // WglErrCode from the OGL driver sets the high 2 bytes to some non-zero value but the OpenGL spec defines the error codes
  // as values in the range of 2 unsigned bytes .
  int const errCode{ wglErrCode & 0x0000'FFFF };
  auto const pr{ std::find_if(std::cbegin(wglErrCodes), std::cend(wglErrCodes), [errCode](auto const & pr) {return pr.first == errCode; }) };

  return pr != std::cend(wglErrCodes) ? pr->second : "";
}

/////

std::string windows_ns::GetLastError(DWORD errCode) noexcept
{
  // Retrieve the system error message for the last-error code

  char * lpMsgBuf{};

  ::FormatMessageA(
    FORMAT_MESSAGE_ALLOCATE_BUFFER |
    FORMAT_MESSAGE_FROM_SYSTEM |
    FORMAT_MESSAGE_IGNORE_INSERTS,
    nullptr,
    errCode,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (decltype(lpMsgBuf))&lpMsgBuf,
    0, nullptr);

  std::string const str{ lpMsgBuf };

  ::LocalFree(lpMsgBuf);
  return str;
}

/////

std::string windows_ns::GetString(unsigned int ids) noexcept
{
  ::CStringA const cstr{ MAKEINTRESOURCE(ids) };

  return cstr.GetString();
}

//#############################################################################

namespace
{
  LRESULT CALLBACK WndProc(
    ::HWND	hWnd,		                    // Handle For This Window
    unsigned int	uMsg,		                      // Message For This Window
    WPARAM	wParam,		                  // Additional Message Information
    LPARAM	lParam)	noexcept            // Additional Message Information
  {
    // Pass All Unhandled Messages To DefWindowProc
    return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
  }

  /////

  ::ATOM RegisterWndClass(::WNDCLASSEX const & wc) noexcept
  {
    constexpr WORD MAX_WND_CLASS_NAME_LEN{ 256U };

    // The maximum length for lpszClassName is 256. If lpszClassName is greater
    // than the maximum length, the RegisterClassEx function will fail.
    assert(::_tcslen(wc.lpszClassName) < MAX_WND_CLASS_NAME_LEN);
    return ::RegisterClassEx(&wc);
  }

  /////

  std::wstring CreateWndClassName() noexcept
  {
    std::wstring wstr{ L"tempWnd_" };
    ::UUID guid{};
    if(auto const guidOK{ ::UuidCreate(&guid) }; guidOK == RPC_S_OK) {

      ::RPC_WSTR str{};
      if(auto const strOK{ ::UuidToStringW(&guid, &str) }; strOK == RPC_S_OK)
      {
        wstr += reinterpret_cast<wchar_t*>(str);

        ::RpcStringFreeW(&str);

        return wstr;
      }
    }
    // fallback 
    auto const & numStr{ std::to_wstring(reinterpret_cast<std::size_t>(wstr.data())) };
    return wstr + numStr;
  }
}