#pragma once

class Material {
public:
    virtual ~Material()
    {}

    virtual bool Scatter(const Ray& ray, Intersection& intersection, glm::vec3& attenuation, Ray& scatteredRay) const = 0;

protected:
    glm::vec3 RandomPointInUnitSphere() const
    {
        std::uniform_real_distribution<float> randomDistribution(0.0f, 1.0f);
        glm::vec3 p;
        do {
            p = 2.0f * glm::vec3(randomDistribution(GetRandomDevice()), randomDistribution(GetRandomDevice()), randomDistribution(GetRandomDevice())) - glm::vec3(1.0f, 1.0f, 1.0f);
        } while (glm::length(p) >= 1.0f);
        return (p);
    }
};
