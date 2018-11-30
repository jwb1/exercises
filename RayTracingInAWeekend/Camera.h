#pragma once

class Camera {
public:
    Camera(
        float distanceToImage,
        int imageWidthInPixels,
        int imageHeightInPixels,
        float fieldOfView
        )
        : m_aspectRatio(float(imageWidthInPixels) / float(imageHeightInPixels))
        , m_fieldOfView(fieldOfView)
    {
        m_imageHeight = 2.0f * std::fabs(distanceToImage) * std::tanf(fieldOfView / 2.0f);
        m_imageWidth = m_imageHeight * m_aspectRatio;
        m_imageLowerLeft = glm::vec3(-(m_imageWidth / 2.0f), -(m_imageHeight / 2.0f), distanceToImage);
    }

    float AspectRatio() const
    {
        return (m_aspectRatio);
    }

    float FieldOfView() const
    {
        return (m_fieldOfView);
    }

    float Height() const
    {
        return (m_imageHeight);
    }

    float Width() const
    {
        return (m_imageWidth);
    }

    Ray MakeEyeRay(const glm::vec2& pixelPosition) const
    {
        return (Ray(
            glm::vec3(0.0f, 0.0f, 0.0f),
            m_imageLowerLeft + glm::vec3(pixelPosition.x * m_imageWidth, pixelPosition.y * m_imageHeight, 0.0f)
        ));
    }

private:
    // Assume camera origin is (0.0f, 0.0f, 0.0f), pointed along -z axis.
    float m_aspectRatio; // Ratio: height / width
    float m_fieldOfView; // In Radians, measuring y side of the image.

    float m_imageHeight; // In 3D coordinates, not pixels.
    float m_imageWidth;  // In 3D coordinates, not pixels.
    glm::vec3 m_imageLowerLeft; // In 3D coordinates.
};
