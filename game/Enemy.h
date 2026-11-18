#pragma once
#include "CharacterSprite.h"
#include "GameAssets.h"
#include "engine/EntityObject.h"

/**
 * @file Enemy.h
 * @brief Nieruchomy przeciwnik z dema gry.
 */

/**
 * @brief Nieruchomy przeciwnik rysowany z tekstur.
 *
 * "Życie" to flaga active z GameObject: po trafieniu kill() ustawia active=false,
 * a gra usuwa takie obiekty z listy (po prostu znikają, bez efektów).
 */
class Enemy : public EntityObject {
    CharacterSprite sprite;   ///< Wygląd przeciwnika.

public:
    static constexpr float BODY_RADIUS = 14.f;   ///< Promień kolizji (postać ma ok. 32 px wysokości przy skali 2).

    /// @brief Tworzy przeciwnika. @param position Pozycja. @param facing Kąt, w który patrzy (rad). @param assets Tekstury gry.
    Enemy(sf::Vector2f position, float facing, GameAssets& assets);

    /// @brief Stoi w miejscu - tylko podtrzymuje animację. @param dt Krok czasu (s).
    void update(float dt) override;
    /// @brief Rysuje przeciwnika. @param bufor Obraz docelowy.
    void draw(Image& bufor) override;

    /// @brief Zabija przeciwnika (ustawia active=false).
    void kill() { setActive(false); }
    /// @brief Zwraca true, gdy przeciwnik żyje.
    bool isAlive() const { return isActive(); }
};
