#pragma once

class Sphere : public Solid {
public:
    Sphere()
        : m_radius(0.0f)
        , m_material(nullptr)
    {}
    Sphere(const glm::vec3& center, float radius, Material* material = nullptr)
        : m_center(center)
        , m_radius(radius)
        , m_material(material)
    {}

    virtual ~Sphere()
    {
        if (m_material) {
            delete m_material;
        }
    }

    glm::vec3 Center() const
    {
        return (m_center);
    }

    float Radius() const
    {
        return (m_radius);
    }

    virtual bool Intersect(const Ray& ray, float tMin, float tMax, Intersection& intersect) const
    {
        glm::vec3 oc = ray.Origin() - m_center;
        float a = glm::dot(ray.Direction(), ray.Direction());
        float b = glm::dot(oc, ray.Direction());
        float c = glm::dot(oc, oc) - m_radius * m_radius;
        float discriminant = b * b - a * c;
        if (discriminant >= 0.0f) {
            // Checking the '-' root first ensures the intersection closer to the ray origin.
            float t = (-b - std::sqrtf(discriminant)) / a;
            if (t < tMax && t > tMin) {
                intersect.point = ray.PointAtParamerter(t);
                intersect.normal = glm::normalize(intersect.point - m_center);
                intersect.t = t;
                intersect.material = m_material;
                return (true);
            }
            t = (-b + std::sqrtf(discriminant)) / a;
            if (t < tMax && t > tMin) {
                intersect.point = ray.PointAtParamerter(t);
                intersect.normal = glm::normalize(intersect.point - m_center);
                intersect.t = t;
                intersect.material = m_material;
                return (true);
            }
        }

        return (false);
    }

private:
    glm::vec3 m_center;
    float m_radius;
    Material* m_material; // For now each solid owns it's own material.
};
