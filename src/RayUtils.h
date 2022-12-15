#ifndef TPM2_GEOM_RAYUTILS_H
#define TPM2_GEOM_RAYUTILS_H

#include "glm/glm.hpp"

struct Ray {
    glm::vec3 orig;
    glm::vec3 dir;
    float t;
};

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
};

bool intersectBox(AABB aabb, Ray &r);


#endif //TPM2_GEOM_RAYUTILS_H
