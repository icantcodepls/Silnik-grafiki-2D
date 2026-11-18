#include "Enemy.h"

Enemy::Enemy(sf::Vector2f position, float angle, GameAssets& assets)
    : EntityObject(position, BODY_RADIUS), sprite(assets.handler, assets.enemyLegs, assets.enemyTorso) {
    setName("Enemy");
    facing = angle;
}

void Enemy::update(float dt) {
    // przeciwnicy sie nie ruszaja
    sprite.setWalking(false);
    sprite.animate(dt);
}

void Enemy::draw(Image& bufor) {
    sprite.setPose(pos, facing, facing); // stoi: nogi i tulow w tym samym kierunku
    sprite.draw(bufor);
}
