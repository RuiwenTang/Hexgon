
#include "src/render/metal/shader_mtl.h"

#include <hexgon/core/log.hpp>

namespace Hexgon {

ShaderMTL::ShaderMTL(const ShaderDescriptor& desc, id<MTLDevice> device)
    : Shader(desc), m_device(device) {}

bool ShaderMTL::Compile() {
  const auto& desc = GetDescriptor();

  NSString* source = nil;

  if (desc.sourceType == ShaderSourceType::kCodeFile) {
    NSString* file_path =
        [[NSString alloc] initWithCString:desc.source.c_str()
                                 encoding:NSUTF8StringEncoding];

    source = [[NSString alloc] initWithContentsOfFile:file_path
                                             encoding:NSUTF8StringEncoding
                                                error:nil];
  } else {
    source = [[NSString alloc] initWithCString:desc.source.c_str()
                                      encoding:NSUTF8StringEncoding];
  }

  if (source == nil) {
    return false;
  }

  NSError* err = [NSError new];

  MTLCompileOptions* opt = [[MTLCompileOptions alloc] init];

  m_library = [m_device newLibraryWithSource:source options:opt error:&err];

  if (!LoadFunction()) {
    auto err_msg =
        [[err localizedDescription] cStringUsingEncoding:NSUTF8StringEncoding];

    HEX_CORE_ERROR("Metal shader compile error: {}", err_msg);

    return false;
  }

  return true;
}

bool ShaderMTL::LoadFunction() {
  if (m_library == nil) {
    return false;
  }

  NSString* entry =
      [[NSString alloc] initWithCString:GetDescriptor().entry_point.c_str()
                               encoding:NSUTF8StringEncoding];

  m_function = [m_library newFunctionWithName:entry];

  return m_function != nil;
}

}
