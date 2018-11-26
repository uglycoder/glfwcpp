// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#include "pch.h"
#include <iostream>

#include "gllmits.hpp"

int main()
{
  auto const & svMap{svLimitsMap.svLimitsMap};
  for( auto const & [key, val]: svMap)
  {
    std::cout
      << std::left << std::setw(42) << val.keyStr  << val.value << '\n';
  }
}
