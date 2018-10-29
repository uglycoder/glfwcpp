// glmcamera.hpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace GLTools_ns
{

class Camera
{
public:
  // Default camera position:
  // origin (0,0,0), direction -ve z-axis, up - +ve y-axis, right +ve x-axis
  struct Position_t
  {
    glm::vec3 position{};
    glm::vec3 cameraDirection{0.0f, 0.0f, -1.0f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    glm::vec3 right{1.0f, 0.0f, 0.0f};
  };

  struct frustrum_t
  {
    float nearZ{};
    float farZ{};
    float aspect{};
    float fovX{};
    float fovY{};
    float nearWindowHeight{};
    float nearWindowWidth{};
    float farWindowHeight{};
    float farWindowWidth{};
  };

  explicit Camera(
    Position_t const & position        = Position_t{}
   ,Position_t const & defaultPosition = Position_t{}
  );

  Position_t defaultPosition() const noexcept;
  void defaultPosition(Position_t const & pos) noexcept;
  void setCameraToDefaultPosition() noexcept;

  // get/Set world camera position.
  glm::vec3 const & position() const noexcept;
  void position( glm::vec3 const & v) noexcept;

  // get camera basis vectors.
  glm::vec3 const & direction() const noexcept;
  glm::vec3 const & up()        const noexcept;
  glm::vec3 const & right()     const noexcept;

  // get frustum properties.
  frustrum_t const & frustrum() const noexcept;

  void projectionMatrix(
    float fovY
    ,float aspect
    ,float zn
    ,float zf
  ) noexcept;

  void setCameraWorldPositionLookAt(
    glm::vec3 const & pos
    ,glm::vec3 const & targetPos
    ,glm::vec3 const & up
  ) noexcept;

  // move a distance d.
  void moveForward(float d) noexcept;
  void moveUp(float d)      noexcept;
  void moveRight(float d)   noexcept;

  // Rotate the camera.
  void rotateAboutLook(float angle)  noexcept;  // roll
  void rotateAboutUp(float angle)    noexcept;  // yaw
  void rotateAboutRight(float angle) noexcept;  // pitch

  // After modifying camera position/orientation, call to rebuild the view matrix.
  glm::mat4 const & updateViewMatrix() noexcept;

  // get view/proj matrices.
  glm::mat4 const & view() const noexcept;
  glm::mat4 const & proj() const noexcept;
  glm::mat4 viewProj()     const noexcept;

private:
  // Camera coordinate system with coordinates relative to world space.
  Position_t m_position;
  Position_t m_defaultPosition;


  // Cache frustum properties.
  frustrum_t m_frustrum;

  // Cache view/proj matrices.
  glm::mat4 m_view{};
  glm::mat4 m_proj{};
};

}
