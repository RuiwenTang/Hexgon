#version 450

layout(push_constant) uniform uPushConstant { vec3 eye; }
pc;

layout(set = 1, binding = 1) uniform _uObj { vec4 color; }
uObj;

layout(location = 0) in vec3 vNom;
layout(location = 1) in vec3 vPos;

layout(location = 0) out vec4 fColor;

void main() {
  vec3 eye_dir = normalize(pc.eye - vPos);

  vec3 diff = max(dot(vNom, eye_dir), 0.3) * vec3(1.0, 1.0, 1.0);

  fColor = vec4(uObj.color.rgb * diff, uObj.color.a);
}