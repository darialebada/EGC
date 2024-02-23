#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
// TODO(student): Declare various other uniforms
uniform float time_elapsed;
uniform int rotate_texture;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.
    vec4 color;

    color = texture(texture_1, texcoord);

    if (color.a < 0.5f) {
        discard;
    }

    out_color = color;
}
