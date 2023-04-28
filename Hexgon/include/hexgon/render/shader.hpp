#pragma once

#include <hexgon/core/base.hpp>
#include <string>
#include <vector>

namespace Hexgon {

enum class ShaderLanguage {
  kGLSL,
  kMetal,
  kHLSL,
};

enum class ShaderType {
  kUndefined,
  kVertex,
  kTessControl,
  kTessEvaluation,
  kGeometry,
  kFragment,
  kCompute,
};

enum class ShaderSourceType {
  kCodeString,
  kCodeFile,
  kBinaryBuffer,
  kBinaryFile,
};

struct ShaderDescriptor {
  ShaderType type = ShaderType::kUndefined;
  ShaderSourceType sourceType = ShaderSourceType::kCodeString;
  std::string source = "";
  std::string entry_point = "";
};

class Shader {
 public:
  Shader(const ShaderDescriptor& desc) : m_desc(desc) {}
  virtual ~Shader() = default;

  const ShaderDescriptor& GetDescriptor() const { return m_desc; }

 protected:
  static bool CompileToSPV(const ShaderDescriptor& desc,
                           std::vector<uint32_t>& spv);

 private:
  ShaderDescriptor m_desc;
};

}  // namespace Hexgon