#include "CharacterSprite.h"

#include <algorithm>

#include "engine/Geometry.h"

CharacterSprite::CharacterSprite(BitmapHandler& handler, BitmapId legs, BitmapId torso, float scale)
    : SpriteObject(handler) {
    setName("CharacterSprite");
    setScale(scale);
    // kolejnosc czesci = kolejnosc rysowania: nogi pod spodem, tulow na wierzchu
    legsPart = addPart(legs, {0, 0}, IntRect({0, 0}, {FRAME, FRAME}), "legs");
    torsoPart = addPart(torso, {0, 0}, IntRect({0, 0}, {FRAME, FRAME}), "torso");
}

void CharacterSprite::setPose(sf::Vector2f position, float aimAngle, float legsAngle) {
    setPosition({Geometry::toPixel(position.x), Geometry::toPixel(position.y)});
    setRotation(aimAngle);                             // obraca caly obiekt = tulow
    getPart(legsPart).rotation = legsAngle - aimAngle; // nogi: kat wzgledem tulowia
}

void CharacterSprite::updateFrames() {
    getPart(legsPart).source = IntRect({legFrame * FRAME, 0}, {FRAME, FRAME});
    getPart(torsoPart).source = IntRect({(flashTimer > 0.f ? 1 : 0) * FRAME, 0}, {FRAME, FRAME});
}

void CharacterSprite::animate(float dt) {
    if (walking) {
        legTimer += dt;
        while (legTimer >= stepInterval) {
            legTimer -= stepInterval;
            legFrame = (legFrame + 1) % LEG_FRAMES;
        }
    } else {
        legFrame = 0; // stoi: nogi w pozycji spoczynkowej
        legTimer = 0.f;
    }
    flashTimer = std::max(0.f, flashTimer - dt);
    updateFrames();
}
