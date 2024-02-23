#include "Object.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh* Object::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* Object::CreateDiamond(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(length / 2, 0, 1), color),
        VertexFormat(corner + glm::vec3(3 * length / 4, length / 2, 1), color),
        VertexFormat(corner + glm::vec3(length / 2, length, 1), color),
        VertexFormat(corner + glm::vec3(length / 4, length / 2, 1), color),
        VertexFormat(corner + glm::vec3(length / 2, length / 2 + 15, 1), color),
        VertexFormat(corner + glm::vec3(length / 2, length / 2 - 15, 1), color),
        VertexFormat(corner + glm::vec3(length, length / 2 - 15, 1), color),
        VertexFormat(corner + glm::vec3(length, length / 2 + 15, 1), color)
    };

    Mesh* diamond = new Mesh(name);
    std::vector<unsigned int> indices = { 
        0, 1, 2, 
        0, 2, 3,
        4, 5, 6,
        4, 6, 7
    };

    diamond->InitFromData(vertices, indices);
    return diamond;
}

Mesh* Object::CreateEnemy(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    int z)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(length / 4, 0, z), color),
        VertexFormat(corner + glm::vec3(3 * length / 4, 0, z), color),
        VertexFormat(corner + glm::vec3(length, length / 2, z), color),
        VertexFormat(corner + glm::vec3(3 * length / 4, length, z), color),
        VertexFormat(corner + glm::vec3(length / 4, length, z), color),
        VertexFormat(corner + glm::vec3(0, length / 2, z), color)
    };

    Mesh* enemy = new Mesh(name);
    std::vector<unsigned int> indices = { 
        0, 1, 2,
        0, 2, 5,
        2, 5, 3,
        5, 3, 4
    };

    enemy->InitFromData(vertices, indices);
    return enemy;
}

Mesh* Object::CreateStar(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    glm::vec3 color)
{
    glm::vec3 corner = leftBottomCorner;


    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(14, 6, 3), color),
        VertexFormat(corner + glm::vec3(18, 8, 3), color),
        VertexFormat(corner + glm::vec3(22, 6, 3), color),
        VertexFormat(corner + glm::vec3(24, 12, 3), color),
        VertexFormat(corner + glm::vec3(18, 16, 3), color),
        VertexFormat(corner + glm::vec3(12, 12, 3), color),
    };

    Mesh* star = new Mesh(name);
    std::vector<unsigned int> indices = { 
        0, 1, 4,
        1, 2, 4,
        1, 3, 5
    };

    star->InitFromData(vertices, indices);
    return star;
}

Mesh* Object::CreateHeart(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    glm::vec3 color)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(9, 4, 0), color),
        VertexFormat(corner + glm::vec3(11, 6, 0), color),
        VertexFormat(corner + glm::vec3(11, 7, 0), color),
        VertexFormat(corner + glm::vec3(10, 8, 0), color),
        VertexFormat(corner + glm::vec3(9, 7, 0), color),
        VertexFormat(corner + glm::vec3(8, 8, 0), color),
        VertexFormat(corner + glm::vec3(7, 7, 0), color),
        VertexFormat(corner + glm::vec3(7, 6, 0), color)
    };

    Mesh* heart = new Mesh(name);
    std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 4,
        4, 5, 6,
        6, 7, 0,
        0, 2, 6
    };

    heart->InitFromData(vertices, indices);
    return heart;
}
