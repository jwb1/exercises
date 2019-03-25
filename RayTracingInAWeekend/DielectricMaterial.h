#pragma once
#include "Material.h"

class DielectricMaterial : public Material {
public:
    explicit DielectricMaterial(float ri)
        : m_refractionIndex(ri)
        , m_reflectionDistribution(0.0f, 1.0f)
    {}

    virtual bool Scatter(const Ray& ray, Intersection& intersection, glm::vec3& attenuation, Ray& scatteredRay) const
    {
        attenuation = glm::vec3(1.0f, 1.0f, 1.0f);

        // Work out the ratio of refraction indicies, and the cosine of the angle
        // between the ray and the normal for the schlick fresnel approximation.
        glm::vec3 outwardNormal;
        float niOverNt;
        float cosine;
        float rayDotNormal = glm::dot(ray.Direction(), intersection.normal);
        if (rayDotNormal > 0.0f) {
            outwardNormal = -intersection.normal;
            niOverNt = m_refractionIndex;
            cosine = rayDotNormal / glm::length(ray.Direction());
            //cosine = std::sqrtf(1.0f - m_refractionIndex * m_refractionIndex * (1.0f - cosine * cosine));
        }
        else {
            outwardNormal = intersection.normal;
            niOverNt = 1.0f / m_refractionIndex;
            cosine = -rayDotNormal / glm::length(ray.Direction());
        }

        // Compute the refracted direction and the probability of using it (vrs reflection).
        glm::vec3 refractedDirection;
        float reflectionProbability = 1.0f;
        if (RefractionDirection(ray.Direction(), outwardNormal, niOverNt, refractedDirection)) {
            reflectionProbability = SchlickFresnel(cosine, m_refractionIndex);
        }

        // The scattered ray is selected randomly.
        if (m_reflectionDistribution(GetRandomDevice()) > reflectionProbability) {
            scatteredRay = Ray(intersection.point, refractedDirection);
        }
        else {
            glm::vec3 reflectionDirection(ReflectionDirection(ray.Direction(), intersection.normal));
            scatteredRay = Ray(intersection.point, reflectionDirection);
        }

        return (true);
    }

private:
    float m_refractionIndex;
    std::uniform_real_distribution<float> m_reflectionDistribution;

    bool RefractionDirection(const glm::vec3& inDirection, const glm::vec3& normal, float niOverNt, glm::vec3& refractedDirection) const
    {
        glm::vec3 normalizedIn(glm::normalize(inDirection));
        float dt = glm::dot(normalizedIn, normal);
        float discriminant = 1.0f - niOverNt * niOverNt * (1.0f - dt * dt);
        if (discriminant > 0.0f) {
            refractedDirection = niOverNt * (normalizedIn - normal * dt) - (normal * std::sqrtf(discriminant));
            return (true);
        }
        else {
            return (false); // Total internal reflection
        }
    }

    float SchlickFresnel(float cosine, float refractionIndex) const
    {
        float r0 = (1.0f - refractionIndex) / (1.0f + refractionIndex);
        r0 = r0 * r0;
        return (r0 + (1.0f - r0) * std::powf((1.0f - cosine), 5.0f));
    }
};
