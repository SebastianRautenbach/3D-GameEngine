   #version 460 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;

    out vec2 TexCoord;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main() {

        vec3 camera_right = vec3(view[0][0], view[1][0], view[2][0]);
        vec3 camera_up = vec3(view[0][1], view[1][1], view[2][1]);

        vec3 billboard_center = vec3(model[3]);

        vec3 vertex_position = billboard_center + 
                       aPos.x * -camera_right + 
                       aPos.y * camera_up;


        gl_Position = projection * view * vec4(vertex_position , 1.0);


        TexCoord = aTexCoord;
    }