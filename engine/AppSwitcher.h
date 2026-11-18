#ifndef GRAFIKA2D_APPSWITCHER_H
#define GRAFIKA2D_APPSWITCHER_H
#include <string>
#include <vector>

#include "Application.h"

/**
 * @file AppSwitcher.h
 * @brief Przełącznik aplikacji: pozwala uruchomić kilka Application w jednym oknie.
 */

/**
 * @brief Aplikacja-gospodarz, która przełącza się między kilkoma innymi aplikacjami.
 *
 * Silnik uruchamia dokładnie jedną Application (Engine::run()). AppSwitcher sam jest
 * Application i przekazuje wszystkie wywołania (zdarzenia, update, draw) aktywnej
 * aplikacji podrzędnej, dzięki czemu np. sandbox i demo gry działają w tym samym oknie.
 *
 * Sterowanie przełączaniem:
 *  - F1, F2, F3... - wybór aplikacji o danym numerze (kolejność dodawania),
 *  - Tab - następna aplikacja.
 *
 * Każda aplikacja dostaje onStart() tylko raz (przy pierwszej aktywacji), więc jej stan
 * (np. rysunek w sandboxie albo rozgrywka) zostaje zachowany po powrocie. Przy każdym
 * przełączeniu wywoływane są onDeactivate() starej i onActivate() nowej aplikacji, a jeśli
 * w międzyczasie zmieniła się rozdzielczość - także onResize().
 *
 * @note Przełącznik nie przejmuje na własność aplikacji - muszą żyć co najmniej tak długo jak on.
 */
class AppSwitcher : public Application {
    /// @brief Pojedyncza zarejestrowana aplikacja.
    struct Entry {
        std::string name;         ///< Nazwa wyświetlana w logu.
        Application* app;         ///< Wskaźnik na aplikację (bez własności).
        bool started = false;     ///< Czy onStart() już wywołano.
        sf::Vector2u lastSize{};  ///< Rozdzielczość, przy której aplikacja była ostatnio aktywna.
    };

    std::vector<Entry> entries;   ///< Zarejestrowane aplikacje.
    std::size_t current = 0;      ///< Indeks aktywnej aplikacji.
    Engine* engine = nullptr;     ///< Silnik uruchamiający przełącznik (ustawiany w onStart()).
    bool started = false;         ///< Czy przełącznik został już uruchomiony przez silnik.

    /// @brief Aktywuje aplikację o podanym indeksie (z obsługą onStart/onResize/onActivate).
    void activate(std::size_t index);

public:
    /**
     * @brief Dodaje aplikację do przełącznika.
     * @param name Nazwa aplikacji (do logu).
     * @param app Aplikacja; musi żyć dłużej niż przełącznik.
     */
    void add(const std::string& name, Application& app);
    /**
     * @brief Wybiera aplikację startową (przed uruchomieniem silnika).
     * @param index Indeks w kolejności dodawania (poza zakresem = bez zmian).
     */
    void setInitial(std::size_t index);
    /// @brief Zwraca liczbę zarejestrowanych aplikacji.
    std::size_t count() const { return entries.size(); }
    /// @brief Zwraca indeks aktywnej aplikacji.
    std::size_t getCurrentIndex() const { return current; }

    /// @brief Uruchamia aktywną aplikację. @param engine Silnik uruchamiający przełącznik.
    void onStart(Engine& engine) override;
    /// @brief Obsługuje klawisze przełączania, resztę zdarzeń oddaje aktywnej aplikacji. @param event Zdarzenie SFML.
    void onEvent(const sf::Event& event) override;
    /// @brief Aktualizuje aktywną aplikację. @param dt Krok czasu (s). @param input Stan wejścia.
    void update(float dt, const Input& input) override;
    /// @brief Rysuje aktywną aplikację. @param frame Bufor klatki.
    void draw(sf::Image& frame) override;
    /// @brief Przekazuje zmianę rozdzielczości aktywnej aplikacji. @param oldSize Stara. @param newSize Nowa.
    void onResize(sf::Vector2u oldSize, sf::Vector2u newSize) override;
    /// @brief Zwraca kolor tła aktywnej aplikacji.
    sf::Color backgroundColor() const override;
};

#endif // GRAFIKA2D_APPSWITCHER_H
