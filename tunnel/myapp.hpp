// myapp.hpp

//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once

#include "../include/common_stl_hdrs.hpp"

#include <globalutils.hpp>

#include <application.hpp>
#include <window.hpp>
#include <glslprogram.hpp>
#include <vertexarrayobject.hpp>
#include <vertexbuffer.hpp>
#include <vertexindexbuffer.hpp>
#include <texture.hpp>

#include <GLFW/glfw3.h>

#pragma warning (push)
#pragma warning (disable:4201 4127)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#pragma warning (pop)

#include "../interface/maths.hpp"

class MyWindow : public GLFWPP_ns::Window
{
  //using GLFWPP_ns::Window::Window;
public:
  explicit MyWindow(
      int width, int height,
      std::string const & title,
      GLFWPP_ns::SetWindowHintsFn hints)
    : Window{width, height, title,hints}
  {
    frameBufferSizeHandler(width, height);
  }

  void registerCallbacks() const noexcept override
  {
    glfwSetKeyCallback(m_hWnd, Window::KeyCallback);
    glfwSetFramebufferSizeCallback(m_hWnd, Window::FrameBufferSizeCallback);
  }

  void keyHandler(int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) noexcept override
  {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(m_hWnd, true);

    std::cout << "in key handler" << std::endl;
  }

  void frameBufferSizeHandler(int width, int height) noexcept override
  {
    ::glViewport(0, 0, width, height);

    auto const & aspect = (float)width / (float)height;
    m_projMatrix = glm::perspective(GLFWPP_ns::Maths_ns::ToDegrees(60.0f), aspect, 0.1f, 100.0f);
  }

  ////////////////////////// RENDER ////////////////////////////////////////////

  void render([[maybe_unused]] double currentTime) override
  {
    static const GLfloat black[]{ 0.0f, 0.0f, 0.0f, 0.0f };

    ::glClearBufferfv(GL_COLOR, 0, black);

    m_program.use();

    m_program.setUniform("offset", (float)currentTime * 0.0003f);

    static std::array<decltype(m_textureCeiling)*, 4> const textures{&m_textureWall, &m_textureFloor, &m_textureWall, &m_textureCeiling};

    for(std::size_t i{}; i < textures.size(); ++i)
    {
      auto const & rotate1   = glm::rotate(glm::mat4(1.0f), GLFWPP_ns::Maths_ns::PIdiv2<float> * i, glm::vec3(0.0f, 0.0f, 1.0f));
      auto const & translate = glm::translate(rotate1, glm::vec3(-0.5f, 0.0f, -10.0f));
      auto const & rotate2   = glm::rotate(translate, GLFWPP_ns::Maths_ns::PIdiv2<float>, glm::vec3(0.0f, 1.0f, 0.0f));
      auto const & mv_matrix = glm::scale(rotate2, glm::vec3(30.0f, 1.0f, 1.0f));
      auto const & mvp = m_projMatrix * mv_matrix;

      m_program.setUniform("mvp", mvp);
      textures[i]->bind();

      ::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    swapBuffers();
  }

  /////////////////////////// INIT /////////////////////////////////////////////

  bool init() override
  {
    auto const att = getContext();
    std::cout << std::boolalpha
              << "Context attributes:\napi = "
              << (att.api == GLFWPP_ns::GL_API::OPENGL ? "OpenGL\n" : "ES\n")
              << "Version: major, minor, revision = "
              << att.version.major << ", " << att.version.minor << ", " << att.version.revision << '\n'
              << "Profile is " << (att.profile == GLFWPP_ns::GL_PROFILE::CORE ? "Core" : "Compat") << '\n'
              << "Forward compatibility is " << att.forward << '\n'
              << "Debug is " << att.debug << '\n';

    char const * const vs_source
    {
        "#version 420 core                                                      \n"
        "                                                                       \n"
        "out VS_OUT                                                             \n"
        "{                                                                      \n"
        "    vec2 tc;                                                           \n"
        "} vs_out;                                                              \n"
        "                                                                       \n"
        "uniform mat4 mvp;                                                      \n"
        "uniform float offset;                                                  \n"
        "                                                                       \n"
        "void main(void)                                                        \n"
        "{                                                                      \n"
        "    const vec2[4] position = vec2[4](vec2(-0.5, -0.5),                 \n"
        "                                     vec2( 0.5, -0.5),                 \n"
        "                                     vec2(-0.5,  0.5),                 \n"
        "                                     vec2( 0.5,  0.5));                \n"
        "    vs_out.tc = (position[gl_VertexID].xy + vec2(offset, 0.5)) * vec2(30.0, 1.0);                  \n"
        "    gl_Position = mvp * vec4(position[gl_VertexID], 0.0, 1.0);       \n"
        "}                                                                      \n"
    };

    char const * const fs_source
    {
        "#version 420 core                                                      \n"
        "                                                                       \n"
        "layout (location = 0) out vec4 color;                                  \n"
        "                                                                       \n"
        "in VS_OUT                                                              \n"
        "{                                                                      \n"
        "    vec2 tc;                                                           \n"
        "} fs_in;                                                               \n"
        "                                                                       \n"
        "layout (binding = 0) uniform sampler2D tex;                            \n"
        "                                                                       \n"
        "void main(void)                                                        \n"
        "{                                                                      \n"
        "    color = texture(tex, fs_in.tc);                                    \n"
        "}                                                                      \n"
    };

    m_program.compileShader(GLFWPP_ns::GLSLShaderType::VERTEX, vs_source);
    m_program.compileShader(GLFWPP_ns::GLSLShaderType::FRAGMENT, fs_source);
    m_program.link();

    m_program.printActiveAttribs(std::cout);
    m_program.printActiveUniforms(std::cout);
    m_program.printActiveUniformBlocks(std::cout);

    m_textureWall    = std::move(GLFWPP_ns::LoadTexture<GLFWPP_ns::OGL_TEXTURE_TARGETS::TWO_D>("brick.ktx", m_textureWall.label()));
    m_textureCeiling = std::move(GLFWPP_ns::LoadTexture<GLFWPP_ns::OGL_TEXTURE_TARGETS::TWO_D>("ceiling.ktx", m_textureCeiling.label()));
    m_textureFloor   = std::move(GLFWPP_ns::LoadTexture<GLFWPP_ns::OGL_TEXTURE_TARGETS::TWO_D>("floor.ktx", m_textureFloor.label()));

    m_textureWall.setParameter(GLFWPP_ns::TEXMINFIL_LINMPLIN);
    m_textureWall.setParameter(GLFWPP_ns::TEXMAGFIL_LINEAR);
    m_textureCeiling.setParameter(GLFWPP_ns::TEXMINFIL_LINMPLIN);
    m_textureCeiling.setParameter(GLFWPP_ns::TEXMAGFIL_LINEAR);
    m_textureFloor.setParameter(GLFWPP_ns::TEXMINFIL_LINMPLIN);
    m_textureFloor.setParameter(GLFWPP_ns::TEXMAGFIL_LINEAR);

    return true;
  }

//  void shutdown() override
//  {
//  }

private:
  GLFWPP_ns::GLSLProgram       m_program{"m_program"};
  GLFWPP_ns::VAO               m_vao{"m_vao"};
  GLFWPP_ns::Texture<GLFWPP_ns::OGL_TEXTURE_TARGETS::TWO_D> m_textureWall{"Wall texture"};
  GLFWPP_ns::Texture<GLFWPP_ns::OGL_TEXTURE_TARGETS::TWO_D> m_textureCeiling{ "Ceiling texture" };
  GLFWPP_ns::Texture<GLFWPP_ns::OGL_TEXTURE_TARGETS::TWO_D> m_textureFloor{ "Floor texture" };

  glm::mat4 m_projMatrix;
};

//////////////////////////////// MyApp /////////////////////////////////////////
class MyApp : public GLFWPP_ns::Application
{
public:
  MyApp(GLFWPP_ns::Window * wnd);
  ~MyApp();

  MyApp(MyApp const &)             = delete;
  MyApp & operator=(MyApp const &) = delete;
  MyApp(MyApp&&)                   = delete;
  MyApp & operator=(MyApp&&)       = delete;


  bool init() override;
  void render(double currentTime) override;
  void run() override;
  void shutdown() override;

private:
  std::unique_ptr<GLFWPP_ns::Window> m_wnd;

};

