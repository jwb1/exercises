#pragma once

struct Intersection {
    glm::vec3 point;
    glm::vec3 normal;
    float t;
};

class Solid {
public:
    virtual bool Intersect(const Ray& ray, float tMin, float tMax, Intersection& intersect) const = 0;
};

class SolidList : public Solid {
public:
    SolidList() {}

    void AddSolid(Solid* solid)
    {
        m_solids.push_back(solid);
    }

    virtual bool Intersect(const Ray& ray, float tMin, float tMax, Intersection& intersect) const
    {
        Intersection currentIntersection;
        bool haveIntersection = false;

        intersect.t = tMax;
        for (const Solid* solid : m_solids) {
            if (solid->Intersect(ray, tMin, intersect.t, currentIntersection)) {
                haveIntersection = true;
                intersect = currentIntersection;
            }
        }

        return (haveIntersection);
    }

private:
    std::vector<Solid*> m_solids;
};
