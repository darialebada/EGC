#pragma once

#include "lab_m1/Tema2/camera_tema.h"

namespace m1
{
    class Tank
    {
    public:
        Tank();
        ~Tank();

        void Init();

        struct projectile_t {
            float x, y, z, radians, radians_turret;
            bool hit;
            time_t start_time;
            glm::vec3 color;
        };

        // variables
        float x, y, z;
	    float radians_rotation;
	    float radians_turret;
        float num_hits;
        time_t start_firing;
	    glm::vec3 color_body;
	    glm::vec3 color_turret;
	    glm::vec3 color_cannon;
	    glm::vec3 color_tracks;
        std::vector<projectile_t> projectiles;
    };
}   // namespace m1

