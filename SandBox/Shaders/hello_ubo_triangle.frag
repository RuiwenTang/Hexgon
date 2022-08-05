#version 450

layout(set = 0, binding = 0) uniform _Transform {
  mat4 uMatrix;
} TransformData;

layout(set = 1, binding = 0) uniform _Color {
  vec4 vColor;
} ColorData ;

layout(location = 0) in vec4 vColor;

layout(location = 0) out vec4 outColor;

void main() { outColor = vColor; }