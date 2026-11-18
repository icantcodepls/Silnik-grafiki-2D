#ifndef GRAFIKA2D_INPUT_H
#define GRAFIKA2D_INPUT_H
#include <array>

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

/**
 * @file Input.h
 * @brief Migawka stanu klawiatury i myszy.
 */

/**
 * @brief Aktualny stan wejścia (które klawisze i przyciski są wciśnięte, gdzie jest mysz).
 *
 * Silnik uzupełnia go w każdej klatce i przekazuje do Application::update().
 */
class Input {
    std::array<bool, sf::Keyboard::KeyCount> keys{};      ///< Stan klawiszy.
    std::array<bool, sf::Mouse::ButtonCount> buttons{};   ///< Stan przycisków myszy.
public:
    sf::Vector2f mouse{0.f, 0.f};   ///< Pozycja myszy we współrzędnych bufora obrazu.

    /// @brief Czy klawisz jest wciśnięty. @param key Klawisz.
    bool isKeyDown(sf::Keyboard::Key key) const {
        const auto i = static_cast<std::size_t>(key);
        return key != sf::Keyboard::Key::Unknown && i < keys.size() && keys[i];
    }
    /// @brief Czy przycisk myszy jest wciśnięty. @param button Przycisk.
    bool isMouseDown(sf::Mouse::Button button) const {
        const auto i = static_cast<std::size_t>(button);
        return i < buttons.size() && buttons[i];
    }

    /// @brief Ustawia stan klawisza (używa silnik). @param key Klawisz. @param down Czy wciśnięty.
    void setKey(sf::Keyboard::Key key, bool down) {
        const auto i = static_cast<std::size_t>(key);
        if (key != sf::Keyboard::Key::Unknown && i < keys.size()) keys[i] = down;
    }
    /// @brief Ustawia stan przycisku myszy (używa silnik). @param button Przycisk. @param down Czy wciśnięty.
    void setMouseButton(sf::Mouse::Button button, bool down) {
        const auto i = static_cast<std::size_t>(button);
        if (i < buttons.size()) buttons[i] = down;
    }
    /// @brief Zeruje stan wszystkich klawiszy i przycisków (np. po utracie fokusu okna).
    void clear() {
        keys.fill(false);
        buttons.fill(false);
    }
};

#endif //GRAFIKA2D_INPUT_H