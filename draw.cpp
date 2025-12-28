#include "draw.h"

Vector2d screen(const Vector2d& coords) {
    return Vector2d(
        (coords.x + 1.0f) * 0.5f * screenWidth,
        (1.0f - (coords.y + 1.0f) * 0.5f) * screenHeight);
}

Vector2d project(const Vector3d& coords) {
    return Vector2d(
        coords.x / coords.z,
		coords.y / coords.z);
}

void drawPixel(const Vector2d& coords, const int size)
{
    const int h = size;
    const int w = size;
	constexpr uint32_t color = 0xFF00FF00; 

    int cx = static_cast<int>(std::round(coords.x));
    int cy = static_cast<int>(std::round(coords.y));

    for (int i = -w/2; i < w/2; i++) {
        for (int j = -h/2; j < h/2; j++) {
            int px = cx + i;
            int py = cy + j;
            if (px >= 0 && px < screenWidth && py >= 0 && py < screenHeight) {
				frameBuffer[py * screenWidth + px] = color;
            }
        }
    }
}

void drawLine(const Vector2d& start, const Vector2d& end)
{
    int x0 = static_cast<int>(std::round(start.x));
    int y0 = static_cast<int>(std::round(start.y));
    int x1 = static_cast<int>(std::round(end.x));
    int y1 = static_cast<int>(std::round(end.y));

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

    while (true)
    {
        drawPixel(Vector2d{ float(x0), float(y0) }, 3);

        if (x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;

        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

Vector3d rotate_xz(Vector3d coords, float angle) {
    const float c = std::cos(angle);
    const float s = std::sin(angle);
    return Vector3d(
        coords.x * c - coords.z * s,
        coords.y,
        coords.x * s + coords.z * c
	);
}

Vector3d computeCenter(const std::vector<Vector3d>& verts)
{
    Vector3d center{ 0, 0, 0 };
    for (const auto& v : verts)
        center += v;

    return center / verts.size();
}

std::vector<Vector3d> rotateMeshInPlace(const std::vector<Vector3d> verts, double angle)
{
    std::vector<Vector3d> res = verts;
    Vector3d center = computeCenter(verts);

    for (auto& v : res)
    {
        // move to local space
        Vector3d local = v - center;

        // rotate
        local = rotate_xz(local, angle);

        // move back to world space
        v = local + center;
    }
    return res;
}


uint32_t LerpColor(uint32_t c1, uint32_t c2, float t)
{
    auto lerp = [](uint8_t a, uint8_t b, float t) {
        return (uint8_t)(a + (b - a) * t);
        };

    uint8_t a1 = (c1 >> 24) & 0xFF;
    uint8_t r1 = (c1 >> 16) & 0xFF;
    uint8_t g1 = (c1 >> 8) & 0xFF;
    uint8_t b1 = c1 & 0xFF;

    uint8_t a2 = (c2 >> 24) & 0xFF;
    uint8_t r2 = (c2 >> 16) & 0xFF;
    uint8_t g2 = (c2 >> 8) & 0xFF;
    uint8_t b2 = c2 & 0xFF;

    return (lerp(a1, a2, t) << 24) |
        (lerp(r1, r2, t) << 16) |
        (lerp(g1, g2, t) << 8) |
        lerp(b1, b2, t);
}

uint32_t FadeToBlack(uint32_t color)
{
    uint8_t a = (color >> 24) & 0xFF;
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;

    r = (uint8_t)(r * 0.95f);
    g = (uint8_t)(g * 0.95f);
    b = (uint8_t)(b * 0.95f);

    return (a << 24) | (r << 16) | (g << 8) | b;
}