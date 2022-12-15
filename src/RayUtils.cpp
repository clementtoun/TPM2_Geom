#include "RayUtils.h"

bool intersectBox(AABB aabb, Ray &r){
    float tmin = (aabb.min.x - r.orig.x) / r.dir.x;
    float tmax = (aabb.max.x - r.orig.x) / r.dir.x;

    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (aabb.min.y - r.orig.y) / r.dir.y;
    float tymax = (aabb.max.y - r.orig.y) / r.dir.y;

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    float tzmin = (aabb.min.z - r.orig.z) / r.dir.z;
    float tzmax = (aabb.max.z - r.orig.z) / r.dir.z;

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin)
        tmin = tzmin;

    if (tzmax < tmax)
        tmax = tzmax;

    r.t = tmin;
    return true;
}
