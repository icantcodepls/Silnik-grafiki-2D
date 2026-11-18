#ifndef GRAFIKA2D_ENGINE_H
#define GRAFIKA2D_ENGINE_H
#include <string>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Application.h"
#include "Input.h"

using namespace std;
using namespace sf;

/**
 * @file Engine.h
 * @brief Główna klasa silnika: okno, pętla gry, bufor obrazu.
 */

/// @brief Ustawienia startowe silnika.
struct EngineSettings {
    string title = "Silnik 2D";          ///< Tytuł okna.
    Vector2u resolution{800,600};        ///< Rozdzielczość bufora obrazu.
    bool fullscreen=false;               ///< Czy zacząć w pełnym ekranie.
    bool vsync=false;                    ///< Synchronizacja pionowa.
    unsigned int fpsLimit=0;             ///< Limit klatek na sekundę (0 = bez limitu).
    float updatePerSecond=60.f;          ///< Liczba kroków logiki na sekundę (stały krok czasu).
};

namespace sf { class Image; }

/**
 * @brief Silnik 2D: tworzy okno, prowadzi pętlę gry ze stałym krokiem czasu i wyświetla bufor obrazu.
 *
 * Aplikacja (Application) rysuje w buforze sf::Image (framebufferze w pamięci procesora);
 * silnik przesyła go do tekstury i skaluje do okna z zachowaniem proporcji (czarne pasy).
 *
 * Wbudowane skróty: F9 / F10 - mniejsza / większa rozdzielczość bufora,
 * F11 lub Alt+Enter - pełny ekran. Błędy trafiają do pliku (Logger).
 */
class Engine {
    EngineSettings settings;            ///< Ustawienia startowe.
    Vector2u resolution_;               ///< Aktualna rozdzielczość bufora.
    bool fullscreen_;                   ///< Czy okno jest w pełnym ekranie.
    string title_;                      ///< Aktualny tytuł okna.
    bool running=false;                 ///< Czy pętla gry działa.
    Application* app=nullptr;           ///< Uruchomiona aplikacja (bez własności).

    RenderWindow window;                ///< Okno SFML.
    Texture texture;                    ///< Tekstura z zawartością bufora.
    Sprite sprite{texture};             ///< Sprite wyświetlający teksturę.
    Image frame;                        ///< Bufor obrazu rysowany przez aplikację.
    Image blank;                        ///< Pusty bufor (do czyszczenia klatki).
    Color bgColor = Color::Black;       ///< Kolor tła okna.
    Input input_;                       ///< Aktualny stan wejścia.

    /// @brief Tworzy (lub odtwarza) okno. @return false przy błędzie.
    bool createWindow();
    /// @brief Odtwarza bufor i teksturę po zmianie rozdzielczości. @return false przy błędzie.
    bool rebuildFrame();
    /// @brief Ustawia widok z zachowaniem proporcji (letterbox).
    void updateView();
    /// @brief Odczytuje zdarzenia okna i przekazuje je aplikacji.
    void processInput();
    /// @brief Rysuje klatkę aplikacji i wyświetla ją w oknie.
    void render();
    /// @brief Przełącza na sąsiednią rozdzielczość z listy. @param direction +1 większa, -1 mniejsza.
    void stepResolution(int direction);
public:
    /// @brief Tworzy silnik (okno powstaje w run()). @param settings Ustawienia startowe.
    explicit Engine(const EngineSettings& settings = EngineSettings());
    ~Engine()=default;
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    /**
     * @brief Uruchamia pętlę gry i blokuje do zamknięcia okna lub quit().
     * @param application Aplikacja do uruchomienia (musi żyć do końca run()).
     * @return 0 po normalnym zakończeniu, 1 przy błędzie.
     */
    int run(Application& application);
    /// @brief Kończy pętlę gry.
    void quit();
    /// @brief Ustawia rozdzielczość bufora (wywołuje Application::onResize()). @param resolution Nowy rozmiar.
    void setResolution(Vector2u resolution);
    /// @brief Przełącza na następną (większą) rozdzielczość.
    void nextResolution(){stepResolution(+1);}
    /// @brief Przełącza na poprzednią (mniejszą) rozdzielczość.
    void previousResolution(){stepResolution(-1);}
    /// @brief Włącza lub wyłącza pełny ekran. @param fullscreen Nowy stan.
    void setFullscreen(bool fullscreen);
    /// @brief Przełącza pełny ekran.
    void toggleFullscreen(){setFullscreen(!fullscreen_);}
    /// @brief Ustawia tytuł okna. @param title Nowy tytuł.
    void setTitle(const string title);
    /// @brief Zwraca rozdzielczość bufora obrazu.
    Vector2u getResolution(){return resolution_;}
    /// @brief Zwraca true w pełnym ekranie.
    bool isFullscreen(){return fullscreen_;}
    /// @brief Zwraca aktualny stan wejścia.
    Input& getInput(){return input_;}
    /// @brief Zamienia współrzędne okna na współrzędne bufora (uwzględnia skalowanie i pasy). @param pos Pozycja w oknie.
    Vector2f toFrameCoords(Vector2i pos);
    /// @brief Zwraca pozycję myszy we współrzędnych bufora.
    Vector2f mousePosition();
};

#endif //GRAFIKA2D_ENGINE_H