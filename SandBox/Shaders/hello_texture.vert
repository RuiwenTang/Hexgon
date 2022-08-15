#version 450

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;

layout(set = 0, binding = 0) uniform _Transform { mat4 uMatrix; }
TransformData;

layout(location = 0) out vec2 vUV;

void main() {
  vUV = aUV;
  gl_Position = TransformData.uMatrix * vec4(aPos, 1.0);
}
