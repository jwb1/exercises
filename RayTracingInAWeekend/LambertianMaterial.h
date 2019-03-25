#pragma once
#include "Material.h"

class LambertianMaterial : public Material {
public:
    explicit LambertianMaterial(const glm::vec3& albedo)
        : m_albedo(albedo)
    {}

    virtual bool Scatter(const Ray& ray, Intersection& intersection, glm::vec3& attenuation, Ray& scatteredRay) const
    {
        glm::vec3 reflectionDirection(intersection.normal + RandomPointInUnitSphere());
        scatteredRay = Ray(intersection.point, reflectionDirection);
        attenuation = m_albedo;
        return (true);
    }

private:
    glm::vec3 m_albedo;
};
