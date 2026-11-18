#ifndef GRAFIKA2D_PLAYER_H
#define GRAFIKA2D_PLAYER_H
#include "EntityObject.h"
#include "Input.h"

/**
 * @file Player.h
 * @brief Gracz sterowany klawiaturą i myszą.
 */

/**
 * @brief Gracz sterowany klawiaturą i myszą.
 *
 *  - W/A/S/D lub strzałki - ruch (ukos jest normalizowany, więc nie jest szybszy),
 *  - kursor myszy - kierunek, w którym gracz jest zwrócony ("celuje").
 *
 * Gracz nie wychodzi poza obszar ustawiony przez setBounds() (zwykle cały bufor).
 * Domyślnie rysuje się jako wypełnione koło z "lufą" wskazującą kierunek celowania -
 * klasy pochodne (np. HotlinePlayer) nadpisują draw() własną grafiką.
 *
 * Użycie w pętli gry (co krok czasu):
 * @code
 * player.handleInput(input);   // odczyt klawiszy i myszy
 * player.update(dt);           // ruch i obrót
 * @endcode
 */
class Player : public EntityObject {
    sf::Vector2f moveDir{0.f, 0.f};   ///< Znormalizowany kierunek ruchu z klawiatury.
    sf::Vector2f aimTarget{0.f, 0.f}; ///< Punkt wskazywany myszą.
    sf::Vector2u bounds{800, 600};    ///< Obszar, w którym gracz może się poruszać.
    float speed = 220.f;              ///< Prędkość (piksele na sekundę).
    sf::Color bodyColor = sf::Color(90, 200, 255); ///< Kolor ciała w domyślnym rysowaniu.

    /// @brief Zatrzymuje gracza w granicach obszaru.
    void clampToBounds();

public:
    /// @brief Tworzy gracza. @param position Pozycja startowa.
    explicit Player(sf::Vector2f position = {0.f, 0.f});

    /// @brief Odczytuje klawisze (ruch) i mysz (celowanie). @param input Stan wejścia.
    void handleInput(const Input& input);
    /// @brief Porusza gracza i obraca go w stronę kursora. @param dt Krok czasu (s).
    void update(float dt) override;
    /// @brief Rysuje koło z lufą. @param bufor Obraz docelowy.
    void draw(Image& bufor) override;

    /// @brief Ustawia obszar ruchu (zwykle rozmiar bufora). @param size Rozmiar obszaru.
    void setBounds(sf::Vector2u size);
    /// @brief Zwraca obszar ruchu.
    sf::Vector2u getBounds() const { return bounds; }
    /// @brief Ustawia prędkość. @param pixelsPerSecond Piksele na sekundę.
    void setSpeed(float pixelsPerSecond) { speed = pixelsPerSecond; }
    /// @brief Zwraca prędkość (px/s).
    float getSpeed() const { return speed; }
    /// @brief Ustawia kolor ciała. @param c Kolor.
    void setBodyColor(sf::Color c) { bodyColor = c; }
    /// @brief Zwraca kolor ciała.
    sf::Color getBodyColor() const { return bodyColor; }
    /// @brief Zwraca punkt wskazywany myszą.
    sf::Vector2f getAimTarget() const { return aimTarget; }
    /// @brief Zwraca jednostkowy wektor w kierunku celowania.
    sf::Vector2f getAimDirection() const;
    /// @brief Zwraca kierunek ostatnio wciśniętych klawiszy ruchu ((0,0), gdy stoi).
    sf::Vector2f getMoveDirection() const { return moveDir; }
};

#endif // GRAFIKA2D_PLAYER_H
