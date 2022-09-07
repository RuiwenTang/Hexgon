#version 450

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNom;

layout(set = 0, binding = 0) uniform _uGlobal { mat4 view_proj; }
uGlobal;

layout(set = 1, binding = 0) uniform _uObj { mat4 obj_mat; }
uObj;

layout(location = 0) out vec3 vNom;
layout(location = 1) out vec3 vPos;

void main() {
  vNom = (uObj.obj_mat * vec4(aNom, 0.0)).xyz;
  vPos = (uObj.obj_mat * vec4(aPos, 1.0)).xyz;

  gl_Position = uGlobal.view_proj * uObj.obj_mat * vec4(aPos, 1.0);
}