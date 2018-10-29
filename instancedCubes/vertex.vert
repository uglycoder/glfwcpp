#version 450 core

in vec4 position;

out VS_OUT
{
  vec4 color;
} vs_out;

uniform mat4 mv_matrix[24];
layout(location = 2) uniform mat4 proj_matrix;

void main(void)
{
  gl_Position = proj_matrix * mv_matrix[gl_InstanceID] * position;
  vs_out.color = position * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);
}
