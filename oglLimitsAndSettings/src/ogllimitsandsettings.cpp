// ogllimitsandsettings.cpp : Defines the exported functions for the DLL application.
//
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#include "stdafx.h"

#include "../interface/ogllimitsandsettings.hpp"

#include "oglwindow.hpp"

#define GL_NO_ERROR                                  0


namespace
{
  enum class OGL_LIMIT_KEYS
  {
    MAX_CLIP_DISTANCES                          = 0x0D32
    , MAX_TEXTURE_SIZE                          = 0x0D33
    , MAX_3D_TEXTURE_SIZE                       = 0x8073
    , MAX_ELEMENTS_VERTICES                     = 0x80E8
    , MAX_ELEMENTS_INDICES                      = 0x80E9
    , NUM_EXTENSIONS                            = 0x821D
    , MAX_VIEWPORTS                             = 0x825B
    , MAX_COMPUTE_UNIFORM_COMPONENTS            = 0x8263
    , MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS        = 0x8264
    , MAX_COMPUTE_ATOMIC_COUNTERS               = 0x8265
    , MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS   = 0x8266
    , MAX_DEBUG_GROUP_STACK_DEPTH               = 0x826C
    , MAX_UNIFORM_LOCATIONS                     = 0x826E
    , MAX_VERTEX_ATTRIB_RELATIVE_OFFSET         = 0x82D9
    , MAX_VERTEX_ATTRIB_BINDINGS                = 0x82DA
    , MAX_LABEL_LENGTH                          = 0x82E8
    , MAX_RENDERBUFFER_SIZE                     = 0x84E8
    , MAX_RECTANGLE_TEXTURE_SIZE                = 0x84F8
    , MAX_TEXTURE_LOD_BIAS                      = 0x84FD
    , MAX_CUBE_MAP_TEXTURE_SIZE                 = 0x851C
    , NUM_COMPRESSED_TEXTURE_FORMATS            = 0x86A2
    , NUM_PROGRAM_BINARY_FORMATS                = 0x87FE
    , MAX_DRAW_BUFFERS                          = 0x8824
    , MAX_VERTEX_ATTRIBS                        = 0x8869
    , MAX_TEXTURE_IMAGE_UNITS                   = 0x8872
    , MAX_DUAL_SOURCE_DRAW_BUFFERS              = 0x88FC
    , MAX_ARRAY_TEXTURE_LAYERS                  = 0x88FF
    , MIN_PROGRAM_TEXEL_OFFSET                  = 0x8904
    , MAX_PROGRAM_TEXEL_OFFSET                  = 0x8905
    , MAX_VERTEX_UNIFORM_BLOCKS                 = 0x8A2B
    , MAX_GEOMETRY_UNIFORM_BLOCKS               = 0x8A2C
    , MAX_FRAGMENT_UNIFORM_BLOCKS               = 0x8A2D
    , MAX_COMBINED_UNIFORM_BLOCKS               = 0x8A2E
    , MAX_UNIFORM_BUFFER_BINDINGS               = 0x8A2F
    , MAX_UNIFORM_BLOCK_SIZE                    = 0x8A30
    , MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS    = 0x8A31
    , MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS  = 0x8A32
    , MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS  = 0x8A33
    , MAX_FRAGMENT_UNIFORM_COMPONENTS           = 0x8B49
    , MAX_VERTEX_UNIFORM_COMPONENTS             = 0x8B4A
    , MAX_VARYING_COMPONENTS                    = 0x8B4B
    , MAX_VERTEX_TEXTURE_IMAGE_UNITS            = 0x8B4C
    , MAX_COMBINED_TEXTURE_IMAGE_UNITS          = 0x8B4D
    , MAX_GEOMETRY_TEXTURE_IMAGE_UNITS          = 0x8C29
    , MAX_TEXTURE_BUFFER_SIZE                   = 0x8C2B
    , MAX_ELEMENT_INDEX                         = 0x8D6B
    , MAX_GEOMETRY_UNIFORM_COMPONENTS           = 0x8DDF
    , NUM_SHADER_BINARY_FORMATS                 = 0x8DF9
    , MAX_VERTEX_UNIFORM_VECTORS                = 0x8DFB
    , MAX_VARYING_VECTORS                       = 0x8DFC
    , MAX_FRAGMENT_UNIFORM_VECTORS              = 0x8DFD
    , MAX_SAMPLE_MASK_WORDS                     = 0x8E59
    , MIN_MAP_BUFFER_ALIGNMENT                  = 0x90BC
    , MAX_VERTEX_SHADER_STORAGE_BLOCKS          = 0x90D6
    , MAX_GEOMETRY_SHADER_STORAGE_BLOCKS        = 0x90D7
    , MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS    = 0x90D8
    , MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS = 0x90D9
    , MAX_FRAGMENT_SHADER_STORAGE_BLOCKS        = 0x90DA
    , MAX_COMPUTE_SHADER_STORAGE_BLOCKS         = 0x90DB
    , MAX_COMBINED_SHADER_STORAGE_BLOCKS        = 0x90DC
    , MAX_SHADER_STORAGE_BUFFER_BINDINGS        = 0x90DD
    , MAX_COMPUTE_WORK_GROUP_INVOCATIONS        = 0x90EB
    , MAX_COLOR_TEXTURE_SAMPLES                 = 0x910E
    , MAX_DEPTH_TEXTURE_SAMPLES                 = 0x910F
    , MAX_INTEGER_SAMPLES                       = 0x9110
    , MAX_SERVER_WAIT_TIMEOUT                   = 0x9111
    , MAX_VERTEX_OUTPUT_COMPONENTS              = 0x9122
    , MAX_GEOMETRY_INPUT_COMPONENTS             = 0x9123
    , MAX_GEOMETRY_OUTPUT_COMPONENTS            = 0x9124
    , MAX_FRAGMENT_INPUT_COMPONENTS             = 0x9125
    , MAX_COMPUTE_UNIFORM_BLOCKS                = 0x91BB
    , MAX_COMPUTE_TEXTURE_IMAGE_UNITS           = 0x91BC
    , MAX_VERTEX_ATOMIC_COUNTERS                = 0x92D2
    , MAX_TESS_CONTROL_ATOMIC_COUNTERS          = 0x92D3
    , MAX_TESS_EVALUATION_ATOMIC_COUNTERS       = 0x92D4
    , MAX_GEOMETRY_ATOMIC_COUNTERS              = 0x92D5
    , MAX_FRAGMENT_ATOMIC_COUNTERS              = 0x92D6
    , MAX_COMBINED_ATOMIC_COUNTERS              = 0x92D7
    , MAX_FRAMEBUFFER_WIDTH                     = 0x9315
    , MAX_FRAMEBUFFER_HEIGHT                    = 0x9316
    , MAX_FRAMEBUFFER_LAYERS                    = 0x9317
    , MAX_FRAMEBUFFER_SAMPLES                   = 0x9318
  };

  OGLLimitsAndSettings_ns::svLimitsMap_t gSingleValueMap
  {
#define SVMAPENTRY( arg ) { static_cast<uint32_t>(OGL_LIMIT_KEYS::arg), OGLLimitsAndSettings_ns::limitsValue_t{-1,#arg} }
    SVMAPENTRY(MAX_3D_TEXTURE_SIZE),
    SVMAPENTRY(MAX_ARRAY_TEXTURE_LAYERS),
    SVMAPENTRY(MAX_CLIP_DISTANCES),
    SVMAPENTRY(MAX_COLOR_TEXTURE_SAMPLES),
    SVMAPENTRY(MAX_COMBINED_ATOMIC_COUNTERS),
    SVMAPENTRY(MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS),
    SVMAPENTRY(MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS),
    SVMAPENTRY(MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS),
    SVMAPENTRY(MAX_COMBINED_SHADER_STORAGE_BLOCKS),
    SVMAPENTRY(MAX_COMBINED_TEXTURE_IMAGE_UNITS),
    SVMAPENTRY(MAX_COMBINED_UNIFORM_BLOCKS),
    SVMAPENTRY(MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS),
    SVMAPENTRY(MAX_COMPUTE_ATOMIC_COUNTERS),
    SVMAPENTRY(MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS),
    SVMAPENTRY(MAX_COMPUTE_SHADER_STORAGE_BLOCKS),
    SVMAPENTRY(MAX_COMPUTE_TEXTURE_IMAGE_UNITS),
    SVMAPENTRY(MAX_COMPUTE_UNIFORM_BLOCKS),
    SVMAPENTRY(MAX_COMPUTE_UNIFORM_COMPONENTS),
    SVMAPENTRY(MAX_COMPUTE_WORK_GROUP_INVOCATIONS),
    SVMAPENTRY(MAX_CUBE_MAP_TEXTURE_SIZE),
    SVMAPENTRY(MAX_DEBUG_GROUP_STACK_DEPTH),
    SVMAPENTRY(MAX_DEPTH_TEXTURE_SAMPLES),
    SVMAPENTRY(MAX_DRAW_BUFFERS),
    SVMAPENTRY(MAX_DUAL_SOURCE_DRAW_BUFFERS),
    SVMAPENTRY(MAX_ELEMENTS_INDICES),
    SVMAPENTRY(MAX_ELEMENTS_VERTICES),
    SVMAPENTRY(MAX_ELEMENT_INDEX),
    SVMAPENTRY(MAX_FRAGMENT_ATOMIC_COUNTERS),
    SVMAPENTRY(MAX_FRAGMENT_INPUT_COMPONENTS),
    SVMAPENTRY(MAX_FRAGMENT_SHADER_STORAGE_BLOCKS),
    SVMAPENTRY(MAX_FRAGMENT_UNIFORM_BLOCKS),
    SVMAPENTRY(MAX_FRAGMENT_UNIFORM_COMPONENTS),
    SVMAPENTRY(MAX_FRAGMENT_UNIFORM_VECTORS),
    SVMAPENTRY(MAX_FRAMEBUFFER_HEIGHT),
    SVMAPENTRY(MAX_FRAMEBUFFER_LAYERS),
    SVMAPENTRY(MAX_FRAMEBUFFER_SAMPLES),
    SVMAPENTRY(MAX_FRAMEBUFFER_WIDTH),
    SVMAPENTRY(MAX_GEOMETRY_ATOMIC_COUNTERS),
    SVMAPENTRY(MAX_GEOMETRY_INPUT_COMPONENTS),
    SVMAPENTRY(MAX_GEOMETRY_OUTPUT_COMPONENTS),
    SVMAPENTRY(MAX_GEOMETRY_SHADER_STORAGE_BLOCKS),
    SVMAPENTRY(MAX_GEOMETRY_TEXTURE_IMAGE_UNITS),
    SVMAPENTRY(MAX_GEOMETRY_UNIFORM_BLOCKS),
    SVMAPENTRY(MAX_GEOMETRY_UNIFORM_COMPONENTS),
    SVMAPENTRY(MAX_INTEGER_SAMPLES),
    SVMAPENTRY(MAX_LABEL_LENGTH),
    SVMAPENTRY(MAX_PROGRAM_TEXEL_OFFSET),
    SVMAPENTRY(MAX_RECTANGLE_TEXTURE_SIZE),
    SVMAPENTRY(MAX_RENDERBUFFER_SIZE),
    SVMAPENTRY(MAX_SAMPLE_MASK_WORDS),
    SVMAPENTRY(MAX_SERVER_WAIT_TIMEOUT),
    SVMAPENTRY(MAX_SHADER_STORAGE_BUFFER_BINDINGS),
    SVMAPENTRY(MAX_TESS_CONTROL_ATOMIC_COUNTERS),
    SVMAPENTRY(MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS),
    SVMAPENTRY(MAX_TESS_EVALUATION_ATOMIC_COUNTERS),
    SVMAPENTRY(MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS),
    SVMAPENTRY(MAX_TEXTURE_BUFFER_SIZE),
    SVMAPENTRY(MAX_TEXTURE_IMAGE_UNITS),
    SVMAPENTRY(MAX_TEXTURE_LOD_BIAS),
    SVMAPENTRY(MAX_TEXTURE_SIZE),
    SVMAPENTRY(MAX_UNIFORM_BLOCK_SIZE),
    SVMAPENTRY(MAX_UNIFORM_BUFFER_BINDINGS),
    SVMAPENTRY(MAX_UNIFORM_LOCATIONS),
    SVMAPENTRY(MAX_VARYING_COMPONENTS),
    SVMAPENTRY(MAX_VARYING_VECTORS),
    SVMAPENTRY(MAX_VERTEX_ATOMIC_COUNTERS),
    SVMAPENTRY(MAX_VERTEX_ATTRIBS),
    SVMAPENTRY(MAX_VERTEX_ATTRIB_BINDINGS),
    SVMAPENTRY(MAX_VERTEX_ATTRIB_RELATIVE_OFFSET),
    SVMAPENTRY(MAX_VERTEX_OUTPUT_COMPONENTS),
    SVMAPENTRY(MAX_VERTEX_SHADER_STORAGE_BLOCKS),
    SVMAPENTRY(MAX_VERTEX_TEXTURE_IMAGE_UNITS),
    SVMAPENTRY(MAX_VERTEX_UNIFORM_BLOCKS),
    SVMAPENTRY(MAX_VERTEX_UNIFORM_COMPONENTS),
    SVMAPENTRY(MAX_VERTEX_UNIFORM_VECTORS),
    SVMAPENTRY(MAX_VIEWPORTS),
    SVMAPENTRY(MIN_MAP_BUFFER_ALIGNMENT),
    SVMAPENTRY(MIN_PROGRAM_TEXEL_OFFSET),
    SVMAPENTRY(NUM_COMPRESSED_TEXTURE_FORMATS),
    SVMAPENTRY(NUM_EXTENSIONS),
    SVMAPENTRY(NUM_PROGRAM_BINARY_FORMATS),
    SVMAPENTRY(NUM_SHADER_BINARY_FORMATS),

#undef SVMAPENTRY
  };

  [[nodiscard]] OGLLimitsAndSettings_ns::OGL_SingleValueLimits GetSVLimits() noexcept;
}


// ############################## interface ####################################
OGLLimitsAndSettings_ns::OGL_SingleValueLimits OGLLimitsAndSettings_ns::GetSingleValueLimits() noexcept
{
  try
  {
    return std::async(std::launch::async, GetSVLimits).get();
  }
  catch(std::exception const & e)
  {
    return{ {}, e.what() };
  }
}
// ########################## end interface ####################################


namespace
{
  extern "C" unsigned int WINAPI glGetError();
  extern "C" void WINAPI glGetIntegerv(unsigned int pname, int  *params);

  void ClearGLErrors() noexcept
  {
    while(glGetError());
  }

  using glGetInteger64v_ptr = void (*)(unsigned int, int64_t * );

  struct limitResult
  {
    int64_t value{};
    std::string errMsg;
  };

    
  limitResult const g_LimitError{0, "Error: could not retrieve value"};

  [[nodiscard]] limitResult GetSVLimit(int glDefine) noexcept
  {
    ClearGLErrors();

    int64_t value{};

    // It seems that glGetInteger64v is a 64bit binary on NVidia, Windows 10 Pro 64bit
    // in both x64 and x86 builds - not checked out on other GPU  Windows versions
    if constexpr(sizeof std::nullptr_t == 4) // x86 build
    {
      glGetIntegerv(glDefine, reinterpret_cast<int32_t*>(&value));
    }
    else // x64 builds
    {
      auto const getInteger64vPtr  = (glGetInteger64v_ptr)::wglGetProcAddress("glGetInteger64v");
      getInteger64vPtr(glDefine, &value);
    }
    
    if(glGetError() != GL_NO_ERROR)
    {
      return g_LimitError;
    }

    return { value };
  }

  OGLLimitsAndSettings_ns::OGL_SingleValueLimits GetSVLimits() noexcept
  {
    if(auto const glcxt{ oglwindow_ns::CreateTemporaryOGL_Window().ctxt_ptr }; glcxt)
    {
      for(auto & [key, value]: gSingleValueMap)
      {
        auto const & [val, msg] {GetSVLimit(key)};
        value.value = val;
        value.errMsg = msg;
      }
      return { gSingleValueMap };
    }
    return { gSingleValueMap, "Error: could not create OpenGL context." };
  }
}
