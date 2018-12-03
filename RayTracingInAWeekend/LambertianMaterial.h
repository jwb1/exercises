#pragma once

class LambertianMaterial : public Material {
public:
    LambertianMaterial(const glm::vec3& albedo, std::mt19937& mtDevice)
        : m_albedo(albedo)
        , m_mtDevice(mtDevice)
    {}

    virtual bool Scatter(const Ray& ray, Intersection& intersection, const glm::vec3& attenuation, Ray& scatteredRay) const
    {
        glm::vec3 reflectionDirection(intersection.normal + RandomPointInUnitSphere());
        scatteredRay = Ray(intersection.point, reflectionDirection);
        attenuation = m_albedo;
        return (true);
    }

private:
    glm::vec3 m_albedo;
    std::mt19937& m_mtDevice;

    glm::vec3 RandomPointInUnitSphere() const
    {
        std::uniform_real_distribution<float> randomDistribution(0.0f, 1.0f);
        glm::vec3 p;
        do {
            p = 2.0f * glm::vec3(randomDistribution(m_mtDevice), randomDistribution(m_mtDevice), randomDistribution(m_mtDevice)) - glm::vec3(1.0f, 1.0f, 1.0f);
        } while (glm::length(p) >= 1.0f);
        return (p);
    }
};
