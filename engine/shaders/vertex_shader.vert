#version 130

in vec2 vertex_position;
in vec4 vertex_color;
in vec2 vertex_uv;

out vec4 fragment_color;
out vec2 fragment_uv;

uniform mat4 projection;

void main()
{
    gl_Position.xy = (projection * vec4(vertex_position, 0.0, 1.0)).xy;

    gl_Position.z = 0.0;

    //Normalize coordinates
    gl_Position.w = 1.0;    

    //fragment_color = vertex_color;

    fragment_uv = vec2(vertex_uv.x, 1.0 - vertex_uv.y);
}
