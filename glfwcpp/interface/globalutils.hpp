// globalutils.hpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once

#include <cstddef>
#include <type_traits>

namespace GlobalUtils_ns
{

template<typename T, std::size_t N>
[[nodiscard]] constexpr std::size_t ArraySize(T (&)[N]) noexcept
{
  return N;
}

template<typename T>
[[nodiscard]] constexpr auto ToUType(T type) noexcept
{
  return static_cast<std::underlying_type_t<T>>(type);
}

// Use this class to make compiler show true type in diagnostic that follows in
// in use - see Item 4, Effective Modern C++, Scott Meyers, ISBN 978-1-491-90399-5
// TypeDisplayer<decltype<variable> variableNameType;
template<typename T>
class TypeDisplayer;

} // namespace GlobalUtils_ns


