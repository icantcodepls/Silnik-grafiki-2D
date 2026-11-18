#pragma once
#include "CharacterSprite.h"
#include "GameAssets.h"
#include "engine/Player.h"

/**
 * @file HotlinePlayer.h
 * @brief Gracz z dema gry (tekstury, laser, strzelanie).
 */

/**
 * @brief Gracz z dema: Player z silnika (ruch WSAD + obrót za kursorem) rozszerzony o:
 *  - tekstury: tułów celuje w kursor, nogi idą w kierunku ruchu i animują krok,
 *  - czerwony celownik laserowy od postaci do kursora,
 *  - broń z ograniczoną szybkostrzelnością (tryShoot()) i błyskiem z lufy.
 */
class HotlinePlayer : public Player {
    CharacterSprite sprite;       ///< Wygląd gracza.
    float legsAngle = 0.f;        ///< Kierunek ostatniego ruchu (kąt nóg).
    float cooldown = 0.f;         ///< Czas do kolejnego możliwego strzału.
    float fireInterval = 0.18f;   ///< Minimalny odstęp między strzałami (s).
    sf::Color laserColor = sf::Color::Red;   ///< Kolor celownika laserowego.

public:
    static constexpr float BODY_RADIUS = 14.f;       ///< Promień kolizji gracza.
    static constexpr float MUZZLE_DISTANCE = 24.f;   ///< Odległość od środka postaci do końca lufy (px).

    /// @brief Tworzy gracza. @param position Pozycja startowa. @param assets Tekstury gry.
    HotlinePlayer(sf::Vector2f position, GameAssets& assets);

    /// @brief Ruch, kąt nóg, animacja i przeładowanie. @param dt Krok czasu (s).
    void update(float dt) override;
    /// @brief Rysuje czerwony laser, a na nim postać. @param bufor Obraz docelowy.
    void draw(Image& bufor) override;

    /// @brief Próbuje strzelić. @return true, gdy broń była gotowa (rozpoczyna przeładowanie i pokazuje błysk).
    bool tryShoot();
    /// @brief Zwraca punkt, z którego wylatuje pocisk (koniec lufy).
    sf::Vector2f getMuzzlePosition() const;
    /// @brief Zwraca kąt nóg = kierunek ostatniego ruchu (rad).
    float getLegsAngle() const { return legsAngle; }
};
