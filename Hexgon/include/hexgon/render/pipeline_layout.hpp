#pragma once

#include <hexgon/core/base.hpp>
#include <hexgon/render/types.hpp>
#include <string>
#include <vector>

namespace Hexgon {

struct BindingSlot {
  uint32_t index = 0;

  uint32_t set = 0;
};

struct BindingDescriptor {
  std::string name = "";

  ResourceType type = ResourceType::kUndefined;

  uint32_t bind_flags = 0;

  BindingSlot slot = {};

  uint32_t array_size = 0;
};

struct PipelineLayoutDescriptor {
  std::vector<BindingDescriptor> bindings = {};
};

}  // namespace Hexgon