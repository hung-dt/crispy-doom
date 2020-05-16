#pragma once

#include "../src/w_wad.h"

template <typename T>
auto cacheLumpName(const char *name, const int tag)
{
  return static_cast<T>(W_CacheLumpName(name, tag));
}

template <typename T>
auto cacheLumpNum(lumpindex_t lump, const int tag)
{
  return static_cast<T>(W_CacheLumpNum(lump, tag));
}
