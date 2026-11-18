#pragma once
#include <chrono>
#include <random>
#include <vector>

#include "Bullet.h"
#include "Enemy.h"
#include "GameAssets.h"
#include "HotlinePlayer.h"
#include "engine/Application.h"

class Engine;

/**
 * @file HotlineGame.h
 * @brief Demo techniczne w stylu Hotline Miami.
 */

/**
 * @brief Demo techniczne w stylu Hotline Miami, napisane jako zwykła aplikacja na silniku.
 *
 * Sterowanie:
 *  - W/A/S/D (lub strzałki) - ruch,
 *  - mysz - celowanie (czerwona kreska od gracza do kursora), LPM - strzał,
 *  - R - nowa runda, Esc - wyjście,
 *  - F9/F10/F11 - rozdzielczość / pełny ekran (obsługuje silnik),
 *  - F1/F2/Tab - przełączanie sandbox <-> gra (AppSwitcher).
 *
 * Pięciu nieruchomych przeciwników stoi w losowych miejscach; trafiony znika.
 * Postacie są rysowane z tekstur (assets/), bez krwi po śmierci.
 */
class HotlineGame : public Application {
    static constexpr int ENEMY_COUNT = 5;   ///< Liczba przeciwników w rundzie.

    Engine* engine = nullptr;          ///< Silnik (ustawiany w onStart()).
    sf::Vector2u size{800, 600};       ///< Aktualny rozmiar bufora.
    GameAssets assets;                 ///< Tekstury; MUSZĄ być przed player (gracz składa się z tekstur).
    HotlinePlayer player;              ///< Gracz.
    std::vector<Enemy> enemies;        ///< Żywi przeciwnicy.
    std::vector<Bullet> bullets;       ///< Pociski w locie.
    /// Generator losowy; ziarno z random_device i zegara (na starszych MinGW random_device bywa deterministyczny).
    std::mt19937 rng{std::random_device{}() ^
                     static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count())};
    int shownEnemyCount = -1;          ///< Liczba przeciwników widoczna w tytule okna (-1 = nieustawiona).

    /// @brief Zaczyna nową rundę (gracz na środek, nowi przeciwnicy).
    void restart();
    /// @brief Rozstawia przeciwników w losowych, wolnych miejscach.
    void spawnEnemies();
    /// @brief Losuje punkt w obrębie ekranu (z marginesem).
    sf::Vector2f randomPoint();
    /// @brief Sprawdza, czy w punkcie można postawić przeciwnika (z dala od gracza i innych).
    bool isFreeSpot(sf::Vector2f p) const;
    /// @brief Odpycha gracza od przeciwników.
    void resolvePlayerCollisions();
    /// @brief Przesuwa pociski i sprawdza trafienia. @param dt Krok czasu (s).
    void updateBullets(float dt);
    /// @brief Ustawia tytuł okna z liczbą pozostałych przeciwników.
    void updateTitle();
    /// @brief Rysuje siatkę tła. @param frame Bufor klatki.
    void drawGrid(sf::Image& frame) const;

public:
    /// @brief Tworzy grę (wczytuje tekstury, tworzy gracza).
    HotlineGame();
    HotlineGame(const HotlineGame&) = delete; // sprite'y wskazuja na assets.handler
    HotlineGame& operator=(const HotlineGame&) = delete;

    /// @brief Zapamiętuje silnik i rozpoczyna pierwszą rundę. @param engine Silnik.
    void onStart(Engine& engine) override;
    /// @brief Obsługuje R (restart), Esc (wyjście) i strzał. @param event Zdarzenie SFML.
    void onEvent(const sf::Event& event) override;
    /// @brief Aktualizuje gracza, przeciwników i pociski. @param dt Krok czasu (s). @param input Stan wejścia.
    void update(float dt, const Input& input) override;
    /// @brief Rysuje planszę, przeciwników, pociski i gracza. @param frame Bufor klatki.
    void draw(sf::Image& frame) override;
    /// @brief Skaluje położenie obiektów do nowej rozdzielczości. @param oldSize Stara. @param newSize Nowa.
    void onResize(sf::Vector2u oldSize, sf::Vector2u newSize) override;
    /// @brief Po przełączeniu z innej aplikacji odświeża tytuł okna.
    void onActivate() override { if (engine) updateTitle(); }
    /// @brief Kolor tła gry.
    sf::Color backgroundColor() const override { return sf::Color(24, 24, 30); }

    /// @brief Zwraca gracza (do testów).
    const HotlinePlayer& getPlayer() const { return player; }
    /// @brief Zwraca przeciwników (do testów).
    const std::vector<Enemy>& getEnemies() const { return enemies; }
    /// @brief Zwraca pociski (do testów).
    const std::vector<Bullet>& getBullets() const { return bullets; }
};
