#ifndef GRAFIKA2D_RECTANGLE_H
#define GRAFIKA2D_RECTANGLE_H
#include "ClosedLine.h"


/**
 * @file Rectangle.h
 * @brief Prostokąt zbudowany z domkniętej łamanej.
 */

/**
 * @brief Prostokąt rysowany jako obrys (ClosedLine).
 */
class Rectangle:public ShapeObject {
    Point2D origin;      ///< Lewy górny róg.
    ClosedLine sides;    ///< Obrys prostokąta.
    Color color;         ///< Kolor obrysu.
public:
    /// @brief Tworzy prostokąt. @param x X rogu. @param y Y rogu. @param h Wysokość. @param w Szerokość. @param color Kolor.
    Rectangle(unsigned int x, unsigned int y, unsigned int h, unsigned int w, Color color=Color::White);
    /// @brief Rysuje obrys. @param bufor Obraz docelowy.
    void draw(Image &bufor) override;
    /// @brief Przesuwa prostokąt. @param dx Przesunięcie poziome. @param dy Przesunięcie pionowe.
    void translate(int dx, int dy) override;
    /// @brief Obraca wokół punktu (x, y). @param x X środka. @param y Y środka. @param angle Kąt (rad).
    void rotate(unsigned int x, unsigned int y, float angle) override;
    /// @brief Skaluje względem punktu (x, y). @param x X środka. @param y Y środka. @param k Współczynnik.
    void scale(unsigned int x, unsigned int y, float k) override;
};


#endif //GRAFIKA2D_RECTANGLE_H