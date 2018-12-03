#pragma once

class Material {
public:
    virtual bool Scatter(const Ray& ray, Intersection& intersection, const glm::vec3& attenuation, Ray& scatteredRay) const = 0;
};
