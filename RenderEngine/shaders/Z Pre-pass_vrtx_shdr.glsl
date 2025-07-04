#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 light_matrix;



void main() {
    gl_Position = light_matrix * model * vec4(aPos, 1.0);
}