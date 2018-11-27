#pragma once

class Ray {
public:
    Ray() {}
    Ray(const glm::vec3& origin, const glm::vec3& direction)
        : m_origin(origin)
        , m_direction(direction)
    {}

    glm::vec3 Origin() const
    {
        return (m_origin);
    }

    glm::vec3 Direction() const
    {
        return (m_direction);
    }

    glm::vec3 PointAtParamerter(float t) const
    {
        return (m_origin + (m_direction * t));
    }

private:
    glm::vec3 m_origin;
    glm::vec3 m_direction;
};
