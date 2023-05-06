#pragma once

#include <hexgon/core/base.hpp>
#include <hexgon/render/pipeline_layout.hpp>
#include <hexgon/render/renderpass.hpp>
#include <hexgon/render/shader.hpp>
#include <vector>

namespace Hexgon {

enum class PrimitiveTopology {
  kPointList,
  kLineList,
  kLineStrip,
  kTriangleList,
  kTriangleStrip,
};

enum class CompareOp {
  kNever,
  kLess,
  kEqual,
  kLessEqual,
  kGreater,
  kNotEqual,
  kGreaterEqual,
  kAlways,
};

enum class StencilOp {
  kKeep,
  kZero,
  kReplace,
  kIncClamp,
  kDecClamp,
  kInvert,
  kIncWrap,
  kDecWrap,
};

enum class BlendOp {
  kZero,
  kOne,
  kSrcColor,
  kInvSrcColor,
  kSrcAlpha,
  kInvSrcAlpha,
  kDstColor,
  kInvDstColor,
  kDstAlpha,
  kInvDstAlpha,
};

enum class BlendArithmetic {
  kAdd,
  kSubtract,
  kRevSubtract,
  kMin,
  kMax,
};

enum class PolygonMode {
  kFill,
  kWireframe,
  kPoints,
};

enum class CullMode {
  kDisabled,
  kFront,
  kBack,
};

enum class LogicOp {
  kDisabled,
};

struct Viewport {
  float x = 0.f;
  float y = 0.f;
  float width = 0.f;
  float height = 0.f;
  float min_depth = 0.f;
  float max_depth = 0.f;
};

struct Scissor {
  int32_t x = 0;
  int32_t y = 0;
  int32_t width = 0;
  int32_t height = 0;
};

struct DepthDescriptor {
  bool test_enabled = false;

  bool write_enabled = false;

  CompareOp op = CompareOp::kLess;
};

struct StencilFaceDescriptor {
  StencilOp stencil_fail = StencilOp::kKeep;

  StencilOp depth_fail = StencilOp::kKeep;

  StencilOp pass = StencilOp::kKeep;

  CompareOp compare = CompareOp::kAlways;

  uint32_t read_mask = 0xFF;

  uint32_t write_mask = 0xFF;

  uint32_t reference = 0;
};

struct StencilDescriptor {
  bool enabled = false;

  bool reference_dynamic = false;

  StencilFaceDescriptor front = {};

  StencilFaceDescriptor back = {};
};

struct DepthBiasDescriptor {
  float constant = 0.f;

  float slope = 0.f;

  float clamp = 0.f;
};

struct RasterizerDescriptor {
  PolygonMode polygon_mode = PolygonMode::kFill;

  CullMode cull_mode = CullMode::kDisabled;

  DepthBiasDescriptor depth_bias = {};

  bool font_ccw = false;

  bool discard = false;

  bool depth_clamp_enabled = false;

  bool scissor_test_enabled = false;

  bool multisampled_enabled = false;

  bool conservative_rasterization = false;

  float line_width = 1.f;
};

struct BlendTargetDescriptor {
  bool enabled = false;

  BlendOp src_color = BlendOp::kOne;

  BlendOp dst_color = BlendOp::kInvSrcAlpha;

  BlendArithmetic color_arithmetic = BlendArithmetic::kAdd;

  BlendOp src_alpha = BlendOp::kOne;

  BlendOp dst_alpha = BlendOp::kInvSrcAlpha;

  BlendArithmetic alpha_arithmetic = BlendArithmetic::kAdd;

  uint8_t color_mask = 0xFF;
};

struct BlendDescriptor {
  std::vector<BlendTargetDescriptor> attachments = {};
};

class RenderPass;

struct GraphicPipelineDescriptor {
  PipelineLayoutDescriptor layout = {};

  RenderPass* render_pass = nullptr;

  Ref<Shader> vertex_shader = nullptr;

  Ref<Shader> fragment_shader = nullptr;

  PrimitiveTopology pirmitive = PrimitiveTopology::kTriangleList;

  std::vector<Viewport> viewports = {};

  std::vector<Scissor> scissors = {};

  DepthDescriptor depth = {};

  StencilDescriptor stencil = {};

  RasterizerDescriptor rasterizer = {};

  BlendDescriptor blend = {};
};

class PipelineState {
 public:
  PipelineState(const GraphicPipelineDescriptor& desc) : m_desc(desc) {}

  virtual ~PipelineState() = default;

  const GraphicPipelineDescriptor& GetDescriptor() const { return m_desc; }

 private:
  GraphicPipelineDescriptor m_desc = {};
};

}  // namespace Hexgon
