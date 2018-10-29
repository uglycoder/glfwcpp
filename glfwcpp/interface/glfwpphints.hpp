// glfwpphints.hpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once

#include "glfwpp.hpp"

namespace GLFWPP_ns
{

 void SetHintsToDefault() noexcept;

/* Window related hints
 * GLFW_RESIZABLE:    *GL_TRUE or  GL_FALSE
 * GLFW_VISIBLE:      *GL_TRUE or  GL_FALSE
 * GLFW_DECORATED:    *GL_TRUE or  GL_FALSE
 * GLFW_FOCUSED:      *GL_TRUE or  GL_FALSE
 * GLFW_AUTO_ICONIFY: *GL_TRUE or  GL_FALSE
 * GLFW_FLOATING:      GL_TRUE or *GL_FALSE
 */
void SetWindowHintResizable(bool flag = true) noexcept;
void SetWindowHintVisible(bool flag = true) noexcept;
void SetWindowHintDecorated(bool flag = true) noexcept;
void SetWindowHintFocused(bool flag = true) noexcept;
void SetWindowHintAutoIconify(bool flag = true) noexcept;
void SetWindowHintFloating(bool flag = false) noexcept;

/* Framebuffer related hints
 * Hard constraints:
 * These must match the available capabilities exactly for window and context
 * creation to succeed.
 * GLFW_STEREO:        GL_TRUE         or *GL_FALSE
 * GLFW_DOUBLEBUFFER: *GL_TRUE         or  GL_FALSE
 **                   *
 * GLFW_RED_BITS 	    8 	0 to INT_MAX or GLFW_DONT_CARE
 * GLFW_GREEN_BITS 	  8 	0 to INT_MAX or GLFW_DONT_CARE
 * GLFW_BLUE_BITS 	  8 	0 to INT_MAX or GLFW_DONT_CARE
 * GLFW_ALPHA_BITS 	  8 	0 to INT_MAX or GLFW_DONT_CARE
 * GLFW_DEPTH_BITS 	  24 	0 to INT_MAX or GLFW_DONT_CARE
 * GLFW_STENCIL_BITS 	8 	0 to INT_MAX or GLFW_DONT_CARE
 *                        *
 * GLFW_SAMPLES 	        0 	      0 to INT_MAX or GLFW_DONT_CARE
 * GLFW_SRGB_CAPABLE 	    GL_FALSE 	GL_TRUE      or GL_FALSE
 *
 * Legacy OpenGL          *
 * GLFW_ACCUM_RED_BITS 	  0 	0 to INT_MAX or GLFW_DONT_CARE
 * GLFW_ACCUM_GREEN_BITS 	0 	0 to INT_MAX or GLFW_DONT_CARE
 * GLFW_ACCUM_BLUE_BITS 	0 	0 to INT_MAX or GLFW_DONT_CARE
 * GLFW_ACCUM_ALPHA_BITS 	0 	0 to INT_MAX or GLFW_DONT_CARE
 * GLFW_AUX_BUFFERS 	    0 	0 to INT_MAX or GLFW_DONT_CARE
 *
*/
void SetFrameBufferHintStereo(bool flag = false) noexcept;
void SetFrameBufferHintDoubleBuffer(bool flag = true) noexcept;

void SetFrameBufferHintColorAlphaBits(int red = 8, int green = 8, int blue = 8, int alpha = 8) noexcept;
void SetFrameBufferHintDepthBits(int depth = 24) noexcept;
void SetFrameBufferHintStencilBits(int number = 8) noexcept;

void SetFrameBufferHintSamples(int number = 0 ) noexcept;
void SetFrameBufferHintSRGB(bool isCapable = false ) noexcept;

void SetFrameBufferHintAccumBits(int red = 0, int green = 0, int blue = 0, int alpha = 0) noexcept;
void SetFrameBufferHintAuxBuffers(int number = 0 ) noexcept;


/* Context related hints
 * Hard constraint:
 * This must match the available capability exactly for window and context
 * creation to succeed.
 * GLFW_CLIENT_API:   *GLFW_OPENGL_API or  GLFW_OPENGL_ES_API
 *                              *
 * GLFW_CONTEXT_VERSION_MAJOR 	1 	Any valid major version number of the chosen client API
 * GLFW_CONTEXT_VERSION_MINOR 	0 	Any valid minor version number of the chosen client API
 *
 * GLFW_OPENGL_FORWARD_COMPAT 	GL_TRUE or *GL_FALSE
 * GLFW_OPENGL_DEBUG_CONTEXT 	 	GL_TRUE or *GL_FALSE
 *                              *
 * GLFW_OPENGL_PROFILE 	*GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_COMPAT_PROFILE or GLFW_OPENGL_CORE_PROFILE
 *
 * GLFW_CONTEXT_ROBUSTNESS 	      *GLFW_NO_ROBUSTNESS, GLFW_NO_RESET_NOTIFICATION or GLFW_LOSE_CONTEXT_ON_RESET
 * GLFW_CONTEXT_RELEASE_BEHAVIOR 	*GLFW_ANY_RELEASE_BEHAVIOR, GLFW_RELEASE_BEHAVIOR_FLUSH or GLFW_RELEASE_BEHAVIOR_NONE
 */
void SetContextHintClientAPI(GL_API api = GL_API::OPENGL) noexcept;

void SetContextHintOGLVersion(unsigned int major = 1, unsigned int minor = 0) noexcept;
void SetContextHintOGLForwardCompatibility(bool flag = false) noexcept;
void SetContextHintOGLDebugContext(bool flag = false) noexcept;

void SetContextHintOGLProfile(GL_PROFILE type = GL_PROFILE::ANY) noexcept;

void SetContextHintOGLRobustness(GL_ROBUSTNESS type = GL_ROBUSTNESS::NONE) noexcept;

void SetContextHintOGLReleaseBehaviour(GL_CONTEXT_RELEASE_BEHAVIOUR type = GL_CONTEXT_RELEASE_BEHAVIOUR::ANY) noexcept;

/* Monitor related hints
 *
 * GLFW_REFRESH_RATE 	0 to INT_MAX or *GLFW_DONT_CARE
 *
 */
void SetMonitorHint(int value = HINT_DONT_CARE) noexcept;
} // namespace GLFWPP_ns

