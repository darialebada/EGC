#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 frag_color;
in vec3 frag_normal;

// Output
layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_normal;


void main()
{
    // TODO(student): Write pixel out color
    // out_color = vec4(1);
    // out_color = vec4(frag_color, 1);
    out_color = vec4(abs(frag_normal), 1);
}
