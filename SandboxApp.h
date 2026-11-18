#ifndef GRAFIKA2D_SANDBOXAPP_H
#define GRAFIKA2D_SANDBOXAPP_H
#include <SFML/Graphics.hpp>

#include "engine/Application.h"
#include "engine/Polygon.h"
class Engine;

/**
 * @file SandboxApp.h
 * @brief Przykładowa aplikacja demonstrująca możliwości silnika.
 */

/**
 * @brief Prosty edytor rysunku (sandbox) korzystający z silnika 2D.
 *
 * Aplikacja przechowuje własne płótno (@c canvas), na którym rysuje
 * kliknięciami i skrótami klawiszowymi, a w każdej klatce kopiuje je do
 * bufora silnika i dodaje podgląd (gumkę) rysowanego kształtu.
 *
 * Sterowanie:
 *  - LPM - piksel (lub kolejny punkt w trybie linii / łamanej / wielokąta),
 *  - L - tryb linii (dwa kliknięcia),
 *  - C - tryb łamanej (ponowne C kończy i rysuje),
 *  - G - tryb wielokąta (ponowne G zamyka i rysuje; Backspace - cofnij punkt, Delete - anuluj),
 *  - S / R / P / E - kwadrat / prostokąt / koło / elipsa w miejscu kursora,
 *  - F - wypełnienie obszaru od kursora,
 *  - 0-4 - czyszczenie płótna (czarny, biały, czerwony, niebieski, zielony),
 *  - F9 / F10 - zmiana rozdzielczości, F11 - pełny ekran, Esc - wyjście,
 *  - F1 / F2 / Tab - przełączanie między sandboxem a demem gry (AppSwitcher).
 */
class SandboxApp : public Application {
    /// @brief Tryb pracy kliknięć myszy.
    enum class Mode {
        Point,      ///< Rysowanie pojedynczych pikseli.
        Line,       ///< Rysowanie odcinka (dwa kliknięcia).
        Polyline,   ///< Rysowanie linii łamanej.
        Polygon     ///< Rysowanie wielokąta.
    };

    Engine* engine = nullptr;        ///< Silnik uruchamiający aplikację (ustawiany w onStart()).
    Image canvas;                    ///< Płótno z gotowym rysunkiem.
    Color bgColor = Color::Black;    ///< Aktualny kolor tła płótna.
    Mode mode = Mode::Point;         ///< Aktualny tryb rysowania.

    Point2D point;                   ///< Punkt używany w trybie Mode::Point.
    LineSegment segment;             ///< Odcinek używany w trybie Mode::Line.
    SegmentedLine segmentedLine;     ///< Łamana rysowana w trybie Mode::Polyline.
    Polygon polygon;                 ///< Wielokąt rysowany w trybie Mode::Polygon.
    bool hasFirstPoint = false;      ///< Czy w trybie linii kliknięto już pierwszy punkt.
    unsigned int firstX = 0, firstY = 0;   ///< Współrzędne pierwszego punktu odcinka.

    /**
     * @brief Czyści płótno podanym kolorem i zapamiętuje go jako kolor tła.
     * @param color Kolor tła.
     */
    void clearCanvas(Color color);
    /**
     * @brief Obsługuje kliknięcie lewym przyciskiem myszy zależnie od trybu.
     * @param x Współrzędna X w buforze obrazu.
     * @param y Współrzędna Y w buforze obrazu.
     */
    void handleClick(unsigned int x, unsigned int y);
    /**
     * @brief Obsługuje wciśnięcie klawisza (skróty rysowania, czyszczenia i wyjścia).
     * @param code Wciśnięty klawisz.
     */
    void handleKey(Keyboard::Key code);
    /// @brief Włącza lub wyłącza tryb rysowania linii.
    void toggleLine();
    /// @brief Włącza tryb łamanej lub kończy go, rysując łamaną na płótnie.
    void togglePolyline();
    /// @brief Włącza tryb wielokąta lub próbuje zamknąć i narysować bieżący wielokąt.
    void togglePolygon();
    /**
     * @brief Rysuje podgląd bieżącego kształtu (łamana, wielokąt, "gumka" do kursora).
     * @param frame Bufor klatki, w którym rysowany jest podgląd.
     */
    void drawPreview(Image& frame);
public:
    /// @brief Zapamiętuje silnik, czyści płótno i wypisuje opis sterowania. @param engine Silnik uruchamiający aplikację.
    void onStart(Engine& engine) override;
    /// @brief Ustawia tytuł okna sandboxa po przełączeniu z innej aplikacji.
    void onActivate() override;
    /// @brief Obsługuje zdarzenia myszy, klawiatury i zamknięcia okna. @param event Zdarzenie SFML.
    void onEvent(const Event& event) override;
    /// @brief Kopiuje płótno do klatki i dodaje podgląd rysowania. @param frame Bufor klatki.
    void draw(Image& frame) override;
    /**
     * @brief Dopasowuje płótno do nowej rozdzielczości, zachowując wspólny fragment rysunku.
     * @param oldSize Poprzednia rozdzielczość.
     * @param newSize Nowa rozdzielczość.
     */
    void onResize(Vector2u oldSize, Vector2u newSize) override;
};



#endif //GRAFIKA2D_SANDBOXAPP_H
