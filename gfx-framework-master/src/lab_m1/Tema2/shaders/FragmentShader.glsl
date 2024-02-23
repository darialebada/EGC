#version 330

// Input

// Output
layout(location = 0) out vec4 out_color;

in vec3 color;

void main()
{
    // Write pixel out color
    out_color = vec4(color, 1);

}
