  #version 460 core
    in vec2 TexCoord;

    out vec4 FragColor;

    uniform sampler2D textureSampler;
    uniform vec3 tint;

    void main() {
        FragColor = texture(textureSampler, TexCoord)  * vec4(tint, 1.0);
    }