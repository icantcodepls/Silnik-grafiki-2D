#ifndef GRAFIKA2D_POLYGON_H
#define GRAFIKA2D_POLYGON_H
#include <optional>
#include <SFML/Graphics/Color.hpp>
#include "ClosedLine.h"

/**
 * @file Polygon.h
 * @brief Wielokąt prosty (bez samoprzecięć), opcjonalnie wypełniony.
 */

/**
 * @brief Wielokąt prosty budowany punkt po punkcie.
 *
 * Punkty można dodawać tylko wtedy, gdy wielokąt (lub otwarta łamana)
 * pozostaje prosty, czyli krawędzie nie przecinają się ani nie nakładają
 * (patrz canAddPoint()). Wielokąt trzeba jawnie zamknąć przez close();
 * dopiero zamknięty jest wypełniany.
 *
 * Wypełnienie realizowane jest algorytmem liniowym (scanline) z regułą
 * parzystości, domyślnie w kolorze obrysu.
 */
class Polygon:public ClosedLine {
    bool closed_ = false;                 ///< Czy wielokąt został zamknięty.
    bool filled_ = true;                  ///< Czy zamknięty wielokąt ma być wypełniany.
    std::optional<sf::Color> fillColor_;  ///< Kolor wypełnienia; brak wartości = kolor obrysu.
public:
    /**
     * @brief Sprawdza, czy dodanie punktu zachowa prostotę wielokąta.
     * @param point Kandydat na nowy wierzchołek.
     * @return @c true, jeśli nowa krawędź nie przecina istniejących.
     */
    bool canAddPoint(Point2D point) const;
    /**
     * @brief Dodaje wierzchołek, o ile canAddPoint() zwraca @c true (w przeciwnym razie nic nie robi).
     * @param point Nowy punkt.
     */
    void addPoint(Point2D point) override;
    /// @brief Sprawdza, czy wielokąt ma co najmniej 3 punkty i krawędź zamykająca nie psuje jego prostoty.
    bool canClose() const;
    /**
     * @brief Zamyka wielokąt.
     * @return @c true, jeśli wielokąt jest zamknięty po wywołaniu; @c false, gdy nie można go zamknąć.
     */
    bool close();
    /// @brief Sprawdza, czy wielokąt jest zamknięty (wymaga też co najmniej 3 punktów).
    bool isClosed() const;
    /// @brief Ponownie otwiera zamknięty wielokąt (można znów dodawać punkty).
    void reopen();
    /**
     * @brief Sprawdza, czy punkt leży w pobliżu pierwszego wierzchołka.
     * @param p Sprawdzany punkt.
     * @param radius Promień w pikselach (domyślnie 8).
     * @return @c true, jeśli odległość od pierwszego wierzchołka nie przekracza @p radius.
     */
    bool isNearFirstPoint(Point2D p, unsigned int radius = 8) const;
    /// @brief Włącza lub wyłącza wypełnianie. @param filled @c true - wypełniaj.
    void setFilled(bool filled) { filled_ = filled; }
    /// @brief Sprawdza, czy wypełnianie jest włączone.
    bool isFilled() const { return filled_; }
    /// @brief Ustawia kolor wypełnienia. @param color Kolor wypełnienia.
    void setFillColor(sf::Color color) { fillColor_ = color; }
    /// @brief Przywraca domyślny kolor wypełnienia (kolor obrysu).
    void resetFillColor() { fillColor_.reset(); }
    /// @brief Usuwa wszystkie punkty i otwiera wielokąt.
    void clear();
    /// @brief Usuwa ostatni punkt i otwiera wielokąt.
    void removeLastPoint();
    /**
     * @brief Rysuje wielokąt.
     *
     * Niezamknięty jest rysowany jak zwykła łamana. Zamknięty: najpierw
     * wypełnienie (jeśli włączone), potem obrys z krawędzią zamykającą.
     *
     * @param bufor Obraz docelowy.
     */
    void draw(Image& bufor) override;
};


#endif //GRAFIKA2D_POLYGON_H
