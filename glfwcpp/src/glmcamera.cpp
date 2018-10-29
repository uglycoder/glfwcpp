// glmcamera.cpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#include "precompiled-header.hpp"

#include "../interface/glmcamera.hpp"

#include <cmath>

#include "../interface/maths.hpp"



GLTools_ns::Camera::Camera(
  Position_t const & position         /*= Position_t{}*/
  ,Position_t const & defaultPosition /*= Position_t{}*/
) :
  m_position{position}
  ,m_defaultPosition{defaultPosition}
{
}

GLTools_ns::Camera::Position_t GLTools_ns::Camera::defaultPosition() const noexcept
{
  return m_defaultPosition;
}

void GLTools_ns::Camera::defaultPosition(Position_t const & pos) noexcept
{
  m_defaultPosition = pos;
}


void GLTools_ns::Camera::setCameraToDefaultPosition() noexcept
{
  m_position = m_defaultPosition;
}

glm::vec3 const & GLTools_ns::Camera::position() const noexcept
{
  return m_position.position;
}

void GLTools_ns::Camera::position( glm::vec3 const & v) noexcept
{
  m_position.position = v;
}

glm::vec3 const & GLTools_ns::Camera::direction() const noexcept
{
  return m_position.cameraDirection;
}

glm::vec3 const & GLTools_ns::Camera::up()const noexcept
{
  return m_position.up;
}

glm::vec3 const & GLTools_ns::Camera::right() const noexcept
{
  return m_position.right;
}

GLTools_ns::Camera::frustrum_t const & GLTools_ns::Camera::frustrum() const noexcept
{
  return m_frustrum;
}

void GLTools_ns::Camera::projectionMatrix(
  float fovY
  ,float aspect
  ,float zn
  ,float zf
) noexcept
{
  // cache properties
  m_frustrum.fovY   = fovY;
  m_frustrum.aspect = aspect;
  m_frustrum.nearZ  = zn;
  m_frustrum.farZ   = zf;

  float const halffovY(0.5f * m_frustrum.fovY);
  m_frustrum.nearWindowHeight = 2.0f * m_frustrum.nearZ * ::tan( halffovY );
  m_frustrum.farWindowHeight  = 2.0f * m_frustrum.farZ * ::tan( halffovY );

  m_frustrum.nearWindowWidth = m_frustrum.aspect * m_frustrum.nearWindowHeight;
  m_frustrum.farWindowWidth  = m_frustrum.aspect * m_frustrum.farWindowHeight;

  m_frustrum.fovX = 2.0f * ::atan(m_frustrum.nearWindowWidth / 2.0f / m_frustrum.nearZ);

  m_proj = glm::perspective(m_frustrum.fovY, m_frustrum.aspect, m_frustrum.nearZ, m_frustrum.farZ);
}

void GLTools_ns::Camera::setCameraWorldPositionLookAt(
  glm::vec3 const & pos
  ,glm::vec3 const & targetPos
  ,glm::vec3 const & up
) noexcept
{
  m_position.position        = pos;
  m_position.cameraDirection = glm::normalize(targetPos - pos);
  m_position.right           = glm::normalize(glm::cross(up, m_position.cameraDirection));
  m_position.up              = glm::cross(m_position.cameraDirection, m_position.right);
}


// move a distance d.
void GLTools_ns::Camera::moveRight(float d) noexcept
{
  m_position.position += d * m_position.right;
}

void GLTools_ns::Camera::moveUp(float d) noexcept
{
  m_position.position += d * m_position.up;
}

void GLTools_ns::Camera::moveForward(float d) noexcept
{
  m_position.position += d * m_position.cameraDirection;
}

// Rotate the camera.
void GLTools_ns::Camera::rotateAboutLook(float angle) noexcept  // roll
{
  // get quaternion for rotation about camera's look axis
  m_position.cameraDirection = glm::normalize(m_position.cameraDirection);
  auto const & rotLook       = glm::angleAxis(angle, m_position.cameraDirection);

  // rotate other axes
  m_position.up    = rotLook * m_position.up;
  m_position.right = rotLook * m_position.right;
}

void GLTools_ns::Camera::rotateAboutUp(float angle) noexcept    // yaw
{
  // get quaternion for rotation about camera's right axis
  m_position.up      = glm::normalize(m_position.up);
  auto const & rotUp = glm::angleAxis(angle, m_position.up);

  // rotate other axes
  m_position.right           = rotUp * m_position.right;
  m_position.cameraDirection = rotUp * m_position.cameraDirection;
}

void GLTools_ns::Camera::rotateAboutRight(float angle) noexcept // pitch
{
  // get quaternion for rotation about camera's right axis
  m_position.right  = glm::normalize(m_position.right);
  auto const & rotR = glm::angleAxis(angle, m_position.right);

  // rotate other axes
  m_position.up              = rotR * m_position.up;
  m_position.cameraDirection = rotR * m_position.cameraDirection;
}

glm::mat4 const & GLTools_ns::Camera::updateViewMatrix() noexcept
{
  // Keep camera's axes orthogonal to each other and of unit length.
  m_position.cameraDirection = glm::normalize(m_position.cameraDirection);
  m_position.right           = glm::normalize(glm::cross(m_position.cameraDirection, m_position.up));

  // right and look already ortho-normal, so no need to normalize cross product.
  m_position.up = glm::cross(m_position.right, m_position.cameraDirection);

  // Build view matrix
  auto const & x = -glm::dot(m_position.right,           m_position.position);
  auto const & y = -glm::dot(m_position.up,              m_position.position);
  auto const & z =  glm::dot(m_position.cameraDirection, m_position.position);

  m_view[0][0] = m_position.right.x;
  m_view[1][0] = m_position.right.y;
  m_view[2][0] = m_position.right.z;
  m_view[3][0] = x;

  m_view[0][1] = m_position.up.x;
  m_view[1][1] = m_position.up.y;
  m_view[2][1] = m_position.up.z;
  m_view[3][1] = y;

  m_view[0][2] = -m_position.cameraDirection.x;
  m_view[1][2] = -m_position.cameraDirection.y;
  m_view[2][2] = -m_position.cameraDirection.z;
  m_view[3][2] = z;

  m_view[0][3] = 0.0f;
  m_view[1][3] = 0.0f;
  m_view[2][3] = 0.0f;
  m_view[3][3] = 1.0f;

  return m_view;
}

glm::mat4 const & GLTools_ns::Camera::view() const noexcept
{
  return m_view;
}

glm::mat4 const & GLTools_ns::Camera::proj() const noexcept
{
  return m_proj;
}

glm::mat4 GLTools_ns::Camera::viewProj() const noexcept
{
  return m_view * m_proj;
}


