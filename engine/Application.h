#ifndef GRAFIKA2D_APPLICATION_H
#define GRAFIKA2D_APPLICATION_H
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "Input.h"

class Engine;

/**
 * @file Application.h
 * @brief Interfejs aplikacji uruchamianej przez Engine.
 */

/**
 * @brief Interfejs aplikacji (gry, edytora, dema) uruchamianej w pętli silnika.
 *
 * Silnik wywołuje metody w tej kolejności: onStart() raz na początku, a potem w pętli
 * onEvent() dla każdego zdarzenia, update() ze stałym krokiem czasu i draw() raz na klatkę.
 * Wszystkie metody poza draw() mają pustą domyślną implementację.
 */
class Application {
public:
    virtual ~Application() = default;
    /// @brief Wywoływana raz po utworzeniu okna. @param engine Silnik uruchamiający aplikację.
    virtual void onStart(Engine& engine) {}
    /// @brief Wywoływana dla każdego zdarzenia okna (klawisze, mysz, zamknięcie...). @param event Zdarzenie SFML.
    virtual void onEvent(const sf::Event& event) {}
    /// @brief Logika ze stałym krokiem czasu. @param dt Krok czasu w sekundach. @param input Stan klawiatury i myszy.
    virtual void update(float dt, const Input& input) {}
    /// @brief Rysuje klatkę w buforze obrazu (czysta metoda wirtualna). @param frame Bufor klatki.
    virtual void draw(sf::Image& frame) = 0;
    /// @brief Wywoływana po zmianie rozdzielczości bufora. @param oldSize Stara. @param newSize Nowa.
    virtual void onResize(sf::Vector2u oldSize, sf::Vector2u newSize) {}
    /// @brief Kolor tła wokół bufora (pasy przy innej proporcji okna).
    virtual sf::Color backgroundColor() const { return sf::Color::Black; }
    /// @brief Wywoływana, gdy aplikacja staje się aktywna w AppSwitcher (np. do ustawienia tytułu okna).
    virtual void onActivate() {}
    /// @brief Wywoływana, gdy AppSwitcher przełącza się na inną aplikację.
    virtual void onDeactivate() {}
};

#endif //GRAFIKA2D_APPLICATION_H