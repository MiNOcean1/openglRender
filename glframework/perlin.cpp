#include "perlin.h"

Perlin::Perlin()
{
}

Perlin::~Perlin()
{
}

std::vector<unsigned char> Perlin::GenerateNoise(int width, int height, float scale)
{
    SEED = static_cast<unsigned int>(glfwGetTime());
    std::vector<unsigned char> data(width * height * 3); // RGB format

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            // Convert to texture size.
            float xf = x / static_cast<float>(width) * scale;
            float yf = y / static_cast<float>(height) * scale;

            // Normalize Perlin noise to [0, 1]
            float noise = SampleNoise(xf, yf) * 0.5f + 0.5f;

            // Map to [0, 255]
            unsigned char value = static_cast<unsigned char>(noise * 255);

            // RGB format
            int index = (y * width + x) * 3;
            data[index] = value;     // Red
            data[index + 1] = value; // Green
            data[index + 2] = value; // Blue
        }
    }

    return data;
}

float Perlin::Smoothstep(float t)
{
    return t * t * (3 - 2 * t);
}

float Perlin::Interpolate(float a0, float a1, float w, bool isLinear)
{
    if (isLinear)
        return a0 * (1 - w) + a1 * w;
    else
        return (a1 - a0) * (3.f - w * 2.f) * w * w + a0;
}

glm::vec2 Perlin::RandomGradient(int ix, int iy)
{
    unsigned int hash = ix * 374761393u + iy * 668265263u + SEED; // Add the seed

    hash = (hash ^ (hash >> 13u)) * 1274126177u;
    hash = (hash ^ (hash >> 16u));

    float angle = (hash % 360) * 3.14159265359f / 180.0f; // Convert to radians

    return glm::vec2(cos(angle), sin(angle)); // Gradient vector
}

float Perlin::SampleNoise(float x, float y)
{
    // Find where the sample is located (corners of the box)
    int x0 = static_cast<int>(floor(x));
    int x1 = x0 + 1;
    int y0 = static_cast<int>(floor(y));
    int y1 = y0 + 1;

    float sx = Smoothstep(x - x0);
    float sy = Smoothstep(y - y0);

    // Top Corners:Calculate weight of each corner then interpolate between them
    float n0 = DotGridGradient(x0, y0, x, y);
    float n1 = DotGridGradient(x1, y0, x, y);
    float ix0 = Interpolate(n0, n1, sx, true); // Top Side Weight

    // Bottom Corners: Do the same for them
    n0 = DotGridGradient(x0, y1, x, y);
    n1 = DotGridGradient(x1, y1, x, y);
    float ix1 = Interpolate(n0, n1, sx, true); // Bottom Side Weight

    return Interpolate(ix0, ix1, sy, true); // Interpolate Top and Bottom Side Weight Influence
}

float Perlin::DotGridGradient(int ix, int iy, float x, float y)
{
    glm::vec2 gradient = RandomGradient(ix, iy); // Random gradient vector
    glm::vec2 distance(x - ix, y - iy); // Distance vector

    return glm::dot(gradient, distance);
}
