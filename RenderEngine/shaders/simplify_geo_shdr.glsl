#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in vec3 vertexPosition[];
out vec3 outVertexPosition;

void main() {
    vec3 avgPosition = (vertexPosition[0] + vertexPosition[1] + vertexPosition[2]) / 3.0;
    outVertexPosition = avgPosition;

    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}
