// myapp.hpp
// Creation date: 2015-03-26 11:25:09 +0000
// Copyright 2015 Simon Parmenter. All rights reserved.

#pragma once

#include "../include/common_stl_hdrs.hpp"

#include <globalutils.hpp>

#include <glfwppapplication.hpp>
#include <glfwppwindow.hpp>
#include <glfwppglslprogram.hpp>
#include <glfwppvertexarrayobject.hpp>
#include <glfwppvertexbuffer.hpp>
#include <glfwppvertexindexbuffer.hpp>

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
   : Window(width, height, title,hints)
     ,m_program("My Shader")
     ,m_vao("VAO")
     ,m_buffer("VBO")
     ,m_indexBuffer("Index Vertex Buffer")
    , m_aspect{ (float)width / (float)height }
     ,m_projMatrix(glm::perspective(50.0f, m_aspect, 0.1f, 1000.0f))
  {
    //assert(false);
  }

  void registerCallbacks() const noexcept override
  {
    glfwSetKeyCallback(m_hWnd, Window::KeyCallback);
    glfwSetFramebufferSizeCallback(m_hWnd, Window::FrameBufferSizeCallback);
  }

  void keyHandler(int key, int /*scancode*/, int action, int /*mods*/) noexcept override
  {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(m_hWnd, true);

    std::cout << "in key handler" << std::endl;
  }

  void frameBufferSizeHandler(int width, int height) noexcept override
  {
    glViewport(0, 0, width, height);

    m_aspect = (float)width / (float)height;
    m_projMatrix = glm::perspective(50.0f, m_aspect, 0.1f, 1000.0f);
  }

  ////////////////////////// RENDER ////////////////////////////////////////////

  void render(double currentTime) override
  {
    static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
    static const GLfloat one = 1.0f;

    glClearBufferfv(GL_COLOR, 0, green);
    glClearBufferfv(GL_DEPTH, 0, &one);

    m_program.use();

    m_program.setUniform("proj_matrix", m_projMatrix);

    std::vector<glm::mat4> cubeMVs;

    for(int i = 0; i < 24; ++i)
    {
      float f = (float)i + (float)currentTime * 0.3f;
      glm::mat4 translate1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -26.0f));
      glm::mat4 translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(sinf(2.1f * f) * 2.0f, cosf(1.7f * f) * 2.0f, sinf(1.3f * f) * cosf(1.5f * f) * 2.0f));
      glm::mat4 rotate1    = glm::rotate(glm::mat4(1.0f), (float)currentTime * 0.45f  *i, glm::vec3(0.0f, 1.0f, 0.0f));
      glm::mat4 rotate2    = glm::rotate(glm::mat4(1.0f), (float)currentTime * 0.21f *i, glm::vec3(1.0f, 0.0f, 0.0f));

      cubeMVs.emplace_back(translate1 * rotate1 * rotate2 * translate2);
    }
    m_program.setUniform("mv_matrix", cubeMVs);

    glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0, 24);

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


    m_program.compileShader("vertex.vert");
    m_program.compileShader("frag.frag");
//    m_program.compileShader("tcs.tcs");
//    m_program.compileShader("tes.tes");
//    m_program.compileShader("geom.gs");
    m_program.link();

//    m_mvLocation   = m_program.getUniformLocation("mv_matrix");
//    m_projLocation = m_program.getUniformLocation("proj_matrix");

    m_program.printActiveAttribs(std::cout);
    m_program.printActiveUniforms(std::cout);
    m_program.printActiveUniformBlocks(std::cout);

    static const GLfloat vertex_positions[] =
    {
      -0.25f, -0.25f, -0.25f,
      -0.25f,  0.25f, -0.25f,
       0.25f, -0.25f, -0.25f,
       0.25f,  0.25f, -0.25f,
       0.25f, -0.25f,  0.25f,
       0.25f,  0.25f,  0.25f,
      -0.25f, -0.25f,  0.25f,
      -0.25f,  0.25f,  0.25f,
    };

    static const GLushort vertex_indices[] =
    {
      0, 1, 2,
      2, 1, 3,
      2, 3, 4,
      4, 3, 5,
      4, 5, 6,
      6, 5, 7,
      6, 7, 0,
      0, 7, 1,
      6, 0, 2,
      2, 4, 6,
      7, 5, 3,
      7, 3, 1
    };

    std::cout << "sizeof(vertex_positions) = " << sizeof(vertex_positions) << '\n';
    std::cout << "Number of floats in vertex_positions = " << GlobalUtils_ns::ArraySize(vertex_positions) << std::endl;

    m_buffer.allocateImmutableStorage(sizeof(vertex_positions), vertex_positions);

    m_buffer.unBind();

    m_indexBuffer.allocateImmutableStorage(sizeof(vertex_indices), vertex_indices);

    m_vao.bind(m_buffer, 0, 0, 3 * sizeof(float));
    m_vao.setAttributeFormat(0, 3, GL_FLOAT, false, 0);
    m_vao.setAttributeBinding(0, 0);
    m_vao.enableAttrib(0);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    return true;
  }

//  void shutdown() override
//  {
//  }

private:
  GLFWPP_ns::GLSLProgram       m_program;
  GLFWPP_ns::VAO               m_vao;
  GLFWPP_ns::VertexBuffer      m_buffer;
  GLFWPP_ns::VertexIndexBuffer m_indexBuffer;

  GLint m_mvLocation   = 0;
  GLint m_projLocation = 0;

  float           m_aspect;

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

