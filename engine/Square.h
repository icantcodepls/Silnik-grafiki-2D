#ifndef GRAFIKA2D_SQUARE_H
#define GRAFIKA2D_SQUARE_H
#include "ClosedLine.h"
#include "ShapeObject.h"


/**
 * @file Square.h
 * @brief Kwadrat zbudowany z domkniętej łamanej.
 */

/**
 * @brief Kwadrat rysowany jako obrys (ClosedLine).
 */
class Square:public ShapeObject {
    Point2D origin;      ///< Lewy górny róg.
    ClosedLine sides;    ///< Obrys kwadratu.
public:
    /// @brief Tworzy kwadrat. @param x X rogu. @param y Y rogu. @param s Bok. @param color Kolor.
    Square(unsigned int x, unsigned int y, unsigned int s, Color color=Color::Black);
    /// @brief Rysuje obrys. @param bufor Obraz docelowy.
    void draw(Image &bufor) override;
    /// @brief Przesuwa kwadrat. @param dx Przesunięcie poziome. @param dy Przesunięcie pionowe.
    void translate(int dx, int dy) override;
    /// @brief Obraca wokół punktu (x, y). @param x X środka. @param y Y środka. @param angle Kąt (rad).
    void rotate(unsigned int x, unsigned int y, float angle) override;
    /// @brief Skaluje względem punktu (x, y). @param x X środka. @param y Y środka. @param k Współczynnik.
    void scale(unsigned int x, unsigned int y, float k) override;
};


#endif //GRAFIKA2D_SQUARE_H