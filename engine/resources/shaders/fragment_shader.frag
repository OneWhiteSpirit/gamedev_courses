#version 130

//in vec4 fragment_color;
in vec2 fragment_uv;

out vec4 color;

uniform float time;
uniform sampler2D sampler;

void main()
{
    vec4 texture_color = texture2D(sampler, fragment_uv);

    color = texture_color;
//    color = fragment_color * vec4((cos(time) + 1.0) * 0.5,
//                                  (cos(time + 2.0) + 1.0) * 0.5,
//                                  (cos(time + 1.0) + 1.0) * 0.5, 0.0) * texture_color;
}
