#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
// TODO(student): Declare various other uniforms
uniform float time_elapsed;
uniform int rotate_texture;
uniform int second_texture;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.
    vec4 color;
    vec2 texcoord2 = texcoord;
    if (rotate_texture == 1) {
        texcoord2 = vec2(texcoord.x + time_elapsed / 4, texcoord.y);
    }

    if (second_texture == 1) {
        vec4 color1 = texture(texture_1, texcoord2);
        vec4 color2 = texture(texture_2, texcoord);
        color = mix(color1, color2, 0.5f);
    } else {
        color = texture(texture_1, texcoord2);
    }
    if (color.a < 0.5f) {
        discard;
    }

    out_color = color;
}
