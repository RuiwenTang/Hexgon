#pragma once

#include <MetalKit/MetalKit.h>

#include <hexgon/render/shader.hpp>

namespace Hexgon {

class ShaderMTL : public Shader {
 public:
  ShaderMTL(const ShaderDescriptor& desc, id<MTLDevice> device);

  ~ShaderMTL() override = default;

  bool Compile();

 private:
  bool LoadFunction();

 private:
  id<MTLDevice> m_device;
  id<MTLLibrary> m_library = nil;
  id<MTLFunction> m_function = nil;
};

}  // namespace Hexgon