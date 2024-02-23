#version 330

// Input
// TODO(student): Get color value from vertex shader

// Output
layout(location = 0) out vec4 out_color;

in vec3 color;

void main()
{
    // TODO(student): Write pixel out color
    out_color = vec4(color, 1);

}
