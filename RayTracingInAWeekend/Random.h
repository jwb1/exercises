#pragma once
#include <random>

class Random {
public:
    Random()
        : m_seedDevice()
        , m_device(m_seedDevice())
    {}

    std::mt19937& GetDevice()
    {
        return (m_device);
    }

private:
    std::random_device m_seedDevice;
    std::mt19937 m_device;
};

std::mt19937& GetRandomDevice()
{
    static Random r;
    return (r.GetDevice());
}
