#version 450

layout(push_constant) uniform uPushConstant { vec3 eye; }
pc;

layout(set = 1, binding = 1) uniform _uObj { vec4 color; }
uObj;

layout(location = 0) in vec3 vNom;
layout(location = 1) in vec3 vPos;

layout(location = 0) out vec4 fColor;

void main() {
  vec3 eye_dir = normalize(vPos - pc.eye);

  float diff = max(dot(vNom, eye_dir), 0.0);

  fColor = vec4(1.0, 1.0, 1.0, 1.0);
}