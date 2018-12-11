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
    //assert(false);
  }

  void registerCallbacks() const noexcept override
  {
    glfwSetKeyCallback(m_hWnd, Window::KeyCallback);
    glfwSetFramebufferSizeCallback(m_hWnd, Window::FrameBufferSizeCallback);
  }

  void keyHandler(int key, int [[maybe_unused]] scancode, int action, int [[maybe_unused]] mods) noexcept override
  {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(m_hWnd, true);

    std::cout << "in key handler" << std::endl;
  }

  void frameBufferSizeHandler(int width, int height) noexcept override
  {
    glViewport(0, 0, width, height);
  }

  ////////////////////////// RENDER ////////////////////////////////////////////

  void render(double [[maybe_unused]] currentTime) override
  {
    static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
    static const GLfloat one = 1.0f;

    glClearBufferfv(GL_COLOR, 0, green);
    glClearBufferfv(GL_DEPTH, 0, &one);

    m_program.use();

    glDrawArrays(GL_TRIANGLES, 0, 3);

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
        "#version 420 core                                                              \n"
        "                                                                               \n"
        "void main(void)                                                                \n"
        "{                                                                              \n"
        "    const vec4 vertices[] = vec4[](vec4( 0.75, -0.75, 0.5, 1.0),               \n"
        "                                   vec4(-0.75, -0.75, 0.5, 1.0),               \n"
        "                                   vec4( 0.75,  0.75, 0.5, 1.0));              \n"
        "                                                                               \n"
        "    gl_Position = vertices[gl_VertexID];                                       \n"
        "}                                                                              \n"
    };

    char const * const fs_source
    {
        "#version 430 core                                                              \n"
        "                                                                               \n"
        "uniform sampler2D s;                                                           \n"
        "                                                                               \n"
        "out vec4 color;                                                                \n"
        "                                                                               \n"
        "void main(void)                                                                \n"
        "{                                                                              \n"
        "    color = texture(s, gl_FragCoord.xy / textureSize(s, 0));                   \n"
        "}                                                                              \n"
    };
    m_program.compileShader(GLFWPP_ns::GLSLShaderType::VERTEX, vs_source);
    m_program.compileShader(GLFWPP_ns::GLSLShaderType::FRAGMENT, fs_source);
//    m_program.compileShader("tcs.tcs");
//    m_program.compileShader("tes.tes");
//    m_program.compileShader("geom.gs");
    m_program.link();

//    m_mvLocation   = m_program.getUniformLocation("mv_matrix");
//    m_projLocation = m_program.getUniformLocation("proj_matrix");

    m_program.printActiveAttribs(std::cout);
    m_program.printActiveUniforms(std::cout);
    m_program.printActiveUniformBlocks(std::cout);

    return true;
  }

//  void shutdown() override
//  {
//  }

private:
  GLFWPP_ns::GLSLProgram       m_program{"m_program"};
  GLFWPP_ns::VAO               m_vao{"m_vao"};
  //GLFWPP_ns::Texture<GLFWPP_ns::OGL_TEXTURE_TARGETS::TWO_D> m_texture{"m_texture 2D"};
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

