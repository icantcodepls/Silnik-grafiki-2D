#include "Geometry.h"

#include <algorithm>
#include <cmath>

namespace Geometry {

float distanceSquaredToSegment(sf::Vector2f p, sf::Vector2f a, sf::Vector2f b) {
    const sf::Vector2f ab = b - a;
    const sf::Vector2f ap = p - a;
    const float len2 = ab.x * ab.x + ab.y * ab.y;
    float t = 0.f;
    if (len2 > 0.f) {
        t = std::clamp((ap.x * ab.x + ap.y * ab.y) / len2, 0.f, 1.f);
    }
    const sf::Vector2f d = p - (a + ab * t);
    return d.x * d.x + d.y * d.y;
}

bool segmentIntersectsCircle(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, float r) {
    return distanceSquaredToSegment(c, a, b) <= r * r;
}

unsigned int toPixel(float v) {
    return v <= 0.f ? 0u : static_cast<unsigned int>(std::lround(v));
}

} // namespace Geometry
