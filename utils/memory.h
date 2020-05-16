#pragma once

#include "../src/z_zone.h"

#include <cstdlib>
#include <new>

// TODO Fix me (remove this function and user smart ptr instead?)
template <typename DataType>
auto createStruct()
{
  auto* mem = malloc(sizeof(DataType));
  return new (mem) DataType{};
}

template <typename DataType>
auto createStruct(const std::size_t sz)
{
  auto *mem = malloc(sizeof(DataType) * sz);
  return new (mem) DataType[sz];
}

template <typename DataType>
auto zmalloc(int size, int tag, void *ptr)
{
  return static_cast<DataType>(Z_Malloc( size, tag, ptr ));
}

