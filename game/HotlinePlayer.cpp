#include "HotlinePlayer.h"

#include <algorithm>
#include <cmath>

#include "engine/Geometry.h"
#include "engine/LineSegment.h"

HotlinePlayer::HotlinePlayer(sf::Vector2f position, GameAssets& assets)
    : Player(position), sprite(assets.handler, assets.playerLegs, assets.playerTorso) {
    setName("HotlinePlayer");
    setRadius(BODY_RADIUS);
}

void HotlinePlayer::update(float dt) {
    const sf::Vector2f before = pos;
    Player::update(dt);

    // nogi ida tam, gdzie postac faktycznie sie przesunela (a nie tam, gdzie celuje tulow);
    // przy zatrzymaniu zostaje ostatni kierunek
    const sf::Vector2f moved = pos - before;
    const bool walking = std::hypot(moved.x, moved.y) > 0.01f;
    if (walking) {
        legsAngle = std::atan2(moved.y, moved.x);
    }
    sprite.setWalking(walking);
    sprite.animate(dt);

    cooldown = std::max(0.f, cooldown - dt);
}

bool HotlinePlayer::tryShoot() {
    if (cooldown > 0.f) {
        return false;
    }
    cooldown = fireInterval;
    sprite.showMuzzleFlash();
    return true;
}

sf::Vector2f HotlinePlayer::getMuzzlePosition() const {
    return pos + getAimDirection() * MUZZLE_DISTANCE;
}

void HotlinePlayer::draw(Image& bufor) {
    // czerwona kreska od postaci do kursora (celowanie bez zmian); postac rysujemy na wierzchu
    LineSegment laser;
    laser.setPoints(Geometry::toPixel(pos.x), Geometry::toPixel(pos.y), Geometry::toPixel(getAimTarget().x),
                    Geometry::toPixel(getAimTarget().y));
    laser.setColor(laserColor);
    laser.draw(bufor);

    sprite.setPose(pos, facing, legsAngle);
    sprite.draw(bufor);
}
