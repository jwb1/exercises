#pragma once

class Material;

struct Intersection {
    glm::vec3 point;
    glm::vec3 normal;
    float t;
    const Material* material;
};
