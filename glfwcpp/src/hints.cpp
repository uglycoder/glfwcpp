//  hints.cpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#include "precompiled-header.hpp"

#include "../interface/hints.hpp"

#include <cassert>


void GLFWPP_ns::SetHintsToDefault() noexcept
{
  ::glfwDefaultWindowHints();
}

////////////////////////////////////////////////////////////////////////////////
void GLFWPP_ns::SetWindowHintResizable(bool flag) noexcept
{
  ::glfwWindowHint(GLFW_RESIZABLE, flag);
}

void GLFWPP_ns::SetWindowHintVisible(bool flag) noexcept
{
  ::glfwWindowHint(GLFW_VISIBLE, flag);
}

void GLFWPP_ns::SetWindowHintDecorated(bool flag) noexcept
{
  ::glfwWindowHint(GLFW_DECORATED, flag);
}

void GLFWPP_ns::SetWindowHintFocused(bool flag) noexcept
{
  ::glfwWindowHint(GLFW_FOCUSED, flag);
}

void GLFWPP_ns::SetWindowHintAutoIconify(bool flag) noexcept
{
  ::glfwWindowHint(GLFW_AUTO_ICONIFY, flag);
}

void GLFWPP_ns::SetWindowHintFloating(bool flag) noexcept
{
  ::glfwWindowHint(GLFW_FLOATING, flag);
}

////////////////////////////////////////////////////////////////////////////////

void GLFWPP_ns::SetFrameBufferHintStereo(bool flag) noexcept
{
  ::glfwWindowHint(GLFW_STEREO, flag);
}

void GLFWPP_ns::SetFrameBufferHintDoubleBuffer(bool flag) noexcept
{
  ::glfwWindowHint(GLFW_DOUBLEBUFFER, flag);
}

void GLFWPP_ns::SetFrameBufferHintColorAlphaBits(int red, int green, int blue, int alpha) noexcept
{
  ::glfwWindowHint(GLFW_RED_BITS,   red);
  ::glfwWindowHint(GLFW_GREEN_BITS, green);
  ::glfwWindowHint(GLFW_BLUE_BITS,  blue);
  ::glfwWindowHint(GLFW_ALPHA_BITS, alpha);
}

void GLFWPP_ns::SetFrameBufferHintDepthBits(int depth) noexcept
{
  ::glfwWindowHint(GLFW_DEPTH_BITS, depth);
}

void GLFWPP_ns::SetFrameBufferHintStencilBits(int number) noexcept
{
  ::glfwWindowHint(GLFW_STENCIL_BITS, number);
}

void GLFWPP_ns::SetFrameBufferHintSamples(int number) noexcept
{
  ::glfwWindowHint(GLFW_SAMPLES, number);
}

void GLFWPP_ns::SetFrameBufferHintSRGB(bool isCapable) noexcept
{
  ::glfwWindowHint(GLFW_SRGB_CAPABLE, isCapable);
}

void GLFWPP_ns::SetFrameBufferHintAccumBits(int red, int green, int blue, int alpha) noexcept
{
  ::glfwWindowHint(GLFW_ACCUM_RED_BITS,   red);
  ::glfwWindowHint(GLFW_ACCUM_GREEN_BITS, green);
  ::glfwWindowHint(GLFW_ACCUM_BLUE_BITS,  blue);
  ::glfwWindowHint(GLFW_ACCUM_ALPHA_BITS, alpha);
}

void GLFWPP_ns::SetFrameBufferHintAuxBuffers(int number) noexcept
{
  ::glfwWindowHint(GLFW_AUX_BUFFERS, number);
}

////////////////////////////////////////////////////////////////////////////////

void GLFWPP_ns::SetContextHintClientAPI(GL_API api) noexcept
{
  switch(api)
  {
  case GL_API::OPENGL:
    ::glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    break;
  case GL_API::ES:
    ::glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    break;
  default:
    assert(false);
  }
}

void GLFWPP_ns::SetContextHintOGLVersion(unsigned int major, unsigned int minor) noexcept
{
  ::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
  ::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
}

void GLFWPP_ns::SetContextHintOGLForwardCompatibility(bool flag) noexcept
{
  ::glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, flag);
}

void GLFWPP_ns::SetContextHintOGLDebugContext(bool flag) noexcept
{
  ::glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, flag);
}

void GLFWPP_ns::SetContextHintOGLProfile(GL_PROFILE type) noexcept
{
  switch(type)
  {
  case GL_PROFILE::ANY:
    ::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    break;
  case GL_PROFILE::COMPAT:
    ::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    break;
  case GL_PROFILE::CORE:
    ::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    break;
  default:
    assert(false);
  }
}

void GLFWPP_ns::SetContextHintOGLRobustness(GL_ROBUSTNESS type) noexcept
{
  switch(type)
  {
  case GL_ROBUSTNESS::NONE:
    ::glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_NO_ROBUSTNESS);
    break;
  case GL_ROBUSTNESS::NO_RESET_NOTIFICATION:
    ::glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_NO_RESET_NOTIFICATION);
    break;
  case GL_ROBUSTNESS::LOSE_CONTEXT_ON_RESET:
    ::glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET);
    break;
  default:
    assert(false);
  }
}

void GLFWPP_ns::SetContextHintOGLReleaseBehaviour(GL_CONTEXT_RELEASE_BEHAVIOUR type) noexcept
{
  switch(type)
  {
  case GL_CONTEXT_RELEASE_BEHAVIOUR::ANY:
    ::glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_ANY_RELEASE_BEHAVIOR);
    break;
  case GL_CONTEXT_RELEASE_BEHAVIOUR::FLUSH:
    ::glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_RELEASE_BEHAVIOR_FLUSH);
    break;
  case GL_CONTEXT_RELEASE_BEHAVIOUR::NONE:
    ::glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_RELEASE_BEHAVIOR_NONE);
    break;
  default:
    assert(false);
  }
}
////////////////////////////////////////////////////////////////////////////////

void GLFWPP_ns::SetMonitorHint(int value) noexcept
{
  ::glfwWindowHint(GLFW_REFRESH_RATE, value);
}
