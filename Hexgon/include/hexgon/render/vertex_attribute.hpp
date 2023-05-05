#pragma once

#include <hexgon/render/format.hpp>
#include <string>
#include <vector>

namespace Hexgon {

struct VertexAttribute {
  std::string name;

  Format format = Format::RGBA32Float;

  uint32_t location = 0;

  uint32_t slot = 0;

  uint32_t offset = 0;

  uint32_t stride = 0;

  uint32_t instanceDivisor = 0;
};

}  // namespace Hexgon