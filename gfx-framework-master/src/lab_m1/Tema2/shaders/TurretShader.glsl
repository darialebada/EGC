#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 object_color;
uniform float object_damage;

// Output value to fragment shader
out vec3 color;
out vec3 position;

void main()
{
    vec3 obj_damage = vec3(object_damage / 10, object_damage / 10, object_damage / 10);
    // change color after damage (num hits)
    color = object_color - obj_damage;

    // rotate object after damage (num hits)
    float radians = object_damage / 10;
    mat3 rotation_matrix = mat3(
        cos(radians), sin(radians), 0,
        -sin(radians), cos(radians), 0,
        0, 0, 1);

    // deform object after damage (num hits) - rescale y
    position = rotation_matrix * v_position;
    position = vec3(position.x, position.y * sin(1.0 - object_damage / 10), position.z);

    gl_Position = Projection * View * Model * vec4(position, 1.0);
}
