#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace Object
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    // Create diamond with given left corner, length and color
    Mesh* CreateDiamond(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);
    // Create enemy with given corner, length, color and coordinate (z = 1 => is in front)
    Mesh* CreateEnemy(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, int z);
    // Create star with given corner, length and color
    Mesh* CreateStar(const std::string& name, glm::vec3 leftBottomCorner, glm::vec3 color);
    // Create heart with given corner, length and color
    Mesh* CreateHeart(const std::string& name, glm::vec3 leftBottomCorner, glm::vec3 color);
}
