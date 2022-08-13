#version 450

layout(set = 0, binding = 0) uniform _Transform { mat4 uMatrix; }
TransformData;

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;

layout(location = 0) out vec4 vColor;

void main() {
  vColor = aColor;

  gl_Position = TransformData.uMatrix * vec4(aPos, 1.0);
}
