//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once

namespace GLFWPP_ns
{

constexpr auto const HINT_DONT_CARE = -1;

enum class GL_API : char {OPENGL, ES, N};
char const * const GL_API_STR[static_cast<char>(GL_API::N)]{"OPENGL", "ES"};

enum class GL_PROFILE : char {ANY, COMPAT, CORE, N};
char const * const GL_PROFILE_STR[static_cast<char>(GL_PROFILE::N)]{"ANY", "COMPAT", "CORE"};

enum class GL_ROBUSTNESS : char {NONE, NO_RESET_NOTIFICATION, LOSE_CONTEXT_ON_RESET, N};
char const * const GL_ROBUSTNESS_STR[static_cast<char>(GL_ROBUSTNESS::N)]{"NONE", "NO_RESET_NOTIFICATION", "LOSE_CONTEXT_ON_RESET"};

enum class GL_CONTEXT_RELEASE_BEHAVIOUR : char {ANY, FLUSH, NONE, N};
char const * const GL_CONTEXT_RELEASE_BEHAVIOUR_STR[static_cast<char>(GL_CONTEXT_RELEASE_BEHAVIOUR::N)]{"ANY", "FLUSH", "NONE"};

struct versionAttributes
{
  int major;
  int minor;
  int revision;
};

struct contextAttributes
{
  GL_API            api;
  versionAttributes version;
  GL_PROFILE        profile;
  bool              forward;
  bool              debug;
};

}

