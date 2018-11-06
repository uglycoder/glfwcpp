#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include "../include/common_stl_hdrs.hpp"

#include "../glad/glad.h"
#include <GLFW/glfw3.h>

#pragma warning(push)
#pragma warning(disable: 4201 4127)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#pragma warning(pop)

