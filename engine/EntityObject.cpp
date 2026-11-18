#include "EntityObject.h"

#include <cmath>

void EntityObject::translate(int dx, int dy) {
    pos.x += static_cast<float>(dx);
    pos.y += static_cast<float>(dy);
}

void EntityObject::scale(unsigned int x, unsigned int y, float k) {
    const sf::Vector2f pivot(static_cast<float>(x), static_cast<float>(y));
    pos = pivot + (pos - pivot) * k;
    radius *= k;
}

void EntityObject::rotate(unsigned int x, unsigned int y, float angle) {
    const sf::Vector2f pivot(static_cast<float>(x), static_cast<float>(y));
    const sf::Vector2f d = pos - pivot;
    const float c = std::cos(angle), s = std::sin(angle);
    pos = {pivot.x + d.x * c - d.y * s, pivot.y + d.x * s + d.y * c};
    facing += angle;
}
