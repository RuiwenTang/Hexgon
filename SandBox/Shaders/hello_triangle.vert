#version 450

layout(set = 0, location = 0) in vec3 aPos;
layout(set = 0, location = 1) in vec4 aColor;

layout(location = 0) out vec4 vColor;

void main() {
  vColor = aColor;

  gl_Position = vec4(aPos, 1.0);
}
