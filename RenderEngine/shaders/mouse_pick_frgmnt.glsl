#version 460 core
out vec4 FragColor;

uniform int pick_color;

void main() {
    FragColor = vec4(
        float((pick_color >> 16) & 0xFF) / 255.0,
        float((pick_color >> 8) & 0xFF) / 255.0,  
        float(pick_color & 0xFF) / 255.0,         
        1.0                                   
    );
}