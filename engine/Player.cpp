#include "Player.h"

#include <algorithm>
#include <cmath>

#include "Geometry.h"
#include "PrimitiveRenderer.h"

Player::Player(sf::Vector2f position) : EntityObject(position, 12.f) {
    setName("Player");
    setColor(sf::Color::White); // kolor "lufy" w domyslnym rysowaniu
}

void Player::setBounds(sf::Vector2u size) {
    bounds = size;
    clampToBounds();
}

void Player::clampToBounds() {
    const float maxX = std::max(radius, static_cast<float>(bounds.x) - 1.f - radius);
    const float maxY = std::max(radius, static_cast<float>(bounds.y) - 1.f - radius);
    pos.x = std::clamp(pos.x, radius, maxX);
    pos.y = std::clamp(pos.y, radius, maxY);
}

void Player::handleInput(const Input& input) {
    sf::Vector2f dir(0.f, 0.f);
    if (input.isKeyDown(sf::Keyboard::Key::W) || input.isKeyDown(sf::Keyboard::Key::Up))
        dir.y -= 1.f;
    if (input.isKeyDown(sf::Keyboard::Key::S) || input.isKeyDown(sf::Keyboard::Key::Down))
        dir.y += 1.f;
    if (input.isKeyDown(sf::Keyboard::Key::A) || input.isKeyDown(sf::Keyboard::Key::Left))
        dir.x -= 1.f;
    if (input.isKeyDown(sf::Keyboard::Key::D) || input.isKeyDown(sf::Keyboard::Key::Right))
        dir.x += 1.f;

    const float len = std::hypot(dir.x, dir.y);
    moveDir = len > 0.f ? dir / len : sf::Vector2f(0.f, 0.f);
    aimTarget = input.mouse;
}

void Player::update(float dt) {
    pos += moveDir * speed * dt;
    clampToBounds();

    const sf::Vector2f toTarget = aimTarget - pos;
    if (toTarget.x * toTarget.x + toTarget.y * toTarget.y >
        1.f) { // kursor dokladnie na graczu - zostaje stary kierunek
        facing = std::atan2(toTarget.y, toTarget.x);
    }
}

sf::Vector2f Player::getAimDirection() const {
    return {std::cos(facing), std::sin(facing)};
}

void Player::draw(Image& bufor) {
    const int cx = static_cast<int>(std::lround(pos.x));
    const int cy = static_cast<int>(std::lround(pos.y));
    PrimitiveRenderer::fillCircle(bufor, cx, cy, static_cast<int>(std::lround(radius)), bodyColor);

    // "lufa" - krotka kreska od srodka na zewnatrz w kierunku celowania
    const sf::Vector2f tip = pos + getAimDirection() * (radius + 7.f);
    PrimitiveRenderer::renderLine(bufor, Geometry::toPixel(pos.x), Geometry::toPixel(pos.y), Geometry::toPixel(tip.x),
                                  Geometry::toPixel(tip.y), getColor());
}
