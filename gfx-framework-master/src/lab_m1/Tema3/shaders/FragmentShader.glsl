#version 330

// Input
in vec3 world_position;
in vec3 world_normal;
in vec2 texcoord;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 light_color;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform sampler2D texture_1;
uniform float time_elapsed;
uniform int rotate_texture;

// TODO(student): Declare any other uniforms
uniform int light_type;

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;

vec3 point_light_contribution(vec3 light_pos, vec3 light_color) {
	
    vec3 N = normalize(world_normal);
    // vectorul directiei luminii L
	vec3 L = normalize(light_pos - world_position);
    // vectorul directiei din care priveste observatorul V
	vec3 V = normalize(eye_position - world_position);
    // vectorul median H
	vec3 H = normalize(L + V);
    // componenta speculara -> lumina reflectata de suprafata obiectului numai in jurul acestei directii
	vec3 R = normalize(reflect(L, world_normal));

    float ambient_light = 0.25f;
    float diffuse_light = material_kd * max(dot(normalize(N), L), 0.f);
    float specular_light = 0.0f;

    if (diffuse_light > 0) {
        specular_light = material_ks * pow(max(dot(N, H), 0), material_shininess);
    }

    float light = 0.0f;

    if (light_type == 0) {
        float dist = distance(light_pos, world_position);
        float att_factor = 0.7f;
	    light = ambient_light + att_factor * (diffuse_light + specular_light);
    } else {
        // spot-light

        light = ambient_light;

        float cut_off = 0.5235;
        float spot_light = dot(-L, light_direction);
        float spot_light_limit = cos(cut_off);
        if (spot_light > cos(cut_off)) {
            // after phong model

            float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            float light_att_factor = linear_att * linear_att;
            light += light_att_factor * (diffuse_light + specular_light);
        }
    }

	return light_color * light * object_color;
}

void main()
{

    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that

    // TODO(student): Write pixel out color
    vec2 texcoord2 = texcoord;

    if (rotate_texture == 1)
    {
        // wave effect - used trigonometric function to modulate the texture coordinates
        // wave effect is a animation => we use time coordinate (time_elapsed)
        vec2 modulated_coords = (vec2(sin(world_position.x * 10 + time_elapsed * 6))) * 0.0035f;

        texcoord2 = vec2(texcoord.x + modulated_coords.x + time_elapsed / 100, texcoord.y);
      
        // no wave effect
        //texcoord2 = vec2(texcoord.x + time_elapsed / 100, texcoord.y);
    }

    vec4 text = texture(texture_1, texcoord2);
  
    vec4 color = vec4(point_light_contribution(light_position, vec3(0, 0, 0) + light_color), 1);
    out_color = color * text;
}