#include "Bullet.h"

#include <cmath>

#include "engine/Geometry.h"
#include "engine/PrimitiveRenderer.h"

Bullet::Bullet(sf::Vector2f position, sf::Vector2f direction, float speed)
    : EntityObject(position, 2.f), velocity(direction * speed), previous(position) {
    setName("Bullet");
    setColor(sf::Color(255, 230, 90));
    facing = std::atan2(direction.y, direction.x);
}

void Bullet::update(float dt) {
    previous = pos;
    pos += velocity * dt;
}

void Bullet::draw(Image& bufor) {
    // krotki slad w kierunku, z ktorego pocisk nadlecial
    const float speed = std::hypot(velocity.x, velocity.y);
    const sf::Vector2f tail = speed > 0.f ? pos - velocity / speed * 8.f : pos;
    PrimitiveRenderer::renderLine(bufor, Geometry::toPixel(tail.x), Geometry::toPixel(tail.y), Geometry::toPixel(pos.x),
                                  Geometry::toPixel(pos.y), getColor());
    PrimitiveRenderer::fillCircle(bufor, static_cast<int>(std::lround(pos.x)), static_cast<int>(std::lround(pos.y)),
                                  static_cast<int>(std::lround(radius)), sf::Color::White);
}
