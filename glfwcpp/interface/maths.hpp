// maths.hpp

//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once

#include <type_traits>

namespace GLFWPP_ns
{
namespace Maths_ns
{
template<typename T> constexpr T PI{T(3.14159265358979323846L)};
template<typename T> constexpr T PIdiv2{PI<T> / 2};
template<typename T> constexpr T PIdiv4{PI<T> / 4};
template<typename T> constexpr T PIx2{PI<T> * 2};

template<typename T>[[nodiscard]] constexpr T PI_times(T N)
{
  static_assert (std::is_arithmetic_v<T>, "");
  return PI<T> * N;
}
}
}
