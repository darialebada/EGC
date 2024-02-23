#pragma once

#include "utils/glm_utils.h"


namespace transform2D
{
    // Translate matrix
    inline glm::mat3 Translate(float translateX, float translateY)
    {
        // TODO(student): Implement the translation matrix
        return glm::mat3(
            1, 0, 0,
            0, 1, 0,
            translateX, translateY, 1);

    }

    // Scale matrix
    inline glm::mat3 Scale(float scaleX, float scaleY)
    {
        // TODO(student): Implement the scaling matrix
        return glm::mat3(
            scaleX, 0, 0,
            0, scaleY, 0,
            0, 0, 1);

    }

    // Rotate matrix
    inline glm::mat3 Rotate(float radians)
    {
        // TODO(student): Implement the rotation matrix
        float s = sin(radians);
        float c = cos(radians);

        return glm::mat3(
            c, s, 0,
            -s, c, 0,
            0, 0, 1);

    }
}   // namespace transform2D
