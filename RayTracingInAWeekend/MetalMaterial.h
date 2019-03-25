#pragma once
#include "Material.h"

class MetalMaterial : public Material {
public:
    explicit MetalMaterial(const glm::vec3& albedo, float roughness = 0.0f)
        : m_albedo(albedo)
        , m_roughness(roughness)
    {}

    virtual bool Scatter(const Ray& ray, Intersection& intersection, glm::vec3& attenuation, Ray& scatteredRay) const
    {
        glm::vec3 reflectionDirection(ReflectionDirection(ray.Direction(), intersection.normal));
        scatteredRay = Ray(intersection.point, reflectionDirection + m_roughness * RandomPointInUnitSphere());
        attenuation = m_albedo;
        return (glm::dot(scatteredRay.Direction(), intersection.normal) > 0.0f);
    }

private:
    glm::vec3 m_albedo;
    float m_roughness;
};
