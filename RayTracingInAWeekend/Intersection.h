#pragma once

class Material;

struct Intersection {
    Intersection()
        : t(FLT_MIN)
        , material(nullptr)
    {}

    glm::vec3 point;
    glm::vec3 normal;
    float t;
    const Material* material;
};
