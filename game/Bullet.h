#pragma once
#include "engine/EntityObject.h"

/**
 * @file Bullet.h
 * @brief Pocisk wystrzeliwany przez gracza.
 */

/**
 * @brief Pocisk lecący prosto ze stałą prędkością.
 *
 * Do wykrywania trafień udostępnia poprzednią i aktualną pozycję - gra sprawdza kolizję
 * odcinka między nimi, więc szybki pocisk nie "przeskoczy" przeciwnika między klatkami.
 */
class Bullet : public EntityObject {
    sf::Vector2f velocity;   ///< Wektor prędkości (px/s).
    sf::Vector2f previous;   ///< Pozycja z poprzedniego kroku.

public:
    /**
     * @brief Tworzy pocisk.
     * @param position Pozycja startowa (koniec lufy).
     * @param direction Kierunek lotu (jednostkowy wektor).
     * @param speed Prędkość w pikselach na sekundę.
     */
    Bullet(sf::Vector2f position, sf::Vector2f direction, float speed = 900.f);

    /// @brief Przesuwa pocisk. @param dt Krok czasu (s).
    void update(float dt) override;
    /// @brief Rysuje smugę i jasną główkę pocisku. @param bufor Obraz docelowy.
    void draw(Image& bufor) override;

    /// @brief Zwraca pozycję z poprzedniego kroku (początek odcinka do testu trafienia).
    sf::Vector2f getPreviousPosition() const { return previous; }
};
