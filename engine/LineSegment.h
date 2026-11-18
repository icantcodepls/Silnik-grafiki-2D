#ifndef GRAFIKA2D_LINESEGMENT_H
#define GRAFIKA2D_LINESEGMENT_H
#include "Point2D.h"
#include "ShapeObject.h"

namespace sf { class Image; }

/**
 * @file LineSegment.h
 * @brief Odcinek między dwoma punktami.
 */

/**
 * @brief Odcinek rysowany algorytmem linii w buforze obrazu; można go przesuwać, skalować i obracać.
 */
class LineSegment:public ShapeObject {
    Point2D p1, p2;   ///< Początek i koniec odcinka.
public:
    LineSegment() = default;
    /// @brief Ustawia oba końce. @param x1 X początku. @param y1 Y początku. @param x2 X końca. @param y2 Y końca.
    void setPoints(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
    /// @brief Zwraca X początku.
    unsigned int getStartX();
    /// @brief Zwraca Y początku.
    unsigned int getStartY();
    /// @brief Zwraca X końca.
    unsigned int getEndX();
    /// @brief Zwraca Y końca.
    unsigned int getEndY();
    /// @brief Rysuje odcinek kolorem obiektu. @param bufor Obraz docelowy.
    void draw(Image& bufor) override;
    /// @brief Przesuwa odcinek. @param dx Przesunięcie poziome. @param dy Przesunięcie pionowe.
    void translate(int dx, int dy) override;
    /// @brief Skaluje względem punktu (x, y). @param x X środka. @param y Y środka. @param k Współczynnik.
    void scale(unsigned int x, unsigned int y, float k) override;
    /// @brief Obraca wokół punktu (x, y). @param x X środka. @param y Y środka. @param angle Kąt (rad).
    void rotate(unsigned int x, unsigned int y, float angle) override;
};

#endif //GRAFIKA2D_LINESEGMENT_H